#include <stdint.h>
#include "reg.h"
#ifndef TASK_H_
#define TASK_H_

/**************************************************/
#define MAX_TASKS  10



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
    //jmp_buf context;
    char *stack;
    int sleep;
    void (*entry)(void);
    void *arg;
    char name[30];
    state_t state;
 } task_t;

task_t tasks[MAX_TASKS];
task_t main_context;

int next_task;
task_t *current_task;
task_t *perfil_task;

#define save(context) setjmp(context)
#define jump(context) longjmp(context, EXECUTION_POINT_REENTERED)
#define wait(time)                          \
if(save(current_task->stack) == EXECUTION_POINT_INITIALIZED) {  \
   current_task->sleep = time;                 \
   jump(main_context.stack);                       \
}
#define yield() wait(0)

void init_thread();
void create_task(void (*entry)(void), void *arg,char *name);
void timer();
void proceed(task_t *task);
void schedule();
void loadDynamically(char *name,char *arg);

#endif