#include "int_vector.h"
#include <stdlib.h>

Vector createVector(int capacity) {
  Vector v = {
	capacity,
	(int*)calloc(capacity, sizeof(int))
  };
  return v;
}

void destroyVector(Vector* v) {
  free(v -> arr);
}
