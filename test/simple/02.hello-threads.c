// Data race of local variable in two parallel threads.

#include <pthread.h>
#include <stdlib.h>

static void*
thread(void* arg) {
  int* ip = (int*) arg;
  *ip *= 3; // This write access causes 3 data races.
  return ip;
}

int
main(void) {

  pthread_t t1;
  pthread_t t2;
  
  int i = 56; // This write access happens before the first fork.

  (void) pthread_create(&t1, NULL, thread, &i);
  (void) pthread_create(&t2, NULL, thread, &i);

  int* ret; // `ret' will point to `i'.

  (void) pthread_join(t1, (void**) &ret);

  *ret += 3; // T2 is still running.

  (void) pthread_join(t2, (void**) &ret);

  return EXIT_SUCCESS;
}
