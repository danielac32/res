/* create.c - create, newpid */

/* avr specific */

#include <stdarg.h>
#include <xinu.h>


static uint32 thrnew(void)
{
    int tid;
    int nexttid = 0;

 
    for (tid = 0; tid < NPROC; tid++)
    {
        nexttid = (nexttid + 1) % NPROC;
        if (PR_FREE == tasks[nexttid].prstate)
        {
            return nexttid;
        }
    }
    return SYSERR;
}


local  uint32  newpid(void)
{
       uint32 i;                   /* iterate through all processes*/
//     static pid32 nextpid = 1;   /* position in table to try or     */
       static pid32 nextpid = 0;   /* position in table to try or     */
                                   /*  one beyond end of table */

       /* check all NPROC slots */

       for (i = 0; i < NPROC; i++) {
              nextpid %= NPROC;    /* wrap around to beginning */
              if (tasks[nextpid].prstate == PR_FREE) {
                     return nextpid++;
              } else {
                     nextpid++;
              }
       }
       return (pid32) SYSERR;
}


//pid32   create(char *name,void (*entry)(void*),void *arg){
pid32  create(char *name,void (*entry)(void*),int nargs,uint32 *args){

       int i=newpid();//taskTop++;
       uint8 *stk = kmalloc(INITSTK);
       struct procent *new_task = &tasks[i];
       

       new_task->prstkptr=stk;//new_task->prstkptr = kmalloc(INITSTK);
       new_task->prstate = PR_CURR;   
       new_task->prprio = 10;
       strcpy(new_task->prname,name);
       //new_task->prname[PNMLEN-1] = NULLCH;
       //for (int i=0 ; i<PNMLEN-1 && (new_task->prname[i]=name[i])!=NULLCH; i++)
       //       ;
       new_task->prsem = -1;
       new_task->prparent = (pid32)getpid();
       new_task->prhasmsg = FALSE;
       
       new_task->prdesc[0] = CONSOLE;  
       new_task->prdesc[1] = CONSOLE;  
       new_task->prdesc[2] = CONSOLE;  
       new_task->sleep = 0;

       new_task->reg[0]=(uint32) entry;
       new_task->reg[14]=(uint32) nargs;
       new_task->reg[15]=(uint32)args;
 
       new_task->reg[1]=(uint32) &stk[INITSTK];
   
       //new_task->id=
     //  prcount++;
       return i;

//return sys(XINU_CREATE,(long)name,(long)entry,(long)arg);//i;
}

 

/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
 

 