// Aggregate pointers.

#include <pthread.h>
#include <stdlib.h>

#define NUM_PTRS (3)

// Passing the array size to threads:

typedef struct {
  int* data[NUM_PTRS];
} Pointers;

static void*
thread(void* arg) {
  Pointers* ptrs = (Pointers*) arg;
  *ptrs->data[0] = 55; // Data race.
  *ptrs->data[2] = 55; // No data race.
  return NULL;
}

int
main(void) {

  Pointers ptrs;
  int      vals[NUM_PTRS];

  for (int i = 0; i < NUM_PTRS; ++i)
    // ptrs.data[i] = &vals[i]; // Has to be unrolled to avoid false positives.
    vals[i] = 0;

  ptrs.data[0] = &vals[0]; // Loop unrolling to avoid false positives.
  ptrs.data[1] = &vals[1]; // Loop unrolling to avoid false positives.
  ptrs.data[2] = &vals[2]; // Loop unrolling to avoid false positives.

  pthread_t t;
  (void) pthread_create(&t, NULL, thread, &ptrs);

  *ptrs.data[1] = 66; // No data race.

  int val1 = vals[0]; // Data race.

  (void) pthread_join(t, NULL);

  int sum = 0;
  for (int i = 0; i < NUM_PTRS; ++i)
    // Unifies all `vals` but we don't care since it's AFTER joining :)
    sum += *ptrs.data[i];

  return EXIT_SUCCESS;
}
