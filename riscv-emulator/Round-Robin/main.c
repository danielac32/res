 
#include "riscv.h"
#include "serial.h"
#include "kmalloc.h"
#include "schedule.h"

char stack0[4096];
char _gp[8];
//unsigned long int arr[100000]={0};



void exception_dump_ctx(struct irq_context *ctx)
{
    myprintf("Exception: PC=%8x Cause=%x Status=%8x\n", ctx->pc, ctx->cause, ctx->status);
    for (int i=0;i<NUM_GP_REG;i++)
    {
         myprintf("REG %d: %8x\n", i, ctx->reg[i]);
    }
}


void *exception(struct irq_context *ctx){
    if (ctx->cause & CAUSE_INTERRUPT)
    {
       // isr();
    }else{
       switch (ctx->cause)
        {
            case CAUSE_ECALL_U:
            case CAUSE_ECALL_S:
            case CAUSE_ECALL_M:
                ctx->pc += 4;
                break;
        }
        if (ctx->cause < CAUSE_MAX_EXC){
            
            uint32_t a0 = ctx->reg[REG_ARG0 + 0];
            uint32_t a1 = ctx->reg[REG_ARG0 + 1];
            uint32_t a2 = ctx->reg[REG_ARG0 + 2];
            uint32_t a3 = ctx->reg[REG_ARG0 + 3];
            uint32_t a4 = ctx->reg[REG_ARG0 + 4];
            uint32_t a5 = ctx->reg[REG_ARG0 + 5];
            uint32_t n  = ctx->reg[REG_ARG0 + 7];
            int      fd = (int)a0;

            myprintf("syscall: %8x\n",a0);

            //ctx = _exception_table[ctx->cause](ctx);
        }else if (ctx->cause == CAUSE_FAULT_LOAD || ctx->cause == CAUSE_FAULT_STORE ||
                 ctx->cause == CAUSE_MISALIGNED_LOAD || ctx->cause == CAUSE_MISALIGNED_STORE)
        {
            myprintf("other");
            exception_dump_ctx(ctx);
        }else
        {
            myprintf("Unhandled exception");
        }
    }

return ctx;
}
unsigned long int find_nth_fibonacci(int n){
 
   if(n==1)
	    return 1;
	if(n==0)
		return 0;

   return find_nth_fibonacci(n-1)+find_nth_fibonacci(n-2);
}
void thread_function(void *ptr){

   myprintf("New Thread Started\n");
   //int *n=(int *)malloc(sizeof(int *));
   myprintf("%d\n",*(int*)ptr);
   myprintf("Result %i\n",find_nth_fibonacci((*(int* )ptr)));
   yield();
   myprintf("New Thread Still running\n");
}


int main(){
    TIMER->VAL=10;
    TIMER->EN=0;
    kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
    kmalloc_debug();
    
  scheduler_begin();
  int n1=1,n2=3,n3=4;
  
  
  myprintf("the number is %i\n",find_nth_fibonacci(n2));
  
  thread_fork(thread_function,&n1);
  
  thread_fork(thread_function,&n2);
  
  thread_fork(thread_function,&n3);
  myprintf("Back to Main\n");
  scheduler_end();
}