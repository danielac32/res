#include "serial.h"
#include "kmalloc.h"
#include "task.h"
#include "riscv.h"


extern void sys_switch(void *ctx_old,void *ctx_new);
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
           w_mie(~((~r_mie()) | (1 << 7)));
           myprintf("CAUSE_INTERRUPT \n");
           w_mie(r_mie() | MIE_MTIE);
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
            ctx = syscall_handler(ctx);

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

struct irq_context *syscall_handler(struct irq_context *ctx)
{
    uint32_t a0 = ctx->reg[REG_ARG0 + 0];
    uint32_t a1 = ctx->reg[REG_ARG0 + 1];
    uint32_t a2 = ctx->reg[REG_ARG0 + 2];
    uint32_t a3 = ctx->reg[REG_ARG0 + 3];
    uint32_t a4 = ctx->reg[REG_ARG0 + 4];
    uint32_t a5 = ctx->reg[REG_ARG0 + 5];
    uint32_t n  = ctx->reg[REG_ARG0 + 7];
    int      fd = (int)a0;

    if(n==XINU_SLEEP){
       tasks[counter_task].sleep = a0;
       struct task_t *ctx = current_task;
       current_task = &main_context;
       sys_switch(ctx->reg, &main_context.reg);
    }else if(n==XINU_YIELD){
       struct task_t *ctx = current_task;
       current_task = &main_context;
       sys_switch(ctx->reg, &main_context.reg);
    }else if(n==XINU_RESCHED){
       current_task = &tasks[a0];
       sys_switch(&main_context.reg, &tasks[a0].reg);
    }else if(n==XINU_CREATE){//syscall(4,name,entry,arg);
       int i=taskTop++;
       uint8_t *stk = kmalloc(STACK_SIZE);
       struct task_t *new_task = &tasks[i];
       new_task->id = i;
       new_task->sleep = 0;
       new_task->entry = (void *)a1;
       new_task->arg = (void *)a2; 
       new_task->state=READY;
       strcpy(new_task->name,(void *)a0);
       new_task->reg[0]=(reg_t) a1;
       new_task->reg[14]=(reg_t) a2;
       new_task->reg[1]=(reg_t) &stk[STACK_SIZE];
       ctx->reg[REG_ARG0 + 0]=i;
    }
    return ctx;
}