#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#include "lib/timer.h"
#include "lib/lib.h"
#include "lib/serial.h"
#include "lib/syscall.h"
#include "driver/terminal.h"
#include "lib/task.h"
#include "driver/cmd.h"


/*
void ftos(float n,char *buff){
char b[30];
int intpart=(int)n;
float fpart = n -(float)intpart;
int afterpoint = fpart*100;
intToStr(intpart,b,10);
strcpy(buff,b);
strcat(buff,".");
intToStr(afterpoint,b,10);
strcat(buff,b);
}*/


#define COOS_DELAY()                    \
if (!setjmp(task_context[task_no])) {   \
    longjmp(main_context,1);            \
} 
 
 
void Task1( ) 
{
  char counter=0;
  
  while(1) 
  {
    myprintf("task1: (%i) \n",counter++);

    COOS_DELAY();
    
  }
}


void Task2( )
{
  char counter = 0;
  
  while(1) 
  {

   myprintf("                   task2: (%i) \n",counter++);
    COOS_DELAY();
  
  }
}


void main()
{ 
  
	myprintf("init kernel 0.0.0.1\n");
    myprintf ("Build date: %s %s\n\n", __DATE__, __TIME__);
	int pid;

    mode=0;
    enable_timer=1;

    memset(mem, 0, sizeof(mem));
    chunklist_init((chunk_t*)mem, sizeof(mem));
    info((chunk_t*)mem);
    memset(__buffer__, 0, sizeof(__buffer__));
    chunklist_init((chunk_t*)__buffer__, sizeof(__buffer__));
    info((chunk_t*)__buffer__);
    
 

/*

    CreateTask(Task1);
    CreateTask(Task2);

    RunSystems();*/
     
/*
    uint32_t intval1=*(uint32_t *)&f; 
    myprintf("to int %l: \n",intval1);

    val2=*(float *)&intval1;   
    ftoa(val2,buff,2);
    myprintf("%s\n",buff);*/
    

    init_task();
    process_attach("terminal", 1, process0,&pid,0);
    process_run(pid);
    //process_attach("null", 100, process1,&pid,0);
    //process_run(pid);
    myprintf("terminal enable\n");

    scheduler();

    while(1);

}

