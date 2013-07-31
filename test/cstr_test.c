#include <stdio.h>
#include <string.h>
#include <json.h>

int main() {
  cstr c = cstr_cat_printf(NULL, "sdsd");
  int i;
  printf("%d %d %s\n", cstr_used(c), cstr_cap(c), c);
  cstr_free(c);

  c = cstr_create(0);
  for(i = 1; i < 100000; i++)
    c = cstr_cat_char(c, i);

  printf("cap %d, used %d ", cstr_cap(c), cstr_used(c));
  cstr_free(c);
  return 0;
}