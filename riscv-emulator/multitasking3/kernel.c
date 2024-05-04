#include "kernel.h"
#include "serial.h"
#include "kmalloc.h"
#include "queue.h"
#include "riscv.h"


struct procent proctab[MAX_NUMBER_OF_TASKS];
 
void insert(int pid){
  uint8_t mask=disable();
  add(&s,pid);
  restore(mask);
}

uint8_t extract(){
return dequeue(&s);
}


uint8_t disable(){
uint8_t temp=(uint8_t)TIMER->EN;
TIMER->EN=0;
return temp;
}
void restore(uint8_t x){
TIMER->EN=x;
}

int   newpid(void)
{
    uint32_t  i;          /* iterate through all processes*/
//  static  pid32 nextpid = 1;  /* position in table to try or  */
     int nextpid = 0;  /* position in table to try or  */
                    /*  one beyond end of table */

    for (i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
        nextpid %= MAX_NUMBER_OF_TASKS;   /* wrap around to beginning */
       // if (kernel.pid[i].state == PR_FREE) {
        if(proctab[i].state == PR_FREE) {
            return nextpid++;
        } else {
            nextpid++;
        }
    }
    return (int) -1;
}


void kill(){
     cli();
     proctab[currpid].state=PR_KILL;
     //isr();
}

void kernel_init(uint16_t stackReserve)
{
    // set up timer 1
    cli();
    TIMER->VAL=1000;
    for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i)
    {
      //kernel.pid[i].state=PR_FREE;
      proctab[i].state=PR_FREE;
    }
    currpid=0;
   // preempt=1;
 
    int pid=newpid();
    struct procent *task=&proctab[pid];
    uint8_t* stack = (uint8_t*)kmalloc(stackReserve);//kernel.stackBottom;
    if(stack==SYSERR){
       kprintf("error malloc %d\n",pid);
       panic();
    }
    task->stackptr=(uint32_t)stack;
    proctab[pid].state=PR_CURR;
    proctab[pid].stackSize=stackReserve;
  
}


int create(
      int       (*procaddr)(),  /* procedure address        */
      int   ssize,      /* stack size in bytes      */
      int       priority,   /* process priority > 0     */
      char      *name,      /* name (for debugging)     */
      int   nargs,      /* number of args that follow   */
      ...
    ){
uint8_t mask=disable();
int pid=newpid();//getNextPID();
struct  procent *prptr;

uint8_t *saddr=kmalloc(ssize);//newStack;; 
if(saddr==SYSERR){
   kprintf("error malloc %s\n",name);
   while(1);
}
prptr = &proctab[pid];
prptr->state = PR_SUSP;
prptr->wait_ticks=0;
prptr->stackbase=saddr;
prptr->stackSize=ssize;
strcpy(prptr->name,name);
prptr->pargs = nargs;
prptr->paddr = (int *)procaddr;
prptr->pid=pid;
int * a = (int *)(&nargs + 1);
for (int i = 0; i < nargs; i++) {
    prptr->parg[i] = (int) *a++;
}
prptr->parg[nargs] = 0;

prptr->reg[0]=(uint32_t)kill;
prptr->reg[14]=(uint32_t)prptr->pargs;
prptr->reg[15]=(uint32_t)&prptr->parg[0];
prptr->reg[1]=(uint32_t)&saddr[ssize];

//ra      = (uint32_t)kill;
//a0      = (uint32_t)prptr->pargs;
//a1      = (uint32_t)&prptr->parg[0];
//epc     = (uint32_t)procaddr;
//sp      = (uint32_t)saddr;

prptr->stackptr=(uint32_t)saddr;
restore(mask);
return pid;
}

void panic()
{
    cli();
    kprintf("Kernel panic!\n");
    kprintf("%d , %s\n",currpid,proctab[currpid].name);
    while(1);
}


/*void __attribute__ ((section(".interrupt"))) isr(){    
 // kprintf("interrupt\n");
  switch(proctab[currpid].state){
         case PR_SUSP:
               goto raw2running;
         case PR_CURR:
               proctab[currpid].state = PR_READY;
               insert(currpid);
               goto launch;
         case PR_KILL:
         case PR_SLEEP:
         case PR_FREE:
         case PR_WAIT:
                 
         default:
                kprintf("%d\n",proctab[currpid].state);
                panic();
  }
  launch:
       currpid=extract();//sacar del queue
       if(proctab[currpid].state == PR_SUSP){// sino esta iniciada
          goto raw2running;
       }else{
          goto normal;// si esta iniciada
       }
  normal:
       proctab[currpid].state = PR_CURR;
       
  raw2running:
      proctab[currpid].state = PR_CURR;
      asm volatile("csrw mepc, %0" : : "r" (proctab[currpid].paddr));
      asm volatile("mret");
}*/
