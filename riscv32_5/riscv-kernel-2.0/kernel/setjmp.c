 
#include <stdint.h>
#include <string.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/kmalloc.h"
//#include <setjmp.h>

void __attribute__ ((section(".interrupt"))) isr(){    
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    //int_isr();
    asm("uret");  
}

extern int setjmp(void *env);
extern void longjmp(void *env,int t); //

#define NELEMS(array) (sizeof(array)/sizeof(array[0]))
#define MAX_TASKS 4

typedef enum {
    EXECUTION_POINT_INITIALIZED = 0,
    EXECUTION_POINT_REENTERED = 1
} EXECUTION_POINT;

typedef struct {
    int id;
    //jmp_buf context;
    char *stack;
    int sleep;
    void (*code)(void *ptr);
    void *data;
 } task_t;

task_t tasks[MAX_TASKS];
task_t main_context;

int next_task=0;
 task_t *current_task = NULL;
 //jmp_buf scheduler;


#define save(context) setjmp(context)
#define jump(context) longjmp(context, EXECUTION_POINT_REENTERED)

int counter=0;


void create_task(void (*code)(), void *data) {
     

    task_t *new_task = &tasks[next_task];
    int res=0;
    new_task->id = next_task++;
    new_task->sleep = 0;
    new_task->code = code;
    new_task->data = data;
    new_task->stack=(char*)kmalloc(512);
    res = setjmp(new_task->stack);//context

    if(res == EXECUTION_POINT_REENTERED) {
	   (current_task->code)(new_task);

    }
    
    //return new_task;
}

#define wait(time)							\
    if(save(current_task->stack) == EXECUTION_POINT_INITIALIZED) {	\
	current_task->sleep = time;					\
	jump(main_context.stack);						\
    }



void timer() {
    int i;
    for(i = 0; i < next_task; ++i) {
	if(tasks[i].sleep > 0) {
	    --tasks[i].sleep;
	}
    }
    if (++counter>10)
    {
        
    }
}

void proceed(task_t *task) {
    current_task = task;
    if(save(main_context.stack) == EXECUTION_POINT_INITIALIZED) {
	jump(current_task->stack);
    }
}

void schedule() {
    static int i;
    while (1) {	
	for(i = 0; i < next_task; ++i) { // round-robin
	    if(tasks[i].sleep == 0) {
		proceed(&tasks[i]);
	    }
	}
	timer();
    }
}


typedef struct {
    int id;
    //jmp_buf context;
    char *stack;
    int sleep;
    void (*code)(void *ptr);
    void *data;
 } tarea;

void task4(void *ptr) {
    myprintf("ejecutando tarea\n");
    for (int i = 0; i < 10; ++i)
    {
        myprintf("%i\n",i);
    }
    while(1) {
       myprintf("task %i\n", current_task->id);
       wait(40);
    }
}

tarea *mitarea;

void task1(void *ptr) {

    while(1) {
       myprintf("task %i\t\t", current_task->id);
       wait(20);
    }
}


void task2(void *ptr) {
      mitarea = ptr;
  int p = mitarea->id;
  myprintf("id: %d\n",p);
    while(1) {
       myprintf("task %i\t\t", current_task->id);
       wait(30);
    }
}

void task3(void *ptr) {
    create_task(task4, NULL);
    while(1) {
       myprintf("task %i\n", current_task->id);
       wait(40);
    }
}



int main(void)
{
    MODE=0;
   sei();
   myprintf("Iniciando kernel\n");
   kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
   kmalloc_debug();
   main_context.stack=(char*)kmalloc(512);

    create_task(task1, NULL);
    create_task(task2, NULL);
    create_task(task3, NULL);
    //create_task(some_task, NULL);
    schedule();
    while(1);
}