
asm("li sp,370000  \n\t"
    "li gp,0x840   \n\t" //antes del puntero de entrada
    "j main       \n\t"
);


#include <stdint.h>
#include <string.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/kmalloc.h"
#include "../include/diskio.h"
#include "../include/ff.h"
#include "../include/timer.h"
#include "../include/shell.h"
#include "../include/syscall.h"
//#include "../include/task.h"

FATFS FatFs;    
FILINFO fno;

int argc=0;
char *argv[10];
int read=0;



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
        
        
       myprintf("%s",fno.fname);

       if(fno.fattrib & AM_DIR){
          i = strlen(path);
          myprintf("/\n");
          memcpy(&path[i],fno.fname,13);
          res = scan_files(path);
          if(res != FR_OK)break;
          path[i]=0;
       }else{
         // myprintf("%s,%s\n",path,fno.fname);
        myprintf("\t\t");
        myprintf("%l\n",fno.fsize);
       } 

       
     }
     f_closedir(&dir);
  }

  return res;
}

 
char instructions_msg[]= " \
\n\
<Planilla_Pago>\n\
      <Cod_Banco>1J63</Cod_Banco>\n\
      <Cod_Agencia>9J57</Cod_Agencia>\n\
      <Cod_Safe>193847</Cod_Safe>\n\
      <Cod_Seguridad_Planilla>082455</Cod_Seguridad_Planilla>\n\
      <Fe_Transmision>05/10/2018</Fe_Transmision>\n\
      <Fe_Recaudacion>05/10/2018</Fe_Recaudacion>\n\
      <Rif_Contribuyente>J075539443</Rif_Contribuyente>\n\
      <Cod_Forma>99044</Cod_Forma>\n\
      <Num_Planilla>9500001056</Num_Planilla>\n\
      <Periodo>102018</Periodo>\n\
      <Cod_Aduana></Cod_Aduana>\n\
      <Cod_Region></Cod_Region>\n\
      <Cancelado_Electronicamente>N</Cancelado_Electronicamente>\n\
      <Monto_Total_Transmision></Monto_Total_Transmision>\n\
      <Monto_Efectivo_Total>0.00</Monto_Efectivo_Total>\n\
      <Monto_Cheque_Total></Monto_Cheque_Total>\n\
      <Monto_Titulos_Total>0.00</Monto_Titulos_Total>\n\
      <Monto_Cert_Total>0</Monto_Cert_Total>\n\
      <Monto_Bonos_Export_Total>0</Monto_Bonos_Export_Total>\n\
      <Monto_Bonos_DPN_Total>0</Monto_Bonos_DPN_Total>\n\
      <Concepto>\n\
         <Cod_Presupuestario>301010200</Cod_Presupuestario>\n\
         <Monto_Concepto>701.81</Monto_Concepto>\n\
      </Concepto>\n\
   </Planilla_Pago>\n\
   <Planilla_Pago>\n\
      <Cod_Banco>1J63</Cod_Banco>\n\
      <Cod_Agencia>9J57</Cod_Agencia>\n\
      <Cod_Safe>193847</Cod_Safe>\n\
      <Cod_Seguridad_Planilla>082455</Cod_Seguridad_Planilla>\n\
      <Fe_Transmision>05/10/2018</Fe_Transmision>\n\
      <Fe_Recaudacion>05/10/2018</Fe_Recaudacion>\n\
      <Rif_Contribuyente>J075539443</Rif_Contribuyente>\n\
      <Cod_Forma>99044</Cod_Forma>\n\
      <Num_Planilla>9500001056</Num_Planilla>\n\
      <Periodo>102018</Periodo>\n\
      <Cod_Aduana></Cod_Aduana>\n\
      <Cod_Region></Cod_Region>\n\
      <Cancelado_Electronicamente>N</Cancelado_Electronicamente>\n\
      <Monto_Total_Transmision></Monto_Total_Transmision>\n\
      <Monto_Efectivo_Total>0.00</Monto_Efectivo_Total>\n\
      <Monto_Cheque_Total></Monto_Cheque_Total>\n\
      <Monto_Titulos_Total>0.00</Monto_Titulos_Total>\n\
      <Monto_Cert_Total>0</Monto_Cert_Total>\n\
      <Monto_Bonos_Export_Total>0</Monto_Bonos_Export_Total>\n\
      <Monto_Bonos_DPN_Total>0</Monto_Bonos_DPN_Total>\n\
      <Concepto>\n\
         <Cod_Presupuestario>301010200</Cod_Presupuestario>\n\
         <Monto_Concepto>701.81</Monto_Concepto>\n\
      </Concepto>\n\
   </Planilla_Pago>\n\
   <Planilla_Pago>\n\
      <Cod_Banco>1J63</Cod_Banco>\n\
      <Cod_Agencia>9J57</Cod_Agencia>\n\
      <Cod_Safe>193847</Cod_Safe>\n\
      <Cod_Seguridad_Planilla>082455</Cod_Seguridad_Planilla>\n\
      <Fe_Transmision>05/10/2018</Fe_Transmision>\n\
      <Fe_Recaudacion>05/10/2018</Fe_Recaudacion>\n\
      <Rif_Contribuyente>J075539443</Rif_Contribuyente>\n\
      <Cod_Forma>99044</Cod_Forma>\n\
      <Num_Planilla>9500001056</Num_Planilla>\n\
      <Periodo>102018</Periodo>\n\
      <Cod_Aduana></Cod_Aduana>\n\
      <Cod_Region></Cod_Region>\n\
      <Cancelado_Electronicamente>N</Cancelado_Electronicamente>\n\
      <Monto_Total_Transmision></Monto_Total_Transmision>\n\
      <Monto_Efectivo_Total>0.00</Monto_Efectivo_Total>\n\
      <Monto_Cheque_Total></Monto_Cheque_Total>\n\
      <Monto_Titulos_Total>0.00</Monto_Titulos_Total>\n\
      <Monto_Cert_Total>0</Monto_Cert_Total>\n\
      <Monto_Bonos_Export_Total>0</Monto_Bonos_Export_Total>\n\
      <Monto_Bonos_DPN_Total>0</Monto_Bonos_DPN_Total>\n\
      <Concepto>\n\
         <Cod_Presupuestario>301010200</Cod_Presupuestario>\n\
         <Monto_Concepto>701.81</Monto_Concepto>\n\
      </Concepto>\n\
   </Planilla_Pago>\n\
   <Planilla_Pago>\n\
      <Cod_Banco>1J63</Cod_Banco>\n\
      <Cod_Agencia>9J57</Cod_Agencia>\n\
      <Cod_Safe>193847</Cod_Safe>\n\
      <Cod_Seguridad_Planilla>082455</Cod_Seguridad_Planilla>\n\
      <Fe_Transmision>05/10/2018</Fe_Transmision>\n\
      <Fe_Recaudacion>05/10/2018</Fe_Recaudacion>\n\
      <Rif_Contribuyente>J075539443</Rif_Contribuyente>\n\
      <Cod_Forma>99044</Cod_Forma>\n\
      <Num_Planilla>9500001056</Num_Planilla>\n\
      <Periodo>102018</Periodo>\n\
      <Cod_Aduana></Cod_Aduana>\n\
      <Cod_Region></Cod_Region>\n\
      <Cancelado_Electronicamente>N</Cancelado_Electronicamente>\n\
      <Monto_Total_Transmision></Monto_Total_Transmision>\n\
      <Monto_Efectivo_Total>0.00</Monto_Efectivo_Total>\n\
      <Monto_Cheque_Total></Monto_Cheque_Total>\n\
      <Monto_Titulos_Total>0.00</Monto_Titulos_Total>\n\
      <Monto_Cert_Total>0</Monto_Cert_Total>\n\
      <Monto_Bonos_Export_Total>0</Monto_Bonos_Export_Total>\n\
      <Monto_Bonos_DPN_Total>0</Monto_Bonos_DPN_Total>\n\
      <Concepto>\n\
         <Cod_Presupuestario>301010200</Cod_Presupuestario>\n\
         <Monto_Concepto>701.81</Monto_Concepto>\n\
      </Concepto>\n\
   </Planilla_Pago>\n\
\n\
\n\
 ";


void hexdump( unsigned char *buf )  {
  int i;
  char s[16];
  
  // A.  hex bytes
  for( i=0 ; i<16 ; i++ ) {
      mysprintf( s, "%2x ", buf[i] & 0x0ff );
      myprintf("%s",s);
     if( i==7 )myprintf(" "); 
   }
   
  myprintf(" "); 
   
   s[1] = '\0';
   
   for( i=0 ; i<16 ; i++ ) {
     if( buf[i] >=0x20 && buf[i] <= 0x7e ) {
       s[0] = buf[i];
 
       myprintf("%s",s);
     } else {
       
       myprintf(".");
     }
     if( i==7 ) myprintf(" "); 
   }
}


int main()
{
   MODE=0;
   ENT0=0;
   FIL file; 
   
   memset(mem, 0, sizeof(mem));
   chunklist_init((chunk_t*)mem, sizeof(mem));
   info((chunk_t*)mem);

   myprintf("Iniciando kernel\n");

   FRESULT res = f_mount(&FatFs, "", 1);
   if (res==FR_OK)
   {
   	   myprintf("OK f_mount\n");
          while(1){
   	   	 int c =get_cmd(&argc,argv);
   	   	 
         if(c){ 
         	 if(!strcmp(argv[0],"test")){
                 f_unlink("config.txt");
                 if(f_open(&file,"config.txt",FA_OPEN_ALWAYS | FA_READ | FA_WRITE)==FR_OK){
                    f_lseek(&file, 0);
                    f_printf(&file,"%S\n",instructions_msg);
                    f_close(&file);
                    myprintf("file ok\n");
                 }else myprintf("error\n");
         	 }else
			 if(!strcmp(argv[0],"reset")){
			 	reset();
			 }else
			 if(!strcmp(argv[0],"rm")){
			 	{
			 	   FRESULT res=	f_stat( argv[1], NULL );
			 	   if(res==FR_OK){
			 	   	  f_unlink(argv[1]);
			 	   	  myprintf("\n->\n");	
			 	   }else myprintf("not found: %s\n",argv[1]);
			 	 }
			 }else
			 if(!strcmp(argv[0],"dump")){
			 	 {
			 	 	UINT bw;
                    char c;
                    //char block[512];
                    char *block=(char*)kmalloc(512*sizeof(char));
                    long offset = 0;
                    long offset2 = 0;
                    unsigned char buf[32];

			 	 	if(f_open(&file,argv[1],FA_OPEN_ALWAYS | FA_READ)==FR_OK){
                       f_lseek(&file, 0);
                       uint32_t j=file.fsize,k=0;
                       myprintf("size: %l\n",j);

                       myprintf("       0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F\n");

                       while(1){
                              FRESULT res = f_read( &file, block, sizeof(block), &bw);
                              if(res!=FR_OK){
                                 myprintf("%i\n",res);
                                 break;
                              } 
                              for(offset = 0 ; offset < sizeof(block) ; offset += 16 ){
                                  myprintf( "%4x   ", offset2+=16 );

                                  for( int b=0 ; b<16 ; b++ ){
                                       buf[b] = block[offset + b];
                                  }
                                  hexdump( buf );
                                  myprintf("\n");
                              }
                              //memcpy(&bin[k],block,sizeof(block));
                              if(bw!=sizeof(block))break;
                              k+=sizeof(block);
                       }
                       f_close(&file);
                    }else{
                    	myprintf("fail opened\n");
                    }
                    kfree(block);
			 	 }
			 }else
			 if(!strcmp(argv[0],"touch")){
			 	 {
			 	   FRESULT res=	f_stat( argv[1], NULL );
			 	   if(res==FR_OK){
			 	   	  myprintf("ya existe %s\n",argv[1]);
			 	   }else myprintf("crear %s\n",argv[1]);
			 	 }
			 }
			 else
			 if(!strcmp(argv[0],"mkdir")){
                 {
                 	FRESULT res = f_mkdir( argv[1]);
                 	if (res==FR_OK)
                 	{
                 	    myprintf("\n->\n");	
                 	}else myprintf("fail mkdir\n");
                 }
			 }else
			 if(!strcmp(argv[0],"cat")){
			 	 {
                    char p[2];
                    UINT bw;
                    char c;
			 	 	if(f_open(&file,argv[1],FA_OPEN_ALWAYS | FA_READ)==FR_OK){
                       f_lseek(&file, 0);
                       uint32_t j=file.fsize;
                       do{
                       	    /*FRESULT res = f_read( &file, p, sizeof(p), &bw);
                            if(res==FR_OK){
                               myprintf("%s",p);
                               memset(p,0,sizeof(p));
                            }else break;
                            if(bw!=sizeof(p))break;*/
                       	    f_read( &file, &c, 1, &bw);
                            myprintf("%c",c);
                       }while(j--);
                       f_close(&file);
                       myprintf("\n->\n");
                    }else{
                       myprintf("error file\n");
                    }
			 	 }
			 }else
			 if(!strcmp(argv[0],"root")){
			 	char buff[256];
			 	strcpy(buff,"/");
			 	f_mount(&FatFs, "", 1);
        scan_files(buff);
			 }else
			 if(!strcmp(argv[0],"echo")){
                 for (int i = 1; i < argc; ++i)
                 {
                 	  myprintf("%s ",argv[i]);
                 }
                 myprintf("\n");
			 }else
			 if(!strcmp(argv[0],"run")){
                 {
                 	myprintf("load bin...\n");
                 	UINT bw;
 					long start_t,end_t,result;
                 	FIL boot;  
                 	char block[512];
                 	void (*p)(void);


                    start_t=MS;
                    if(f_open(&boot,argv[1],FA_OPEN_ALWAYS | FA_READ)==FR_OK){
         			   f_lseek(&boot, 0);
                       uint32_t j=boot.fsize,k=0;
                        
                       char *bin = (char*)kmalloc(j+1000);
          			   
                       if(bin!=NULL){
                          while(1){
                              FRESULT res = f_read( &boot, block, sizeof(block), &bw);
                              if(res!=FR_OK){
                                 myprintf("%i\n",res);
                                 break;
                              } 
                              memcpy(&bin[k],block,sizeof(block));
                              k+=sizeof(block);
                          }
                          f_close(&boot);

                          end_t=MS;
           			      result= end_t - start_t;
                          myprintf("jump app ---time: %l\n",result);
                          p=(void*)bin;
                          (*p)();
                          kfree(bin); ///fr((chunk_t*)mem,(void*)bin); 
                       }else myprintf("error boot\n");

                   }else myprintf("not exist : %s\n",argv[1]);
                 }
			 }else
			 if(!strcmp(argv[0],"malloc")){
			 	 if(!strcmp(argv[1],"-i")){
                     {
                      
                     	info((chunk_t*)mem);
                     	 
                     }

			 	 }else
			 	 if(!strcmp(argv[1],"-t")){
                      
			 	 }else
                 if(!strcmp(argv[1],"-f")){
                     {
                     	 
                     }
			 	 }/*else
			 	 if(!strcmp(argv[1],"float")){

			 	 }*/
			 }
			 for (int i = 0; i < argc; ++i){
                  //kfree(argv[i]);
                  do_free((chunk_t*)mem,argv[i]); 
             }
         }
   	   }


   }else{
   	   myprintf("FAIL f_mount\n");
   }

   while(1);
   return 0;
}