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
/*asm volatile (
"mv   %0, a0"
: "=r" (res));*/
return res;
}

 
 

int main(void *p){
 
  MOS_Continue(r);
 
  __svc(SYSCALL_PRINTSTR,*(uint32_t*)p);
  int current;

	//for(;;){  
          
         
         MOS_Break(r);
         current = __svc(SYSCALL_GETPID,0);
 
         __svc(SYSCALL_KILL,current);
         myprintf("fin de la tarea\n");
         
 

         /* for (int i = 0; i < 100; ++i){
               myprintf("task2 |counter: %i|\n",i);*/
               
               
        
        //  }    
 
	//}
}