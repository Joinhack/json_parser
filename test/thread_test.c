#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <json.h>

void *parse(void *data) {
  json_ctx *ctx;
  char *ptr;
  char *p = (char*)data;
  int j;
  ctx = json_ctx_new();
  cstr path = cstr_create(128);
  ptr = rindex(p, '/');
  if(ptr != NULL) {
    cstr_ncat(path, p, ptr - p);
    cstr_cat_char(path, '/');
  }
  cstr_cat(path, "test1.json");
  FILE *f = fopen(path,"r");
  j = json_parse_file(ctx, f);
  printf("parse file result: %d\n", j);
  if(j)
    printf("error:%s, line:%d col:%d token:%s\n", ctx->err, ctx->lineno, ctx->colno ,ctx->token);
  fclose(f);
  cstr_free(path);
  json_ctx_free(ctx);
  return NULL;
}

int main(int args, char *argv[]) {
  int i, s = 10;
  pthread_t thr_ids[s];
  void *rs;
  for(i = 0; i < s; i++) {
    pthread_create(&thr_ids[i], NULL, parse, argv[0]);
  }
  for(i = 0; i < s; i++) {
    pthread_join(thr_ids[i], &rs);
  }
  return 0;
}