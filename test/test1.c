#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <json.h>


int main(int argc, char *argv[]) {
  int i;
  char *p ="{null:[0x12]}";
  long start = (long)clock();
  int j;
  json_ctx ctx = {0};
  
  j = json_parse(&ctx, p, strlen(p));
  if(!j)
    json_free(ctx.rs);
  
  p = "[{\"asdasd\":\"adasd\"}]";
  j = json_parse(&ctx, p, strlen(p));
  if(!j)
    json_free(ctx.rs);

  p = "{1:\"\\\\b\\naaa\",2:0.2}";
  j = json_parse(&ctx, p, strlen(p));
  printf("%d\n", ctx.rs->o.str.len);
  if(!j)
    json_free(ctx.rs);

  long end = (long)clock();
  printf("\nclock delta: %ld\n", end - start);
  return 0;
}