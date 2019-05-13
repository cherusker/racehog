// https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html

#include <pthread.h>
#include <stdlib.h>

static int rmw    = 0; // read, modify, write
static int cmpxch = 0; // compare + exchange

static void*
thread(void* arg) {

  __sync_fetch_and_sub(&rmw, 20);              // Data race.
  __sync_val_compare_and_swap(&cmpxch, 0, 33); // Data race.

  return NULL;
}

int
main(void) {

  int rmw_add    = 450;
  int cmpxch_old = 0;
  int cmpxch_new = 22;

  pthread_t t;

  (void) pthread_create(&t, NULL, thread, NULL);

  __sync_fetch_and_add(&rmw, rmw_add);
  __sync_val_compare_and_swap(&cmpxch, cmpxch_old, cmpxch_new);

  rmw    = 0; // Data race.
  cmpxch = 0; // Data race.

  (void) pthread_join(t, NULL);

  return EXIT_SUCCESS;
}
