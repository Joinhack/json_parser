#ifndef JSON_H
#define JSON_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "setting.h"
#include "arraylist.h"
#include "cstr.h"
#include "dict.h"
#include "codec.h"

enum json_type {
  json_type_null,
  json_type_int,
  json_type_bool,
  json_type_double,
  json_type_string,
  json_type_array,
  json_type_object
};

typedef struct json_object {
  enum json_type o_type;
  union data {
    //boolean
    int b;
    //double
    double d;
    //int
    int64_t i;

    //string
    cstr str;

    //array
    arraylist *array;
    dict *dict;
  } o;
  void (*free)(struct json_object *obj);
} json_object;

typedef struct json_ctx {
  void *scanner;
  json_object *rs;
  cstr buf;
} json_ctx;


json_object *json_new(enum json_type);

void json_free(json_object *o);

json_ctx *json_ctx_new();

void json_ctx_free(json_ctx *ctx);

int json_parse(json_ctx *ctx, char *buf, int len);

extern dict_opts json_dict_opts;

void yyerror(void *scan, const char* fmt, ...);

#ifdef USE_SETTING

static inline void* setting_malloc(size_t s) {
  setting *setting = get_setting();
  return setting->malloc(s);
}

static inline void* setting_realloc(void *p, size_t s) {
  setting *setting = get_setting();
  return setting->realloc(p, s);
}

static inline void setting_free(void *p) {
  setting *setting = get_setting();
  setting->free(p);
}

#define malloc(s) setting_malloc(s)

#define realloc(p, s) setting_realloc(p, s)

#define free(p) setting_free(p)

#endif

#endif
