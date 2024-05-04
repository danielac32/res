#include <stdint.h>
#include <string.h>
#include "riscv.h"
#include "serial.h"
#include "kmalloc.h"
#include "task.h"





 
void user_task4(void *p)
{
  myprintf("Task4: Created! %s\n",p);
  myprintf("Task4: Now, return to kernel mode\n");
  
 
  int i=0;
  //yield();
  while (1) {
    myprintf("                          Task4: %d\n",i+=100);
    //yield();
    //syscall(1,150,0,0);//
     delay(1);
   
  }
}
 

void user_task3(void *p)
{
  myprintf("Task3: Created! %s\n",p);
  myprintf("Task3: Now, return to kernel mode\n");
  
   int pid4=task_create("core4",&user_task4,"victoria");
  int i=0;
  //yield();
  while (1) {
    myprintf("                          Task3: %d\n",i+=100);
    //yield();
    //syscall(1,150,0,0);//
    delay(150);
   
  }
}
 

void user_task2(void *p)
{
  myprintf("Task2: Created! %s\n",p);
  myprintf("Task2: Now, return to kernel mode\n");
  
  int pid4=task_create("core3",&user_task3,"victoria");
  int i=0;
  //yield();
  while (1) {
    myprintf("                          Task2: %d\n",i+=10);
    //yield();
    //syscall(1,150,0,0);//
    delay(150);
   
  }
}
 
 
void user_task0(void *p)
{
  myprintf("Task0: Created! %s\n",p);
  myprintf("Task0: Now, return to kernel mode\n");
  int i=0;
  //yield();//
  while (1) {
    myprintf("Task0: %d\n",i++);

   
    //yield();//
    //syscall(1,100,0,0);//
    delay(100);
 
  }
}

void user_task1(void *p)
{
  myprintf("Task1: Created! %s\n",p);
  myprintf("Task1: Now, return to kernel mode\n");
  int pid3=task_create("core2",&user_task2,"orma");
   

  int i=0;
  //yield();
  while (1) {
    myprintf("              Task1: %d\n",i++);
    //yield();//
    //syscall(1,10,0,0);//
    delay(10);

  }
}

 uint32_t timer_scratch[NCPU][5];


#define interval 20000000 
int main()
{
 
    
    kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
    kmalloc_debug();
    //struct jmp_buf *context;
    

    myprintf("OS start\n");
    task_init();
    int pid1=task_create("core0",&user_task0,"veronica");
    int pid2=task_create("core1",&user_task1,"daniel");
   
    myprintf("RUNNING %d\n",taskTop);
    kmalloc_debug();

 
    

    int i=0;
    
  while (1) {

   
    if(tasks[counter_task].sleep == 0 && tasks[counter_task].state==READY){
        task_go(counter_task);
    }
  

    counter_task = (counter_task + 1) % taskTop; // Round Robin Scheduling

    for(int i = 0; i < taskTop; ++i) {
        if(tasks[i].sleep > 0) {
           --tasks[i].sleep;
        }
    }
 
  }

 
    
   
   return 0;
}