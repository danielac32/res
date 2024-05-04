asm("li sp,79997   \n\t"
    "li gp,79998   \n\t"
    "j main        \n\t"
);
 
 
#include <stdint.h>
#include <string.h>
#include <setjmp.h>
#include "lib/lib.h"
#include "lib/timer.h"
#include "lib/serial.h"
#include "lib/kmalloc.h"
 
 

void main(){
  enable_timer=1;
  mode=0;

   
  syscall_print("run kernel\n");
  while(1);
 
  _exit();
 
}