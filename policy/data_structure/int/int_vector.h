#ifndef INT_VECTOR_H
#define INT_VECTOR_H

typedef struct {
  int capacity;
  int* arr;
} Vector;

Vector createVector(int capacity);

void destroyVector(Vector* v);

#endif
