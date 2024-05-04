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


  //__svc(SYSCALL_PRINTSTR,*(uint32_t*)p);


  char *buff = (char *)__svc(SYSCALL_MALLOC,100);
  if(buff!=NULL){
     //__svc(SYSCALL_PRINTSTR,(uint32_t)"malloc ok\n");

     uint32_t file = __svc(SYSCALL_OPEN,*(uint32_t*)p);


     if(!file){
     	__svc(SYSCALL_PRINTSTR,(uint32_t)"file ok\n");
     	uint32_t size = __svc(SYSCALL_FSIZE,0);
        
         char c;
         for (uint32_t i = 0; i < size; ++i){  
              c = __svc(SYSCALL_READ,0);

              myprintf("%c",(char)c);
         }

         __svc(SYSCALL_CLOSE,0);
     }else{
       __svc(SYSCALL_PRINTSTR,(uint32_t)"file fail\n");
     }

  	 __svc(SYSCALL_FREE,(uint32_t)buff);
  }else{
  	__svc(SYSCALL_PRINTSTR,(uint32_t)"error malloc\n");
  }

  __svc(SYSCALL_KILL,pid);
}