#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *abc="abcdefghijklmnñopqrstuvwxyz";

int random_int(int min,int max){
	return min + rand() % (max +1 -min);
}

int main(int argc, char const *argv[])
{
	//printf("%s\n",argv[1]);
	int num=atoi(argv[1]);
	int count=0;
	int size=0;
	for (int i = 0; i < num; ++i)
	{
		 size=random_int(2,11);
		 char *word=(char *)malloc(size*sizeof(char));
		 for (int j = 0; j < size; ++j)
		 {
		 	  word[count]=abc[random_int(0,27)];
		 	  count++;
		 }
		 count=0;
		 printf("%s\n",word);
		 free(word);
	}
	return 0;
}