#ifndef ARRAY_LIST
#define ARRAY_LIST


struct json_object;

typedef struct arraylist {
  struct json_object **vec;
  int len;
  int cap;
} arraylist;

#ifdef __cplusplus
extern "C" {
#endif


arraylist *arraylist_new();

void arraylist_add(arraylist *l, struct json_object *o);

void arraylist_move(arraylist *d, arraylist *s);

void arraylist_free(arraylist *l);

static inline struct json_object* arraylist_get(arraylist *l, int i) {
  if(i >= l->len)
    return NULL;
  return l->vec[i];
}

#ifdef __cplusplus
}
#endif


#endif
