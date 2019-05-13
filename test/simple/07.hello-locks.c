// Locking 101:  Oops, I forgot a lock somwhere ...

#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t lock_x = PTHREAD_MUTEX_INITIALIZER; // Protects `x`.
static pthread_mutex_t lock_y = PTHREAD_MUTEX_INITIALIZER; // Protects `y`.
static pthread_mutex_t lock_z = PTHREAD_MUTEX_INITIALIZER; // Protects `z`.

static int x;
static int y;
static int z;

static void*
thread(void* arg) {
  (void) pthread_mutex_lock(&lock_x);
  x = 55;
  y = 66; // "Accidentally" protected, thanks to overprotection in `main`.
  z = 77; // Data race.
  (void) pthread_mutex_unlock(&lock_x);
  (void) pthread_mutex_lock(&lock_y);
  y = 88; // Protected as intended.
  (void) pthread_mutex_unlock(&lock_y);
  return NULL;
}

int
main(void) {

  pthread_t t;
  (void) pthread_create(&t, NULL, thread, NULL);

  if (pthread_mutex_trylock(&lock_x) == 0) {
    (void) pthread_mutex_lock(&lock_y);
    x = 33;
    y = 44; // Overly protects `y` with `lock_x` and `lock_y`.
    (void) pthread_mutex_unlock(&lock_y);
    (void) pthread_mutex_unlock(&lock_x);
  }

  (void) pthread_mutex_lock(&lock_z);
  z = 55; // Sadly a data race;  `thread` was not implemented carefully enough.
  (void) pthread_mutex_unlock(&lock_z);

  (void) pthread_join(t, NULL);

  return EXIT_SUCCESS;
}
