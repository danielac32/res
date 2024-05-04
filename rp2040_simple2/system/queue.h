#include "rp2040.h"
#ifndef QUEUE_H
#define QUEUE_H

typedef struct 
{
    uint8_t data[MAX_NUMBER_OF_TASKS];
    uint8_t front,rear;
}Queue;

void initQueue(Queue *);
uint8_t add(Queue *,uint8_t);
uint8_t delete(Queue *);
uint8_t front(Queue *,uint8_t *);
uint8_t isEmpty(Queue *);
uint8_t isFull(Queue *);
uint8_t enqueue(Queue *,uint8_t);
uint8_t dequeue(Queue *);
uint8_t lastqueue(Queue *);
//void ready(int);
//void insert(int);

extern  Queue s;

#endif 