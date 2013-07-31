#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <json.h>


int main(int argc, char *argv[]) {
  int i;
  char *ptr,*p ="{\"null\":[0x12]}";
  long start = (long)clock();
  int j;
  cstr k;
  json_ctx *ctx;
  
  ctx = json_ctx_new();
  j = json_parse(ctx, p, strlen(p));
  json_ctx_free(ctx);
  
  p = "[{\"1\":\"2\"}]";
  ctx = json_ctx_new();
  j = json_parse(ctx, p, strlen(p));
  json_ctx_free(ctx);

  p = "{\"a1asd\":\"\\\\\\b\\naaa\",2:0.2}";
  k = cstr_new("a1asd", 5);
  for(i = 0; i < 1; i++) {
    ctx = json_ctx_new();
    j = json_parse(ctx, p, strlen(p));
    if(!j) {
      cstr val = ((json_object*)(dict_find(ctx->rs->o.dict, k)->value))->o.str;
      printf("len:%d,%d %s\n", cstr_used(val),cstr_cap(val), val);
    }
    
    json_ctx_free(ctx);
  }

  cstr_free(k);

  p = "{\"\\u4e2d\\u6587\":\"\\u6d4b\\u8bd5\"}";
  ctx = json_ctx_new();
  j = json_parse(ctx, p, strlen(p));
  printf("unicode parse result:%d, %d\n", j, ctx->rs->o.dict->size);
  k = (cstr)ctx->rs->o.dict->head->key;
  printf("key:%s ", k);

  printf("\n");
  json_ctx_free(ctx);

  p = "{\"中文\":\"\\u6d4b\\u8bd5\"}";
  ctx = json_ctx_new();
  j = json_parse(ctx, p, strlen(p));
  printf("unicode parse result:%d, %d\n", j, ctx->rs->o.dict->size);
  k = (cstr)ctx->rs->o.dict->head->key;
  printf("key:%s ", k);
  printf("value:%s\n", json_get(ctx->rs, "中文")->o.str);
  printf("\n");
  json_ctx_free(ctx);

  p = "[1]";
  ctx = json_ctx_new();
  j = json_parse(ctx, p, strlen(p));
  printf("value:%lld \n", json_get(ctx->rs, "0")->o.i);
  json_ctx_free(ctx);  


  p = "[{ \"1\":\"\",}]";
  ctx = json_ctx_new();
  j = json_parse(ctx, p, strlen(p));
  printf("should error %d, msg:%s, token:%s\n", j, ctx->err, ctx->token);
  json_ctx_free(ctx);

  ctx = json_ctx_new();
  cstr path = cstr_create(128);
  ptr = rindex(argv[0], '/');
  if(ptr != NULL) {
    cstr_ncat(path, argv[0], ptr - argv[0]);
    cstr_cat_char(path, '/');
  }
  cstr_cat(path, "test1.json");
  FILE *f = fopen(path,"r");
  j = json_parse_file(ctx, f);
  printf("parse file result: %d\n", j);
  fclose(f);
  cstr_free(path);
  json_ctx_free(ctx);


  long end = (long)clock();
  printf("\nclock delta: %ld\n", end - start);
  return 0;
}