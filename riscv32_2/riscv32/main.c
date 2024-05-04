#include <stdlib.h>
#include <stdio.h>


#define reg (*(volatile char *)0x300000)
void putc_( char ch){
     reg=ch;
} 
void print( char *str){
  while(*str){
    putc_(*str++);
  }
}
char variable[100];
void main(){
while(1){	
  
	sprintf(variable,"counter\n");
    print(variable);
}
}
