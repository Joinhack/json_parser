#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <json.h>


int main(int argc, char *argv[]) {
  int i;
  char *p ="{null:[0x12]}";
  
  long start = (long)clock();
  json_object *j;
  
  j = json_parse(p, strlen(p));
  if(j)
    json_free(j);
  
  p = "[{\"asdasd\":\"adasd\"}]";
  j = json_parse(p, strlen(p));
  if(j)
    json_free(j);

  p = "{1:\"\\\\b\\naaa\",2:0.2}";
  j = json_parse(p, strlen(p));
  printf("%d\n", j->o.str.len);
  if(j)
    json_free(j);


  long end = (long)clock();
  printf("\nclock delta: %ld\n", end - start);
  return 0;
}