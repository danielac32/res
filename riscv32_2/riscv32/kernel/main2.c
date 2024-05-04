#include <stdint.h>
#include <stdlib.h>
#include "lib/timer.h"
#define reg (*(volatile char *)0x300000)
#define mode (*(volatile char *)0x300001)
#define timer (*(volatile uint32_t *)0x300002)
#define enable_timer (*(volatile char *)0x300003)

void putc_( char ch){
     reg=ch;
} 
void print( char *str){
  while(*str){
    putc_(*str++);
  }
}
void reverse(char *str,int len){
  int i=0,j=len-1,temp;
  while(i<j){
    temp = str[i];
    str[i]=str[j];
    str[j]=temp;
    i++;
    j--;
  }
}

int intToStr(int x,char str[],int d){
  int i=0;
  while(x){
    str[i++]=(x%10)+'0';
    x=x/10;
  }
  while(i<d){
    str[i++]='0';
  }
  reverse(str,i);
  str[i]='\0';
  return i;
}
float _pow_(float base,float ex){
  if(ex==0){
    return 1;
  }else if(ex<0){
    return 1/_pow_(base,-ex);
  }else if((int)ex %2 ==0){
    float half__pow_=_pow_(base,ex/2);
    return half__pow_ * half__pow_;
  }else{
    return base * _pow_(base,ex-1);
  }
}
void ftoa(float n,char *res,int afterpoint){
  int ipart=(int)n;
  float fpart = n -(float)ipart;
  int i= intToStr(ipart,res,0);
  if(afterpoint!=0){
     res[i]='.';
     fpart=fpart * _pow_(10,afterpoint);
     intToStr((int)fpart,res+i+1,afterpoint);
  }
}


int counter=0;
char buff3[80];

void call(){
 asm("nop");
 asm("nop");
 print("                             ");
intToStr(counter,buff3,10);
print(buff3);
print("\n");
 counter++;
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
    /*
    asm("nop \n\t"  
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
    ); */ 
    /*asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    call();
    asm("nop");
    asm("lw      ra,12(sp)");
    asm("lw      s0,8(sp)");
    asm("addi    sp,sp,16");
    asm("uret");*/
}




float my=0.00;

char buff2[100];
long count=0;
char *str="printf test\n";
char array[]={'d','a','n','i','e','l','\n'};
void main(){
	mode=0;
  enable_timer=1;


	print(str);
  my=9.88;

  ftoa(my,buff2,2);
  print(buff2);
  print("\n");
  my=0.00;

for (int i = 0; i < 7; ++i)
{
     reg=array[i];
}

	  while(count<10000){
      intToStr(count,buff2,10);
      print(buff2);
      print("   ");

      ftoa(my,buff2,2);
      print(buff2);
      print("\n");
      my+=0.01;
      count++;
    }   
    for (int i = 0; i < 10; ++i)
    {
    	putc_('a');
    }
    if (my>=9.1)
    {
      print("\nfloat ok\n");
    }
    print("\nprintf end\n");
    asm("nop");
    asm("nop");
   while(1);

 }