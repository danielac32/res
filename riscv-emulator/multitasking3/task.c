
#include "serial.h"
#include "kmalloc.h"
#include "context.h"
#include "task.h"
#include "riscv.h"
#include <string.h>

extern void sys_switch(void *ctx_old,void *ctx_new);
 extern void sys_switch2(void *ctx_old,void *ctx_new,uint32_t ret);

 
 
void task_init(){
 taskTop =0;
 counter_task =0;
 preempt = QUANTUM;
}
// create a new task
int task_create(char *name,void (*entry)(void*),void *arg)
{
       int i=taskTop++;
       uint8_t *stk = kmalloc(STACK_SIZE);
       struct task_t *new_task = &tasks[i];
       new_task->id = i;
       new_task->sleep = 0;
       new_task->entry = (void *)entry;
       new_task->arg = (void *)arg; 
       new_task->state=READY;
       strcpy(new_task->name,(void *)name);
       new_task->reg[0]=(uint32_t) entry;
       new_task->reg[14]=(uint32_t) arg;
       new_task->reg[1]=(uint32_t) &stk[STACK_SIZE];
}

// switch to task[i]
void task_go(int i) {
  uint8_t mask=disable();
  current_task = &tasks[i];
  sys_switch(&main_context.reg, &tasks[i].reg);
  restore(mask);
}

// switch back to os
void yield() {
 struct task_t *ctx = current_task;
 current_task = &main_context;
 sys_switch(ctx->reg, &main_context.reg);
}


void delay(long time)
{ 
       tasks[counter_task].sleep = time;
       struct task_t *ctx = current_task;
       current_task = &main_context;
       sys_switch(ctx->reg, &main_context.reg);
}


void clock(){
task_go(counter_task);
counter_task = (counter_task + 1) % taskTop;
}


void __attribute__ ((section(".interrupt"))) isr(){  
 
     struct task_t *ctx = current_task;
     current_task = &main_context;
     
      // yield();
    //kprintf("timer\n");
    
 
}