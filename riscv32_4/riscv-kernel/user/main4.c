//riscv32-unknown-elf-gcc -fpic -nostartfiles main.c -o rv32.elf  -march=rv32im -mabi=ilp32
//riscv32-unknown-elf-objcopy -O binary rv32.elf app.bin

asm("j main");
#include "../lib/applib.h"


float calc(float a,float b){
return (a)*(b);
}

void fibo(long n){
long t1=0,t2=1,nextTerm;
for (int i = 1; i <= n; ++i)
{
     myprintf("%l\n",t1);
     nextTerm = t1 + t2;
     t1=t2;
     t2=nextTerm;
}
}

void main(int pid,uint32_t funcpointer){

 myprintf("\n\n********************START APPLICATION**** (pid: %l),(pointer: %x)\n\n",pid,funcpointer);

 
ptr=&num;
myprintf("test pointer: %i, %i\n",num,*ptr);
myprintf("%i, %i\n",num,count);
fprint(44.99);
myprintf("\n");
fprint(my);
 

 asm("nop \n\t"
    "nop \n\t"
    "nop \n\t"
    "nop \n\t"
    "nop \n\t"
    "nop \n\t"
    "nop \n\t"
    "nop \n\t"
);

bool test=false;
if(!test){
    myprintf("\n\n\n**********************************************************\n");
    myprintf("application running........\n");
    int l=99;
    myprintf("int : %i\n",l);
    myprintf("str : %s\n",instructions_msg);
    myprintf("\n\n\n**********************************************************\n");
}
test=true;
if(test){
  /*
itoa(buff2,19877);
myprintf("%s\n",buff2);

itoh(buff2,1024);
myprintf("%s\n",buff2);
*/
//count=0;
float a=777.99;
float b=66.01;
long time=0;
calc(a,b);
fprint(calc(a,b));
myprintf("\n");

 

myprintf("daniel quintero\n");
myprintf("prueba riscv\n"); 

myprintf("%s",str);
myprintf("%s",str2);
myprintf("%s",str3);
myprintf("%s",str4);


while(count<100){
 
      myprintf(". %i - ",count);
      fprint(my);
      myprintf("\n");

      my+=0.1;
      count++;
    }   
/********************************/
myprintf("fibonacci start\n");
fibo(50);
myprintf("fibonacci end\n");
myprintf("%s",str5);
count=100;


while(count!=0){
if(get_time()-time>= 100){
   time=get_time();
   myprintf("into timer : ");
   myprintf("%i\n",count);
 
   count--;
}
}
//myprintf(instructions_msg);
//myprintf("%f\n",my);
}
 myprintf("\n\n********************END APPLICATION********************\n\n");
myprintf("application killed........\n");
kill_task(pid);//exit task
}


