#include "rp2040.h"
#include "kernel.h"
#ifndef QUEUE_H
#define QUEUE_H

 
struct queue {
  struct procent *first;
  struct procent *last;
};
extern struct queue ready;


typedef struct {
  struct procent *first;
  struct procent *last;
}queue_t;

extern  queue_t ready_queue;


void task_enqueue(struct procent *t);
struct procent* task_dequeue();

void queue_init(queue_t *q);
void queue_push(queue_t *q, struct procent *t);
void queue_pushsort(queue_t *q, struct procent *t);
struct procent* queue_pop(queue_t *q);
struct procent* queue_peek(queue_t *q);

#endif 