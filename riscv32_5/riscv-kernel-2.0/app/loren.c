

asm("j main");

#include <stddef.h>
#include <stdint.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/syscall.h"
#include "../include/ff.h"


char abc[]={"abcdefghijklmnñopqrstuvwxyz1234567890"};
char name[]={"loren"};

int random_int(int min,int max){
	return min + rand() % (max +1 - min);
}


void main(){
	static int num=0;
	static int count=0;
	int pid=get_id();
    void *arg=get_args(pid);

    num=atoi((char*)arg);
    

	myprintf("%i words %s\n",num,name);
	for (int i = 0; i < num; ++i)
	{
		 int size = random_int(2,11);
		 do{
		 	size = random_int(2,10);
		 }while(size>10);

		 char *word=(char *)k_malloc(size*sizeof(char));
		 
		 for (int j = 0; j < size; ++j)
		 {
		 	  word[count]=abc[random_int(0,37)];
		 	  count++;
		 }
		 count=0;
		 myprintf("%s\n",word);
		 k_free(word);
	}
	exit_task(pid);
}