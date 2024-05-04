#include <os.h>




char *text="esto es una prueba del kernel para rp2040";
struct  cmdent  {     /* Entry in command table */
  char  *cname;     /* Name of command    */
  bool cbuiltin;   /* Is this a builtin command? */
};

const struct  cmdent  cmdtab[] = {
  {"echo",  true},
  {"cat",   false},
  {"clear", true},
  {"date",  false},
  {"devdump", false},
  {"echo",  false},
  {"exit",  true},
  {"help",  false},
  {"kill",  true},
  {"memdump", false},
  {"memstat", true}, /* Make built-in */
  {"ps",    false},
  {"sleep", false},
  {"uptime",  false},
  {"?",   false},
  {"fopen",   false},
  {"fclose",   false},
  {"fcreat",   false},
  {"fread",   false},
  {"fwrite",   false},
  {"ls",   false},
  {"cd",   false},
  {"pwd",   false},
  {"cat",   false},
  {"rm",   false},
  {"mkdir",   false},
  {"touch",   false},
  {"mount",   false},
  {"writegpio",   false},
  {"readgpio",   false},
  
};

uint32_t  ncmd = sizeof(cmdtab) / sizeof(struct cmdent);

 

 

FATFS FatFs;   
FIL Fil;     
FILINFO fno;
FIL file;  

 

 

FRESULT scan_files(char *path){
  FRESULT res;
  DIR dir;
  UINT i;
  static FILINFO fno;
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

 char buff[256];

void main() {

printf("iniciando sdcard\n");
 
if (f_mount(&FatFs, "", 1) == FR_OK) { 


    printf("sd ok\n");


}else{
    printf("error sd\n");
      return;
}


 strcpy(buff,"/");
     scan_files(buff);
     
while(1){
         printf("%s",SHELL_PROMPT);
         while(!uart0_available());
          while(uart0_available()){
              len = readBytes(buf,sizeof(buf));
        }
        if (len == EOF) {
           break;
        }
        if (len <= 1) {
            continue;
        }

        buf[len] = SH_NEWLINE;  /* terminate line */

  

    ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);

    /* Handle parsing error */

    if (ntok == SYSERR) {
      printf("%s\n",SHELL_SYNERRMSG);
      continue;
    }

    /* If line is empty, go to next input line */

    if (ntok == 0) {
      printf( "\n");
      continue;
    }
    
    for (j = 0; j < ncmd; j++) {
      src = cmdtab[j].cname;
      cmp = tokbuf;
      diff = false;
      while (*src != NULLCH) {
        if (*cmp != *src) {
          diff = true;
          break;
        }
        src++;
        cmp++;
      }
      if (diff || (*cmp != NULLCH)) {
        continue;
      } else {
        break;
      }
    }

    /* Handle command not found */

    if (j >= ncmd) {
        printf("command %s not found %d\n",tokbuf,len);
        continue;
    }else{
        printf("ok");
        printf(" %d ",j);
      for (int i=0; i<ntok; i++) {
          args[i] = &tokbuf[tok[i]];
          printf("%s",args[i]);
          printf(" ");
      }
      if(!strcmp(args[0],"kill")){
        systick_disable();
        PEND_SV();
      }else if(!strcmp(args[0],"exit")){
        //reset();
      }
      printf("\n");

        
    }

 

    }
}


/*

#define SYS_CSR    (PPB_BASE + 0xE010)
#define SYS_RVR    (PPB_BASE + 0xE014)
#define SYS_CVR    (PPB_BASE + 0xE018)
#define SYS_CALIB  (PPB_BASE + 0xE01C)
#define NVIC_ISER   (PPB_BASE + 0xE100)
#define NVIC_ICER   (PPB_BASE + 0xE180)
#define NVIC_ISPR   (PPB_BASE + 0xE200)
#define NVIC_ICPR   (PPB_BASE + 0xE280)


reg_wr(CLOCKS_BASE+0x78, 0);

reg_wr(XOSC_BASE+0x00, 0xaa0);
reg_wr(XOSC_BASE+0x0c, 47);
reg_wr(XOSC_BASE+0x00+0x2000, 0xfab000);

while(1) {
    if (((reg_rd(XOSC_BASE+0x04)) & 0x80000000)!=0){
        break;
    }
}

reg_wr(CLOCKS_BASE+0x30, 2);
reg_wr(CLOCKS_BASE+0x3c, 0);
reg_wr(CLOCKS_BASE+0x48, 0x880);

reset_release_wait(RESET_IO_BANK0);
reset_release_wait(RESET_PADS_BANK0);
reset_release_wait(RESET_UART0);


uart_init();

reg_wr(SYS_CSR, 4);
reg_wr(SYS_RVR, 12000000-1);
reg_wr(SYS_CVR, 12000000-1);
reg_wr(SYS_CSR, 7);

reg_wr(NVIC_ISER, 1 << 13);

asm volatile("cpsie i");

gpio_init(0, GPIO_FUNC_UART);
gpio_init(1, GPIO_FUNC_UART);

*/


/*
gpio irq


void irq13() {
    gpio_set(20, 0);
    delay(1000000);
    gpio_set(20, 1);
    gpio_int_ack(7);
}


    gpio_init(18, GPIO_FUNC_SIO);
    gpio_init(19, GPIO_FUNC_SIO);
    gpio_init(20, GPIO_FUNC_SIO);
    gpio_dir(18, 1);
    gpio_dir(19, 1);
    gpio_dir(20, 1);

    gpio_init(0, GPIO_FUNC_SIO);
    gpio_init(1, GPIO_FUNC_SIO);
    gpio_init(2, GPIO_FUNC_SIO);
    gpio_pullup(0, 1);
    gpio_pullup(1, 1);
    gpio_pullup(2, 1);

    gpio_init(7, GPIO_FUNC_SIO);
    gpio_pullup(7, 1);
    delay(10000);
    gpio_int_set(7, 1, GPIO_INT_EDGE_FALL);
    nvic_init();
    nvic_register_irq(13, irq13);
    nvic_enable(13);

    gpio_set(19, 1);
    gpio_set(20, 1);
    */
