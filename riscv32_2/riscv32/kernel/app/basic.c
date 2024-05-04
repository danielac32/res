asm("j main");
#include <stdint.h>
#include <string.h>
//#include "../lib/applib.h"

#define reg (*(volatile char *)0xFA002)
#define mode (*(volatile char *)0xFA001)
#define workregister1 (*(volatile uint32_t *)0xFA008)
#define workregister2 (*(volatile uint32_t *)0xFA009)

void putc_( char ch){
     reg=ch;
} 
void print( char *str){
  while(*str){
    putc_(*str++);
  }
}



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


void kill_task(int pid){
workregister1=pid;
workregister2=200;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

int d=899;
long d2=1002255;
float f=99.8;


void main(int pid,uint32_t funcpointer){
ptr=str;


print(ptr);
print(instructions_msg);
if(d==899)print("ok d\n");
if(d2==1002255)print("ok d2\n");
if(f<100 && f>99)print("ok float\n");

kill_task(pid);

}
