

asm("j main");

#include <stddef.h>
#include <stdint.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/syscall.h"
 

typedef enum {
    _KILL = 0,
    _RUNNING = 1
} t_state;

static struct task_t{
    int id;
    char *stack;
    int sleep;
    void (*entry)(void);
    void *arg;
    char name[30];
    t_state state;
};



void main(){

  static int pid=0;
  static void *arg;
  static int num_t=0;
  static int i=0;
  static struct task_t *perfil;
  pid=get_id();
  num_t=get_num_task();
  arg=get_args(pid);
  num_t-=1;
  myprintf("tasks: %i\n",num_t);
  //perfil=k_malloc(sizeof((struct task_t*));

  for (i = 0; i < num_t; ++i){
      perfil=(struct task_t*)struct_task(i);
      myprintf("id: %i, entry: %8x , stack pointer: %8x , name: %s , arg: %s\n",perfil->id,perfil->entry,perfil->stack,perfil->name,perfil->arg);
  }
  exit_task(pid);
}