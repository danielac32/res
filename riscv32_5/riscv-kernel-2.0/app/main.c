

asm("j main");

#include <stddef.h>
#include <stdint.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/syscall.h"


static char instructions_msg[]= " \
\n\
                   riscv.\n\
\n\
         5555555555555555555555555\n\
        5555                   5555\n\
       5555                     5555\n\
      5555                       5555\n\
     5555       5555555555555555555555\n\
    5555       555555555555555555555555\n\
   5555                             5555\n\
  5555                               5555\n\
 5555                                 5555\n\
5555555555555555555555555555          55555\n\
 55555           555555555           55555\n\
   55555           55555           55555\n\
     55555           5           55555\n\
       55555                   55555\n\
         55555               55555\n\
           55555           55555\n\
             55555       55555\n\
               55555   55555\n\
                 555555555\n\
                   55555\n\
                     5\n\
\n\
\n\
 ";

 

 

void main(){
  myprintf("%s\n",instructions_msg);
  static int counter=0;
  void *context;
  static int pid;
  pid=get_id();
  static char *arg;
  arg=get_args(pid);
  
  while(1){
      //int pid=get_id();
      //char *arg=get_args(pid);
      myprintf("%i,%s\n",counter,arg);
      counter++;
      context=get_context();
      k_yield(context);

  }
}