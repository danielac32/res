#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define reg (*(volatile char *)0xFA01)
#define sfr1 (*(volatile uint32_t *)0xFA02)
#define sfr2 (*(volatile uint32_t *)0xFA03)
#define TIMER         (*(volatile uint32_t *)0xfa04)
#define ENABLE_TIMER  (*(volatile char *)0xfa05)

#define exit()         \
asm("li a7,10 \n\t"    \
    "ecall    \n\t");

void putc_( char ch){
     reg=ch;
} 
void print( char *str){
  while(*str){
    putc_(*str++);
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
      //calls();
         print("interrupt\n");
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

 int intToStr(int32_t x,char str[],int d){
  int32_t i=0;
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

float f=0.1;
char buff[10];
void main(){
	  asm("nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
      );
     ENABLE_TIMER=0;
     asm("nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
      );
     print("RISCV32IMC cpu 64k memory\n");

     for (int i = 0; i < 100; ++i)
     {
     	ftoa(f,buff,2);
     	print(buff);
     	print("\n");
     	f+=0.1;
     }
     print("end test\n");
     ENABLE_TIMER=1;
     //exit();
     while(1){
     	asm("nop");
     }
}