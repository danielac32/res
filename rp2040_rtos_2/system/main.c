#include "resets.h"
#include "gpio.h"
#include "uart.h"
#include "xosc.h"
#include "nvic.h"
#include "systick.h"
#include <queue.h>
#include <string.h>
#include "kmalloc.h"
#include <rp2040.h>
#include "kernel.h"
#include <limits.h>

#define LED   25

 
 




//void __attribute__((naked))  pendsv_handler()  {
 //init();
//}




 
void delay2(long t) {
  while (t--)
    asm volatile ("nop");
}
 
 void core1(){
  while(1){
    printf("core 1: %d\n",getpid());
    delay2(100000);
  }
 }
 
 


 void ilde(){
  int i=0;
  uint8_t enable=0;
  while(1){
    printf("ilde : %d\n",getpid());
    if (!enable)
    {
        if(++i>5){
          enable=1;
          kill();
        }
    }
    delay2(1000);
  }
 }
 

 void blink(){

  int y=0;
  int x=0;
  int pid=create((void *)ilde,4096,1,"ilde",0);//pid=kernel_taskCreate("blink",&core1, 128, (void*)0);
  queue_push(&ready_queue, &proctab[pid]);//task_enqueue2(&proctab[pid]);//insert(pid); 
   
  while(1){
    gpio_set(25, 1);
    delay2(10000);
    gpio_set(25, 0);
    delay2(10000);
    if(!x){
       if(++ y > 10){
          if(proctab[3].state==PR_KILL){
             x=1;
             unlock(3);
          }
       }
    }
  }
 }
 

int task1(int args,char *argv[]){
while(1){
  printf("\t\ttask1\n");
  delay2(100000);
}
return 0;
}

int  main() {
disable();
meminit();
list();

kernel_init();



int pid=create(task1,4096,1,"task1",0);//pid=kernel_taskCreate("task1",&task1, 128, (void*)0);
queue_push(&ready_queue, &proctab[pid]);//task_enqueue2(&proctab[pid]);//insert(pid);
   
pid=create((void *)core1,4096,1,"shell",0);//pid=kernel_taskCreate("blink",&core1, 128, (void*)0);
queue_push(&ready_queue, &proctab[pid]);//task_enqueue2(&proctab[pid]);//insert(pid);
    
pid=create((void *)blink,4096,1,"blink",0);//pid=kernel_taskCreate("blink",&core1, 128, (void*)0);
queue_push(&ready_queue, &proctab[pid]);//task_enqueue2(&proctab[pid]);//insert(pid);



init_stack(dummy + 32);
systick_init();
systick_set(12000);
enable();
printf("running..\n");
resched();

currpid=0;
while(1){



    proctab[currpid].sp=swch(proctab[currpid].sp);
    currpid++;
    if(currpid >= ntask){
       currpid = 0;
    }
}
return 0;
}



