

asm("j main");

#include <stddef.h>
#include <stdint.h>
#include "serial.h"
#define TX (*(volatile uint8_t *)0xFA030)



char instructions_msg[]= " \
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

/*
void print(char *str){
    while(*str){
       TX=*str++;
    }
}*/
void main(){
 myprintf("%s",instructions_msg);
 // print(instructions_msg);
for (int i = 0; i < 100; ++i)
{
     myprintf("%i\n",i);
}
uint16_t size=sizeof(instructions_msg);
uint16_t suma=0;

for (uint16_t i = 0; i < size; ++i)
{
     suma += instructions_msg[i];
}
myprintf("%l,%l\n",size,suma);
return;
}