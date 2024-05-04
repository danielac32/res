#include <stdint.h>
#ifndef KERNEL_H
#define KERNEL_H
#define MAX_NUMBER_OF_TASKS 10

 
long tick;
 
void clkinit(void);
 
void kernel_init(uint16_t stackReserve);

int create(
      int      (*procaddr)(),  /* procedure address        */
      int   ssize,      /* stack size in bytes      */
      int       priority,   /* process priority > 0     */
      char      *name,      /* name (for debugging)     */
      int   nargs,      /* number of args that follow   */
      ...
    );

void init();

void panic();
int   newpid(void);



 
#define PR_FREE     0   /* Process table entry is unused    */
#define PR_CURR     1   /* Process is currently running     */
#define PR_READY    2   /* Process is on ready queue        */
#define PR_RECV     3   /* Process waiting for message      */
#define PR_SLEEP    4   /* Process is sleeping          */
#define PR_SUSP     5   /* Process is suspended         */
#define PR_WAIT     6   /* Process is on semaphore queue    */
#define PR_RECTIM   7   /* Process is receiving with timeout    */
#define PR_KILL   8   /* Process is receiving with timeout    */

 

#define MAX_ARG 4

struct procent
{
    uint32_t sp;
    uint32_t stackptr;
    uint16_t state;
    uint32_t stackSize;
    uint8_t *stackbase;
    int pargs; 
    uint32_t reg[17];             
    void *parg[MAX_ARG]; 
    uint32_t *paddr;
    int pid;
    uint8_t prio;
    uint16_t wait_ticks;
    uint8_t name[16];
};

extern struct procent proctab[];
uint8_t currpid;
uint32_t temp_sp;
//uint8_t preempt;
void insert(int pid);
uint8_t extract();
uint8_t getpid();
#endif /* KERNEL_H */
