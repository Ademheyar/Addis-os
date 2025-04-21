//#include <Addis/Libs/String/String.h>
//#include <Libs/Stdlib/Stdlib.h>
#include <Addis/Libs/Math/Math.h>

static uint32_t x = 123456789;
static uint32_t y = 362436069;
static uint32_t z = 521288629;
static uint32_t w = 88675123;

//static unsigned long int next = 1;

int rand(void) {
	uint32_t t;

	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return abs(w = w ^ (w >> 19) ^ t ^ (t >> 8));
}
/*
int rand() {
  next = next * 1103515245 + 12345;
  return (unsigned int)(next / 65536) % 32768;
}*/

void srand(unsigned int seed) {
	w ^= seed;
}

/*
void srand(unsigned int seed) {
  next = seed;
}*/


