#include <stdint.h>
#include "klib.h"
#include "lib.h"
#include "task.h"
#include "serial.h"


void k_syscall(uint32_t num,uint32_t arg0,uint32_t arg1,uint32_t arg2){
register uint32_t a7 asm("a7")=num;
register uint32_t a0 asm("a0")=arg0;
register uint32_t a1 asm("a1")=arg1;
register uint32_t a2 asm("a2")=arg2;
asm("ecall");
}


//
void k_print(char *str){
   k_syscall(PRINTSTR,(uint32_t)str,0,0);
}
uint8_t k_openfile(char *str){
k_syscall(OPENFILE,(uint32_t)str,0,0);
return workregister2;
}
void k_closefile(){
k_syscall(CLOSEFILE,0,0,0);
}
uint8_t k_readfile(){
k_syscall(READFILE,0,0,0);
return (uint8_t)workregister3;
}

void k_readbuff(void *p,uint32_t si){
k_syscall(READFILEBUFF,(uint32_t)p,si,0);
}

uint32_t k_filesize(){
k_syscall(FILESIZE,0,0,0);
return workregister3;
}

int k_SerialAvailable(){
workregister2=SERIALAVAILABLE;
asm("nop");
return keyAvailable;
}

uint8_t k_SerialRead(){
workregister2=SERIALREAD;
asm("nop");
return (uint8_t)keyRead;
}

void k_fprint(float f){
char buff[30];
ftoa(f,buff,2);
myprintf("%s",buff);
}

void k_exit_cpu(){
k_syscall(EXITCPU,0,0,0);
}
 
void k_putc(char ch){
k_syscall(PRINTCHAR,(uint32_t)ch,0,0);
}

void k_kill_task(int pid){
workregister1=pid;
workregister2=EXITTASK;
asm("nop");
}
void k_run_task(int pid){
workregister1=pid;
workregister2=RUN;
asm("nop");
}

uint32_t k_get_time(){
return timer;
}
