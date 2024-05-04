asm("li sp,89997   \n\t"
    "li gp,89998   \n\t"
    "j main        \n\t"
);
 
 
#include <stdint.h>
#include <string.h>
#include "reg.h"



void print(char *str){   
while(*str){          
	TX=*str;          
	str++;            
}  
}


void __attribute__ ((section(".interrupt"))) isr(){     

       save_context;

       print("\t interrupt enable\t");
       
       restore_context;
}   

void __attribute__ ((section(".svc"))) syscall_isr(){     
scv_init;
register uint32_t a7 asm("a7");
if(a7==1){
	print("SYSCALL 1 ok\n");
}
scv_return;
}
 
char *str="\nriscv32 test\n\n";
void main(){
MODE=0;
ENT0=0;
print(str);
SYSCALL(1);
print("end\n");
while(1){
	//print("\tloop\n");
}
}