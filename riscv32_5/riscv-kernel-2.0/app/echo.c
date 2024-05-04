

asm("j main");

#include <stddef.h>
#include <stdint.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/syscall.h"


 
 

 void main(){
  void *context;
  int pid=get_id();
  void *arg=get_args(pid);

  myprintf("%s\n",(char*)arg);
   // context=get_context();
    //k_yield(context);

  exit_task(pid);
  
}
