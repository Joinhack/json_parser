#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include "cstr.h"
#include "setting.h"

#define CSH_USED(c) (c->len)

cstr cstr_create(size_t len) {
  setting *setting = get_setting();
  char *c = setting->malloc(len + CSTRHLEN + 1);
  cstrhdr *csh = (cstrhdr*)c;
  csh->cap = len;
  csh->len = 0;
  csh->buf[0] = 0;
  return (cstr)csh->buf;
}

cstr cstr_new(const char *c, size_t len) {
  cstr s = cstr_create(len);
  cstrhdr *csh = CSTR_HDR(s);
  memcpy(s, c, len);
  s[len] = '\0';
  csh->len = len;
  return s;
}

void cstr_free(cstr s) {
  setting *setting = get_setting();
  char *ptr = CSTR_REALPTR(s);
  setting->free(ptr);
}

cstr cstr_extend(cstr s, size_t add) {
  setting *setting = get_setting();
  cstrhdr *csh = CSTR_HDR(s);
  size_t used = CSTR_HDR_USED(csh);
  if((csh->cap - used) >= add) return s;
  csh->cap = (used + add)*2;
  csh = setting->realloc((void*)csh, csh->cap + CSTRHLEN + 1);
  return (cstr)csh->buf;
}

cstr cstr_ncat(cstr s, const char *b, size_t l) {
  cstrhdr *csh;
  if(s == NULL)
    return cstr_new(b, l);
  s = cstr_extend(s, l);
  csh = CSTR_HDR(s);
  memcpy(csh->buf + CSH_USED(csh), b, l);
  csh->len += l;
  s[CSTR_HDR_USED(csh)] = '\0';
  return (cstr)csh->buf;
}

cstr* cstr_split(char *s, size_t len, const char *b, size_t slen, size_t *l) {
  setting *setting = get_setting();
  cstr *array = NULL;
  size_t i, cap = 0, size = 0, beg = 0;
  array = setting->malloc(sizeof(cstr)*cap);
  for(i = 0; i < len - (slen - 1); i++) {
    if(size + 1 >= cap ) {
      cap += 5;
      array = setting->realloc(array, sizeof(cstr)*cap);
    }
    if(s[i] == b[0] && memcmp(s + i, b, slen) == 0) {
      array[size] = cstr_new(s + beg, i - beg);
      beg = i + slen;
      size++;
    }
  }
  array[size++] = cstr_new(s + beg, len - beg);
  *l = size;
  return array;
}

void cstr_clear(cstr s) {
  cstrhdr *csh = CSTR_HDR(s);
  csh->len = 0;
  csh->buf[0] = '\0';
}

cstr cstr_cat_vprintf(cstr s, const char *fmt, va_list arg) {
  va_list cpy;
  setting *setting = get_setting();
  char *buf, *t;
  size_t buflen = 16;

  while(1) {
    buf = setting->malloc(buflen);
    if (buf == NULL) return NULL;
    buf[buflen-2] = '\0';
    va_copy(cpy, arg);
    vsnprintf(buf, buflen, fmt, cpy);
    if (buf[buflen-2] != '\0') {
        setting->free(buf);
        buflen *= 2;
        continue;
    }
    break;
  }
  t = cstr_ncat(s, buf, strlen(buf));
  setting->free(buf);
  return t;
}

cstr cstr_cat_printf(cstr s, const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  s = cstr_cat_vprintf(s, fmt, arg);
  va_end(arg);
  return s;
}

void cstr_tolower(cstr s) {
  size_t i;
  for(i = 0; i < cstr_used(s); i++) s[i] = tolower(s[i]);
}

void cstr_toupper(cstr s) {
  size_t i;
  for(i = 0; i < cstr_used(s); i++) s[i] = toupper(s[i]);
}

cstr cstr_dup(cstr s) {
  size_t used = cstr_used(s);
  cstr ns = cstr_create(used);
  cstr_ncat(ns, s, used);
  return ns;
}