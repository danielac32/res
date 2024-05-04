//riscv32-unknown-elf-gcc -fpic -nostartfiles main.c -o rv32.elf  -march=rv32im -mabi=ilp32
//riscv32-unknown-elf-objcopy -O binary rv32.elf app.bin

asm("j main");
#include "../lib/applib.h"

 

void fibo(long n){
long t1=0,t2=1,nextTerm;
for (int i = 1; i <= n; ++i)
{
     myprintf("%l\n",t1);
     nextTerm = t1 + t2;
     t1=t2;
     t2=nextTerm;
}
myprintf("target: %l\n",n);
}


static long time __attribute__ ((section(".data")))=0;

void main(int pid,uint32_t funcpointer){

 myprintf("\n\n********************START APPLICATION**** (pid: %l),(pointer: %x)\n\n",pid,funcpointer);
/********************************/
//if(get_time()-time>= 1000){
  // time=get_time();
   myprintf("fibonacci start\n");
   fibo(100);
   myprintf("fibonacci end\n");
   //myprintf(instructions_msg);
   //myprintf("%l\n",time);
//}
   myprintf("%s\n",instructions_msg);
 myprintf("\n\n********************END APPLICATION********************\n\n");
//myprintf("%f\n",my);
myprintf("application killed........\n");
kill_task(pid);//exit task
}


