#include <stdint.h>
#include "reg.h"
#include "timer.h"
#include "serial.h"
//#include "kmalloc.h"
#include "shell.h"
#include "syscall.h"
#include "task.h"

/*
void k_yield(void *stack){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_YIELD));
  asm volatile ("ecall");
  if (!setjmp(stack)) {                               
       asm volatile("lw a0,0(gp)");                     
       asm volatile("j longjmp");                      
  }
}
*/

int get_num_task(){
  int result;
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              \
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_N_TASK)); 
  asm volatile ("ecall");
  asm volatile ("lw %0,0(gp)" : "=r" (result));
  return result;
}

void *struct_task(int pid){
  void *result;
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              \
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_PERFIL_TASK));
  asm volatile ("mv a1, %0" : : "r" (pid)); 
  asm volatile ("ecall");
  asm volatile ("lw %0,0(gp)" : "=r" (result));
  return result;
}


void *get_args(int pid){
  void *result;
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              \
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_GETARG));
  asm volatile ("mv a1, %0" : : "r" (pid)); 
  asm volatile ("ecall");
  asm volatile ("lw %0,0(gp)" : "=r" (result));
  return result;
}

int get_id(){
  int result;
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              \
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_GETPID)); 
  asm volatile ("ecall");
  asm volatile ("lw %0,0(gp)" : "=r" (result));
  return result;
}
void *get_context(){
  void *result;
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              \
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_CONTEXT)); 
  //asm volatile ("mv a1, %0" : : "r" (id)); 
  asm volatile ("ecall");
  asm volatile ("lw %0,0(gp)" : "=r" (result));
  return result;
}

int exist(char *path){
  int result;
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              \
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_EXIST)); 
  asm volatile ("mv a1, %0" : : "r" ((uint32_t)path)); 
  asm volatile ("ecall");
  asm volatile ("lw %0,0(gp)" : "=r" (result));
  return result;
}

int openFile(char *path,FIL *file,int mode){
  int result;
  void *r;
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              \
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_OPEN)); 
  asm volatile ("mv a1, %0" : : "r" ((uint32_t)path)); 
  asm volatile ("mv a2, %0" : : "r" (file)); 
  asm volatile ("mv a3, %0" : : "r" (mode)); 
  asm volatile ("ecall");
  asm volatile ("lw %0,0(gp)" : "=r" (result));
  //asm volatile ("lw %0,4(gp)" : "=r" (r));
  //file=r;
  return result;
}
void close(FIL *file){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_CLOSE)); 
  asm volatile ("mv a1, %0" : : "r" (file)); 
  asm volatile ("ecall");
}

void seek(FIL *file,uint32_t pos){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_SEEK));
  asm volatile ("mv a1, %0" : : "r" (file)); 
  asm volatile ("mv a2, %0" : : "r" (pos)); 
  asm volatile ("ecall");
}

void readbyte(FIL *file,char *data){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_READ));
  asm volatile ("mv a1, %0" : : "r" (file)); 
  asm volatile ("ecall");
  asm volatile ("lw %0,0(gp)" : "=r" (*data));
}

int readbuff(FIL *file,char *buff,int size){
  void *addr;
  int res=0;
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_READBUFF));
  asm volatile ("mv a1, %0" : : "r" (file)); 
  asm volatile ("mv a2, %0" : : "r" (size)); 
  asm volatile ("mv a3, %0" : : "r" ((uint32_t)buff)); 
  asm volatile ("ecall");
  //asm volatile ("lw %0,0(gp)" : "=r" (addr));
  asm volatile ("lw %0,0(gp)" : "=r" (res));
  return res;
  //buff=addr;
}

void writebyte(FIL *file,char data){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_WRITE));
  asm volatile ("mv a1, %0" : : "r" (file)); 
  asm volatile ("mv a2, %0" : : "r" (data)); 
  asm volatile ("ecall");
}

void writebuff(FIL *file,char *buff,int size){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_WRITEBUFF));
  asm volatile ("mv a1, %0" : : "r" (file)); 
  asm volatile ("mv a2, %0" : : "r" (size)); 
  asm volatile ("mv a3, %0" : : "r" ((uint32_t)buff)); 
  asm volatile ("ecall");
}

void kprint(char *str){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_PRINTSTR));
  asm volatile ("mv a1, %0" : : "r" ((uint32_t)str)); 
  asm volatile ("ecall");
}
void exit_task(int pid){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_EXIT));
  asm volatile ("mv a1, %0" : : "r" (pid)); 
  asm volatile ("ecall");
}
void kill(int pid){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_KILL));
  asm volatile ("mv a1, %0" : : "r" (pid)); 
  asm volatile ("ecall");
}

void *k_malloc(int length){
  void *addr;
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_MALLOC));
  asm volatile ("mv a1, %0" : : "r" (length)); 
  asm volatile ("ecall");
  asm volatile ("lw %0,0(gp)" : "=r" (addr));
  return addr;
}

void k_free(void *ptr){
  asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              
  asm volatile ("mv a0, %0" : : "r" (SYSCALL_FREE));
  asm volatile ("mv a1, %0" : : "r" (ptr)); 
  asm volatile ("ecall");
}