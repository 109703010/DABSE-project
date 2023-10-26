#ifndef AT_QUEUE_H
#define AT_QUEUE_H

#include "access_tree.h"

typedef struct {
  int head;
  int size;
  int capacity;
  AccessTree** arr;
} ATQueue;

ATQueue createATQueue();

void pushATQueue(ATQueue* stk, AccessTree* AT);

AccessTree* popATQueue(ATQueue* stk);

int isEmptyATQueue(ATQueue* q);

void destroyATQueue(ATQueue* stk);

#endif
