#include <pthread.h>
#include <stdlib.h>

/* ---------------------------------------------------------------- */
/*                            Definitions                           */
/* ---------------------------------------------------------------- */

#define NUM_COORDS (2)

#define MAIN_TASK_ID   (0)
#define THREAD_TASK_ID (1)

typedef struct {
  int x;
  int y;
} Coordinates;

typedef struct {
  long id;
  Coordinates coords[NUM_COORDS];
} Shape;

typedef struct {
  pthread_mutex_t xs;
  pthread_mutex_t ys;
} ShapeLocks;

/* ---------------------------------------------------------------- */
/*                         Global Variables                         */
/* ---------------------------------------------------------------- */

static Shape* s1;
static Shape* s2;

static int counter;

/* ---------------------------------------------------------------- */
/*                         Helper Functions                         */
/* ---------------------------------------------------------------- */

static void writeXs(Shape* r) {
  (void) __sync_fetch_and_add(&counter, 1);
  for (int i = 0; i < NUM_COORDS; ++i)
    r->coords[i].x = 2;
}

static void writeYs(Shape* r) {
  ++counter;
  for (int i = 0; i < NUM_COORDS; ++i)
    r->coords[i].y = 3;
}

static void(*tasks[])() = {
  writeXs, // MAIN_TASK_ID
  writeYs  // HELPER_TASK_ID
};

/* ---------------------------------------------------------------- */
/*                              Threads                             */
/* ---------------------------------------------------------------- */

static void* create(void* arg) {
  static int x, y = 2;
  Shape* d = malloc(sizeof(Shape));
  d->id = *((long*)arg);
  x = 1;
  for (int i = 0; i < NUM_COORDS; ++i) {
    d->coords[i].x = x;
    d->coords[i].y = y;
  }
  return d;
}

static void* setup(void* arg) {
  (void) tasks[THREAD_TASK_ID](s1);
  (void) tasks[THREAD_TASK_ID](s2);
  return NULL;
}

static void* manipulate(void* arg) {
  ShapeLocks* locks = (ShapeLocks*)arg;
  for (int i = 0; i < NUM_COORDS; ++i) {
    (void) pthread_mutex_lock(&locks->ys);
    s2->coords[i].x = s2->coords[i].y;
    (void) pthread_mutex_unlock(&locks->ys);
  }
  return NULL;
}

/* ---------------------------------------------------------------- */
/*                               Main                               */
/* ---------------------------------------------------------------- */

int main(void) {

  pthread_t t1; // create s1
  pthread_t t2; // create s2
  pthread_t t3; // setup
  pthread_t t4; // manipulate

  long id1 = 1;
  long id2 = 2;

  ShapeLocks locks = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
  };

  (void) pthread_create(&t1, NULL, create, &id1);
  (void) pthread_create(&t2, NULL, create, &id2);

  (void) pthread_join(t1, (void**) &s1);
  (void) pthread_join(t2, (void**) &s2);

  (void) pthread_create(&t3, NULL, setup, NULL);

  (void) tasks[MAIN_TASK_ID](s1);
  (void) tasks[MAIN_TASK_ID](s2);

  (void) pthread_join(t3, NULL);

  (void) pthread_create(&t4, NULL, manipulate, &locks);

  for (int i = 0; i < NUM_COORDS; ++i) {
    if (pthread_mutex_trylock(&locks.xs) == 0) {
      (void) pthread_mutex_lock(&locks.ys);
      s1->coords[i].y = s2->coords[i].x;
      (void) pthread_mutex_unlock(&locks.ys);
      (void) pthread_mutex_unlock(&locks.xs);
    }
  }

  (void) pthread_join(t4, NULL);

  (void) free(s1);
  (void) free(s2);
}
