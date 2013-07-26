
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

%}

%option noyywrap
%option bison-bridge
%option reentrant

intcosnt           ([+-]?[0-9]+)
hexconst           ("0x"[0-9A-Za-z]+)
dconst             ([+-]?[0-9]*(\.[0-9]+)?([eE][+-]?[0-9]+)?)
whitespace         ([ \t\r\n]*)
strconst           ([\\\.a-zA-Z_0-9-]*)

%%

"false"            { yylval->bconst=0; return tok_bool_constant; }
"true"             { yylval->bconst=1; return tok_bool_constant; }
"null"             { return tok_null; }
{whitespace}       { /* do nothing */                 }

{intcosnt} {
  errno = 0;
  yylval->iconst = strtoll(yytext, NULL, 10);
  if (errno == ERANGE) {
    integer_overflow(yytext);
  }
  return tok_int_constant;
}

{hexconst} {
  errno = 0;
  yylval->iconst = strtoll(yytext+2, NULL, 16);
  if (errno == ERANGE) {
    integer_overflow(yytext);
  }
  return tok_int_constant;
}

{dconst} {
  yylval->dconst = atof(yytext);
  return tok_double_constant;
}

{strconst} {
  char *buf = malloc(yyleng + 1);
  int pos = 0;
  char *ptr, *begin = yytext;
  memset(buf, 0, yyleng + 1);
  for(;;) {
    ptr = strchr(begin, '\\');
    if(ptr == NULL) {
      memcpy(buf + pos, begin, yyleng - (begin - yytext));
      yylval->s = buf;
      return tok_str_constant;
    } else {
      if((ptr - begin) > 0)
        memcpy(buf + pos, begin, ptr - begin);
      pos += ptr - begin;
      begin = ptr;
    }
    if(++ptr - yytext == yyleng) {
      goto err;
    }
    switch(*ptr) {
    case '\\':
      buf[pos++] = '\\';
      begin+=2;
      continue;
    case '"':
      buf[pos++] = '\"';
      begin+=2;
      continue;
    case '/':
      buf[pos++] = '/';
      begin+=2;
      continue;
    case 'b':
      buf[pos++] = '\b';
      begin+=2;
      continue;
    case 'f':
      buf[pos++] = '\f';
      begin+=2;
      continue;
    case 'n':
      buf[pos++] = '\n';
      begin+=2;
      continue;
    case 'r':
      buf[pos++] = '\r';
      begin+=2;
      continue;
    case 't':
      buf[pos++] = '\t';
      begin+=2;
      continue;
    case 'u':
      
      continue;
    }
  }
  err:
    free(buf);
    return -1;
}

"{" {
  return tok_obj_start;
}

"}" {
  return tok_obj_end;
}

"[" {
  return tok_array_start;
}

"]" {
  return tok_array_end;
}

"," {
  return tok_comma;
}

\" {
  return tok_quote;
}

":" {
  return tok_colon;
}

%%

