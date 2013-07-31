#include <stdio.h>
#include <string.h>
#include <json.h>

int main() {
  cstr c = cstr_cat_printf(NULL, "sdsd");
  printf("%d %d %s\n", cstr_used(c), cstr_len(c), c);
  cstr_free(c);
  return 0;
}