#include "queue.h"
#include "rp2040.h"
#include "uart.h"

Queue s;
void initQueue(Queue *q){
  q->front = q->rear=0;
  for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i)
  {
       q->data[i]=0;
  }
}

uint8_t add(Queue *q,uint8_t n){
if(isFull(q)){
   printf("error push\n");
   while(1);
   return 0;
}
q->rear= (q->rear+1) % MAX_NUMBER_OF_TASKS;
q->data[q->rear]=n;
return 1;
}


uint8_t delete(Queue *q){
if(isEmpty(q)){
   printf("error pop\n");
   while(1);
   return 0;
}
q->front =(q->front + 1)%MAX_NUMBER_OF_TASKS;
return 1;
}

uint8_t front(Queue *q,uint8_t *val){
if(isEmpty(q)){
   printf("error top\n");
    while(1);
   return 0;
}
*val=(q->data[(q->front+1)%MAX_NUMBER_OF_TASKS]);
return 1;
}
uint8_t isEmpty(Queue *q){
  return q->rear == q->front;
}
uint8_t isFull(Queue *q){
  return (q->rear +1)%MAX_NUMBER_OF_TASKS == q->front;
}


uint8_t enqueue(Queue *q,uint8_t n){
if(isFull(q)){
    printf("(en)error push\n");
    while(1);
   return 0;
}
q->rear= (q->rear+1) % MAX_NUMBER_OF_TASKS;
q->data[q->rear]=n;
return 1;
}

uint8_t dequeue(Queue *q){
uint8_t val;
if(isEmpty(q)){
   printf("(de)error top\n");
    while(1);
   return 0;
}
val=(q->data[(q->front+1)%MAX_NUMBER_OF_TASKS]);
q->front =(q->front + 1)%MAX_NUMBER_OF_TASKS;
return val;
}
uint8_t lastqueue(Queue *q){
 if(isEmpty(q)){
   printf("error top\n");
    while(1);
   return 0;
}
return (q->data[(q->front)%MAX_NUMBER_OF_TASKS]); 
}