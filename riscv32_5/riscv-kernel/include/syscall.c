#include <stdint.h>
#include "reg.h"
#include "timer.h"
#include "serial.h"
#include "kmalloc.h"
#include "shell.h"
#include "syscall.h"


uint32_t __svc(uint32_t x,uint32_t x2){     
uint32_t res=0;              
uint32_t (*__svc__)(uint32_t,uint32_t)=(uint32_t (*)(uint32_t,uint32_t))SYSCALL_INTERRUPT_POINTER;//void (*__svc__)(void *,uint32_t)=(void (*)(void*,uint32_t))SYSCALL_INTERRUPT_POINTER;
res = __svc__(x,x2); 
return res;
}



//estas llamadas son del emulador en gcc
uint8_t k_READ(){
asm volatile (
    "mv   a7, %0"
    : : "r" (SYSCALL_READ));

asm volatile (
    "mv   a0, %0"
    : : "r" (0xFFFF));

//register uint32_t a7 asm("a7")=SYSCALL_READ;
//register uint32_t a0 asm("a0")=0xFFFF;   //syscall event cpu                      
asm("ecall"); 
return (uint8_t)WRG1;//return available file
}


void k_READBUFF(uint8_t *buff,uint8_t size){

asm volatile (
    "mv   a7, %0"
    : : "r" (SYSCALL_READBUFF));

asm volatile (
    "mv   a0, %0"
    : : "r" (size));

asm volatile (
    "mv   a0, %0"
    : : "r" ((uint32_t)buff));


//register uint32_t a7 asm("a7")=SYSCALL_READBUFF;
//register uint32_t a0 asm("a0")=size;   //syscall event cpu     
//register uint32_t a1 asm("a1")=(uint32_t)buff;   //syscall event cpu  
asm("ecall"); 
}

void k_WRITE(uint8_t val){
	
}
void k_WRITEBUFF(uint8_t *buff,uint8_t size){
	
} 
uint8_t k_OPEN(char *file){
asm volatile (
    "mv   a7, %0"
    : : "r" (SYSCALL_OPEN));

asm volatile (
    "mv   a0, %0"
    : : "r" ((uint32_t)file));

/*
register uint32_t a7 asm("a7")=SYSCALL_OPEN;
register uint32_t a0 asm("a0")=(uint32_t)file;*/   //syscall event cpu                      
asm("ecall"); 
return (uint8_t)WRG1;//return available file
}

void k_CLOSE(){
asm volatile (
    "mv   a7, %0"
    : : "r" (SYSCALL_CLOSE));

asm volatile (
    "mv   a0, %0"
    : : "r" (0xFFFF));

/*
register uint32_t a7 asm("a7")=SYSCALL_CLOSE;
register uint32_t a0 asm("a0")=0xFFFF;   //syscall event cpu 
*/                     
asm("ecall"); 
}
void k_SEEK(uint32_t point){
asm volatile (
    "mv   a7, %0"
    : : "r" (SYSCALL_SEEK));

asm volatile (
    "mv   a0, %0"
    : : "r" (point));

/*
register uint32_t a7 asm("a7")=SYSCALL_SEEK;
register uint32_t a0 asm("a0")=point;   //syscall event cpu  
*/                    
asm("ecall");
}
uint32_t k_FSIZE(){
asm volatile (
    "mv   a7, %0"
    : : "r" (SYSCALL_FSIZE));

asm volatile (
    "mv   a0, %0"
    : : "r" (0xFFFF));

/*
register uint32_t a7 asm("a7")=SYSCALL_FSIZE;
register uint32_t a0 asm("a0")=0xFFFF;   //syscall event cpu 
*/                     
asm("ecall"); 
return WRG1; 
} 
uint8_t k_OPENDIR(char *dir){
	
}

///////////////////////////////////////////////////////////////////////////