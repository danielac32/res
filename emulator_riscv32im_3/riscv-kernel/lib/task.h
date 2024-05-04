#include <stdint.h>

#ifndef TASK_H
#define TASK_H

#define MAX_PROCESSES 8
#define MAX_NAME_LEN  10
 
static char *nametsk[]={"task1","task2","task3","task4","task5","task6","task7","task8"};

char mem[100000];// ram for programs dynamic
char __buffer__[50000];
uint32_t counter_timer;

typedef struct chunk_s {
    struct chunk_s  *next;
    size_t          size;
    char            free;
} chunk_t;


struct pcb{
  int pid;                   
  int prio;                  
  int attached; 
  char *p;            
  void (*function)(int,uint32_t);           
  char name[MAX_NAME_LEN]; 
};
struct pcb processlist[MAX_PROCESSES];

struct kernel
{
  int i;
  int j;
  int counter;
  int task;
  void (*tsk)(int,uint32_t);
};
struct kernel core;
 

void init_task();

void process_attach(char *name, int prio, void (*function)(int,uint32_t),int *pid,size_t len);
int process_detach(int pid);
void list_process();
void process_run(int pid);
void process_modify(char *name, int prio, void (*function)(int,uint32_t),int pid);
 
int scheduler();

int lauch_cmd(int case_,char *url,char *value);

void* do_malloc (chunk_t* it, size_t len);
void do_free (chunk_t* it, void *p);
void chunklist_init (chunk_t* it, size_t heap_size);
void info(chunk_t* it);
void* all (chunk_t* it, size_t size);
void fr (chunk_t* it, void* p);
#endif