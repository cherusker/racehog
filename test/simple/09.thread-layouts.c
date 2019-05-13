#include <pthread.h>
#include <stdlib.h>

#define UNUSED(X) do { (void) (X); } while (0)

static int l1;
static int l2;
static int l3;
static int l4;

/**
 * Layout 1 "Sequential":
 *
 * |
 * 1
 * |--.
 * 2  3
 * |--'
 * 4
 * |--.
 * 5  6
 * |--'
 * 7
 * |
 */

static void*
l1_thread_1(void* arg) {
  UNUSED(arg);
  l1 = 66;
  return NULL;
}

static void*
l1_thread_2(void* arg) {
  UNUSED(arg);
  l1 = 77;
  return NULL;
}

static void
l1_sequential(void) {
  pthread_t t1;
  pthread_t t2;
  l1 = 55;
  (void) pthread_create(&t1, NULL, l1_thread_1, NULL);
  l1 = 55;
  (void) pthread_join(t1, NULL);
  (void) pthread_create(&t2, NULL, l1_thread_2, NULL);
  l1 = 55;
  (void) pthread_join(t2, NULL);
  l1 = 55;
}

/**
 * Layout 2 "Containing":
 *
 * |
 * 1
 * |-----.
 * 2     |
 * |--.  |
 * 3  4  5
 * |__'  |
 * 6     |
 * |_____'
 * 7
 * |
 */

static void*
l2_thread_1(void* arg) {
  UNUSED(arg);
  l2 = 66;
  return NULL;
}

static void*
l2_thread_2(void* arg) {
  UNUSED(arg);
  l2 = 77;
  return NULL;
}

static void
l2_containing(void) {
  pthread_t t1;
  pthread_t t2;
  l2 = 55;
  (void) pthread_create(&t1, NULL, l2_thread_1, NULL);
  l2 = 55;
  (void) pthread_create(&t2, NULL, l2_thread_2, NULL);
  l2 = 55;
  (void) pthread_join(t2, NULL);
  l2 = 55;
  (void) pthread_join(t1, NULL);
  l2 = 55;
}

/**
 * Layout 3 "Interleaving":
 *
 * |
 * 1
 * |--.
 * 2  3
 * |-----.
 * 4  |  5
 * |--'  |
 * 6     |
 * |-----'
 * 7
 * |
 */

static void*
l3_thread_1(void* arg) {
  UNUSED(arg);
  l3 = 66;
  return NULL;
}

static void*
l3_thread_2(void* arg) {
  UNUSED(arg);
  l3 = 77;
  return NULL;
}

static void
l3_interleaving(void) {
  pthread_t t1;
  pthread_t t2;
  l3 = 55;
  (void) pthread_create(&t1, NULL, l3_thread_1, NULL);
  l3 = 55;
  (void) pthread_create(&t2, NULL, l3_thread_2, NULL);
  l3 = 55;
  (void) pthread_join(t1, NULL);
  l3 = 55;
  (void) pthread_join(t2, NULL);
  l3 = 55;
}

/**
 * Layout 4 "Subthreading":
 *
 * |
 * 1
 * |--.
 * |  2
 * |  |--.
 * 3  4  5
 * |  |--'
 * |  6
 * |__'
 * 7
 * |
 */

static void*
l4_thread_2(void* arg) {
  UNUSED(arg);
  l4 = 77;
  return NULL;
}

static void*
l4_thread_1(void* arg) {
  UNUSED(arg);
  pthread_t t;
  l4 = 66;
  (void) pthread_create(&t, NULL, l4_thread_2, NULL);
  l4 = 66;
  (void) pthread_join(t, NULL);
  l4 = 66;
  return NULL;
}

static void
l4_subthreading(void) {
  pthread_t t;
  l4 = 55;
  (void) pthread_create(&t, NULL, l4_thread_1, NULL);
  l4 = 55;
  (void) pthread_join(t, NULL);
  l4 = 55;
}

int
main(void) {
  (void) l1_sequential();
  (void) l2_containing();
  (void) l3_interleaving();
  (void) l4_subthreading();
}
