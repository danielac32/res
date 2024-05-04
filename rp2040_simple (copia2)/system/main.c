#include "resets.h"
#include "gpio.h"
#include "uart.h"
#include "xosc.h"
#include "nvic.h"
#include "systick.h"
#include "queue.h"
#include <string.h>
#include "kmalloc.h"

#define LED   25
void delay(int t) {
  while (t--)
    asm volatile ("nop");
}
 
void kill(){
 while(1){
    gpio_set(LED, 0);
    delay(100000);
    gpio_set(LED, 1);
    delay(10000);
  }
}
 

/* Size of our user task stacks in words */
#define PICCOLO_OS_STACK_SIZE 256

/* Number of user task */
#define PICCOLO_OS_TASK_LIMIT 10

/* Exception return behavior */
#define PICCOLO_OS_THREAD_PSP 0xFFFFFFFD
 
struct process{
  unsigned int *stkaddr;
  unsigned int *sp;
  uint8_t in_use;
};
struct process task_t[PICCOLO_OS_TASK_LIMIT];
int currpid=0;
int ntask=0;
int lastTask;
//typedef uint32_t piccolo_sleep_t;

void piccolo_yield(void);
void piccolo_task_init(void);
int piccolo_create_task(void (*pointer_to_task_function)(void));
//void piccolo_sleep(piccolo_sleep_t *pointer_to_task_function, int ticks);
void piccolo_init();
void piccolo_start();


unsigned int *__piccolo_os_create_task(unsigned int *stack,
                                       void (*start)(void));
void __piccolo_task_init(void);
unsigned int *__piccolo_pre_switch(unsigned int *stack);
void __piccolo_task_init_stack(unsigned int *stack);

//piccolo_os_internals_t piccolo_ctx;
unsigned int *__piccolo_os_create_task(unsigned int *task_stack,void (*pointer_to_task_function)(void)) {

  task_stack += PICCOLO_OS_STACK_SIZE - 17; /* End of task_stack, minus what we are about to push */
  task_stack[8] = (unsigned int) PICCOLO_OS_THREAD_PSP;
  task_stack[14] = (unsigned int) kill;
  task_stack[15] = (unsigned int) pointer_to_task_function;
  task_stack[16] = (unsigned int) 0x01000000; /* PSR Thumb bit */
 // task_stack = __piccolo_pre_switch(task_stack);

  return task_stack;
}

int piccolo_create_task(void (*pointer_to_task_function)(void)) {
  int pid=ntask;
  struct process *tsk=&task_t[pid];
  tsk->stkaddr=(unsigned int *)kmalloc(PICCOLO_OS_STACK_SIZE);
  tsk->sp=__piccolo_os_create_task(tsk->stkaddr,pointer_to_task_function);
  tsk->in_use = 1;
  ntask++;
  return pid;
}

 
void __piccolo_task_init(void) {
  unsigned int dummy[32];
  __piccolo_task_init_stack(dummy + 32);
}

void piccolo_init() {
    currpid=0;
    ntask=0;
  __piccolo_task_init();
}

void piccolo_start() {
  
  currpid=0;
  while(1){
    task_t[currpid].sp=__piccolo_pre_switch(task_t[currpid].sp);
    currpid++;
    if(currpid >= ntask){
       currpid = 0;
    }
  }

}


 
void __attribute__((naked))  svccall_handler_c(uint32_t *sp){
  asm("cpsid  i");
  uint32_t svc_number = ((char *)sp[6])[-2];
  uint32_t svc_arg1 = sp[0];
  uint32_t svc_arg2 = sp[1];
  uint32_t result = svc_arg1 + svc_arg2;

  //printf("svc Number: %d\n", svc_number);
  switch(svc_number){
      case 0:
         *((uint32_t*)0xE000ED04) = (1 << 28);
      break;
      case 1:
           
      break;
      case 5:
         printf("aquiiiiiiiiiiiiiiiiiiiiiiiiiii: %d , %d\n",svc_arg1,svc_arg2);
      break;
  }
  asm volatile("cpsie  i\n"
               "ldr r0,=0xFFFFFFFD\n"
               "bx r0\n");
 // *((uint32_t*)0xE000ED04) = (1 << 28);//PEND_SV();
}



//void __attribute__((naked))  pendsv_handler()  {
 //init();
//}


void systick_handler() {
*((uint32_t*)0xE000ED04) = (1 << 28);
}

 
 
void task1(){
  

  while(1){
     printf("task1\n");
     delay(1000);
    // piccolo_yield();
  }
}

void task2(){
 
  while(1){
    printf("\t\ttask2\n");
    delay(2000);
    //piccolo_yield();//piccolo_sleep(&t, 75);
  }
}

void task3(){
 
  while(1){
    printf("\t\t\task3\n");
    delay(3000);
    //piccolo_yield();//piccolo_sleep(&t, 75);
  }
}

void task4(){
  int i=0;
  while(1){
    printf("\t\ttask4\n");
    if(!(++i % 100)){
      asm volatile ("mov r0, %0" : : "r" (ntask));
      asm volatile ("mov r1, %0" : : "r" (currpid));
      asm volatile ("svc #5");
     // break;
    }
    delay(4000);
  }
}

void task5(){
  int i=0;
  while(1){
     
    gpio_set(LED, 0);
    delay(10000);
    gpio_set(LED, 1);
    delay(100000);
  
  }
}

 
 

int  main() {
kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
kmalloc_debug();

char *p=kmalloc(1000);
kmalloc_debug();

initQueue(&s);
systick_init();
systick_set(12000);
piccolo_init();

printf("PICCOLO OS Demo Starting...\n");

 
printf("id: %d\n",piccolo_create_task(&task1));
printf("id: %d\n",piccolo_create_task(&task2));
printf("id: %d\n",piccolo_create_task(&task3));
printf("id: %d\n",piccolo_create_task(&task4));
printf("id: %d\n",piccolo_create_task(&task5));
 
__asm__("CPSIE i");
piccolo_start();
 

while(1);
return 0;
}
