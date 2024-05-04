asm("li sp,44997   \n\t"
    "li gp,44998   \n\t"
    "j main        \n\t"
);
/*
asm("li sp,32765   \n\t"
    "li gp,32766   \n\t"
    "jal ra,setup  \n\t"
    "j main        \n\t"
);
*/
 
#include <stdint.h>
#include <string.h>
#include "lib/lib.h"
#include "lib/serial.h"

int flag=0;
long h=0;

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
  0x67, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', 'a', 'p', 'p', ' ',
  'i', 'n', 's', 'i','d','e',' ','r','a','m', 0x0A, 0x00
};


static unsigned char rawData[246] = {
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
void call();


void __attribute__ ((section(".interrupt"))) isr(){     
      
      asm("nop              \n\t"
      	  "nop              \n\t"
      	  
      	  "addi  sp,sp,-120 \n\t"
           
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
        );
    
         asm("nop              \n\t"
             "nop              \n\t"
             "nop              \n\t"
             "nop              \n\t");
           
             call();

         asm("nop              \n\t"
             "nop              \n\t"
             "nop              \n\t"
             "nop              \n\t");
             
   
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

#define PRINTSTR 4
#define PRINTCHAR 11
#define SERIALAVAILABLE 12
#define SERIALREAD 13
#define YIELD 5
#define EXITTASK 200
#define EXITCPU 10
#define OPENFILE 14
#define READFILE 15
#define FILESIZE 16
#define CLOSEFILE 17
#define TIMER 18
#define FPRINT 19
#define RUN 20
#define ALLOC 21
#define FREE 22
#define KINIT 23
#define READFILEBUFF 24

void syscall(uint32_t num,uint32_t arg0,uint32_t arg1,uint32_t arg2){
register uint32_t a7 asm("a7")=num;
register uint32_t a0 asm("a0")=arg0;
register uint32_t a1 asm("a1")=arg1;
register uint32_t a2 asm("a2")=arg2;
asm("ecall");
}

uint8_t openfile(char *str){
syscall(OPENFILE,(uint32_t)str,0,0);
return workregister2;
}
void closefile(){
syscall(CLOSEFILE,0,0,0);
}
uint8_t readfile(){
syscall(READFILE,0,0,0);
return (uint8_t)workregister3;
}

void readbuff(void *p,uint32_t si){
syscall(READFILEBUFF,(uint32_t)p,si,0);
}

uint32_t filesize(){
syscall(FILESIZE,0,0,0);
return workregister3;
}

int SerialAvailable(){
workregister2=SERIALAVAILABLE;
asm("nop");
return keyAvailable;
}

uint8_t SerialRead(){
workregister2=SERIALREAD;
asm("nop");
return (uint8_t)keyRead;
}

void fprint(float f){
char buff[30];
ftoa(f,buff,2);
myprintf("%s",buff);
}

void exit_cpu(){
syscall(EXITCPU,0,0,0);
}
 
void kputc(char ch){
syscall(PRINTCHAR,(uint32_t)ch,0,0);
}

void kill_task(int pid){
workregister1=pid;
workregister2=EXITTASK;
asm("nop");
}
void run_task(int pid){
workregister1=pid;
workregister2=RUN;
asm("nop");
}

uint32_t get_time(){
return timer;
}


void call(){
    if(flag){
       if(!(h % 100)){
           myprintf("                                 interrupt\n");
       }
       if(!(h % 1)){
            if(SerialAvailable()){
               char p=SerialRead();
               
               if(p=='x')exit_cpu();
            }
       }
       h++;
    }       
}


float f=0.0;
char *list[7]={"daniel","eduardo","quintero","villegas","veronica","garcia","herrera"};




void main(){


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

  /*for (int i = 0; i < 10; ++i)
  {    
       floatprint(f);
       myprintf("\n\r");
       f+=0.1;
  }*/

asm("nop");
asm("nop");
myprintf("%s",instructions_msg);
myprintf("jump app\n");

void (*p)(void);
p=(void*)rawData;
(*p)();



 

myprintf("x for init\n");
while(1){
    if(SerialAvailable()){
       char p=SerialRead();
       myprintf("%c",p);
       if(p=='x')break;
    }
}
//mode=0;
char *buff;

uint8_t i=openfile("link.ld");
char data;
uint32_t size=0;
uint32_t l=0;

if(!i){ 
   size=filesize();
   myprintf("file ok:  %l\n",size);
   
   while(l<size){
         data=readfile();
         myprintf("%c",data);
         l++;
   }
   l=0;

}else myprintf("file not found\n");

i=openfile("main.c");
if(!i){
   size=filesize();
   myprintf("file ok:  %l\n",size);
   
   while(l<size){
         data=readfile();
         myprintf("%c",data);
         l++;
   }
   l=0;

}else myprintf("file not found\n");

i=openfile("basic.bas");
if(!i){
   size=filesize();
   myprintf("file ok:  %l\n",size);
   
   while(l<size){
         data=readfile();
         myprintf("%c",data);
         l++;
   }
   l=0;

}else myprintf("file not found\n");

enable_timer=1;
long f=0;
flag=1;
long h;
while(1){

   for (int i = 0; i < 3; ++i)
   {
      f+=i+(f|i);
      myprintf("%l\n",f);
   }
   if(get_time()-h>100){
      h=get_time();
      void (*p)(void);
      p=(void*)rawData2;
      (*p)();
   }
   
}
}//instruction: fce42623 , pc: 00000ea8