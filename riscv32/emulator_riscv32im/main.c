#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cpu.h"

void openconf(char *str){
    FILE * file = NULL;
    unsigned long lsize = 0;
    char buff[2000];
    char *pch;
    char i;
    printf("\n\nopen--%s\n\n",str);
	file = fopen(str, "rb");
    if(file == NULL){
       printf("Could not open file!");
       exit(-1);
    }
	fseek(file, 0, SEEK_END);
    lsize = ftell(file);
    rewind(file);

	fread(buff, sizeof(char), lsize, file);
    //printf("%s\n",buff );
	{
		pch=strtok(buff,";:");
	    //printf("%s\n",pch);
	    pch=strtok(NULL,";:");
		riscv.values[0]=strtol(pch, NULL, 16);
		//printf("%08x\n",values[0]);

		for(i=0;i<9;i++){
			pch=strtok(NULL,";:");
	        //printf("%s\n",pch);
	        pch=strtok(NULL,";:");
		    riscv.values[i+1]=strtol(pch, NULL, 16);
		   // printf("---%08x\n",riscv.values[i+1]);
		}
		
		printf("end\n");
		
	}
	fclose(file);
	//
	riscv.UART_TX_ADDR=riscv.values[7];
	printf("uart addr: %08x\n",riscv.UART_TX_ADDR);
    riscv.PROGRAM_COUNTER_START_VAL=riscv.values[0];
    printf("program counter start addr: %08x\n",riscv.PROGRAM_COUNTER_START_VAL);
    riscv.RAM_SIZE=riscv.values[6];
    printf("ram size: %08x\n",riscv.RAM_SIZE);
    riscv.base_text=riscv.values[0];
    printf("base text: %08x\n",riscv.base_text);
    riscv.base_text_limit=riscv.values[1];
    printf("base text limit: %08x\n",riscv.base_text_limit);
    riscv.base_ram=riscv.values[2];
    printf("base ram: %08x\n",riscv.base_ram);
    riscv.base_ram_limit=riscv.values[3];
    printf("base ram limit: %08x\n",riscv.base_ram_limit);
    //base_sp=values[4];
    //base_gp=values[5];
    riscv.next=(uint8_t)riscv.values[8];
    riscv.debug_inst=(uint8_t)riscv.values[9];
    printf("next: %i , debug: %i\n",riscv.next , riscv.debug_inst);
}


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
	fread(riscv.memory, sizeof(char), lsize, file);
	fclose(file);
}
/********************************************************************/
void main(int argc, char *argv[])
{
	char *exec;
	char *config;
    init_variable();
	if(argc>1){
	   exec=argv[1];
	   config=argv[2];
	   openconf(config);
	   openrom(exec);
	}else{
	   printf("fail\n");
	   exit(1);
	}
	init_soc();//init cpu
}