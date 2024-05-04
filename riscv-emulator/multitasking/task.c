
#include "serial.h"
#include "kmalloc.h"
#include "context.h"
#include "task.h"
#include <string.h>

extern void sys_switch(void *ctx_old,void *ctx_new);
void printdetalle(int pid){
myprintf("name: %s,arg: %s,ra: %8x,entry: %8x\n",tasks[pid].name,ctx_tasks[pid].a0,ctx_tasks[pid].ra,tasks[pid].entry);
}
 
 
void task_init(){
 taskTop =0;
 counter_task =0;
}
// create a new task
int task_create(char *name,void (*entry)(void*),void *arg)
{
  return syscall(XINU_CREATE,(long)name,(long)entry,(long)arg);//i;
}

// switch to task[i]
void task_go(int i) {
  //ctx_now = &ctx_tasks[i];
  //sys_switch(&ctx_os, &ctx_tasks[i]);
  /*
  current_task = &tasks[i];
  sys_switch(&main_context.reg, &tasks[i].reg);*/
  syscall(XINU_RESCHED,i,0,0);
}

// switch back to os
void yield() {
  /*struct context *ctx = ctx_now;
  ctx_now = &ctx_os;
  sys_switch(ctx, &ctx_os);*/
  syscall(XINU_YIELD,0,0,0);
}


void delay(volatile int time)
{
     syscall(XINU_SLEEP,time,0,0);
}

