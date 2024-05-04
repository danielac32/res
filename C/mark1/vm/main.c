//virtual memory
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<stdint.h>

 
int main(int argc, char const *argv[])
{
    char *p=(char*)malloc(100*sizeof(char));
    if(p==NULL)printf("error malloc\n");
    printf("%d\n",sizeof(p) );
    memset(p,0x77,800);
    for (int i = 0; i < 32; ++i)
    {
        printf("%02x\n",p[i] );
    }
    memset(p,0x00,800);
    free(p);
    printf("\n\n\n\n");
     for (int i = 0; i < 32; ++i)
    {
        printf("%02x\n",p[i] );
    }
	return 0;
}