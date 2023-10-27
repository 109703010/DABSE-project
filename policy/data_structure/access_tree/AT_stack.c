#include "AT_stack.h"
#include <stdlib.h>

ATStack createATStack() {
  int init_capacity = 64;
  
  ATStack stk = {
	init_capacity,
	0,
	(AccessTree**)malloc(sizeof(AccessTree*) * init_capacity)
  };

  return stk;
}

void pushATStack(ATStack* stk, AccessTree* c) {
  if(stk -> size == stk -> capacity) {
	stk -> capacity *= 2;
	stk -> arr = (AccessTree**)realloc(stk -> arr, sizeof(AccessTree*) * stk->capacity);
  }
  stk -> arr[stk -> size++] = c;
}

AccessTree* popATStack(ATStack* stk) {
  return stk -> arr[--stk ->size];
}

AccessTree* topOfATStack(ATStack* stk) {
  return stk -> arr[0];
}

int isEmptyATStack(ATStack* stk) {
  return stk -> size == 0;
}

void destroyATStack(ATStack* stk) { 
  free(stk -> arr);
}
