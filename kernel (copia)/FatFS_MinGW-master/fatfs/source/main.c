#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ff.h"
#include "diskio.h"
#include <stdlib.h>
#define FILE_NAME       "message.txt"
#define WRITE_TEXT      "hello fatfs 0123456789 +_<>"
#define WRITE_TEXT_LEN  strlen((WRITE_TEXT))

static FATFS FatFs;   /* Work area (filesystem object) for logical drive */

/**
 * create the specified file
 * @param name: the file to be created
 * @return: see @dat_ret_t
 */
static int32_t create_file(const char *name) {
    FIL fil;

    if (f_open(&fil, name, FA_OPEN_ALWAYS | FA_WRITE)) {
        return 1;
    }
    f_close(&fil);
    return 0;
}

/**
 * create the specified folder
 * @param name: the folder to be created
 * @return: see @dat_ret_t
 */
static int32_t create_folder(const char *name) {
    if (!f_stat(name, NULL)) {  /* if the folder is exist already */
        return 0;
    }

    if (f_mkdir(name)) {
        return 1;
    }
    return 0;
}

static int32_t write_file(const char *name, const char *p, uint32_t len) {
    FIL fil;
    uint32_t bw;

    if (f_open(&fil, name, FA_OPEN_ALWAYS | FA_WRITE)) {
        return 1;
    }
    if (f_write(&fil, p, len, &bw) || bw != len) {
        f_close(&fil);
        return 2;
    }
    f_close(&fil);
    return 0;
}

static int32_t read_file(const char *name, char *p, uint32_t len) {
    FIL fil;
    uint32_t br;

    if (f_open(&fil, name, FA_READ)) {
        return 1;
    }
    if (f_read(&fil, p, len, &br) || br != len) {
        f_close(&fil); 
        return 2;
    }
    f_close(&fil); 
    return 0;
}


FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                printf("%s\n",path);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                //printf("%s/%s\n", path, fno.fname);
                printf("    %s\n",fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}


int main(void)
{
    static BYTE work[FF_MAX_SS];
    char buffer[100];
    FRESULT res;
    rambuf = malloc(CFG_RAMDISK_SIZE);
    if ((res = f_mkfs("0:/", FM_ANY, 0, work, sizeof(work))) != FR_OK) {
        printf("mkfs error! res=%d\n", res);
        return 1;
    }
    printf("mkfs successfully\n");

    if ((res = f_mount(&FatFs, "0:", 1)) != FR_OK) {
        printf("mount error! res=%d\n", res);
        return 1;
    }
    printf("mount successfully\n");

    //===============================================================================
    // your code begin here
    


    create_folder("bin");
    create_file("/bin/boot.bin");
    create_file("/bin/init.bin");
    create_file("/bin/start.bin");
    create_file("/bin/kernel.elf");
    create_folder("shell");
    create_folder("other");
    create_folder("/boot");
    create_folder("/boot/root.txt");
    create_folder("/boot/start.bin");
    create_folder("/boot/bootloader.bin");
    create_folder("/boot/hibernate.bin");
    

    char *text = malloc(5000);
    memset(text,'a',5000);


 
    if (write_file("/bin/boot.bin", text, 5000)) {
        printf("error 1\n");
        return 1;
    }
    
    if (write_file("/boot/bootloader.bin", text, 5000)) {
         printf("error 2\n");
        return 1;
    }

    char buff[256];
    strcpy(buff, "/");
    res = scan_files(buff);
    printf("test OK\n");

    
    FILE *fptr;
    fptr = fopen("fs2.img","w");

    if(fptr == NULL)
    {
      printf("Error!");   
      exit(1);             
    }
    fwrite(rambuf, 200000, 1, fptr); 
    fclose(fptr);


  #if 0
    char buff[256];


    unsigned int i;
    unsigned int max = 10; 
    char folder[sizeof("YearMoDaHoMiSe")];
    char fullname[sizeof("/data/YearMoDaHoMiSe/CIM_ECG_PARM.txt")];
 
    create_folder("/data");
    create_folder("/data/root");
    create_folder("/data/app");



    for (i = 0; i < max; i++) {
        memset(folder, 0, sizeof(folder));
        sprintf(folder, "/data/%08d", i);
        create_folder(folder);

        memset(fullname, 0, sizeof(fullname));
        sprintf(fullname, "%s/%s", folder, "CIM_ECG_PARM.txt");
        create_file(fullname);

        if (write_file(fullname, WRITE_TEXT, WRITE_TEXT_LEN)) {
            printf("write failed at %d, line=%d\n", i, __LINE__);
            return 1;
        }
        memset(buffer, 0, sizeof(buffer));
        if (read_file(fullname, buffer, WRITE_TEXT_LEN)) {
            printf("read failed at %d, line=%d\n", i, __LINE__);
            return 1;
        }
        if (strcmp(buffer, WRITE_TEXT)) {
            printf("compare failed at %d, line=%d. the read buffer:%s\n", i, __LINE__, buffer);
            return 1;
        }
    }
    
    strcpy(buff, "/");
        res = scan_files(buff);
    printf("test OK\n");
   #endif
    //-------------------------------------------------------------------------------

    return 0;
}

