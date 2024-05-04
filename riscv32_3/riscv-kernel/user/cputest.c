
#define reg (*(volatile char *)0x300000)

void putc_( char ch){
     reg=ch;
} 
void print( char *str){
  while(*str){
    putc_(*str++);
  }
}

char *str="cpu test\n";

void main()
{
	 print(str);
	 while(1);
}