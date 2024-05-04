#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "kmalloc.h"
#include "reg.h"
#include "task.h"
#include "serial.h"
#include "syscall.h"
#include "ff.h"


void init_thread(){
next_task=0;
current_task = NULL;
}
void create_task(void (*entry)(void), void *arg,char *name) {

    task_t *new_task = &tasks[next_task];
    int res=0;
    new_task->id = next_task++;
    new_task->sleep = 0;
    new_task->entry = entry;
    new_task->arg = (void*)kmalloc(200*sizeof(void));
    strcpy(new_task->arg,arg);
    new_task->stack = (char*)kmalloc(512*sizeof(char));
    strcpy(new_task->name,name);
    new_task->state=RUNNING;
    myprintf("ok\n");
    res = setjmp(new_task->stack);//context
    
    if(res == EXECUTION_POINT_REENTERED) {

       (current_task->entry)();

    }
    
    //return new_task;
}

#define wait(time)                          \
    if(save(current_task->stack) == EXECUTION_POINT_INITIALIZED) {  \
    current_task->sleep = time;                 \
    jump(main_context.stack);                       \
    }



void timer() {
    int i;
    for(i = 0; i < next_task; ++i) {
    if(tasks[i].sleep > 0) {
        --tasks[i].sleep;
    }
    }
}

void proceed(task_t *task) {
    current_task = task;
    if(save(main_context.stack) == EXECUTION_POINT_INITIALIZED) {
    jump(current_task->stack);
    }
}

void schedule() {
    static int i;
    while (1) { 
    for(i = 0; i < next_task; ++i) { // round-robin
        if(tasks[i].sleep == 0) {
           if(tasks[i].state==RUNNING) proceed(&tasks[i]);
        }
    }
    timer();
    }
}
 
void loadDynamically(char *name,char *arg){
    FIL boot;  
    char block[128];
    UINT bw;
    long start_t,end_t,result;
    void (*tsk)(void);
    char *bin;
                    
    //myprintf("file: %s\n",name);
                    

    FRESULT res= f_stat( name, NULL );
    if(res==FR_OK){
        if(f_open(&boot,name,FA_OPEN_EXISTING | FA_READ)==FR_OK){
           f_lseek(&boot, 0);
           uint32_t j=boot.fsize,k=0;
           bin = (char*)kmalloc(j+1000);
           //kmalloc_debug();
                       
           if(bin!=NULL || bin!=0){
              while(1){
                  FRESULT res = f_read( &boot, block, sizeof(block), &bw);
                  if(res!=FR_OK || bw!=sizeof(block)){
                     myprintf("ok\n");
                     break;
                  } 
                  memcpy(&bin[k],block,sizeof(block));
                  k+=sizeof(block);
              }
              f_close(&boot);
              tsk=(void (*)(void))bin;

              create_task(tsk, arg,"dynamic load");

           }else{
              myprintf("error boot\n");
           } 
        }
    }else{
        myprintf("not found : %s\n",name);
    } 
}
/*
void *get_pointer_alloc(int size){
size+=1000;
Threads[task_cnt].alloc_t.mem = (void*)kmalloc(size);
Threads[task_cnt].alloc_t.size=size;
if(Threads[task_cnt].alloc_t.mem !=NULL)return Threads[task_cnt].alloc_t.mem;
else return NULL;
}

void CreateTask(void (*tsk)(void *ptr),void* arg,char *n){ 
     int   res;
     char  rdy;
     void (*jump)(void* ptr);
     //int temp;
     
     Threads[task_cnt].TaskID=task_cnt;
     Threads[task_cnt].TaskPtr=tsk;
     Threads[task_cnt].stack=(char *)kmalloc(512);
     Threads[task_cnt].state = READY;
     Threads[task_cnt].args = arg;
     Threads[task_cnt].ptr = &Threads[task_cnt];

     strcpy(Threads[task_cnt].name,n);
 
     if (Threads[task_cnt].TaskPtr!= NULL){
         res = setjmp(Thread.stack);
         if (res == 0){
             task_no=0;
             //temp=task_no;
             //task_no=task_cnt;
             jump = Threads[task_cnt].TaskPtr;//tsk_p[task_no];
             jump(Threads[task_cnt].ptr);   
             //tsk(Threads[task_cnt].ptr);                       // invoke task
         }
     }else myprintf("fail task\n");
     //task_no=temp;
     task_cnt++;
}

 
void run(){
     while(1){
      //
        if(Threads[task_no].state==READY){

           if(!setjmp(Thread.stack)) {   
               longjmp(Threads[task_no].stack);            
           }  
           //myprintf("%i\n",task_no);  
        }

        if (++task_no > task_cnt) task_no = 0;
     }
}
*/