/*
 * CS533 Course Project
 * Thread Queue ADT
 * queue.c
 *
 * Feel kfree to modify this file. Please thoroughly comment on
 * any changes you make.
 */

#include "riscv.h"
#include "serial.h"
#include "kmalloc.h"
#include "queue.h"
 

void thread_enqueue(struct queue * q, struct thread * t) {

  struct queue_node * temp = kmalloc(sizeof(struct queue_node));

  temp->t = t;
  temp->next = NULL;

  if(!q->head) {
    q->head = q->tail = temp;
    return;
  }

  q->tail->next = temp;
  q->tail = temp;  

}

struct thread * thread_dequeue(struct queue * q) {

  if(!q->head) {
    return NULL;
  }

  struct thread * t = q->head->t;
  struct queue_node * temp = q->head;
  q->head = q->head->next;
  kfree(temp);

  if(!q->head) {
    q->tail = NULL;
  }

  return t;

}

int is_empty(struct queue * q) {
  return !q->head;
}