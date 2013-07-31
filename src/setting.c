#include <stdlib.h>
#include "setting.h"

static setting default_setting = {
  .malloc = malloc,
  .realloc = realloc,
  .free = free
};

setting *current_setting = &default_setting;

void set_setting(setting *s) {
  current_setting = s;
}
