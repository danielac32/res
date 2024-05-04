#include <stdint.h>
#include "reg.h"
#ifndef TASK_H_
#define TASK_H_

/**************************************************/
/*struct util
{ 
	void *arg;
	int cc;
};*/
struct info
{   
	uint8_t pid; 
	const char name[32];
};

struct task
{
  void *arg;
  int (*entry)(void*,uint8_t);
  struct info info_t;
};

typedef struct stack
{
  uint8_t *stack_pointer;
  struct task tsk; 
}task_t;

task_t *current_thread; 
/**************************************************/
uint8_t curr;
uint8_t current;
uint8_t next;
uint8_t count_task;   
void *queue_task[MAXTHREAD];
uint8_t *alloc_task[MAXTHREAD];



void save_context(void *t);
void restore_context(void *t);
void thread_switch(void *old,void *new);
void save_context_kernel();
void restore_context_kernel(); 

int kill(uint8_t pid);
void create_task(int (*function)(void*,uint8_t),uint8_t *count,void *arg);
void create_task2(char *str,uint8_t *count,void *arg);
void jump_task(uint8_t pid);
void init_threads();

#endif