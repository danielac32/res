#include <stdint.h>
#include <stddef.h>
#include <string.h>
 

#ifndef TASK_H_
#define TASK_H_
 

#define MAX_TASK 10
#define STACK_SIZE 1024
#define QUANTUM 10
int preempt;

typedef enum {
    NONE=0,
    KILL = 1,
    RUNNING = 2,
    READY=3
} state_t;
 

struct task_t{
    int id;
 
    uint32_t reg[16];
    long sleep;
    void (*entry)(void*);
    void *arg;
    char name[30];
    state_t state;
 };
 

struct task_t tasks[MAX_TASK];
struct task_t main_context;
struct task_t *current_task;

int taskTop;  // total number of task
int counter_task;

void task_init();
void printdetalle(int pid);
int task_create(char *name,void (*entry)(void*),void *arg);
void task_go(int i);
void yield();
void delay(long time);



#endif