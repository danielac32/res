 
#include"schedule.h"
#include "riscv.h"
#include "serial.h"
#include "kmalloc.h"
#define STACK_SIZE 1024

TCB* current_thread;
struct queue ready_list;
struct queue done_list;

void thread_switch(TCB * old, TCB * new);
void thread_start(TCB * old, TCB * new);

void scheduler_begin(){
 
 /*int *random_ptr=(int *)kmalloc(sizeof(int *));
 *random_ptr=100;*/
 myprintf("Scheduler begin\n");
 current_thread=(TCB *)kmalloc(sizeof(TCB));
 current_thread->state=RUNNING;
 ready_list.head=NULL;
 ready_list.tail=NULL;
 done_list.head=NULL;
 done_list.tail=NULL;
 /*
 current_thread->initial_function=increment;//Initialize to a function of type void (void *)
 current_thread->initial_argument=random_ptr;//Initialize to pointer 
 current_thread->stack_pointer=(unsigned char *)kmalloc(STACK_SIZE)+STACK_SIZE;//Allocate memory for stack and move the pointer to end of the stack so that the stack will grow upwards.
 */
}

void thread_fork(void (*target)(void*), void * arg) {
   TCB *new_thread;
   new_thread=(TCB *)kmalloc(sizeof(TCB));
   new_thread->stack_pointer=(unsigned char *)kmalloc(STACK_SIZE)+STACK_SIZE;
   new_thread->initial_function=target;
   new_thread->initial_argument=arg;
   current_thread->state=READY;
   new_thread->state=RUNNING;
   thread_enqueue(&ready_list,current_thread);
   TCB * temp=current_thread;
   current_thread=new_thread;
   myprintf("thread started\n");
   thread_start(temp,current_thread);
}



void thread_wrap(){
  current_thread->initial_function(current_thread->initial_argument); 
  current_thread->state=DONE;
  yield(); 
}
/*int main(int argc, char **argv){
  //int shared_var=10;
  myprintf("Main thread started shared variable is %d\n",shared_var);
  //TCB * threads[5];
 //Allocate memory for the current thread

 
 //current_thread->initial_function(current_thread->initial_argument);
 //myprintf("%d",current_thread->stack_pointer);
 inactive_thread=(TCB *) kmalloc(sizeof(TCB *));
 thread_start(inactive_thread,current_thread);
 myprintf("Main thread restarted shared var is %d\n",shared_var);  
 
 //yield();
 myprintf("Main thread ended and shared_var is %d\n",shared_var);
 return 0;
}*/

void yield() {
  
if(current_thread->state!=DONE){
	current_thread->state=READY;
	TCB *ready_thread=thread_dequeue(&ready_list);
	ready_thread->state=RUNNING;
	TCB * temp = current_thread;
  current_thread = ready_thread;
	thread_switch(temp,current_thread);
}
	
    //thread_switch(current_thread, inactive_thread);
}

void scheduler_end(){
  while(!is_empty(&ready_list)){
    yield();	
  }
  myprintf("Scheduler ended\n");
}
