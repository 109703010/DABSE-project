#include "access_tree.h"
#include <stdlib.h>

AccessTree* createTree(char data, AccessTree* left, AccessTree* right) {
  AccessTree* AT = (AccessTree*)malloc(sizeof(AccessTree));
  Vector* v = (Vector*)malloc(sizeof(Vector));

  *AT = (AccessTree) {
	data,
	left,
	right,
	v,
  };

  return AT;
}

void destroyTree(AccessTree* AT) {
  if(AT == NULL) {
	return;
  }

  destroyTree(AT -> left);
  destroyTree(AT -> right);

  destroyVector(AT -> v);
}
