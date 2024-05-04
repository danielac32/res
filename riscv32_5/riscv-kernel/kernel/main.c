
//asm("mv a7, %[flag]" : : [flag] "I" (SYSCALL_YIELD));  \


asm("li sp,250000  \n\t"
    "li gp,0x840   \n\t" //antes del puntero de entrada
    "j main       \n\t"
);
 
 
#include <stdint.h>
#include <string.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/kmalloc.h"
#include "../include/timer.h"
#include "../include/shell.h"
#include "../include/syscall.h"
#include "../include/task.h"


#define delay(){                          \
  for (long i = 0; i < 1000000; ++i)      \
  {                                       \
        asm("nop");                       \
  }                                       \
}



int task1(void *p,uint8_t pid);
int task2(void *p,uint8_t pid);
 
 
int thread_shell(void *p,uint8_t pid){
   MOS_Continue(pointer[0]);
   myprintf("shell pid: %i\n",*(int *)p);
   int argc=0;
   char *argv[10];
   int read=0;
   while(1){

         int c =get_cmd(&argc,argv);
         if(c){ 

           if(!strcmp(argv[0],"load")){
               create_task2(argv[1],&count_task,(void*)"hola tarea\n");
               
               myprintf("ok\n");
               delay();
               myprintf("\n");
           }else if(!strcmp(argv[0],"kill")){
                uint8_t pid=atoi(argv[1]);

                if (pid>count_task)
                {
                   myprintf("error pid: (%i)\n",pid);
                }else{
                   int err=kill(pid);
                   if(!err)myprintf("killed: %i\n",pid);
                   else myprintf("error : %i\n",pid);
                }
                
                delay();
                myprintf("\n");
           }else if (!strcmp(argv[0],"list")){
                for (int i = 0; i < count_task; ++i){
                    task_t *current_thread=(task_t*)queue_task[i];

                     myprintf("\n----------------------%i--------------------------\n",i);
                     myprintf("stack: %8x\n",current_thread->stack_pointer);
                    // myprintf("arg: %8x\n",current_thread->tsk.arg);
                     myprintf("pid(%i)\n",current_thread->tsk.info_t.pid);
                     myprintf("entry(%i)\n",current_thread->tsk.entry);
                     myprintf("\n--------------------------------------------------\n");
                }
                myprintf("ok\n");
                delay();
                myprintf("\n");
           }else if(!strcmp(argv[0],"echo")){
                
                 char p[50];
                 memset(p,'\0',50);
                 if(argc>1){
                   // p=(char*)kmalloc(50*sizeof(char*));
                    for (int i = 1; i < argc; ++i)
                    {
                         strcat(p,argv[i]);
                         strcat(p," ");
                    }
                 }
                 strcat(p,"\n");
                 create_task2("bin/echo.bin",&count_task,(void*)p);
                 //kfree(p);
                 
           }else if(!strcmp(argv[0],"cat")){
                 create_task2("bin/cat.bin",&count_task,(void*)argv[1]);
           }
           for (int i = 0; i < argc; ++i)
           {
             kfree(argv[i]);
           }
        }
        
        MOS_Break(pointer[0]);
    }
}

int task2(void *p,uint8_t pid){
   MOS_Continue(pointer[1]);
   myprintf("task pid: %i\n",*(int *)p);
   while(1){
         
         for (int i = 0; i < 100; i+=10)
         {
              //myprintf("TASK 2: %i\t",i);
              
              MOS_Break(pointer[1]);
         }
   }
}
 
 



int main(){
MODE=0;
 
//WRG6=(uint32_t)"riscv32 emulator gcc\n";

__svc(SYSCALL_PRINTSTR,(uint32_t)"riscv32 emulator gcc\n");


init_shell();
init_threads();
 
create_task(thread_shell,&count_task,(void*)1);
create_task(task2,&count_task,(void*)2);
 

current=0;
next=1;
 
 while(1){
    
    //proceso del kernel para saltar a cada tarea 
    current_thread=(task_t*)queue_task[current];
    if(current_thread->tsk.entry != NULL){
       current_thread->tsk.entry(NULL,0);
       thread_switch(queue_task[current],queue_task[next]);
 
       curr++;
       current++;                         
       next++;                                                             
       if(next > count_task-1) next=0;      
       if(current >= count_task){           
          current=0;                        
          curr=0;                             
       }            
    }
 }
 return 0;
}
 
