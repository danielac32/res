#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "task.h"
#include "syscall.h"
#include "lib.h"
#include "serial.h"
//char *nametsk[]={"task1","task2","task3","task4","task5","task6","task7","task8"};

void init_task(){
  for (int i = 0; i < MAX_PROCESSES; ++i)
  {
       processlist[i].pid = 0;//pid
       processlist[i].attached=0;
       //alloc[i].enable=0;
  }
  core.task=0;
  core.counter=0;
  core.i=0;
  core.j=0;
}
 


void process_modify(char *name, int prio, void (*function)(int,uint32_t),int pid){
      if(pid>0){
         strcpy(processlist[pid].name, name);
         processlist[pid].prio = prio*1;
         processlist[pid].function = function;
            //alloc[pid].enable=1;
      }
}
void process_attach(char *name, int prio, void (*function)(int,uint32_t),int *pid,size_t len)
{
  int i = 0;
  if(processlist[core.task].attached != 1){
     processlist[core.task].pid = core.task;
     strcpy(processlist[core.task].name, name);
     processlist[core.task].prio = prio*1;
     processlist[core.task].function = function;
     //alloc[core.task].enable=1;
     
     if(len!=0){
        processlist[core.task].p=all((chunk_t*)mem,len+512);//RESERVAR UN SECTOR PARA EL PROGRAMA A CARGAR 
        //info((chunk_t*)mem);
     }
     *pid=core.task;
  }
  core.task++;
}

void process_run(int pid){
  processlist[pid].attached=1;
}
 



void list_process(){
for(int i = 0; i < MAX_PROCESSES; i++)
    {
      if(processlist[i].attached == 1)
      {
           myprintf("\n_________________________________________\n");
           myprintf("pid: %l\n",processlist[i].pid);
           myprintf("name: %s\n",processlist[i].name);
           myprintf("prio: %l\n",processlist[i].prio);
           myprintf("point: %x\n",processlist[i].function);
           myprintf("\n_________________________________________\n");           
      }
    }
}

int scheduler(){
  int i = 0;
  void (*p)(int,uint32_t);
  
  while(1){
    for(i = 0; i < MAX_PROCESSES; i++){
      if(processlist[i].attached == 1){
        if((counter_timer % processlist[i].prio)==0){
            p = (void *)processlist[i].function;
            (*p)(i,(uint32_t)processlist[i].function);   
        }
      }
    }
    counter_timer++;
  }
  return 0;
}

int lauch_cmd(int case_,char *url,char *value){
int i;
int ch;
uint32_t si=0;
uint32_t l=0;
char *bin;
void (*p)(int pid);//kill
void (*p1)(char *str);//cat,echo
i=openfile((uint32_t)url);
if(i)return -1;//error
si=filesize();
bin=do_malloc((chunk_t*)mem,si+512);
l=0;
while(l<si+2){
      ch=readfile();
      bin[l]=ch;
      l++;
}
closefile();
switch(case_){
      case 0://kill
          p = (void *)bin;
          (*p)(atol(value));
      break;
      case 1://cat,echo
          p1 = (void *)bin;
          (*p1)(value);
      break;
      case 2:

      break;
}
do_free((chunk_t*)mem,bin);  
return 0;
}


void* do_malloc (chunk_t* it, size_t len) {
    chunk_t *ch;

    if (!len) {
        return (NULL);
    }
    for (; it; it = it->next) {
        if (!it->free) { /* occupied */
            continue;
        }
        if (len + sizeof(chunk_t) > it->size) {
            continue; /* free but too small  */
        }
        if (len + sizeof(chunk_t) + sizeof(chunk_t) >= it->size) {
            /* free and just perfect in size, reserve it! */
            it->free = 0;
            return ((char*)it + sizeof(chunk_t));
        }
        /* free but big, split it! */
        ch = (chunk_t*)((char*)it + len + sizeof(chunk_t));
        ch->next = it->next;
        it->next = ch;

        ch->free = it->free;

        ch->size = it->size - (len + sizeof(chunk_t));
        it->size -= ch->size;

        it->free = 0;
        return ((char*)it + sizeof(chunk_t));
    }
    /* No more free memory */
    return (NULL);
}


void do_free (chunk_t *it, void *p) {
    if (!p) {
        return;
    }
    memset(p, 0, sizeof(p));
    /* mark chunk as free */
    ((chunk_t*)((char*)p - sizeof(chunk_t)))->free = 1;

    /* merge free chunks */
    for (; it; it = it->next) {
        while (it->free && it->next && it->next->free) {
            /* merge with next free */
            it->size += it->next->size;
            it->next = it->next->next;
        }
    }

    return;
}


void chunklist_init (chunk_t *it, size_t heap_size) {
    it->free = 1;
    it->size = heap_size;
    it->next = NULL;
}

void info(chunk_t* it) {
    chunk_t* ch = it;

    myprintf("============================\n");
    for (; ch; ch = ch->next) {
        myprintf("chunk:%l, size:%l, %s\n",(char*)ch - (char*)it,ch->size,ch->free ? "FREE" : "RESERVED");
    }
    myprintf("============================\n\n");
}

void* all (chunk_t* it, size_t size) {
    void* p = do_malloc(it, size);
    memset(p, 0, sizeof(p));
    myprintf("malloc: size:%l ptr:0x%x\n", size, p ? ((char*)p - (char*)it) : 0);
    //info(it);
    return p;
}

void fr (chunk_t* it, void* p) {
    chunk_t* ch = it;
    //memset(p, 0, sizeof(p));
    do_free(it, p);
    myprintf("free: size: %l ,ptr:0x%x\n", ch->size,p ? ((char*)p - (char*)it) : 0);
    //info(it);
}

 