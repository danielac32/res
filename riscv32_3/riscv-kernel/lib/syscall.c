#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "syscall.h"
#include "lib.h"
#include "task.h"
#include "serial.h"

void syscall(uint32_t num,uint32_t arg0,uint32_t arg1,uint32_t arg2){
register uint32_t a7 asm("a7")=num;
register uint32_t a0 asm("a0")=arg0;
register uint32_t a1 asm("a1")=arg1;
register uint32_t a2 asm("a2")=arg2;
asm("ecall");
}

void* kmalloc(long size){
workregister1=size;
workregister3=0;
workregister2=ALLOC;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return (void*)workregister3;
}

void mfree(void *p){
workregister1=(uint32_t)p;
workregister2=FREE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

uint8_t SerialRead(){
workregister1=0;
workregister2=SERIALREAD;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister1;
}

void fprint(float f){
workregister1=*(uint32_t *)&f;     
workregister2=FPRINT; 
mode=1;                 
while(mode){            
  asm("nop");           
  asm("nop");           
}
}

int SerialAvailable(){
workregister1=0;
workregister2=SERIALAVAILABLE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister1;
}

void exit_cpu(){
workregister1=0;
workregister2=EXITCPU;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}


void kputc(char ch){
workregister1=(uint32_t)ch;
workregister2=PRINTCHAR;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
workregister1=0;
}

void yield(long long int point,long long int point2){
workregister1=point;
workregister3=point2;
workregister2=YIELD;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

void kill_task(int pid){
workregister1=pid;
workregister2=EXITTASK;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}
void run_task(int pid){
workregister1=pid;
workregister2=RUN;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

void exit_(){
workregister1=0;
workregister2=EXITTASK;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

void closefile(){
workregister1=0;
workregister2=CLOSEFILE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
workregister1=0;
}

int openfile(uint32_t str){
workregister1=str;
workregister2=OPENFILE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister2;
}
char readfile(){
workregister1=0;
workregister3=0;
workregister2=READFILE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return (char)workregister3;
}

void readbuff(void *p,uint32_t si){
workregister1=(uint32_t)p;
workregister2=READFILEBUFF;
workregister3=si;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

uint32_t filesize(){
workregister1=0;
workregister3=0;
workregister2=FILESIZE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister3;
}

uint32_t get_time(){
workregister3=0;
workregister2=TIMER;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister3;
}

void CreateTask(void (*tsk)(void)){
  if(task_cnt < OOS_MAX_TASKS){
     tsk_p[task_cnt++] = tsk;
  }
}

void RunSystems(){
  // --------------------------
  // Init tasks
  // --------------------------
  for (task_no=0;task_no<OOS_MAX_TASKS;task_no++){
    if (tsk_p[task_no] != 0)                // if task was registered
    {
       
      res = setjmp(main_context);
      
      //myprintf("res: %i , task: %i , point: %i , context: %i\n",res,task_no,tsk_p[task_no],main_context);
       
      if (res == 0)
      {
        tsk = tsk_p[task_no];
        tsk();                             // invoke task
      }
      
    }
  }
  // --------------------------
  // Scheduler loop
  // --------------------------
  task_no = 0;
  while(1){
       
      if (!setjmp(main_context)) {   
           longjmp(task_context[task_no],1);            
      } 

      if (++task_no >= OOS_MAX_TASKS)        // next task
      {
           task_no = 0;                          // round-robin
      }
       
  }
}


void calls(){
  if(mode==1){
     switch(workregister2){
            case TIMER:
                 workregister3=timer;//counter_timer;
            break;
            case CLOSEFILE:
                 syscall(CLOSEFILE,0,0,0);
            break;
            case FILESIZE:
                 syscall(FILESIZE,0,0,0);
            break;
            case READFILE:
                 syscall(READFILE,0,0,0);
            break;
            case READFILEBUFF:
                 syscall(READFILEBUFF,workregister1,workregister3,0);
            break;
            case OPENFILE:
                 syscall(OPENFILE,workregister1,0,0);
            break;
            case EXITCPU:
                 syscall(EXITCPU,0,0,0);
            break;
            case SERIALAVAILABLE:
                 workregister1=keyAvailable;
            break;
            case SERIALREAD:
                 workregister1=(uint8_t)keyRead;
                 ///putc_((char)workregister1);
            break;
            case PRINTCHAR://print char
                 syscall(PRINTCHAR,workregister1,0,0);
                 //putc_((char)workregister1);
            break;
            case PRINTSTR://print str
                 
                 syscall(PRINTSTR,workregister1,0,0);
                 //sprintf(buff,"point: %lx\n",workregister1);
                 //print(buff);
            break;
            case YIELD:
                 if (!setjmp((void*)workregister1)) {   
                      longjmp((void*)workregister3,1);            
                 }
            break;
            case EXITTASK:// exit
                 //syscall(EXITTASK,workregister1,0,0);  
                 if((int)workregister1>0 && processlist[(int)workregister1].attached==1){
                     processlist[(int)workregister1].attached = 0; 
                     processlist[(int)workregister1].pid = -1;
                     strcpy(processlist[(int)workregister1].name, "");
                     processlist[(int)workregister1].prio = -1;
                     processlist[(int)workregister1].function = 0;
                     core.task--;
                     fr((chunk_t*)mem,processlist[(int)workregister1].p); 
                     
                     //info((chunk_t*)mem);
                 }
            break;
            case RUN:
                 if((int)workregister1>0){
                     processlist[(int)workregister1].attached=1;
                 }
            break;
            case ALLOC:
                 workregister3=(uint32_t)do_malloc((chunk_t*)__buffer__,workregister1);
            break;
            case FREE:
                 do_free((chunk_t*)__buffer__,(void*)workregister1);  
            break;
            case KINIT:

            break;
            case FPRINT:
                 {

                  float val2=*(float *)&workregister1;
                  ftoa(val2,buff,2);
                  myprintf("%s",buff);
                 }
            break;
     }
     mode=0;
  }
  /*
        for (int i=0; i<OOS_MAX_TASKS; i++){
            if (task_delay[i]>0){
                task_delay[i]--;
            }
        }*/
 }
