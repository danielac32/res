#include <xinu.h>
#include <prototypes.h>
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
       //kprintf("interrupt: %x\n",ctx->cause);
       // clkhandler();
 
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
            kprintf("error cpu\n");
            exception_dump_ctx(ctx);
            while(1);
        }else
        {
            kprintf("Unhandled exception");
        }
    }

return ctx;
}
