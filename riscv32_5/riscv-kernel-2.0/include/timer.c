#include <stdint.h>
#include <string.h>
#include "reg.h"
#include "timer.h"
#include "serial.h"
#include "task.h"
#include "syscall.h"
#include "kmalloc.h"
#include "ff.h"

FIL file,*resfile;

void int_isr(){
    asm("nop");
    asm("nop");
    uint32_t trap;
    asm volatile ("mv   %0, a7" : "=r" (trap));

    switch(trap){
        case TRAP_CALL:
            {
                  uint32_t event;
                  asm volatile ("mv   %0, a0" : "=r" (event));
                  switch(event){
                          case SYSCALL_EXIST:
                                {
                                  void *point;
                                  asm volatile ("mv   %0, a1" : "=r" (point));
                                  FRESULT res= f_stat(point, NULL );
                                  asm volatile("sw %0,0(gp)" : : "r" (res));
                                }
                          break;
                          case SYSCALL_OPEN:
                               {
                                  void *point;//nombre del archivo
                                  FRESULT res=0;
                                  uint8_t mode;
                                  asm volatile ("mv   %0, a1" : "=r" (point));
                                  asm volatile ("mv   %0, a2" : "=r" (resfile));
                                  asm volatile ("mv   %0, a3" : "=r" (mode));
                                  res=f_open(resfile,point,mode);
                                  //resfile=&file;
                                  asm volatile("sw %0,0(gp)" : : "r" (res));
                                  //asm volatile("sw %0,4(gp)" : : "r" (resfile));
                                }
                          break;
                          case SYSCALL_CLOSE:
                                asm volatile ("mv   %0, a1" : "=r" (resfile));
                               
                                f_close(resfile);
                          break;
                          case SYSCALL_SEEK:
                                {
                                    DWORD pos;
                                    asm volatile ("mv   %0, a1" : "=r" (resfile));
                                    asm volatile ("mv   %0, a2" : "=r" (pos));
                                    f_lseek (resfile, pos);
                                }
                          break;
                          case SYSCALL_OPENDIR:

                          break;
                          case SYSCALL_READ:
                               { 
                                    char c;
                                    UINT bw;
                                    asm volatile ("mv   %0, a1" : "=r" (resfile));
                                    f_read(resfile, &c, 1, &bw);
                                    asm volatile("sw %0,0(gp)" : : "r" (c));
                               }
                          break;
                          case SYSCALL_READBUFF:
                               { 
                                    char *c;//[513];
                                    UINT bw;
                                    int size;
                                    void *addr;
                                    FRESULT res;
                                    asm volatile ("mv   %0, a1" : "=r" (resfile));
                                    asm volatile ("mv   %0, a2" : "=r" (size));
                                    asm volatile ("mv   %0, a3" : "=r" (addr));
                                    c=addr;
                                    res = f_read(resfile, c, size, &bw);
                                   
                                    asm volatile("sw %0,0(gp)" : : "r" (bw));
                                   
                                    //asm volatile("sw %0,0(gp)" : : "r" (c));
                                    //asm volatile("sw %0,0(gp)" : : "r" (bw));
                               }
                          break;
                          case SYSCALL_WRITE:
                                {
                                    char c;
                                    UINT bw;
                                    asm volatile ("mv   %0, a1" : "=r" (resfile));
                                    asm volatile ("mv   %0, a2" : "=r" (c));
                                    f_write(resfile,&c, 1, &bw);
                                }
                          break;
                          case SYSCALL_WRITEBUFF:
                                {
                                    UINT bw;
                                    int size;
                                    void *addr;
                                    char *buff;
                                    asm volatile ("mv   %0, a1" : "=r" (resfile));
                                    asm volatile ("mv   %0, a2" : "=r" (size));
                                    asm volatile ("mv   %0, a3" : "=r" (addr));
                                    buff=addr;
                                    f_write(resfile,buff, size, &bw);
                                }
                          break;
                          case SYSCALL_CREATETASK :

                          break;
                          case SYSCALL_DELETETASK :

                          break;
                          case SYSCALL_GETARG:
                                {
                                    int id;
                                    asm volatile ("mv   %0, a1" : "=r" (id));
                                    asm volatile("sw %0,0(gp)" : : "r" (tasks[id].arg));
                                }
                          break;
                          case SYSCALL_CONTEXT :
                               {  
                                
                                 //int id;
                                 // asm volatile ("mv   %0, a1" : "=r" (id));
                                  asm volatile("sw %0,0(gp)" : : "r" (current_task->stack));
                               }
                          break;
                          

                          case SYSCALL_GETPID :
                                asm volatile("sw %0,0(gp)" : : "r" (current_task->id));
                          break;
                          case SYSCALL_YIELD :
                               //asm volatile ("mv  a0, %0" : : "r" (Thread.stack));
                               //asm volatile ("mv  a0, %0" : : "r" (main_context.stack));
                               asm volatile("sw %0,0(gp)" : : "r" (main_context.stack));
                               //asm volatile("sw a0,0(gp)");
                              // reg1=(uint32_t)Thread.stack;
                          break;
                          case SYSCALL_WAIT :

                          break;
                          case SYSCALL_SLEEP :

                          break;

                          case SYSCALL_EXIT:
                               {
                                  int pid;
                                  asm volatile (
                                  "mv   %0, a1"
                                  : "=r" (pid));
                                  tasks[pid].state=KILL;
                                  tasks[pid].sleep=0;
                                  kfree(tasks[pid].entry);
                                  //kfree(tasks[pid].arg);
                                  kfree(tasks[pid].stack);
                                  current_task=&tasks[pid];
                                  next_task--;
                               }
                          break;
                          case SYSCALL_N_TASK :
                                asm volatile("sw %0,0(gp)" : : "r" (next_task));
                          break;
                          case SYSCALL_PERFIL_TASK:
                                {
                                  int pid;
                                  void *point;
                                  asm volatile ("mv   %0, a1" : "=r" (pid));
                                  perfil_task=&tasks[pid];
                                  asm volatile("sw %0,0(gp)" : : "r" (perfil_task));
                                }
                          break;
                          case SYSCALL_KILL :
                               {
                                  int pid;
                                  asm volatile (
                                  "mv   %0, a1"
                                  : "=r" (pid));
                                  tasks[pid].state=KILL;
                                  tasks[pid].sleep=0;
                                  myprintf("free entry\n");
                                  kfree(tasks[pid].entry);
                                  myprintf("free arg\n");
                                  kfree(tasks[pid].arg);
                                  myprintf("free stack\n");
                                  kfree(tasks[pid].stack);
                                  current_task=&tasks[pid];
                                  myprintf("killed :%i\n",pid);

                                  next_task--;
                               }
                          break;
                          case SYSCALL_PRINTSTR:
                               {  
                                  void *addr;
                                  asm volatile (
                                  "mv   %0, a1"
                                  : "=r" (addr));
                                  myprintf("%s",addr);
                               }
                          break;
                          case SYSCALL_MALLOC:
                                {
                                  int size;
                                  void *point;
                                  asm volatile (
                                  "mv   %0, a1"
                                  : "=r" (size));
                                  point=(void *)kmalloc(size*sizeof(void));
                                  asm volatile("sw %0,0(gp)" : : "r" (point));
                                }
                          break;
                          case SYSCALL_FREE:
                               {
                                  void *point;
                                  asm volatile (
                                  "mv   %0, a1"
                                  : "=r" (point));
                                  kfree(point);
                               }
                          break;
                          default:
                             myprintf("SYSCALL: %l\n",event);
                          break;
                  }
            }
        break;
    }
}
 

void __attribute__ ((section(".interrupt"))) isr(){    
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    //int_isr();
    /*if (MODE==1)
    {
       MODE=0;
       int pid=0;
       asm volatile ("lw %0,0(gp)" : "=r" (pid));
       tasks[pid].state=KILL;
       tasks[pid].sleep=0;
       myprintf("free entry\n");
       kfree(tasks[pid].entry);
       myprintf("free arg\n");
       kfree(tasks[pid].arg);
       myprintf("free stack\n");
       kfree(tasks[pid].stack);
       current_task=&tasks[pid];
       myprintf("killed :%i\n",pid);
       next_task--;

    }*/
    asm("uret");  
}


 
void __attribute__ ((section(".svc"))) syscall_isr(){    
    
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    int_isr();
    trap_ret();
    //asm("sret");   

    /*asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    int trap;
    asm volatile (
    "mv   %0, a7"
    : "=r" (trap));

    switch(trap){
        case 0x1237:
            {
                  int event;
                  asm volatile (
                  "mv   %0, a0"
                  : "=r" (event));
                  switch(event){
                          case SYSCALL_PRINTSTR :
                          {
                                  void *addr;
                                  asm volatile (
                                  "mv   %0, a1"
                                  : "=r" (addr));
                                  myprintf("%s",addr);
                               }
                          break;
                  }
                    trap_ret2();
             }
    }
    asm("sret");*/
}