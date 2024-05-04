asm("j main");
#include "../lib/nwlib.h"


char instructions_msg[]= " \
\n\
                SIFIVE, INC.\n\
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
char str[]={"daniel quintero\n"};
char *ptr;

int d=899;
long d2=1002255;
float f=99.8;


void main(int pid,uint32_t funcpointer){
ptr=str;

myprintf(ptr);
myprintf("%s\n",instructions_msg);
myprintf("%l , %l\n",d,d2);
fprint(f);
myprintf("\n");
strcpy(buff,"riscv32\n");
myprintf("%s\n",buff);

kill_task(pid);
}
