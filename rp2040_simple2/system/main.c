#include <string.h>
#include "resets.h"
#include "gpio.h"
#include "uart.h"
#include "xosc.h"
#include "nvic.h"
#include "systick.h"
#include "kmalloc.h"
#include "timer.h"

 
 


#include "lfs.h"
#include "lfs_util.h"
 
#include "ff.h"
#include "fatfs.h"



#define LED   25

 

 

 
void __attribute__((naked))  svccall_handler_c(uint32_t *sp){
  asm("cpsid  i");
  uint32_t svc_number = ((char *)sp[6])[-2];
  uint32_t svc_arg1 = sp[0];
  uint32_t svc_arg2 = sp[1];
  uint32_t result = svc_arg1 + svc_arg2;

  //printf("svc Number: %d\n", svc_number);
  switch(svc_number){
      case 0:
         *((uint32_t*)0xE000ED04) = (1 << 28);
      break;
      case 1:
           
      break;
      case 5:
         printf("aquiiiiiiiiiiiiiiiiiiiiiiiiiii: %d , %d\n",svc_arg1,svc_arg2);
      break;
  }
  asm volatile("cpsie  i\n"
               "ldr r0,=0xFFFFFFFD\n"
               "bx r0\n");
 // *((uint32_t*)0xE000ED04) = (1 << 28);//PEND_SV();
}



void __attribute__((naked))  pendsv_handler()  {
 //init();
}


void systick_handler() {

//*((uint32_t*)0xE000ED04) = (1 << 28);
}

 
 
/*
 
struct lfs_config config;
  lfs_t lfs;
  lfs_file_t file;
  lfs_dir_t dir;



  static int static_read(const struct lfs_config *c, lfs_block_t block,
    lfs_off_t offset, void *buffer, lfs_size_t size) {
    //Serial.printf("    ram rd: block=%d, offset=%d, size=%d\n", block, offset, size);
    uint32_t index = block * c->block_size + offset;
    memcpy(buffer, (uint8_t *)(c->context) + index, size);
    return 0;
  }
  static int static_prog(const struct lfs_config *c, lfs_block_t block,
    lfs_off_t offset, const void *buffer, lfs_size_t size) {
    //Serial.printf("    ram wr: block=%d, offset=%d, size=%d\n", block, offset, size);
    uint32_t index = block * c->block_size + offset;
    memcpy((uint8_t *)(c->context) + index, buffer, size);
    return 0;
  }
  static int static_erase(const struct lfs_config *c, lfs_block_t block) {
    uint32_t index = block * c->block_size;
    memset((uint8_t *)(c->context) + index, 0xFF, c->block_size);
    return 0;
  }
  static int static_sync(const struct lfs_config *c) {
    
    return 0;
  }

 


bool begin(uint32_t size) {
    //Serial.println("configure "); delay(5);


    char *ptr = kmalloc(size);
    if (!ptr) return false;
    memset(ptr, 0xFF, size); // always start with blank slate
    size = size & 0xFFFFFF00;
    memset(&lfs, 0, sizeof(lfs));
    memset(&config, 0, sizeof(config));
    config.context = ptr;
    config.read = &static_read;
    config.prog = &static_prog;
    config.erase = &static_erase;
    config.sync = &static_sync;
    if ( size > 1024*1024 ) {
      config.read_size = 256; // Must set cache_size. If read_buffer or prog_buffer are provided manually, these must be cache_size.
      config.prog_size = 256;
      config.block_size = 2048;
      config.block_count = size / config.block_size;
      config.block_cycles = 900;
      config.cache_size = 256;
      config.lookahead_size = 512; // (config.block_count / 8 + 7) & 0xFFFFFFF8;
    }
    else {
      config.read_size = 64;
      config.prog_size = 64;
      config.block_size = 256;
      config.block_count = size / 256;
      config.block_cycles = 50;
      config.cache_size = 64;
      config.lookahead_size = 64;
    }
    config.name_max = LFS_NAME_MAX;
    config.file_max = 0;
    config.attr_max = 0;
 
    if (lfs_format(&lfs, &config) < 0) return false;
    //Serial.println("formatted");
    if (lfs_mount(&lfs, &config) < 0) return false;
    //Serial.println("mounted atfer format");
    return true;
  }





int file_open(const char *path, int flags,lfs_file_t *file)
{
    int ret;
    ret = lfs_file_open(&lfs, file, path, flags);
    if(ret != LFS_ERR_OK) return 0;
    else return 1;
}

int file_write(void *pbuf, int size, lfs_file_t *file)
{
    return lfs_file_write(&lfs, file, pbuf, size);
}

int file_close(lfs_file_t *file)
{
    return lfs_file_close(&lfs, file);
}

int file_read(void *pbuf, int size, lfs_file_t *file)
{
    return lfs_file_read(&lfs, file, pbuf, size);
}


int available(lfs_file_t *file) {
    if (!file) return 0;
    lfs_soff_t pos = lfs_file_tell(&lfs, file);
    if (pos < 0) return 0;
    lfs_soff_t size = lfs_file_size(&lfs, file);
    if (size < 0) return 0;
    return size - pos;
  }

char *txt="hola daniel";
*/

/*
struct fatfs* fs;
 FATFS FatFs; 







static int32_t create_file(const char *name) {
    FIL fil;

    if (f_open(&fil, name, FA_OPEN_ALWAYS | FA_WRITE)) {
        return 1;
    }
    f_close(&fil);
    return 0;
}


static int32_t create_folder(const char *name) {
    if (!f_stat(name, NULL)) {  /* if the folder is exist already 
        return 0;
    }

    if (f_mkdir(name)) {
        return 1;
    }
    return 0;
}
*/


FRESULT scan_files(char *path){
  FRESULT res;
  DIR dir;
  UINT i;
  FILINFO fno;
  res = f_opendir(&dir,path);
  if(res==FR_OK){
     for (;;)
     {
       res = f_readdir(&dir,&fno);
       if(res != FR_OK || fno.fname[0]==0)break;
        
        
        printf("%s",fno.fname);

       if(fno.fattrib & AM_DIR){
          i = strlen(path);
          printf("/\n");
          memcpy(&path[i],fno.fname,13);
          res = scan_files(path);
          if(res != FR_OK)break;
          path[i]=0;
       }else{
         // printf("%s,%s\n",path,fno.fname);
        printf("\t\t");
        printf("%d\n",fno.fsize);
       } 

       
     }
     f_closedir(&dir);
  }

  return res;
}

int main()
{
 
    kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
    kmalloc_debug();

/*
    if(begin(20000)){
       printf("fs ok\n");



     lfs_file_t fil;
     char byte;
     file_open("test.txt",LFS_O_CREAT,&fil);
     for (int i = 0; i < 100; ++i)
     {
       file_write(&i,1,&fil);
     }
     file_close(&fil);
     
     file_open("test.txt",LFS_O_RDONLY,&fil);
     //file_read(buff,11,&fil);
     //printf("%s\n",buff );
     while(available(&fil)){
          file_read(&byte,1,&fil);
          printf("%x\n",byte );
     }
     file_close(&fil);

    }else{
      printf("fail\n");
    }
*/

    timer_init();



 UINT bw;
char buff[256];
beginFat();
printf("size: %d\n",capacity() );


FILE file;
f_unlink("config.txt");
      if(f_open(&file,"config.txt",FA_OPEN_ALWAYS | FA_READ | FA_WRITE)==FR_OK){
         f_lseek(&file, 0);
         /*for (int i = 0; i < 5; ++i)
         {
            f_write(&file,"jkjvkjkjjfrftgujhgjujfrjftjgj\n", 31, &bw);
         }*/
         
         f_printf(&file,"%S\n","123456789abcdefghi");
         f_printf(&file,"%S\n","123456789abcdefghi");
         f_printf(&file,"%S\n","123456789abcdefghi");
         f_printf(&file,"%S\n","123456789abcdefghi");
         f_printf(&file,"%S\n","123456789abcdefghi");
         f_close(&file);
      }else printf("error\n");
   


   if(f_open(&file,"config.txt",FA_OPEN_ALWAYS | FA_READ)==FR_OK){
          f_lseek(&file, 0);
          

          char c;
          uint32_t j=f_size(&file);
          char p[160];
          
         /* FRESULT res= f_read( &file, p, j, &bw);
          if(res==FR_OK){
             printf("%s",p);
          } 
*/

          do{
            f_read( &file, &c, 1, &bw);
            printf("%c",c);
          }while(j--);


           f_close(&file);
       }


strcpy(buff, "/");
scan_files(buff);



  /*static BYTE work[FF_MAX_SS];
    char buffer[100];
    FRESULT res;

   
    

    if ((res = f_mount(&FatFs, "0:", 1)) != FR_OK) {
        printf("mount error! res=%d\n", res);
        printf("format...\n");
        if ((res = f_mkfs("0:/", FM_ANY, 0, work, sizeof(work))) != FR_OK) {
             printf("mkfs error! res=%d\n", res);
             return 1;
        }
        printf("mkfs successfully\n");
    }
    printf("mount successfully\n");

    //===============================================================================
    // your code begin here
    
  
    char buff[256];

    create_folder("carpeta1");

    create_folder("carpeta2");

    create_folder("carpeta3");


    create_file("test");
    
    create_file("app");
    
    create_file("root");
    
    create_file("opt");
    
    strcpy(buff, "/");
    res = scan_files(buff);
    printf("test OK\n");*/


   return 0;
}