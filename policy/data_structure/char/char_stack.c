#include "char_stack.h"
#include <stdlib.h>

Stack createStack() {
  int init_capacity = 64;
  
  Stack stk = {
	init_capacity,
	0,
	(char*)malloc(sizeof(char) * init_capacity)
  };

  return stk;
}

void pushStack(Stack* stk, char c) {
  if(stk -> size == stk -> capacity) {
	stk -> capacity *= 2;
	stk -> arr = (char*)realloc(stk -> arr, stk -> capacity);
  }
  stk -> arr[stk -> size++] = c;
}

char popStack(Stack* stk) {
  return stk -> arr[--stk ->size];
}

char topOfStack(Stack* stk) {
  return stk -> arr[0];
}

int isEmptyStack(Stack* stk) {
  return stk -> size == 0;
}

void destroyStack(Stack* stk) {
  free(stk -> arr);
}
