asm("j main");


void main(){
	char *str="program test\n";
	for (int i = 0; i < 14; ++i)
	{
		(*(volatile char *)0xFA002)=str[i];
	}
	while(1);
}