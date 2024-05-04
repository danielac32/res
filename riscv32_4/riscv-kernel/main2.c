
asm("li sp,62765  \n\t"
    "li gp,62766   \n\t"
    "j main         \n\t"
);


#include <stdint.h>
#include <string.h>
#include "lib/lib.h"
#include "lib/serial.h"




char instructions_msg[]= " \
\n\
              1 SIFIVE, INC.\n\
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
char instructions_msg2[]= " \
\n\
              2 SIFIVE, INC.\n\
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
char instructions_msg3[]= " \
\n\
              3 SIFIVE, INC.\n\
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

 char instructions_msg4[]= " \
\n\
              4 SIFIVE, INC.\n\
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
unsigned char rawData[246] = {
    0x6F, 0x00, 0xC0, 0x08, 0x13, 0x01, 0x01, 0xFE, 0x23, 0x2E, 0x81, 0x00,
    0x13, 0x04, 0x01, 0x02, 0x93, 0x07, 0x05, 0x00, 0xA3, 0x07, 0xF4, 0xFE,
    0xB7, 0xA7, 0x0F, 0x00, 0x93, 0x87, 0x27, 0x00, 0x03, 0x47, 0xF4, 0xFE,
    0x23, 0x80, 0xE7, 0x00, 0x13, 0x00, 0x00, 0x00, 0x03, 0x24, 0xC1, 0x01,
    0x13, 0x01, 0x01, 0x02, 0x67, 0x80, 0x00, 0x00, 0x13, 0x01, 0x01, 0xFE,
    0x23, 0x2E, 0x11, 0x00, 0x23, 0x2C, 0x81, 0x00, 0x13, 0x04, 0x01, 0x02,
    0x23, 0x26, 0xA4, 0xFE, 0x6F, 0x00, 0x00, 0x02, 0x83, 0x27, 0xC4, 0xFE,
    0x13, 0x87, 0x17, 0x00, 0x23, 0x26, 0xE4, 0xFE, 0x83, 0xC7, 0x07, 0x00,
    0x13, 0x85, 0x07, 0x00, 0x97, 0x00, 0x00, 0x00, 0xE7, 0x80, 0x00, 0xFA,
    0x83, 0x27, 0xC4, 0xFE, 0x83, 0xC7, 0x07, 0x00, 0xE3, 0x9E, 0x07, 0xFC,
    0x13, 0x00, 0x00, 0x00, 0x83, 0x20, 0xC1, 0x01, 0x03, 0x24, 0x81, 0x01,
    0x13, 0x01, 0x01, 0x02, 0x67, 0x80, 0x00, 0x00, 0x13, 0x01, 0x01, 0xFE,
    0x23, 0x2E, 0x11, 0x00, 0x23, 0x2C, 0x81, 0x00, 0x13, 0x04, 0x01, 0x02,
    0x97, 0x07, 0x00, 0x00, 0x93, 0x87, 0x07, 0x05, 0x23, 0x24, 0xF4, 0xFE,
    0x23, 0x26, 0x04, 0xFE, 0x6F, 0x00, 0xC0, 0x01, 0x03, 0x25, 0x84, 0xFE,
    0x97, 0x00, 0x00, 0x00, 0xE7, 0x80, 0x40, 0xF8, 0x83, 0x27, 0xC4, 0xFE,
    0x93, 0x87, 0x17, 0x00, 0x23, 0x26, 0xF4, 0xFE, 0x03, 0x27, 0xC4, 0xFE,
    0x93, 0x07, 0x90, 0x00, 0xE3, 0xD0, 0xE7, 0xFE, 0x13, 0x00, 0x00, 0x00,
    0x83, 0x20, 0xC1, 0x01, 0x03, 0x24, 0x81, 0x01, 0x13, 0x01, 0x01, 0x02,
    0x67, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x70, 0x75, 0x20,
    0x74, 0x65, 0x73, 0x74, 0x0A, 0x00
};
 


unsigned char rawData2[] = {
    0x6F, 0x00, 0xC0, 0x08, 0x13, 0x01, 0x01, 0xFE, 0x23, 0x2E, 0x81, 0x00,
    0x13, 0x04, 0x01, 0x02, 0x93, 0x07, 0x05, 0x00, 0xA3, 0x07, 0xF4, 0xFE,
    0xB7, 0xA7, 0x0F, 0x00, 0x93, 0x87, 0x27, 0x00, 0x03, 0x47, 0xF4, 0xFE,
    0x23, 0x80, 0xE7, 0x00, 0x13, 0x00, 0x00, 0x00, 0x03, 0x24, 0xC1, 0x01,
    0x13, 0x01, 0x01, 0x02, 0x67, 0x80, 0x00, 0x00, 0x13, 0x01, 0x01, 0xFE,
    0x23, 0x2E, 0x11, 0x00, 0x23, 0x2C, 0x81, 0x00, 0x13, 0x04, 0x01, 0x02,
    0x23, 0x26, 0xA4, 0xFE, 0x6F, 0x00, 0x00, 0x02, 0x83, 0x27, 0xC4, 0xFE,
    0x13, 0x87, 0x17, 0x00, 0x23, 0x26, 0xE4, 0xFE, 0x83, 0xC7, 0x07, 0x00,
    0x13, 0x85, 0x07, 0x00, 0x97, 0x00, 0x00, 0x00, 0xE7, 0x80, 0x00, 0xFA,
    0x83, 0x27, 0xC4, 0xFE, 0x83, 0xC7, 0x07, 0x00, 0xE3, 0x9E, 0x07, 0xFC,
    0x13, 0x00, 0x00, 0x00, 0x83, 0x20, 0xC1, 0x01, 0x03, 0x24, 0x81, 0x01,
    0x13, 0x01, 0x01, 0x02, 0x67, 0x80, 0x00, 0x00, 0x13, 0x01, 0x01, 0xFE,
    0x23, 0x2E, 0x11, 0x00, 0x23, 0x2C, 0x81, 0x00, 0x13, 0x04, 0x01, 0x02,
    0x97, 0x07, 0x00, 0x00, 0x93, 0x87, 0x07, 0x05, 0x23, 0x24, 0xF4, 0xFE,
    0x23, 0x26, 0x04, 0xFE, 0x6F, 0x00, 0xC0, 0x01, 0x03, 0x25, 0x84, 0xFE,
    0x97, 0x00, 0x00, 0x00, 0xE7, 0x80, 0x40, 0xF8, 0x83, 0x27, 0xC4, 0xFE,
    0x93, 0x87, 0x17, 0x00, 0x23, 0x26, 0xF4, 0xFE, 0x03, 0x27, 0xC4, 0xFE,
    0x93, 0x07, 0x90, 0x00, 0xE3, 0xD0, 0xE7, 0xFE, 0x13, 0x00, 0x00, 0x00,
    0x83, 0x20, 0xC1, 0x01, 0x03, 0x24, 0x81, 0x01, 0x13, 0x01, 0x01, 0x02,
    0x67, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x78, 0x65, 0x63,
    0x75, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x66, 0x72, 0x6f, 0x6d, 0x20, 0x72,
    0x61, 0x6d, 0x0A, 0x00
};



long time;
int tg=0;
void call(){
  if(timer - time >10){
         time=timer;
         if (!tg)
         {
           tg=0xff;
         }else{
           tg=0x00;
         }
         led=tg;
      }
}
void __attribute__ ((section(".interrupt"))) isr(){     
      
      asm("nop             \n\t"
          "lw  ra,12(sp)   \n\t"
          "lw  s0,8(sp)    \n\t"
          "addi  sp,sp,16  \n\t"
          "nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
      );
      asm("addi  sp,sp,-120 \n\t"

          "sw    ra,116(sp) \n\t"
          "sw    t6,112(sp) \n\t"
          "sw    t5,108(sp) \n\t"
          "sw    t4,104(sp) \n\t"
          "sw    t3,100(sp) \n\t"

          "sw    s11,96(sp) \n\t"
          "sw    s10,92(sp) \n\t"
          "sw    s9,88(sp)  \n\t"
          "sw    s8,84(sp)  \n\t"
          "sw    s7,80(sp)  \n\t"
          "sw    s6,76(sp)  \n\t"
          "sw    s5,72(sp)  \n\t"
          "sw    s4,68(sp)  \n\t"
          "sw    s3,64(sp)  \n\t"
          "sw    s2,60(sp)  \n\t"

          "sw    a7,56(sp)  \n\t"
          "sw    a6,52(sp)  \n\t"
          "sw    a5,48(sp)  \n\t"
          "sw    a4,44(sp)  \n\t"
          "sw    a3,40(sp)  \n\t"
          "sw    a2,36(sp)  \n\t"
          "sw    a1,32(sp)  \n\t"
          "sw    a0,28(sp)  \n\t"

          "sw    s1,24(sp)  \n\t"
          "sw    s0,20(sp)  \n\t"

          "sw    t2,16(sp)  \n\t"
          "sw    t1,12(sp)  \n\t"
          "sw    t0,8(sp)   \n\t"
          "sw    tp,4(sp)   \n\t"

          "sw    gp,0(sp)   \n\t"
          //"addi  s0,sp,16  \n\t"
        );
    
         call();


      asm("nop              \n\t"
          "lw    ra,116(sp) \n\t"
          "lw    t6,112(sp) \n\t"
          "lw    t5,108(sp) \n\t"
          "lw    t4,104(sp) \n\t"
          "lw    t3,100(sp) \n\t"

          "lw    s11,96(sp) \n\t"
          "lw    s10,92(sp) \n\t"
          "lw    s9,88(sp)  \n\t"
          "lw    s8,84(sp)  \n\t"
          "lw    s7,80(sp)  \n\t"
          "lw    s6,76(sp)  \n\t"
          "lw    s5,72(sp)  \n\t"
          "lw    s4,68(sp)  \n\t"
          "lw    s3,64(sp)  \n\t"
          "lw    s2,60(sp)  \n\t"

          "lw    a7,56(sp)  \n\t"
          "lw    a6,52(sp)  \n\t"
          "lw    a5,48(sp)  \n\t"
          "lw    a4,44(sp)  \n\t"
          "lw    a3,40(sp)  \n\t"
          "lw    a2,36(sp)  \n\t"
          "lw    a1,32(sp)  \n\t"
          "lw    a0,28(sp)  \n\t"

          "lw    s1,24(sp)  \n\t"
          "lw    s0,20(sp)  \n\t"

          "lw    t2,16(sp)  \n\t"
          "lw    t1,12(sp)  \n\t"
          "lw    t0,8(sp)   \n\t"
          "lw    tp,4(sp)   \n\t"
          
          "lw    gp,0(sp)   \n\t"
          "addi  sp,sp,120  \n\t"
          "uret             \n\t"
     ); 
   
}


char ch;
 
float f=0.0;
char *list[7]={"daniel","eduardo","quintero","villegas","veronica","garcia","herrera"};
void (*p)(void);
void main(){

     /*asm( "nop            \n\t"
          "nop            \n\t"
          "nop            \n\t"
          "nop            \n\t"
          "li a0,0  \n\t"
          "addi a0,sp,0  \n\t"
          "addi  sp,sp,-4 \n\t"
          "li a5,100      \n\t"
          "sw    a5,0(sp) \n\t"
          "nop            \n\t"
          "lw    a5,0(sp) \n\t"
          "addi  sp,sp,4  \n\t"
     );*/
  
  enable_timer=1;
  myprintf("riscv32imc test virtual cpu\n\r");

  for (int i = 0; i < 7; ++i)
  {
    myprintf("%s\n",list[i]);
  }
  for (int i = 0; i < 7; ++i)
  {
       strcpy(list[i],"empty");
  }
  for (int i = 0; i < 7; ++i)
  {
    myprintf("%s\n",list[i]);
  }

   myprintf("jump app\n");
  p=(void*)rawData2;
  (*p)();

  f=0;


  for (int i = 0; i < 10; ++i)
  {    
       floatprint(f);
       myprintf("\n\r");
       f+=0.1;
  }
  myprintf("%s",instructions_msg);
  myprintf("%s",instructions_msg2);
  myprintf("%s",instructions_msg3);
  myprintf("%s",instructions_msg4);
  myprintf("jump app\n");

  
  p=(void*)rawData;
  (*p)();
  
 
  myprintf("end\n");
  while(1){
     if (keyAvailable)
      {   
          ch=keyRead;
         
          myprintf("%c",ch);
      }
  }
}