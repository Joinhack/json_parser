%{
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-label"

#define USE_SETTING

#include <stdio.h>
#include "setting.h"
#include "json.h"
#include "json_yy.h"

void integer_overflow(char* text) {
  fprintf(stderr, "This integer is too big: \"%s\"\n", text);
  exit(1);
}


static inline int append_char(json_ctx *ctx, char c) {
  ctx->buf = cstr_cat_char(ctx->buf, c);
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
whitespace         ([ \t\r\n]*)
escape             \\["\\/bfnrt]
unicode            \\u[0-9a-fA-F]{4}

%%

"false"            { yylval->bconst=0; return tok_bool_constant; }
"true"             { yylval->bconst=1; return tok_bool_constant; }
"null"             { return tok_null; }
{whitespace}       { /* do nothing */                 }

<INITIAL>{intcosnt} {
  yylval->iconst = strtoll(yytext, NULL, 10);
  if (errno == ERANGE) {
    integer_overflow(yytext);
  }
  return tok_int_constant;
}

<INITIAL>{hexconst} {
  yylval->iconst = strtoll(yytext+2, NULL, 16);
  if (errno == ERANGE) {
    integer_overflow(yytext);
  }
  return tok_int_constant;
}

<INITIAL>{dconst} {
  yylval->dconst = atof(yytext);
  return tok_double_constant;
}

<INITIAL>\" {
  BEGIN(STRING_STATE);
}

<INITIAL>[\[\]{},:"]   { return yytext[0]; }


<STRING_STATE>\" {
  BEGIN(INITIAL);
  yyextra->buf[cstr_used(yyextra->buf)] = '\0';
  yylval->s = yyextra->buf;
  yyextra->buf = NULL;
  return str_const;
}

<STRING_STATE>{escape} {
  append_escape(yyextra, yytext[1]);
}

<STRING_STATE>[\x20-\x7E] { 
  append_char(yyextra, yytext[0]);
}

%%

