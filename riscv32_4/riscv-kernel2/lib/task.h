#include <stdint.h>
#include <stddef.h>


#ifndef TASK_H
#define TASK_H

#define MAX_PROCESSES 8
#define MAX_NAME_LEN  10
 
 

struct pcb{
  int pid;                   
  int prio;                  
  int attached; 
  char *p;            
  void (*function)(int,uint32_t);           
  char name[MAX_NAME_LEN]; 
};
//struct pcb tsk;

 

void init_task();

void createtask(int prio, void (*function)(int,uint32_t),int *pid,size_t len);
int killtask(int pid);
void list_process();
void runtask(int pid);
void modifytask(int prio, void (*function)(int,uint32_t),int pid); 
int scheduler();

int lauch_exec(int case_,char *url,char *value);
 
#endif