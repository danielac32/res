
#include <os.h>
#include "flash.h"
extern void terminal();

extern unsigned int _FS_start;

char *txt="1234567890\n";

#define SIZE 1200

char prog[]={
0x00,0xe0,0x00,0x00,0x02,0x23,0x18,0x1c,0x02,0x4b,0x7b,0x44,0x1b,0x68,0x19,0x1c,
0x02,0xdf,0xfe,0xe7,0x16,0x00,0x00,0x00,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
0x39,0x00,0x18,0x00,0x00,0x00}; 

void blink() {
    /*int rval;
    int fd, i, j;
    //char buf1[SIZE], buf2[SIZE];
    char *buf1, *buf2;
    //testbitmask();
    buf1 = getmem(SIZE*sizeof(char));
    buf2 = getmem(SIZE*sizeof(char));
    
    fd = _fcreat("a.txt", O_CREAT);
    memset(buf1,'a',SIZE);
   
    rval = _fwrite(fd,buf1,SIZE);
 

    _fseek(fd,-rval); 
    _fread(fd, buf2, SIZE);

    printf("\n\rContent of file %s\n",buf2);

    _fclose(fd);
   
 
   
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
        
   
    freemem(buf1,SIZE*sizeof(char));
    freemem(buf2,SIZE*sizeof(char));*/


   while (1) {
    gpio_set(25, 1);
    sleep(50);
    gpio_set(25, 0);
    sleep(100);
  }
}

void ilde() {

   while (1) {
       PEND_SV();
  }
}


int main(int argc, char const *argv[])
{
	  disable();
    meminit();
    list();

    systick_init();
    initialice();

    
    
    write(CONSOLE,txt,strlen(txt));//test

    int pid=create((void *)terminal,4096,1,"shell",0); 
    ready(pid);

    pid=create((void *)blink,4096,1,"blink",0); 
    ready(pid);

    pid=create((void *)ilde,256,1,"ilde",0); 
    ready(pid);

    pid=create((void *)prog,256,1,"test",0); 
    ready(pid);

    systick_set(12000);
    enable();
    PEND_SV();//rtos_start();
	return 0;
}