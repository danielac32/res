#include <stdint.h>
#define reg (*(volatile char *)0x300000)
#define mode (*(volatile char *)0x300001)
#define timer (*(volatile uint32_t *)0x300002)
#define enable_timer (*(volatile char *)0x300003)

#define workregister1 (*(volatile uint32_t *)0x300004)
#define workregister2 (*(volatile uint32_t *)0x300005)
#define workregister3 (*(volatile uint32_t *)0x300006)
#define workregister4 (*(volatile uint32_t *)0x300007)
#define workregister5 (*(volatile uint32_t *)0x30000a)
#define workregister6 (*(volatile uint32_t *)0x30000b)
#define workregister7 (*(volatile uint32_t *)0x30000c)

#define keyAvailable (*(volatile uint8_t *)0x300008)
#define keyRead (*(volatile uint8_t *)0x300009)
#define SPACE 0x20
#define ENTER 0xd
#define UP 0x48

//char mem[2048];
char buff[100];

//unsigned long counter_timer=0;
/*
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

int intToStr(uint32_t x,char str[],int d){
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
*/
