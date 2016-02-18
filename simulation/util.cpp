#include "util.h"

// [min, max)
unsigned int rand_number(unsigned int min, unsigned int max) {
	unsigned int num = rand()%(max - min) + min;
	return num;
}