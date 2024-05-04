asm("j main");

#include <stddef.h>
#include <stdint.h>
#include "serial.h" 

#define base_io 0xFA000
#define MODE (*(volatile uint8_t *)0xFA001)
#define TX (*(volatile uint8_t *)0xFA002)
#define TIMER0 (*(volatile uint32_t *)0xFA003)
#define ENT0 (*(volatile uint8_t *)0xFA004)
#define ENRX (*(volatile uint8_t *)0xFA005)
#define RX (*(volatile uint8_t *)0xFA006)
#define LED0 (*(volatile uint8_t *)0xFA007)


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

int main(){
 myprintf("%s",instructions_msg);
 return 0;
}