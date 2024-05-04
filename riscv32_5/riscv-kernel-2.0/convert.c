#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
	
	FILE * file = NULL;
  FILE * out=NULL;
    unsigned long lsize = 0;
    

    //argv[1]
    out  = fopen ("data.txt", "w");


    file = fopen(argv[1], "rb");
    if(file == NULL){
       printf("Could not open file!");
       exit(-1);
    }
	char buff[512];

    printf("\n\nunsigned char raw[]={\n");
    fprintf(out, "\n\nunsigned char raw[]={\n");
    int counter=1;

    int c=getc(file);
    while(c!=EOF){
    	  printf("0x%02x,",c & 0xff );
        fprintf(out, "0x%02x,",c & 0xff );
    	  c=getc(file);
          if(!(counter%16)){
               printf("\n");
               fprintf(out, "\n");
          }
          counter++;
    }

	/*while(fgets(buff,sizeof(buff),file) != NULL){
           
          
		  for (int i = 0; i < 16; ++i)
		  {
		  	   printf("%02x,",buff[i] & 0xff );
		  }
          printf("\n");
	}*/
  fprintf(out, "\n};\n\n\n");
	printf("\n};\n\n\n");
	fclose(file);
  fclose(out);
	return 0;
}