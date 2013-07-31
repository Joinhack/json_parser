#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "setting.h"
#include "json.h"
#include "json_yy.h"
#include "json_ll.h"


void yyerror(json_ctx *ctx, void *scan, const char* fmt, ...) {
  va_list arg;
  ctx->token = cstr_ncat(ctx->token, yyget_text(scan), yyget_leng(scan));
  va_start(arg, fmt);
  ctx->err = cstr_cat_printf(ctx->err, fmt, arg);
  va_end(arg);
}

json_ctx *json_ctx_new() {
  json_ctx *ctx;
  setting *setting = get_setting();
  ctx = setting->malloc(sizeof(struct json_ctx));
  memset(ctx, 0, sizeof(struct json_ctx));
  return ctx;
}

void json_ctx_free(json_ctx *ctx) {
  setting *setting = get_setting();
  if(ctx->err)
    cstr_free(ctx->err);
  if(ctx->token)
    cstr_free(ctx->token);
  if(ctx->rs)
    json_free(ctx->rs);
  setting->free(ctx);
}

void json_ctx_free(json_ctx *ctx);

static inline int key_compare(const void *k1, const void *k2) {
  int l1 = cstr_used((cstr)k1);
  if(l1 != cstr_used((cstr)k2))
    return -1;
  return memcmp(k1, k2, l1);
}

static inline void _general_free(void *o) {
  setting *setting = get_setting();
  setting->free(o);
}

static inline void _json_free(json_object *o) {
  _general_free((void*)o);
}

static inline void key_free(void *o) {
  cstr_free((cstr)o);
}

static inline void json_value_free(void *v) {
  json_free((json_object*)v);
}

static inline unsigned int json_key_hash(const void *key) {
  return dict_generic_hash(key, cstr_used((cstr)key));
}

dict_opts json_dict_opts = {
  .hash = json_key_hash,
  .key_compare = key_compare,
  .key_free = key_free,
  .value_free = json_value_free
};

static inline void json_string_free(json_object *o) {
  if(o->o.str)
    cstr_free(o->o.str);
  _json_free(o);
}

static inline void json_array_free(json_object *o) {
  if(o->o.array)
    arraylist_free(o->o.array);
  _json_free(o);
}

static inline void json_dict_free(json_object *o) {
  if(o->o.dict)
    dict_free(o->o.dict);
  _json_free(o);
}

json_object *json_new(enum json_type type) {
  setting *setting = get_setting();
  json_object *o = setting->malloc(sizeof(json_object));
  memset(o, 0, sizeof(json_object));
  o->o_type = type;
  switch(type) {
  case json_type_int:
  case json_type_null:
  case json_type_bool:
  case json_type_double:
    o->free = _json_free;
    break;
  case json_type_string:
    o->free = json_string_free;
    break;
  case json_type_array:
    o->o.array = arraylist_new();
    o->free = json_array_free;
    break;
  case json_type_object:
    o->free = json_dict_free;
    break;
  default:
    setting->free(o);
    return NULL;
  }
  return o;
}

void json_free(json_object *o) {
  o->free(o);
}

int json_parse(json_ctx *ctx, char *buf, int len) {
  int rs;
  yylex_init(&ctx->scanner);
  yyset_extra(ctx, ctx->scanner);
  yy_scan_bytes(buf, len, ctx->scanner);
  rs = yyparse(ctx, ctx->scanner);
  yylex_destroy(ctx->scanner);
  return rs;
}

int json_parse_file(json_ctx *ctx, FILE *file) {
  int rs;
  yylex_init(&ctx->scanner);
  yyset_extra(ctx, ctx->scanner);
  yyset_in(file, ctx->scanner);
  rs = yyparse(ctx, ctx->scanner);
  yylex_destroy(ctx->scanner);
  return rs;
}

