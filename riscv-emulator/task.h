#include <stdint.h>
#ifndef TASK_H_
#define TASK_H_

/**************************************************/
#define MAX_TASKS  10
extern int setjmp(void *env);
extern void longjmp(void *env,int t); //

 

typedef enum {
    EXECUTION_POINT_INITIALIZED = 0,
    EXECUTION_POINT_REENTERED = 1
} EXECUTION_POINT;

typedef enum {
    KILL = 0,
    RUNNING = 1
} state_t;

typedef struct {
    int id;
    char *stack;
    int sleep;
    void (*entry)(void*);
    void *arg;
    char name[30];
    state_t state;
 } task_t;


task_t tasks[MAX_TASKS];
task_t main_context;

static int next_task=0;
static task_t *current_task=NULL;

#define save(context) setjmp(context)
#define jump(context) longjmp(context, EXECUTION_POINT_REENTERED)
#define wait(time)                          \
if(save(current_task->stack) == EXECUTION_POINT_INITIALIZED) {  \
   current_task->sleep = time;                 \
   jump(main_context.stack);                       \
}
#define yield() wait(0)
 
static void create_task(void (*entry)(void*), void *arg,char *name) {

    task_t *new_task = &tasks[next_task];
    int res=0;
    new_task->id = next_task++;
    new_task->sleep = 0;
    new_task->entry = entry;
    new_task->arg = arg; 
    new_task->stack = (char*)kmalloc(512);

    if(new_task->stack==NULL){
       myprintf("ERROR %s\n",name);
       return;
    }
    //strcpy(new_task->name,name);
    new_task->state=RUNNING;
    myprintf("%s %d-> ok\n",name,new_task->id);

    res = setjmp(new_task->stack);//context
    
    if(res == EXECUTION_POINT_REENTERED) {

       (current_task->entry)(current_task->arg);

    }
}
 
static void proceed(task_t *task) {
    current_task = task;
    if(save(main_context.stack) == EXECUTION_POINT_INITIALIZED) {
    jump(current_task->stack);
    }
}

static void schedule() {
    while (1) { 
        for(int i = 0; i < next_task; ++i) { // round-robin
            if(tasks[i].sleep == 0 && tasks[i].state==RUNNING) {
               proceed(&tasks[i]);
            }
        }
        for(int i = 0; i < next_task; ++i) {
            if(tasks[i].sleep > 0) {
               --tasks[i].sleep;
            }
        }
    }
}

#endif