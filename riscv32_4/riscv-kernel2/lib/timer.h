#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include "lib.h"
 
#ifndef TIMER_H
#define TIMER_H

#define TASKS 10    // number of tasks, set as many as required
jmp_buf main_context;                         // context of scheduler
jmp_buf task_context[TASKS];
static unsigned int task_no=0;
static unsigned int task_cnt=0;
void (*tsk_p[TASKS])(void);


#define syscall(num,arg0,arg1,arg2){  \
register uint32_t a7 asm("a7")=num;   \
register uint32_t a0 asm("a0")=arg0;  \
register uint32_t a1 asm("a1")=arg1;  \
register uint32_t a2 asm("a2")=arg2;  \
asm("ecall");                         \
}

//asm("mv a7, %[flag]" : : [flag] "I" (SYSCALL_YIELD));  \


#define syscall_print(str){        \
workregister1=(uint32_t)str;       \
workregister2=SYSCALL_PRINTSTR;    \
mode=1;                            \
while(mode){                       \
  asm("nop");                      \
  asm("nop");                      \
}                                  \
}


#endif

/*
asm("li sp,79997   \n\t"
    "li gp,79998   \n\t"
    "j main        \n\t"
);
 
 
#include <stdint.h>
#include <string.h>
#include <setjmp.h>
#include "lib/lib.h"
#include "lib/timer.h"
#include "lib/serial.h"
#include "lib/kmalloc.h"
 
//#define yield() if (!setjmp(task_context[task_no])) {longjmp(main_context,1);}



//#define yield2(current) if(!setjmp(current)) {longjmp(main_context,1);}
#define yield2(pid) {                                             \
    if(!setjmp(task_context[pid])) {longjmp(main_context,1);}	  \
}


#define yield(pid){                \
workregister1=pid;                 \
workregister2=SYSCALL_YIELD;              \
mode=1;                                   \
while(mode){                              \
  asm("nop");                             \
  asm("nop");                             \
}                                         \
}

#define process(){                        \
workregister2=44;                         \
mode=1;                                   \
while(mode){                              \
  asm("nop");                             \
  asm("nop");                             \
}                                         \
}                                     


//#define yield(current) if(!setjmp(current)) {longjmp(main_context,1);}

 

void task1(){
  
  while(1){
     void (*current)=task_context[task_no];
     myprintf("task1-> task: %i ,main: %i\t",current,main_context);
     yield(task_no);
  }
}

void task2(){

  while(1){
     void (*current)=task_context[task_no];
     myprintf("\ttask2-> task: %i ,main: %i\n",current,main_context);
     yield(task_no);
  }
}


void CreateTask(void (*tsk)(void)){
  for (int i = task_cnt; i < TASKS; ++i)
  {
      tsk_p[i] = 0;
  }
  if(task_cnt < TASKS){
     tsk_p[task_cnt++] = tsk;
  }
}


void InitAll(){
int   res;
char  rdy;
void (*tsk)(void);

  for (task_no=0;task_no<TASKS;task_no++){
    if (tsk_p[task_no] != 0)                // if task was registered
    {
       
      res = setjmp(main_context);
       
      if (res == 0)
      {
        tsk = tsk_p[task_no];
        tsk();                             // invoke task
      }
    }
  }
}

void run(){
 
while(1){
      if (!setjmp(main_context)) {   
           longjmp(task_context[task_no],1);            
      } 
      if (++task_no >= task_cnt) task_no = 0;
  }
}

void main(){
  enable_timer=1;
  mode=0;

  CreateTask(task1);
  CreateTask(task2);
 
  InitAll();
  syscall_print("run kernel\n");

  run();
  _exit();
 
}

*/