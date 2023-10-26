#ifndef ACCESS_TREE_H
#define ACCESS_TREE_H

#include "../int/int_vector.h"

typedef struct AccessTree {
  char data;
  struct AccessTree* left;
  struct AccessTree* right;
  Vector* v;
} AccessTree;

AccessTree* createTree(char data, AccessTree* left, AccessTree* right);

void destroyTree(AccessTree* AT);

#endif
