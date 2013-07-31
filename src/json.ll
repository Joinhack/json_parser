%{
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-label"

#define USE_SETTING

#include <stdio.h>
#include "setting.h"
#include "json.h"
#include "json_yy.h"

#define COLNO_ADD(i) (yyextra->colno += i)


#define ERROR(m) yyextra->err = cstr_cat(yyextra->err, m)

#define integer_overflow() \
  ERROR("This integer is too big")


static inline int append_char(json_ctx *ctx, char c) {
  ctx->buf = cstr_cat_char(ctx->buf, c);
  return 0;
}

static inline int append_unicode(json_ctx *ctx, const char *ptr) {
  unsigned char utf8[4];
  char *end_ptr;
  long ucs2;
  int rs;
  ucs2 = strtol (ptr, & end_ptr, 16);
  rs = ucs2_to_utf8(ucs2, utf8);
  if(rs < 0)
    return -1;
  ctx->buf = cstr_ncat(ctx->buf, (char*)utf8, rs);

  return 0;
}

static inline int append_escape (json_ctx *ctx, char escape) {
  switch(escape) {
  case '\\': return append_char(ctx, '\\');
  case 'n': return append_char(ctx, '\n');
  case '/': return append_char(ctx, '/');
  case 'r': return append_char(ctx, '\r');
  case 't': return append_char(ctx, '\t');
  case '"': return append_char(ctx, '"');
  case 'b': return append_char(ctx, '\b');
  case 'f': return append_char(ctx, '\f');
  default:
    return -1;
  }
  return 0; 
}

%}


%option noyywrap
%option bison-bridge
%option reentrant
%option extra-type="json_ctx *"

%x STRING_STATE

intcosnt           ([+-]?[0-9]+)
hexconst           ("0x"[0-9A-Za-z]+)
dconst             ([+-]?[0-9]*(\.[0-9]+)?([eE][+-]?[0-9]+)?)
whitespace         ([ \t\r]*)
escape             \\["\\/bfnrt]
unicode            \\u[0-9a-fA-F]{4}
utf8_2             [\xC2-\xDF][\x80-\xBF]
utf8_3             [\xE1-\xEF][\x80-\xBF]{2}
utf8_4             [\xF0-\xF4][\x80-\xBF]{3}

%%

<INITIAL>false { 
  COLNO_ADD(yyleng);
  yylval->bconst=0; 
  return tok_bool_constant;
}

<INITIAL>true {
  COLNO_ADD(yyleng);
  yylval->bconst=1; 
  return tok_bool_constant; 
}

<INITIAL>null {
  COLNO_ADD(yyleng);
  return tok_null; 
}

<INITIAL>{whitespace} {
  COLNO_ADD(yyleng);
}

<INITIAL>[\n] {
  yyextra->lineno++;
  yyextra->colno = 0;
}

<INITIAL>{intcosnt} {
  yylval->iconst = strtoll(yytext, NULL, 10);
  if (errno == ERANGE) {
    integer_overflow();
    return -1;
  }
  COLNO_ADD(yyleng);
  return tok_int_constant;
}

<INITIAL>{hexconst} {
  yylval->iconst = strtoll(yytext+2, NULL, 16);
  if (errno == ERANGE) {
    integer_overflow();
    return -1;
  }
  COLNO_ADD(yyleng);
  return tok_int_constant;
}

<INITIAL>{dconst} {
  yylval->dconst = atof(yytext);
  COLNO_ADD(yyleng);
  return tok_double_constant;
}

<INITIAL>\" {
  COLNO_ADD(1);
  BEGIN(STRING_STATE);
}

<INITIAL>[\[\]{},:"]   { COLNO_ADD(1); return yytext[0]; }

<INITIAL>. {
  ERROR("error charactor");
  return -1;
}

<STRING_STATE>\" {
  BEGIN(INITIAL);
  if(yyextra->buf)
    yyextra->buf[cstr_used(yyextra->buf)] = '\0';

  yylval->s = yyextra->buf;
  yyextra->buf = NULL;
  COLNO_ADD(1);
  return str_const;
}

<STRING_STATE>{escape} {
  append_escape(yyextra, yytext[1]);
  COLNO_ADD(2);
}

<STRING_STATE>{unicode} {
  append_unicode(yyextra, yytext + 2);
  COLNO_ADD(5);
}

<STRING_STATE>{utf8_2} { 
  append_char(yyextra, yytext[0]);
  append_char(yyextra, yytext[1]);
  COLNO_ADD(2);
}

<STRING_STATE>{utf8_3} { 
  append_char(yyextra, yytext[0]);
  append_char(yyextra, yytext[1]);
  append_char(yyextra, yytext[2]);
  COLNO_ADD(3);
}

<STRING_STATE>{utf8_4} { 
  append_char(yyextra, yytext[0]);
  append_char(yyextra, yytext[1]);
  append_char(yyextra, yytext[2]);
  append_char(yyextra, yytext[3]);
  COLNO_ADD(4);
}

<STRING_STATE>[\x20-\x7E] { 
  append_char(yyextra, yytext[0]);
  COLNO_ADD(1);
}

<STRING_STATE>. {
  ERROR("error charactor in string");
  return -1;
}

%%

