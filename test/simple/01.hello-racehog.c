// Reference parameters.

#include <stdlib.h>

static int
foo(int a,
    int b) {
  return a + b;
}

static int
foo_ref(const int* a,
        const int* b) {
  return *a + *b;
}

int
main(void) {
  int a  = 8;
  int b  = 7;
  int x1 = foo(a, b);
  int x2 = foo_ref(&a, &b);
  return EXIT_SUCCESS;
}
