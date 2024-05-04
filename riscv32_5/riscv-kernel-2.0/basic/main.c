#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//asm("b main");
//-----------------------------------------------------------------
// Registers
//-----------------------------------------------------------------
#define UART_UDR		(*((volatile unsigned int*) (0xE0000000)))

void putc_( char ch){
     UART_UDR=ch;
} 

void print(const  char *str){
  while(*str){
    putc_(*str++);
  }
}
 

 char *str="hola danielquintero\n";
 char *abc="abcdefghijklmnñopqrstuvwxyz";

 

char buffer[1000];
int main(char *str){
   

	for (int i = 0; i < 10; ++i)
	{
		 print(str);
	}
 

	return 0;
}