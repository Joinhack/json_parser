%{

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define USE_SETTING

#include "json.h"


#define FMT_KEY(k, fmt, v) \
{\
char buf[64] = {0};\
snprintf(buf, 64, fmt, v);\
cstr c =  cstr_new(buf, strlen(buf));\
k = c;\
}


%}

%union {
  cstr s;
  int s_len;
  int64_t iconst;
  double dconst;
  int bconst;
  json_object *json;
  arraylist *list;
  dict *dict;
}

%{
#include "json_ll.h"
%}

%define api.pure full
%lex-param {void * scanner}
%parse-param {json_ctx *ctx}
%parse-param {void * scanner}


%token<s>     str_const
%token<iconst> tok_int_constant
%token<dconst> tok_double_constant
%token<bconst> tok_bool_constant
%token tok_obj_start tok_obj_end  tok_null 

%type<json> OBJECT
%type<dict> MEMBERS
%type<dict> PAIR
%type<json> VALUE
%type<json> STRING
%type<json> ARRAY
%type<list> ELEMENTS

%destructor {json_free($$);} VALUE ARRAY STRING OBJECT
%destructor {arraylist_free($$);} ELEMENTS
%destructor {cstr_free($$);} str_const
%destructor {dict_free($$);} MEMBERS PAIR

%%

JSON: OBJECT {
  ctx->rs = $1;
}
| ARRAY {
  ctx->rs = $1;
}

OBJECT: '{' '}' {
  json_object *o = json_new(json_type_object);
  $$ = o;
}
| '{' MEMBERS '}' {
  json_object *o = json_new(json_type_object);
  o->o.dict = $2;
  $$ = o;
}

MEMBERS: PAIR {
  $$ = $1;
}
| PAIR ',' MEMBERS {
  dict_move($3, $1);
  dict_free($1);
  $$ = $3;
}

PAIR: str_const ':' VALUE {
  dict *d = dict_new(&json_dict_opts);
  dict_replace(d, $1, $3);
  $$ = d;
}
| tok_int_constant ':' VALUE {
  char *key;
  dict *d = dict_new(&json_dict_opts);
  FMT_KEY(key, "%lld", $1);
  dict_replace(d, key, $3);
  $$ = d;
}
| tok_double_constant ':' VALUE {
  char *key;
  dict *d = dict_new(&json_dict_opts);
  FMT_KEY(key, "%lf", $1);
  dict_replace(d, key, $3);
  $$ = d;
}
|tok_null ':' VALUE {
  char *key;
  dict *d = dict_new(&json_dict_opts);
  FMT_KEY(key, "%s", "null");
  dict_replace(d, key, $3);
  $$ = d;
}
|tok_bool_constant ':' VALUE {
  char *key;
  dict *d = dict_new(&json_dict_opts);
  FMT_KEY(key, "%s", $1 == 1?"true":"false");
  dict_replace(d, key, $3);
  $$ = d;
}

ARRAY: '[' ']' {
  json_object *o = json_new(json_type_array);
  $$ = o;
}
| '[' ELEMENTS ']' {
  json_object *o = json_new(json_type_array);
  arraylist_move(o->o.array, $2);
  arraylist_free($2);
  $$ = o;
}

ELEMENTS: VALUE {
  arraylist *l = arraylist_new();
  arraylist_add(l, $1);
  $$ = l;
}
| VALUE ',' ELEMENTS {
  arraylist_add($3, $1);
  $$ = $3;
}

STRING: str_const {
  json_object *o = json_new(json_type_string);
  o->o.str = $1;
  $$ = o;
}

VALUE: STRING {
  $$ = $1;
}
| tok_int_constant {
  json_object *o = json_new(json_type_int);
  o->o.i = $1;
  $$ = o;
} 
| tok_double_constant {
  json_object *o = json_new(json_type_double);
  o->o.d = $1;
  $$ = o;
} 
| tok_bool_constant {
  json_object *o = json_new(json_type_bool);
  o->o.b = $1;
  $$ = o;
}
| tok_null {
  json_object *o = json_new(json_type_null);
  $$ = o;
}
| ARRAY {
  $$ = $1;
}
| OBJECT {
  $$ = $1;
}

;

%%









