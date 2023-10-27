#ifndef AT_STACK_H
#define AT_STACK_H

#include "access_tree.h"

typedef struct {
  int capacity;
  int size;
  AccessTree** arr;
} ATStack;

ATStack createATStack();

void pushATStack(ATStack* stk, AccessTree* AT);

AccessTree* popATStack(ATStack* stk);

AccessTree* topOfATStack(ATStack* stk);

int isEmptyATStack(ATStack* stk);

void destroyATStack(ATStack* stk);

#endif
