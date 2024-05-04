#include <stdint.h>
#include <stddef.h>
#include "kmalloc.h"
#include "task.h"
#include "serial.h"
#include "syscall.h"


void jump_task(uint8_t pid){
task_t *start=(task_t*)queue_task[pid];
start->tsk.entry(&start->tsk.arg,start->tsk.info_t.pid);
}

void create_task(int (*function)(void*,uint8_t),uint8_t *count,void *arg){
   if(function!=NULL){

      task_t *new=(task_t*)kmalloc(sizeof(task_t*));

      new->stack_pointer=(uint8_t*)kmalloc(STACK_SIZE);
 
      new->tsk.entry=function;

      new->tsk.info_t.pid=*count;

      new->tsk.arg=arg;
      
      queue_task[*count]=(void *)new;
      
      *count+=1;

      jump_task(new->tsk.info_t.pid);
      
   } 
}

void create_task2(char *str,uint8_t *count,void *arg){
	   int file=k_OPEN(str);
     uint8_t c;
     uint32_t l=0;
     
     char *p;

     if(!file){

         uint32_t size=k_FSIZE();

         alloc_task[*count]=(char*)kmalloc(size*sizeof(char));

         if(alloc_task[*count]==NULL){
            myprintf("error malloc (load)\n");
            return;
         }
         for(uint32_t i = 0; i < size; ++i){  
              alloc_task[*count][l]=k_READ();   
              l++;
         }
         k_CLOSE();
          

         task_t *new=(task_t*)kmalloc(sizeof(task_t*));

         new->stack_pointer=(uint8_t*)kmalloc(STACK_SIZE);

         new->tsk.info_t.pid=*count;

         new->tsk.arg=arg;

         new->tsk.entry=(int (*)(void*,uint8_t))alloc_task[*count];

         queue_task[*count]=(void *) new;
        
         *count+=1;

         jump_task(new->tsk.info_t.pid);
         
         
     }else{
         myprintf("file not found\n");
     }

}

int kill(uint8_t pid){
//myprintf("\npid: %i\n",pid);
if(pid < 1){
   return 1;
}else{
void **res=(void**)kmalloc(STACK_SIZE*sizeof(void));

int p=0;
void *point=queue_task[pid];//guardar la esctructura de la tarea 
queue_task[pid]=NULL; // anular estructura

for (int i = 0; i < count_task; ++i){
     if(queue_task[i]!=NULL){
        res[p]=queue_task[i];
        p++;
     }
}
for (int i = 0; i < p; ++i){
     queue_task[i]=NULL;
     queue_task[i]=res[i];
}

count_task -= 1;
kfree(point);
kfree(res);
kfree(alloc_task[pid]);
return 0;
} 
/*
if(pid == 0 || pid == 1 ){   
	//no se puede borrar la tarea 0 , 1
	return 1;//
}else{

void **res=(void**)kmalloc(STACK_SIZE*sizeof(void));

int p=0;
void *point=queue_task[pid];//guardar la esctructura de la tarea 
queue_task[pid]=NULL; // anular estructura

for (int i = 0; i < count_task; ++i){
     if(queue_task[i]!=NULL){
        res[p]=queue_task[i];
        p++;
     }
}
for (int i = 0; i < p; ++i){
     queue_task[i]=NULL;
     queue_task[i]=res[i];
}

count_task -= 1;
kfree(point);
kfree(res);
kfree(alloc_task[pid]);
return 0;
}*/
}


void init_threads(){
count_task=0;
current=0;
next=0;
}