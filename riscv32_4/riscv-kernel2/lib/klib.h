
#include <stdint.h>
#ifndef KLIB_H
#define KLIB_H
#define PRINTSTR 4
#define PRINTCHAR 11
#define SERIALAVAILABLE 12
#define SERIALREAD 13
#define YIELD 5
#define EXITTASK 200
#define EXITCPU 10
#define OPENFILE 14
#define READFILE 15
#define FILESIZE 16
#define CLOSEFILE 17
#define TIMER 18
#define FPRINT 19
#define RUN 20
#define ALLOC 21
#define FREE 22
#define KINIT 23
#define READFILEBUFF 24
 

void k_syscall(uint32_t num,uint32_t arg0,uint32_t arg1,uint32_t arg2);
uint8_t k_openfile(char *str);
void k_closefile();
uint8_t k_readfile();
void k_readbuff(void *p,uint32_t si);
uint32_t k_filesize();
int k_SerialAvailable();
uint8_t k_SerialRead();
void k_fprint(float f);
void k_print(char *str);
void k_exit_cpu();
void k_putc(char ch);
void k_kill_task(int pid);
void k_run_task(int pid);
uint32_t k_get_time();



#endif