#include <stdlib.h>
#include <pthread.h>

static int race;

static void
fooA(void) {
  race = 66;
}

static void
fooB(void) {
  race = 77;
}

static void(*dispatch[])() = { fooA, fooB };

static void*
thread(void* arg) {
  (void) dispatch[1]();
  return NULL;
}

int
main(void) {

  pthread_t t;

  (void) pthread_create(&t, NULL, thread, NULL);

  (void) dispatch[0]();

  (void) pthread_join(t, NULL);

  return EXIT_SUCCESS;
}
