asm("j main");
#include <stddef.h>
#include <stdint.h>
#include "../include/reg.h"
#include "../include/serial.h"
 
void *r;
 
uint32_t __svc(uint32_t x,uint32_t x2){     
uint32_t res=0;              
uint32_t (*__svc__)(uint32_t,uint32_t)=(uint32_t (*)(uint32_t,uint32_t))SYSCALL_INTERRUPT_POINTER;//void (*__svc__)(void *,uint32_t)=(void (*)(void*,uint32_t))SYSCALL_INTERRUPT_POINTER;
res = __svc__(x,x2);//jump svc handler --- __svc__(&&label,x);
return res;
}

 
 

int main(void *p,uint8_t pid){
  //MOS_Continue(r);
  __svc(SYSCALL_PRINTSTR,*(uint32_t*)p);
  __svc(SYSCALL_KILL,pid);
}