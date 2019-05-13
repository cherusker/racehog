// Simple, unambiguous function pointers.

#include <stdlib.h>

static int
add(int x,
    int y) {
  return x + y;
}

static int
sub(int x,
    int y) {
  return x - y;
}

static int
mul(int x,
    int y) {
  return x * y;
}
     
static int
(*getMul(void))(int, int) {
  return mul;
}

static int
apply(int (*fn)(int, int),
      int   x,
      int   y) {
  return fn(x, y);
}

int
main(void) {

  int x = 5;
  int y = 3;

  int (*sub_fn)(int, int) = &sub;

  int res_add = apply(add,      x, y); // Direct pointer.
  int res_sub = apply(sub_fn,   x, y); // Pointer via variable.
  int res_mul = apply(getMul(), x, y); // Pointer via function call.

  return EXIT_SUCCESS;
}
