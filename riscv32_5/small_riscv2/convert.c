#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


int main(int argc, char const *argv[])
{
	
	FILE * file = NULL;
    unsigned long lsize = 0;
    

    //argv[1]
    file = fopen("app.bin", "rb");
    if(file == NULL){
       printf("Could not open file!");
       exit(-1);
    }
	char buff[512];

    printf("\n\nunsigned char raw[]={\n");
    int counter=1;

    int c=getc(file);
    while(c!=EOF){
    	  printf("0x%02x,",c & 0xff );
    	  c=getc(file);
          if(!(counter%16))printf("\n");
          counter++;
    }

	/*while(fgets(buff,sizeof(buff),file) != NULL){
           
          
		  for (int i = 0; i < 16; ++i)
		  {
		  	   printf("%02x,",buff[i] & 0xff );
		  }
          printf("\n");
	}*/

	printf("\n};\n\n\n");
	fclose(file);
   
	return 0;
}