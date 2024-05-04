


#include <stdint.h>
#include <string.h>
#include "riscv.h"
#include "serial.h"
#include "kmalloc.h"
#include "task.h"
#include "queue.h"
#include "kernel.h"
Queue q;
#define curr_mode (*(volatile uint8_t *)0x4000500)

static void dly(long t){
  for (long i = 0; i < t; ++i)
  {
       yield();//asm("nop");
  }
}


 
/*
int task1(int args,char *argv[]){
   uint8_t mask=disable();

    for (int i = 0; i < args; ++i)
    {
         kprintf("%s ",argv[i]);
    }
    kprintf("\n");
    restore(mask);
  while(1){
      kprintf("task1\n");
  }
}

void core1(){
  
  while(1){
      kprintf("core1\n");
  }
}

void core2(){
  
  while(1){
      kprintf("core2\n");
  }
}
*/
void user_task2(void *p)
{
  
  int i=0;
  //yield();
  while (1) {
    kprintf("              Task2: %d\n",i++);
    //delay(2000);
  }
}
void user_task1(void *p)
{
  
  int i=0;
  //yield();
  while (1) {
    kprintf("Task1: %d,%d,%d\n",i++,TIMER->EN,curr_mode);
//    delay(1000);
  }
}
int main()
{
    kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
    kmalloc_debug();
    initQueue(&s);
    initQueue(&q);
  
     
    TIMER->VAL=512;
    TIMER->EN = 0;

    kprintf("ok\n");
    int pid;
    task_init();
    int pid1=task_create("core0",&user_task1,"veronica");
    int pid2=task_create("core1",&user_task2,"daniel");

    /*pid=create(task1,512,1,"task1",2,"prueba","argumento");//pid=kernel_taskCreate("task1",&task1, 128, (void*)0);
    insert(pid);
   
    pid=create((void *)core1,512,1,"shell",0);//pid=kernel_taskCreate("blink",&core1, 128, (void*)0);
    insert(pid);
    
    pid=create((void *)core2,512,1,"blink",0);//pid=kernel_taskCreate("blink",&core1, 128, (void*)0);
    insert(pid);
    kmalloc_debug();
    sei();*/

    //asm volatile("csrw mepc, %0" : : "r" (proctab[pid].paddr));
    //asm volatile("mret");
    TIMER->EN=1;
  while (1) {
       if(tasks[counter_task].sleep == 0 && tasks[counter_task].state==READY){
        task_go(counter_task);
    }
    counter_task = (counter_task + 1) % taskTop; // Round Robin Scheduling
 
    for(int i = 0; i < taskTop; ++i) {
        if(tasks[i].sleep > 0) {
           --tasks[i].sleep;
        }
    }
  }

 
    
   
   return 0;
}