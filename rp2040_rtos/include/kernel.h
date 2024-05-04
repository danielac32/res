#include <rp2040.h>
#ifndef KERNEL_H
#define KERNEL_H
#define MAX_NUMBER_OF_TASKS 12
 
#define STACK_SIZE    4096
#define TASK_COUNT    16

#define ERR_OK      0
#define ERR_NORES   1
#define ERR_PARAMS    2
#define QUANTUM 10
typedef void (*thread_func)(void* args);
typedef unsigned int uint;

typedef struct {
  uint r4, r5, r6, r7, r8, r9, r10, r11;
  uint r0, r1, r2, r3, r12, lr, pc, psr;
} context_t;


/*
typedef struct task_t {
  uint *sp;
  uint status;
  uint param;
  int pid;
  struct task_t *next;
} task_t;
*/


void rtos_init();
void rtos_start();
uint rtos_ticks();

uint thread_start(thread_func func, void *args);
void thread_sleep(uint ticks);
void thread_terminate();

#define MAX_NUMBER_OF_TASKS 12


int create(
      int       (*procaddr)(),  /* procedure address        */
      int   ssize,      /* stack size in bytes      */
      int       priority,   /* process priority > 0     */
      char      *name,      /* name (for debugging)     */
      int   nargs,      /* number of args that follow   */
      ...
    );

void init();

void panic();
int   newpid(void);
void delay(long);


#define MAX_ARG 4
#define PR_FREE   0 /* Process table entry is unused  */
#define PR_CURR   1 /* Process is currently running   */
#define PR_READY  2 /* Process is on ready queue    */
#define PR_RECV   3 /* Process waiting for message    */
#define PR_SLEEP  4 /* Process is sleeping      */
#define PR_SUSP   5 /* Process is suspended     */
#define PR_WAIT   6 /* Process is on semaphore queue  */
#define PR_RECTIM 7 /* Process is receiving with timeout  */
#define PR_KILL     8

typedef struct task_t
{
    uint32_t  *prstkptr;  /* Saved stack pointer      */
    uint32_t  *prstkbase; /* Base of run time stack   */
    uint32_t    *sp;
    uint32_t  prstklen; /* Stack length in bytes    */
    int pargs;              
    void *parg[MAX_ARG]; 
    int *paddr;
    int pid;
    uint8_t prio;
    uint16_t state;
    uint32_t wait_ticks;
    uint32_t param;
    uint8_t name[16];
    struct task_t *next;
}task_t;


void kernel_init();
extern task_t *active_task;
//extern procent proctab[];
extern task_t task[];
uint8_t currpid;
uint8_t preempt;
uint8_t ntask;
int getpid();
extern  uint32_t  disable(void);
extern  void  enable(void);
extern  void  restore(uint32_t);
void kill(int);
void ready(int);
#endif /* KERNEL_H */
