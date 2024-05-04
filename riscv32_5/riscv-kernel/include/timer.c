#include <stdint.h>
#include "reg.h"
#include "timer.h"
#include "serial.h"
#include "task.h"
#include "syscall.h"
#include "kmalloc.h"
/*
uint32_t i=0;

uint32_t array[20];
uint32_t array2[20];


void call(){
    //myprintf("\tinterrupt: %i\t",i++);
	for (int i = 0; i < 20; ++i)
	{
		array2[i]=i*2;
		array[i]=array2[i];
	}
	for (int i = 0; i < 20; ++i)
	{
		i=array2[i]+array[i];
	}
	i++;
}
*/
void __attribute__ ((section(".interrupt"))) isr(){    
       //save_context;
/*asm("nop");               
asm("nop");               
asm("nop");               
asm("nop");               
asm("nop");               
asm("nop");               
//save_context;

call();

//restore_context;
asm("uret");  */     


}   



#define mem(x) (*(volatile uint8_t *)x)
int count=0;
uint32_t __attribute__ ((section(".svc"))) syscall_isr(uint32_t op,uint32_t x){     

switch(op){
     case SYSCALL_PRINTSTR:
          {
             char *p;
             p = (uint8_t*)x;
             myprintf("%s",p);
             /*uint32_t point = x;
             while(mem(point)!='\0'){
                myprintf("%c",(char)mem(point));
                point++;
             }*/

          }
     break;
     case SYSCALL_GETPID:
          return current;
     break;
     case SYSCALL_KILL:
          {
            int err= kill(x);
            if(!err){
               //myprintf("killed: %i\n",x);
            }else{
               myprintf("kill error : %i\n",x);
            }
          }
     break;
     case SYSCALL_MALLOC:
          {
             void *p=(void*)kmalloc(x*sizeof(void));
             return (uint32_t)p;
          }
     break;
     case SYSCALL_FREE:
          {
            kfree((void*)x);
          }
     break;
     case SYSCALL_OPEN:
          {
             char *p;
             p = (uint8_t*)x;
            // myprintf("file: %s\n",p);
             return k_OPEN(p);
          }
     break;
     case SYSCALL_FSIZE:
          {
             return k_FSIZE();
          }
     break;
     case SYSCALL_READ:
          {
             return k_READ();
          }
     break;
     case SYSCALL_CLOSE:
          k_CLOSE();
     break;
}
return 0;
/*
if(op==SYSCALL_PRINTSTR){
    uint32_t point = x;
    while(mem(point)!='\0'){
          myprintf("%c",(char)mem(point));
          point++;
    }
}

if(op==SYSCALL_GETPID){
    return current;
}

if(op==SYSCALL_KILL){
 
   int err= kill(x);
   if(!err){
   	  //myprintf("killed: %i\n",x);
   }else{
   	  myprintf("kill error : %i\n",x);
   }
}

return 0;*/
}