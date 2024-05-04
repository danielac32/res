
#include "task.h"
 
#include <stdint.h>

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



#define NCPU 8             // maximum number of CPUs
#define CLINT 0x2000000
#define CLINT_MTIMECMP(hartid) (CLINT + 0x4000 + 4*(hartid))
#define CLINT_MTIME (CLINT + 0xBFF8) // cycles since boot.

// which hart (core) is this?
static inline uint32_t r_mhartid()
{
  uint32_t x;
  asm volatile("csrr %0, mhartid" : "=r" (x) );
  return x;
}

// Machine Status Register, mstatus
#define MSTATUS_MPP_MASK (3 << 11) // previous mode.
#define MSTATUS_MPP_M (3 << 11)
#define MSTATUS_MPP_S (1 << 11)
#define MSTATUS_MPP_U (0 << 11)
#define MSTATUS_MIE (1 << 3)    // machine-mode interrupt enable.

static inline uint32_t r_mstatus()
{
  uint32_t x;
  asm volatile("csrr %0, mstatus" : "=r" (x) );
  return x;
}

static inline void w_mstatus(uint32_t x)
{
  asm volatile("csrw mstatus, %0" : : "r" (x));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void w_mepc(uint32_t x)
{
  asm volatile("csrw mepc, %0" : : "r" (x));
}

static inline uint32_t r_mepc()
{
  uint32_t x;
  asm volatile("csrr %0, mepc" : "=r" (x));
  return x;
}

// Machine Scratch register, for early trap handler
static inline void w_mscratch(uint32_t x)
{
  asm volatile("csrw mscratch, %0" : : "r" (x));
}

// Machine-mode interrupt vector
static inline void w_mtvec(uint32_t x)
{
  asm volatile("csrw mtvec, %0" : : "r" (x));
}

// Machine-mode Interrupt Enable
#define MIE_MEIE (1 << 11) // external
#define MIE_MTIE (1 << 7)  // timer
#define MIE_MSIE (1 << 3)  // software

static inline uint32_t r_mie()
{
  uint32_t x;
  asm volatile("csrr %0, mie" : "=r" (x) );
  return x;
}

static inline void w_mie(uint32_t x)
{
  asm volatile("csrw mie, %0" : : "r" (x));
}
