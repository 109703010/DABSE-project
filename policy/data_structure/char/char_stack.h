#ifndef CHAR_STACK_H
#define CHAR_STACK_H

typedef struct {
  int capacity;
  int size;
  char* arr;
} Stack;

Stack createStack();

void pushStack(Stack* stk, char c);

char popStack(Stack* stk);

char topOfStack(Stack* stk);

int isEmptyStack(Stack* stk);

void destroyStack(Stack* stk);

#endif
