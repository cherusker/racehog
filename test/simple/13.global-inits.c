#include <stdlib.h>
#include <pthread.h>

typedef struct {
  void(*func)(int*);
  int* val;
} Pair;

static void
set55(int* ptr) {
  *ptr = 55;
}

static void(*func_a)(int*) = set55;
static void(*func_b)(int*) = set55;

static int val_a;
static int val_b;

static Pair p[] = {
  { set55, &val_a },
  { set55, &val_b }
};

static void*
thread(void* arg) {
  p[0].func(p[0].val);
  p[1].func(p[1].val);
  return NULL;
}

int
main(void) {

  pthread_t t;
  (void) pthread_create(&t, NULL, thread, NULL);

  (void) func_a(&val_a);
  (void) func_b(&val_b);

  (void) pthread_join(t, NULL);
  
  return EXIT_SUCCESS;
}
