#include "serial.h"
#include "kmalloc.h"
#include "task.h"
#include "riscv.h"

extern void sys_switch(void *ctx_old,void *ctx_new);
void exception_dump_ctx(struct irq_context *ctx)
{
    kprintf("Exception: PC=%8x Cause=%x Status=%8x\n", ctx->pc, ctx->cause, ctx->status);
    for (int i=0;i<NUM_GP_REG;i++)
    {
         kprintf("REG %d: %8x\n", i, ctx->reg[i]);
    }
}

 


void *exception(struct irq_context *ctx){
    if (ctx->cause & CAUSE_INTERRUPT)
    {    
        
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
            kprintf("other");
            exception_dump_ctx(ctx);
        }else
        {
            kprintf("Unhandled exception");
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

    
    return ctx;
}