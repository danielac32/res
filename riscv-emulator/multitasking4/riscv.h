
#include "task.h"
typedef struct
{
	uint8_t EN;
	uint32_t VAL;
}IRQ;
#define TIMER    ( ( IRQ * )  0x4000200 )
#define cli() TIMER->EN=0
#define sei() TIMER->EN=1
uint8_t disable();
void restore(uint8_t);
extern void exception_syscall();


#define REG_RA                         1
#define REG_SP                         2
#define REG_ARG0                       10
#define REG_RET                        REG_ARG0
#define NUM_GP_REG                     32
#define NUM_CSR_REG                    3

#define CAUSE_MISALIGNED_FETCH         0
#define CAUSE_FAULT_FETCH              1
#define CAUSE_ILLEGAL_INSTRUCTION      2
#define CAUSE_BREAKPOINT               3
#define CAUSE_MISALIGNED_LOAD          4
#define CAUSE_FAULT_LOAD               5
#define CAUSE_MISALIGNED_STORE         6
#define CAUSE_FAULT_STORE              7
#define CAUSE_ECALL_U                  8
#define CAUSE_ECALL_S                  9
#define CAUSE_ECALL_M                  11
#define CAUSE_PAGE_FAULT_INST          12
#define CAUSE_PAGE_FAULT_LOAD          13
#define CAUSE_PAGE_FAULT_STORE         15
#define CAUSE_INTERRUPT                (1 << 31)
#define CAUSE_MAX_EXC      (CAUSE_PAGE_FAULT_STORE + 1)

struct irq_context
{
    uint32_t pc;
    uint32_t status;
    uint32_t cause;
    uint32_t reg[32];
};


extern void sys_switch(void *ctx_old,void *ctx_new);
void exception_dump_ctx(struct irq_context *ctx);
void *exception(struct irq_context *ctx);
struct irq_context *syscall_handler(struct irq_context *ctx);
