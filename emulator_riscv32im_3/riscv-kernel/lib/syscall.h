#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <setjmp.h>


#ifndef SYSCALL_H
#define SYSCALL_H

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

void kputc(char ch);
void syscall(uint32_t num,uint32_t arg0,uint32_t arg1,uint32_t arg2);
int SerialAvailable();
void exit_cpu();
uint8_t SerialRead();
void yield(long long int point,long long int point2);
void kill_task(int pid);
void exit_();
void closefile();
int openfile(uint32_t str);
char readfile();
void readbuff(void *p,uint32_t si);
uint32_t filesize();
void calls();
uint32_t get_time();
void fprint(float f);
void run_task(int pid);
void* kmalloc(long size);
void mfree(void *p);


/*
unsigned long counter_timer;

#define OOS_MAX_TASKS   4     // number of tasks, set as many as required
#define OOS_STOP   -2
int task_cnt;    // counts registered coos tasks
int  task_delay[OOS_MAX_TASKS];
jmp_buf main_context;                         // context of scheduler
jmp_buf task_context[OOS_MAX_TASKS];
unsigned char task_no;

//#define COOS_DELAY()if (!setjmp(task_context[task_no])) {longjmp(main_context,1);}

void (*tsk_p[OOS_MAX_TASKS])(void);

void CreateTask(void (*tsk)(void));
void RunSystems(void);


int   res;
char  rdy;
void (*tsk)(void);
*/


#endif