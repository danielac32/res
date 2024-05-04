#include "queue.h"
#include "rp2040.h"
#include "uart.h"
#include "kmalloc.h"

struct queue ready;



 void task_enqueue(struct procent *t) {
  if (!ready.first) {
    ready.first = t;
    ready.last = t;
  } else {
    ready.last->next = t;
    ready.last = t;
  }
  t->next = 0;
}

 struct procent* task_dequeue() {
  struct procent *tmp = ready.first;
  if (!tmp)
    return 0;

  ready.first = tmp->next;
  if (!ready.first)
    ready.last = 0;

  return tmp;
}


 void queue_init(queue_t *q) {
  q->first = 0;
  q->last = 0;
  ready.first = 0;
  ready.last = 0;
}

 void queue_push(queue_t *q, struct procent *t) {
  if (q->first == 0) {
    t->next = 0;
    q->first = t;
    q->last = t;
  } else {
    t->next = 0;
    q->last->next = t;
    q->last = t;
  }
}



 struct procent* queue_pop(queue_t *q) {
  struct procent *tmp = q->first;
  if (!tmp)
    return 0;

  q->first = tmp->next;
  if (!q->first)
    q->last = 0;

  return tmp;
}

 struct procent* queue_peek(queue_t *q) {
  return q->first;
}

void queue_pushsort(queue_t *q, struct procent *t) {
  struct procent *tmp;

  if (q->first == 0) {
    q->first = t;
    q->last = t;
    t->next = 0;
  } else {
    tmp = q->first;
    if (t->wait_ticks < tmp->wait_ticks) {
      t->next = tmp;
      q->first = t;
    } else {
      while (tmp->next && (t->wait_ticks > tmp->next->wait_ticks))
        tmp = tmp->next;

      if (tmp->next == 0) {
        tmp->next = t;
        t->next = 0;
        q->last = t;
      } else {
        t->next = tmp->next;
        tmp->next = t;
      }
    }
  }
}