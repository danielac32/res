#include <xinu.h>

void __attribute__((naked))  svccall_handler_c(uint32 *sp){
  asm("cpsid  i");
  uint32 svc_number = ((char *)sp[6])[-2];
  uint32 svc_arg1 = sp[0];
  uint32 svc_arg2 = sp[1];
  uint32 result = svc_arg1 + svc_arg2;

 
  switch(svc_number){
      case 0:
         *((uint32*)0xE000ED04) = (1 << 28);
      break;
      case 1:
           
      break;
      case 5:
         printf("aquiiiiiiiiiiiiiiiiiiiiiiiiiii: %d , %d\n",svc_arg1,svc_arg2);
      break;
  }
  asm volatile("cpsie  i\n"
               "ldr r0,=0xFFFFFFFD\n"
               "bx r0\n");
 // *((uint32_t*)0xE000ED04) = (1 << 28);//PEND_SV();
}
