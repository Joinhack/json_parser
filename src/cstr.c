#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "cstr.h"
#include "setting.h"

#define CSH_USED(c) (c->len - c->free)

cstr cstr_create(size_t len) {
  setting *setting = get_setting();
  char *c = setting->malloc(len + HLEN + 1);
  cstrhdr *csh = (cstrhdr*)c;
  csh->len = len;
  csh->free = len;
  csh->buf[len] = 0;
  return (cstr)csh->buf;
}

cstr cstr_new(const char *c, size_t len) {
  cstr s = cstr_create(len);
  cstrhdr *csh = CSTR_HDR(s);
  memcpy(s, c, len);
  s[len] = '\0';
  csh->free = 0;
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
  if(csh->free >= add) return s;
  csh->len = (used + add)*2;
  csh = setting->realloc((void*)csh, csh->len + HLEN + 1);
  csh->free = csh->len - used;
  return (cstr)csh->buf;
}

cstr cstr_ncat(cstr s, const char *b, size_t l) {
  cstrhdr *csh;
  if(s == NULL)
    s = cstr_create(l);
  else 
    s = cstr_extend(s, l);
  csh = CSTR_HDR(s);
  memcpy(csh->buf + CSH_USED(csh), b, l);
  csh->free -= l;
  s[CSTR_HDR_USED(csh)] = '\0';
  return (cstr)csh->buf;
}

void cstr_clear(cstr s) {
  cstrhdr *csh = CSTR_HDR(s);
  csh->free = csh->len;
  csh->buf[0] = '\0';
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