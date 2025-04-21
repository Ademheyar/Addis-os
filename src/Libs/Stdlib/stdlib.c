#include <Libs/Stdlib/Stdlib.h>



int rand_range(int lower, int upper) {
  return (rand() % (upper - lower + 1)) + lower;
}

int abs(int n) {
  return n < 0 ? -n : n;
}
