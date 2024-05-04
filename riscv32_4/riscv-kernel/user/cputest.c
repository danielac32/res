asm("j main");

#define reg (*(volatile char *)0xFA002)
#define mode (*(volatile char *)0xFA001)
#define workregister1 (*(volatile unsigned long *)0xFA008)
#define workregister2 (*(volatile unsigned long *)0xFA009)


void putc_( char ch){
     reg=ch;
} 
void print( char *str){
  while(*str){
    putc_(*str++);
  }
}



void main(int pid,unsigned long funcpointer){
	 char *str="execute from ram\n";
	 for (int i = 0; i < 10; ++i)
	 {
	 	 print(str);
	 } 
	 workregister1=pid;
     workregister2=200;
     mode=1;
     while(mode){
           asm("nop");
           asm("nop");
     }
}