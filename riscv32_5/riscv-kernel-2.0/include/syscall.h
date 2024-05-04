#include <stdint.h>
#include "ff.h"

#ifndef SYSCALL_H_
#define SYSCALL_H_

extern int setjmp(void *env);
extern void longjmp(void *env,int t); //


/*
#define k_yield(stack){                                    \
      void *__stack__;                                     \
      trap_call(SYSCALL_YIELD,0,0,0,0);                    \
      if (!setjmp(stack)) {                                \
          asm volatile("lw a0,0(gp)");                     \
          asm volatile ("mv   %0, a0" : "=r" (__stack__)); \
          longjmp(__stack__);                              \
      }                                                    \
}
*/


#define k_yield(stack){                                    \
      asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));      \
      asm volatile ("mv a0, %0" : : "r" (SYSCALL_YIELD));  \
      asm volatile ("ecall");                              \
      if (!setjmp(stack)) {                                \
          asm volatile("lw a0,0(gp)");                     \
          asm volatile("li a1,1");                         \
          asm volatile("j longjmp");                       \
      }                                                    \
}



int get_num_task();//return next_task
void *struct_task(int pid);

int exist(char *path);
int openFile(char *path,FIL *file,int mode);
void close(FIL *file);
void seek(FIL *file,uint32_t pos);
void readbyte(FIL *file,char *data);
int readbuff(FIL *file,char *buff,int size);
void writebyte(FIL *file,char data);
void writebuff(FIL *file,char *buff,int size);
void kprint(char *str);
void kill(int pid);
void *k_malloc(int length);
void k_free(void *ptr);
void deleteFile(char *path);
void *get_context();
int get_id();
void *get_args(int pid);
void exit_task(int pid);

/*
#define openFile(path,file,result,mode)\
trap_call(SYSCALL_OPEN,(uint32_t)path,file,0,0);\
asm volatile("lw %0,0(gp)" : "=r" (*result));\
asm volatile("lw %0,4(gp)" : "=r" (*file));*/

/*
#define k_yield(x,y)                      \
k_get_next_context(&y);              \
if (!setjmp(x)) {                  \
  longjmp(y);                  \
}
*/
/*
#define k_get_next_context(main_context){  \
reg2=88;                                \
MODE=1;                                  \
     while(MODE){                        \
      asm("nop");                      \
      asm("nop");                      \
     }                                   \
*main_context=(void*)reg1;               \
}

 
#define k_kill_task(pid){                   \
reg2=2;                                   \
reg1=pid;                                 \
MODE=1;                                  \
     while(MODE){                        \
      asm("nop");                      \
      asm("nop");                      \
     }                                   \
}
*/

//f_mount (FATFS* fs, const TCHAR* path, BYTE opt);


#endif