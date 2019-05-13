#include <stdlib.h>
#include <pthread.h>

static void*
thread(void* arg) {
  static int foo;
  foo = 22;
  return NULL;
}

int
main(void) {
  pthread_t t1;
  pthread_t t2;

  (void) pthread_create(&t1, NULL, thread, NULL);
  (void) pthread_create(&t2, NULL, thread, NULL);

  (void) pthread_join(t2, NULL);
  (void) pthread_join(t1, NULL);

  return EXIT_SUCCESS;
}
