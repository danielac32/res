#include "kernel.h"
#include "resets.h"
#include "gpio.h"
#include "uart.h"
#include "xosc.h"
#include "nvic.h"
#include "systick.h"
#include "queue.h"
#include <string.h>
#include <rp2040.h>
#include "kmalloc.h"


struct procent proctab[MAX_NUMBER_OF_TASKS];
 queue_t ready_queue;
 queue_t sleep_queue;
struct  procent *active;
 uint32_t sys_time;
 


void unlock(int pid){
disable();
proctab[pid].state=PR_CURR;
printf("unlock : %s %d %d\n", proctab[pid].name,proctab[pid].state,proctab[pid].pid);
queue_push(&ready_queue, &proctab[pid]);

*((uint32_t*)0xE000ED04) = (1 << 28);
//asm("svc 4");
}


uint8_t getpid(){
   return active->pid;
}

void delay(long t){
uint32_t mask = disable();
active->state=PR_SLEEP;
active->wait_ticks= t;
//queue_pushsort(&sleep_queue, active);
PEND_SV();
restore(mask);
}

void kill(){
disable();
proctab[active->pid].state=PR_KILL;
printf("kill: %s %d %d\n", proctab[active->pid].name,proctab[active->pid].state,proctab[active->pid].pid);
*((uint32_t*)0xE000ED04) = (1 << 28);
}

int   newpid(void)
{
    uint32_t  i;          /* iterate through all processes*/
//  static  pid32 nextpid = 1;  /* position in table to try or  */
     int nextpid = 0;  /* position in table to try or  */
                    /*  one beyond end of table */

    for (i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
      //  nextpid %= MAX_NUMBER_OF_TASKS;   /* wrap around to beginning */
       // if (kernel.pid[i].state == PR_FREE) {
        if(proctab[i].state == PR_FREE) {
            return nextpid++;
        } else {
            nextpid++;
        }
    }
    return (int) -1;
}

void kernel_init(){
 for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i){
    	proctab[i].state=PR_FREE;
 }
 currpid=0;
 preempt=1;
 ntask=0;
 queue_init(&ready_queue);
 queue_init(&sleep_queue);
}



uint32_t *prepare_task_stack(uint32_t *task_stack,uint32_t size,void *entry){
  task_stack += size - 17; /* End of task_stack, minus what we are about to push */
  task_stack[8] = (uint32_t) 0xFFFFFFFD;
  task_stack[14] = (uint32_t) kill;
  task_stack[15] = (uint32_t) entry;
  task_stack[16] = (uint32_t) 0x01000000; /* PSR Thumb bit */
  return task_stack;
}


int create(
      int       (*procaddr)(),  /* procedure address        */
      int   ssize,      /* stack size in bytes      */
      int       priority,   /* process priority > 0     */
      char      *name,      /* name (for debugging)     */
      int   nargs,      /* number of args that follow   */
      ...
    ){
uint32_t mask=disable();
int pid=newpid();//getNextPID();
struct  procent *prptr;

uint32_t *saddr=(uint32_t*)getmem(ssize);//newStack;; 
if(saddr==(uint32_t *)SYSERR){
   printf("error malloc %s\n",name);
   while(1);
}
prptr = &proctab[pid];
prptr->state = PR_CURR;
prptr->wait_ticks=0;
prptr->prstkbase=saddr;
prptr->prstklen=ssize;
strcpy(prptr->name,name);
prptr->pargs = nargs;
prptr->paddr = (int *)procaddr;
prptr->pid=pid;
ntask++;
/*void * a = (void *)(&nargs + 1);
for (int i = 0; i < nargs; i++) {
     prptr->parg[i] = (void) *a++;
}
prptr->parg[nargs] = 0;*/

prptr->sp =  prepare_task_stack(saddr,ssize,procaddr);
restore(mask);
printf("pid: %d\n",pid );
return pid;
}


void __attribute__((naked))  svccall_handler_c(uint32_t *sp){
  asm("cpsid  i");
  uint32_t svc_number = ((char *)sp[6])[-2];
  uint32_t svc_arg1 = sp[0];
  uint32_t svc_arg2 = sp[1];
  uint32_t result = svc_arg1 + svc_arg2;

  //printf("svc Number: %d\n", svc_number);
  switch(svc_number){
      case 0:
         *((uint32_t*)0xE000ED04) = (1 << 28);
      break;
      case 1:
           
      break;
      case 4:
            for (int i = 0; i < 6; ++i)
            {
                active = queue_pop(&ready_queue);
                printf(">%s   %d\n",active->name,active->state );
            }
      break;
      case 5:
         printf("aquiiiiiiiiiiiiiiiiiiiiiiiiiii: %d , %d\n",svc_arg1,svc_arg2);
      break;
  }
  asm volatile("cpsie  i\n"
               "ldr r0,=0xFFFFFFFD\n"
               "bx r0\n");
 // *((uint32_t*)0xE000ED04) = (1 << 28);//PEND_SV();
}



void systick_handler() {

*((uint32_t*)0xE000ED04) = (1 << 28);
}


void resched(){
	
	struct  procent *tmp;
	currpid = 0;
    uint8_t enable=0;
	while(1){
         

         iterate:
         active = queue_pop(&ready_queue);
         
         if (active->state==PR_CURR)
         {
             queue_push(&ready_queue, active);
         }else{
            printf("else : name: %s state: %d pid: %d\n", active->name,active->state,active->pid);
            continue;
         }

      

         

       
	    /* if(tmp->state == PR_SLEEP) {
		      if (tmp->wait_ticks == 0) {
			      tmp = queue_pop(&sleep_queue);
			      tmp->state = PR_CURR;
			      queue_push(&ready_queue, tmp);
			      tmp = queue_peek(&sleep_queue);
			      printf("sali\n");
		      } else {
		      	 tmp->wait_ticks--;
		      	 printf("%d\n",tmp->wait_ticks );
			     break;
		      }
	     }*/

         active->sp=swch(active->sp);
         
         //queue_push(&ready_queue, active);
           //         active->sp=swch(active->sp);
                    
        /* switch(active->state){
         	   case PR_CURR:
                    //active->state = PR_CURR;
                    queue_push(&ready_queue, active);
                    active->sp=swch(active->sp);
         	    break;
         	    case PR_SLEEP:
                     goto iterate;
         	    break;
         }*/
         
 


         

        
        
        



		/*for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i)
        {
        	 if(proctab[i].state==PR_SLEEP){
        	 	if(proctab[i].wait_ticks>0)proctab[i].wait_ticks--;
        	 	if(proctab[i].wait_ticks==0){
                   proctab[i].state=PR_CURR;
                   task_enqueue(&proctab[i]);//insert(i);
        	 	}
        	 }
        }

		switch(proctab[currpid].state){
               case PR_SUSP:
                    goto raw2running;
               case PR_CURR:
                    proctab[currpid].state = PR_READY;
                    printf("push %d\n", currpid);
                    task_enqueue(&proctab[currpid]);//insert(currpid);
                    goto launch;
               case PR_KILL:
                   printf("\nkill thread :%s\n",proctab[currpid].name);
                   proctab[currpid].state=PR_FREE;
                   proctab[currpid].wait_ticks=0;
                   proctab[currpid].paddr=NULL;
                   proctab[currpid].prio=0;
                   freemem((char *)proctab[currpid].prstkbase,proctab[currpid].prstklen);
                   goto launch;
               break;
               case PR_SLEEP:
                   	goto launch;
               case PR_FREE:
               case PR_WAIT:
               default:
                 printf("errorrrrrr\n");

		}
		launch:
          prptr = task_dequeue();//currpid=extract();//sacar del queue
          currpid = prptr->pid;
          printf("pop %d\n", currpid);
          if(proctab[currpid].state == PR_SUSP){// sino esta iniciada
             goto raw2running;
          }else{
             goto normal;// si esta iniciada
          }
        normal:
          proctab[currpid].state = PR_CURR;
          proctab[currpid].sp=swch(proctab[currpid].sp);
        raw2running:
          proctab[currpid].state = PR_CURR;
          proctab[currpid].sp=swch(proctab[currpid].sp);*/
	}
}

