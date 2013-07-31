#ifndef SETTING_H
#define SETTING_H

typedef struct setting {

  void* (*malloc)(size_t size);

  void* (*realloc)(void *p, size_t size);

  void (*free)(void *m);

} setting;

#ifdef __cplusplus
extern "C" {
#endif

void set_setting(setting *s);

extern setting *current_setting;

static inline setting *get_setting() {
  return current_setting;
}

static inline void* setting_malloc(size_t s) {
  setting *setting = get_setting();
  return setting->malloc(s);
}

static inline void* setting_realloc(void *p, size_t s) {
  setting *setting = get_setting();
  return setting->realloc(p, s);
}

static inline void setting_free(void *p) {
  setting *setting = get_setting();
  setting->free(p);
}

#ifdef __cplusplus
}
#endif


#endif

