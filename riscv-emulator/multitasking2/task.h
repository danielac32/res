#include <stdint.h>
#include <stddef.h>
#include <string.h>
 

#ifndef TASK_H_
#define TASK_H_

/**************************************************/
 /* Xinu system calls */

enum {
  XINU_NULLPROCESS = 0,
  XINU_UPTIME,
  XINU_PUTC,
  XINU_CLOSE, /* Devices */
  XINU_CONTROL,
  XINU_GETC,
  XINU_INIT,
  XINU_OPEN,
  XINU_READ,
  XINU_SEEK,
  XINU_WRITE,

  XINU_CREATE, /* Processes */ 
  XINU_CHPRIO,
  XINU_KILL,  /* Exit */
  XINU_GETPRIO,
  XINU_READY,
  XINU_RECEIVE,
  XINU_RECVCLR,
  XINU_RECVTIME,
  XINU_RESCHED_CNTL,
  XINU_RESUME,
  XINU_SEND,
  XINU_SIGNAL,
  XINU_SIGNALN,
  XINU_SLEEP,
  XINU_SLEEPMS,
  XINU_SUSPEND,
  XINU_UNSLEEP,
  XINU_WAKE,
  XINU_WAKEUP, 
  XINU_YIELD,
  XINU_RESCHED,
  
  XINU_MKBUFPOOL, /* Buffers */
  XINU_FREEBUF,
  XINU_FREEMEM,
  XINU_GETBUF,
  
  XINU_KPRINTF, /* Kernel */
  XINU_KPUTC,
  XINU_GETDEV,
  XINU_GETMEM,
  XINU_GETSTK,
  
  XINU_PTCOUNT, /* Ports */
  XINU_PTCREATE,
  XINU_PTDELETE,
  XINU_PTINIT,
  XINU_PTRCV,
  XINU_PTRESET,
  
  XINU_SEMCOUNT, /* Semaphores */
  XINU_SEMCREATE,
  XINU_SEMRESET,
  XINU_SEMDELET,

};



#define reg_t uint32_t // RISCV32: register is 32bits
// define reg_t as uint64_t // RISCV64: register is 64bits

// Saved registers for kernel context switches.
struct context {
  reg_t ra;
  reg_t sp;

  // callee-saved
  reg_t s0;
  reg_t s1;
  reg_t s2;
  reg_t s3;
  reg_t s4;
  reg_t s5;
  reg_t s6;
  reg_t s7;
  reg_t s8;
  reg_t s9;
  reg_t s10;
  reg_t s11;
  reg_t a0;
};

typedef enum {
    NONE=0,
    KILL = 1,
    RUNNING = 2,
    READY=3
} state_t;
 

struct task_t{
    int id;
    //void *stack;
    //void *sp;
    reg_t reg[15];
    int sleep;
    void (*entry)(void*);
    void *arg;
    char name[30];
    state_t state;
 };


#define MAX_TASK 10
#define STACK_SIZE 1024
//uint8_t task_stack[MAX_TASK][STACK_SIZE];
struct context ctx_os;
struct context ctx_tasks[MAX_TASK];
struct context *ctx_now;

struct task_t tasks[MAX_TASK];
struct task_t main_context;
struct task_t *current_task;

int taskTop;  // total number of task
int counter_task;

void task_init();
void printdetalle(int pid);
int task_create(char *name,void (*entry)(void*),void *arg);
void task_go(int i);
void yield();
void delay(volatile int time);



static long syscall(long num, long arg0, long arg1, long arg2)
{
  register long a7 asm("a7") = num;
  register long a0 asm("a0") = arg0;
  register long a1 asm("a1") = arg1;
  register long a2 asm("a2") = arg2;
  asm volatile ("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7));
  return a0;
}

// which hart (core) is this?
static inline reg_t r_mhartid()
{
  reg_t x;
  asm volatile("csrr %0, mhartid" : "=r" (x) );
  return x;
}

// Machine Status Register, mstatus
#define MSTATUS_MPP_MASK (3 << 11) // previous mode.
#define MSTATUS_MPP_M (3 << 11)
#define MSTATUS_MPP_S (1 << 11)
#define MSTATUS_MPP_U (0 << 11)
#define MSTATUS_MIE (1 << 3)    // machine-mode interrupt enable.

static inline reg_t r_mstatus()
{
  reg_t x;
  asm volatile("csrr %0, mstatus" : "=r" (x) );
  return x;
}

static inline void w_mstatus(reg_t x)
{
  asm volatile("csrw mstatus, %0" : : "r" (x));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void w_mepc(reg_t x)
{
  asm volatile("csrw mepc, %0" : : "r" (x));
}

static inline reg_t r_mepc()
{
  reg_t x;
  asm volatile("csrr %0, mepc" : "=r" (x));
  return x;
}

// Machine Scratch register, for early trap handler
static inline void w_mscratch(reg_t x)
{
  asm volatile("csrw mscratch, %0" : : "r" (x));
}

// Machine-mode interrupt vector
static inline void w_mtvec(reg_t x)
{
  asm volatile("csrw mtvec, %0" : : "r" (x));
}

// Machine-mode Interrupt Enable
#define MIE_MEIE (1 << 11) // external
#define MIE_MTIE (1 << 7)  // timer
#define MIE_MSIE (1 << 3)  // software

static inline reg_t r_mie()
{
  reg_t x;
  asm volatile("csrr %0, mie" : "=r" (x) );
  return x;
}

static inline void w_mie(reg_t x)
{
  asm volatile("csrw mie, %0" : : "r" (x));
}


#endif