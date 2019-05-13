// Arrays, structs, and global data.

#include <pthread.h>
#include <stdlib.h>

#define NUM_COORDS (2)
#define NUM_XS     (5)
#define NUM_YS     (3)

typedef struct {
  long x; // Written by `main`.
  long y; // Written by `thread`.
} Coordinate;

typedef struct {
  Coordinate c[NUM_COORDS];
  int        xs[NUM_XS]; // Only read by `main` and `thread`.
  double     ys[NUM_YS]; // Written by `main` and `thread`.
} Data;

static Data data;

static void
readXs(void) {
  int buf;
  for (int x = 0; x < NUM_XS; ++x)
    buf += data.xs[x]; // Only read;  no data race.
}

static void
writeYs(double val) {
  for (int y = 0; y < NUM_YS; ++y)
    data.ys[y] = 22.22; // Write;  data race.
}

static void*
thread(void* arg) {
  for (int c = 0; c < NUM_COORDS; ++c)
    data.c[c].y = 22; // Only writes `y`;  no data race.
  (void) readXs();
  (void) writeYs(22.22);
  return NULL;
}

int
main(void) {

  pthread_t t;
  (void) pthread_create(&t, NULL, thread, NULL);

  for (int c = 0; c < NUM_COORDS; ++c)
    data.c[c].x = 33; // Only writes `x`;  no data race.

  (void) readXs();
  (void) writeYs(33.33);

  (void) pthread_join(t, NULL);

  return EXIT_SUCCESS;
}
