


#include <stdint.h>
#include <string.h>
#include "riscv.h"
#include "serial.h"
#include "kmalloc.h"
#include "task.h"

#include "lfs.h"
#include "lfs_util.h"
 

 
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

#define curr_mode (*(volatile uint8_t *)0x4000500)



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

int main()
{

    kprintf("riscv32\n");
    kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
    kmalloc_debug();
 

    if(begin(20000)){
       kprintf("fs ok\n");



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
     //kprintf("%s\n",buff );
     while(available(&fil)){
          file_read(&byte,1,&fil);
          kprintf("%x\n",byte );
     }
     file_close(&fil);

    }else{
      kprintf("fail\n");
    }

   
   return 0;
}