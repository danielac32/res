#include <stdint.h>
#include <string.h>

#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/kmalloc.h"
 
//extern uint32_t stack_top;
//extern uint32_t stack_size;

extern int setjmp(void *env);
extern void longjmp(void *env,int t); //

typedef enum {
  NEW,
  READY,
  RUNNING,
  KILL,
  BLOCKED,
  DONE
}state_type;
// Task structure
typedef struct Task
{
  void (*TaskPtr)();
  char *stack;
  //unsigned char state;
  unsigned char TaskID;
  void* args;
  char name[20];
  state_type state;
}Task;
 
Task Thread;
Task Threads[10];

 
#define save(context)setjmp(context)
#define store(context)longjmp(context,0)

unsigned int task_no=0;
unsigned int task_cnt=0;

#define yield()if (!save(Threads[task_no].stack)) {store(Thread.stack);}
 
void int_isr(){
    
}



void __attribute__ ((section(".interrupt"))) isr(){    
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    int_isr();
    asm("uret");  
}
 
void CreateTask(void (*tsk)(void *ptr),void* arg,char *n){ 
     int   res;
     char  rdy;
     void (*jump)(void* ptr);
     int temp;
     
     Threads[task_cnt].TaskID=task_cnt;
     Threads[task_cnt].TaskPtr=tsk;
     Threads[task_cnt].stack=(char *)kmalloc(200);
     Threads[task_cnt].state = READY;
 

     strcpy(Threads[task_cnt].name,n);
 
     if (Threads[task_cnt].TaskPtr!= 0){
         temp=task_no;
         task_no=task_cnt;
         res = save(Thread.stack);
         jump = Threads[task_cnt].TaskPtr;//tsk_p[task_no];
         if (res == 0){
             jump(NULL);                             // invoke task
         }
     }
     task_no=temp;
     task_cnt++;
}

 
void run(){
     while(1){
  
	    if(Threads[task_no].state==READY){
           if (!save(Thread.stack)) {   
               store(Threads[task_no].stack);            
           }       
        }
        if (++task_no >= task_cnt) task_no = 0;
     }
}

int a =0;
int b=0;
int c=0;
void task1(void *ptr);
void task2(void *ptr);
void task3(void *ptr);

 

void task1(void *ptr){
  void *main_context;

  while(1){
  	 
  	 
     //get_next_context(&main_context);
     myprintf("task1 %l, %l\n",a++,main_context);
     if(a>100)a=0;
     //void (*__svc__)(void *)=(void (*)(void *))SYSCALL_INTERRUPT_POINTER;//void (*__svc__)(void *,uint32_t)=(void (*)(void*,uint32_t))SYSCALL_INTERRUPT_POINTER;
     //__svc__(task_user->stack); 
     
     //__yield__(task_user->stack,main_context);
     yield();
  }
}


static int m=0;
 
void task2(void *ptr){

  void *main_context; 
 


  while(1){

  	 if(b==200){
        if(m==0){
     	  CreateTask(task3,(void*)"core3","tarea 3");
           m=1;
           //run();
        }
  	 }

     myprintf("                 task2 id(%i)\n",b+=2);
     if(b>200)b=0;
     yield();
     
    
    //get_next_context(&main_context);
  // __yield__(task_user2->stack,main_context);
  }
}

int y=0;
//mitarea *task_user3;
void task3(void *ptr){
/* char *p;
  p = (uint8_t*)arg;
  myprintf("arg: %s\n",p);
  void *main_context;*/
  void *main_context; 
  char *p;
 /* task_user3 = ptr;
  p = (uint8_t*)task_user3->args;
  myprintf("*********************\n");
  myprintf("*  entry: %8x\n",task_user3->TaskPtr);
  myprintf("*  stack: %8x\n",task_user3->stack);
  myprintf("*  id: %i\n",task_user3->TaskID);
  myprintf("*  args: %s\n",p);
  myprintf("*  name: %s\n",task_user3->name);
  myprintf("*  state: %i\n",task_user3->state);
  myprintf("*********************\n");*/

  while(1){
   
     myprintf("                                      task3 %l\n",c+=3);
     if(!(c%27)){
     	if(y==0){
            //kill_task(0);
     	    y=1;
     	 }
     }
     if(c>100)c=0;
     yield();
     // get_next_context(&main_context);
   //__yield__(task_user3->stack,main_context);
  }
}



 
int main(){
   MODE=0;
   ENT0=1;
   myprintf("Iniciando kernel\n");
   //rtcMem=(uint32_t*)kmalloc(10*sizeof(uint32_t));
   kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
   kmalloc_debug();
 
   for (int i = 0; i < 10; ++i){
   	      Threads[i].TaskPtr=0;
   }
   Thread.stack=(char *)kmalloc(200);
   Thread.state = READY;

   CreateTask(task1,(void*)"shell","tarea 1");
   CreateTask(task2,(void*)"core2","tarea 2");
   //CreateTask(task3,(void*)"core3","tarea 3",0);
   //InitAll();
   myprintf("run kernel\n");

  run();

while(1){
	//myprintf("end\n");
}
return 0;
}