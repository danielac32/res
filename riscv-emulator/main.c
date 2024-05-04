#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cpu.h"
#include "riscv.h"


void openrom(char *str){
    FILE * file = NULL;
    unsigned long lsize = 0;
    printf("\n\nopen--%s\n\n",str);
    file = fopen(str, "rb");
    if(file == NULL){
       printf("Could not open file!");
       exit(-1);
    }
	fseek(file, 0, SEEK_END);
    lsize = ftell(file);
    rewind(file);
	fread(memory, sizeof(char), lsize, file);
	fclose(file);
}
/********************************************************************/
void main(int argc, char *argv[])
{
 
	char *exec;
	char *config;
    //init_variable();
	if(argc>1){
	   exec=argv[1];
       space_reserved=atol(argv[2]);
       __debug__=atoi(argv[3]);
	   openrom(exec);
	}else{
	   printf("fail\n");
	   exit(1);
	}
	printf("%i,%i\n",space_reserved,__debug__ );
	rv32(0x400);
}