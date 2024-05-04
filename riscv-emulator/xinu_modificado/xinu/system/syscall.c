#include <xinu.h>
#include <stdarg.h>
 
uint32 a[5];

long sys(long num, long arg0, long arg1, long arg2,...)
{
  register long a7 asm("a7") = num;
  register long a0 asm("a0") = arg0;
  register long a1 asm("a1") = arg1;
  register long a2 asm("a2") = arg2;

  va_list ap; 
  
  va_start(ap, arg2);
  for (int i = 0; i < arg2; i++) {
    //kprintf("%s\n", va_arg(ap, char*));
      a[i]=va_arg(ap, uint32);
  }
  va_end(ap);
  register long a3 asm("a3") = (uint32)a;

  asm volatile ("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a3), "r"(a7));
  return a0;
}


struct irq_context *syscall_handler(struct irq_context *ctx)
{
    uint32 a0 = ctx->reg[REG_ARG0 + 0];
    uint32 a1 = ctx->reg[REG_ARG0 + 1];
    uint32 a2 = ctx->reg[REG_ARG0 + 2];
    uint32 a3 = ctx->reg[REG_ARG0 + 3];
    uint32 a4 = ctx->reg[REG_ARG0 + 4];
    uint32 a5 = ctx->reg[REG_ARG0 + 5];
    uint32 n  = ctx->reg[REG_ARG0 + 7];
    int      fd = (int)a0;

    if(n==XINU_SLEEP){
       /*tasks[counter_task].sleep = a0;
       struct procent *ctx = current_task;
       current_task = &main_context;
       sys_switch(ctx->reg, &main_context.reg); */
       sleep(a0);
    }else if(n==XINU_YIELD){
       /*struct procent *ctx = current_task;
       current_task = &main_context;
       sys_switch(ctx->reg, &main_context.reg);*/
      yield();
    }else if(n==XINU_RESCHED){//solo en el main
       /*
       current_task = &tasks[a0];
       sys_switch(&main_context.reg, &tasks[a0].reg);
       */
       resched();
    }else if(n==XINU_CREATE){//syscall(4,name,entry,arg);
 
       ctx->reg[REG_ARG0 + 0]=create((void *)a0,(void *)a1,a2,a3);
    }
    return ctx;
}
