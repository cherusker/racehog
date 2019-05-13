#include <pthread.h>
#include <stdlib.h>

typedef struct {
  int    i;
  double d;
} FooType;

static void*
thread(void* arg) {

  FooType* foo = (FooType*) arg;

  foo->d = 99.99; // R.a.c.e.

  return NULL;
}

int
main(void) {

  FooType* foo = (FooType*) malloc(sizeof(FooType));
  if (foo == NULL)
    return EXIT_FAILURE;

  pthread_t t;
  (void) pthread_create(&t, NULL, thread, foo);

  foo->i = 77;
  foo->d = 88.88; // R.a.c.e.

  (void) pthread_join(t, NULL);

  (void) free(foo);

  return EXIT_SUCCESS;
}
