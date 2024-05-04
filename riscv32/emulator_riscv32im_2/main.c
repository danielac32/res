#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include "cpu.h"


/********************************/
//memory sections (size memory 64000) 
/*
50 k flash
12 k ram
2 k sp

base_text        :0x0000;   
base_text_limit  :0xC350;--------->50KS
base_ram         :0xC350;--------->50K
base_ram_limit   :0xF230;--------->62K LIMIT
base_sp          :0xF9FE;--------->63.998K
base_gp          :0xFA00;--------->64K
uart             :0xFA01;--------->
sfr1             :0xFA02;
sfr2             :0xFA03;
*/
/********************************************************************/
 

void openrom(char *str){
    FILE * file = NULL;
    FILE *vm;
    uint32_t lsize = 0;
    uint32_t res=0;
    char *data;
    printf("\n\nopen--%s\n\n",str);

    file = fopen(str, "rb");
    if(file == NULL){
       printf("Could not open file!");
       exit(-1);
    }
    //
	fseek(file, 0, SEEK_END);
    lsize = ftell(file);
    res = 64500 - lsize;

    data = (char *)calloc(lsize+res,sizeof(char));
    if (data==NULL)
    {
    	printf("error calloc\n");
    	exit(-1);
    }
    rewind(file);
	fread(data, sizeof(char), lsize, file);
	fclose(file);
    //
    vm=fopen("vm.bin","w+");
    fwrite(data,1,lsize+res,vm);
	fclose(vm);

    
	free(data);
	printf("loaded flash %i\n",lsize+res);
}


int main(int argc, char *argv[])
{
	char *exec;
    
	if(argc>1){
	   exec=argv[1];
       openrom(exec);
       
      // write_vm("vm.bin");
	}else{
	   printf("fail\n");
	   exit(1);
	}
	init_soc(); 
	return 0; 
}