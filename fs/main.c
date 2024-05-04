#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "fs.h"


//extern struct thrent thrtab[];
#define SIZE 1200


int main(int argc, char const *argv[])
{
	int rval;
    int fd, i, j;
    char *buf1, *buf2;
    mkbsdev(0, 0, 0); /* device "0" and default blocksize (=0) and count */
    mkfs(0,DEFAULT_NUM_INODES); /* bsdev 0*/
    //testbitmask();
    buf1 = malloc(SIZE*sizeof(char));
    buf2 = malloc(SIZE*sizeof(char));
    

    fd = _fcreat("boot.bin", O_CREAT);
     memset(buf1,0x56,SIZE);
     _fwrite(fd,buf1,SIZE);
    _fclose(fd);


    _fcreat("start.elf", O_CREAT);
    _fcreat("test.txt", O_CREAT);
    _fcreat("config.txt", O_CREAT);





    
	fd = _fcreat("texto.txt", O_CREAT);
       
    for(i=0; i<SIZE; i++)
    {
        j = i%(127-33);
        j = j+33;
        buf1[i] = (char) j;
    }


    //strcpy(buf1,"daniel eduardo quintero villegas");
   


    rval = _fwrite(fd,buf1,SIZE);

    if(rval == 0)
    {
        printf("\n\r File write failed");
    }
    

    _fseek(fd,-rval); 
    
   rval = _fread(fd, buf2, rval);
    buf2[rval] = '\0';
    
    if(rval == 0)
    {
        printf("\n\r File read failed");
    }
        
    printf("\n\rContent of file:  %s\n",buf2);
    
    rval = _fclose(fd);
    if(rval != OK)
    {
        printf("\n\rReturn val for fclose : %d",rval);
    }
     

   
    printf("root fs\n");
    for (int i=0; i < DIRECTORY_SIZE; i++)
    {
    	char *name = fsd.root_dir.entry[i].name;

    	int size = core_inode[get_inode_by_num(0, i)].size;


        if ((fsd.root_dir.entry[i].inode_num != UNUSED))
        {
            printf("---->%s  %d\n", name,size);//return get_inode_by_num(0, i);    
        }
    	//printf("---->%s  %d\n", name,size);
      
    }
        
   
    free(buf1);
    free(buf2);
	return 0;
}

