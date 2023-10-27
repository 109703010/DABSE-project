#include "AT_queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ATQueue createATQueue() {
  int init_capcity = 32;

  ATQueue q = {
	0,
	0,
	init_capcity,
	(AccessTree**)malloc(sizeof(AccessTree*) * init_capcity)
  };

  return q;
}

void pushATQueue(ATQueue* q, AccessTree* AT) {
  if(q->size == q -> capacity) {
	AccessTree** newQueue = (AccessTree**)malloc(sizeof(AccessTree*) * q->capacity * 2);
	memcpy(newQueue, q->arr + q->head, q->capacity - q->head);
	memcpy(newQueue, q -> arr, q -> head);
	free(q -> arr);
	q -> arr = newQueue;
	q -> head = 0;
	q -> capacity *= 2;
  }
  int tail = (q->head + q->size) % q->capacity;
  q -> arr[tail] = AT;
  ++q -> size;
}

AccessTree* popATQueue(ATQueue* q) {
  int head = q -> head;
  q->head = (q->head + 1) % q->capacity;
  --q -> size;
  return q -> arr[head];
}

int isEmptyATQueue(ATQueue* q) {
  return q->size == 0;
}

void destroyATQueue(ATQueue* q) {
  q-> head = 0;
  q -> size = 0;
  q -> capacity = 0;
  free(q -> arr);
}
