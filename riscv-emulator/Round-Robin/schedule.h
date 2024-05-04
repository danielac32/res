 
#include "queue.h"
typedef enum {
  NEW,
  READY,
  RUNNING,
  BLOCKED,
  DONE
  
}state_type;
struct thread{ //Representation of Thread COntrol Block
  unsigned char* stack_pointer;
  void (*initial_function)(void*);//This is a variable that has void * as an argument
  void* initial_argument; //This is kind of generic variable for defining intitial functions for threads
  state_type state;
};
typedef struct thread TCB;
extern TCB *current_thread;

void thread_fork(void (*target)(void*), void * arg);
void yield();
void scheduler_begin();
void scheduler_end();


