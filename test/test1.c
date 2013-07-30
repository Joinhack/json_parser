#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <json.h>


int main(int argc, char *argv[]) {
  int i;
  char *p ="{\"null\":[0x12]}";
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
      printf("len:%d,%d %s", cstr_used(val),cstr_len(val), val);
    }
    
    json_ctx_free(ctx);
  }

  long end = (long)clock();
  printf("\nclock delta: %ld\n", end - start);
  return 0;
}