#ifndef CSTR_H
#define CSTR_H


#define cstr char *
typedef struct {
    uint32_t cap;
    uint32_t len;
    char buf[];
} cstrhdr;

#define CSTRHLEN sizeof(cstrhdr)
#define CSTR_REALPTR(s) ((char*)(s - CSTRHLEN))
#define CSTR_HDR(s) ((cstrhdr*)(s - CSTRHLEN))
#define cstr_cap(s) CSTR_HDR(s)->cap
#define CSTR_HDR_USED(s) (s->len)
#define cstr_used(s) (CSTR_HDR_USED(CSTR_HDR(s)))

#ifdef __cplusplus
extern "C" {
#endif

cstr cstr_create(size_t len);
cstr cstr_new(const char *c, size_t len);
void cstr_free(cstr s);
cstr cstr_dup(cstr s);
cstr cstr_ncat(cstr s, const char *b, size_t l);
void cstr_clear(cstr s);
void cstr_tolower(cstr s);
cstr cstr_cat_vprintf(cstr s, const char *fmt, va_list arg);
cstr cstr_cat_printf(cstr s, const char *fmt, ...);
void cstr_toupper(cstr s);
cstr cstr_extend(cstr s, size_t add);
cstr* cstr_split(char *s, size_t len, const char *b, size_t slen, size_t *l);

inline static cstr cstr_wrap(const char *buf, const char *s, size_t len) {
  cstrhdr *hdr = (cstrhdr*)buf;
  hdr->cap = 0;
  hdr->len = len;
  memcpy(hdr->buf, s, len);
  hdr->buf[len] = 0;
  return (cstr)hdr->buf;
}

inline static cstr cstr_cat(cstr s, const char *b) {
  return cstr_ncat(s, b, strlen(b));
}

inline static cstr cstr_cat_char(cstr s, char c) {
  cstrhdr *csh;
  if(s == NULL)
    s = cstr_create(16);
  else
    s = cstr_extend(s, 1);
  csh = CSTR_HDR(s);
  s[cstr_used(s)] = c;
  csh->len++;
  return s;
}


#ifdef __cplusplus
}
#endif



#endif /*end common str define*/
