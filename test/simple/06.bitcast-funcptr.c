// Casting function pointers to an extent where we might hit undefined
//   behaviour?  Anyways ... still worx with RaceHog :)

#include <pthread.h>
#include <stdlib.h>

static int race;

static void
f123(int* i) {
  *i = 123; // Data race.
}

static void
f312(int* i) {
  *i = 312; // Data race.
}

static void
apply(void (*fn)(int* i)) {
  (void) fn(&race);
}

static void*
thread(void* arg) {
  // void* -> void (*)(int*)
  void (*fn)(int*) = (void (*)(int*))arg;
  (void) apply(fn);
  return NULL;
}

int
main(void) {

  pthread_t t;
  // void (*)(int*) -> void*
  (void) pthread_create(&t, NULL, thread, f123);

  void (*my312)(void*) = (void (*)(void*))&f312;

  // Oops, I think I have misread the manual of `f312` ...  however, someone
  // suggested to simply put a cast in there (that seems to be _the_ magic
  // bullet when programming C):

  // void (*)(void*) -> void (*)(int*)
  (void) apply((void (*)(int*)) my312); // Phew, it works now! :)

  (void) pthread_join(t, NULL);

  return EXIT_SUCCESS;
}
