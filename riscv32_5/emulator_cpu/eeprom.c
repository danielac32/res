
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eeprom.h"


 void
 eeprom_fill(uint32_t addr, unsigned char value, int size)
 {
   if((addr > EEPROM_END_ADDR) || (addr+size > EEPROM_END_ADDR+1) || size < 0) {
     fprintf(stderr, "eeprom_fill: Bad address and/or size (addr = %04x, size = %d)\n", addr, size);
 
     /* Abort here so that we break into the debugger. If a debugger isn't
      * attached, well we might as well crash anyway.
      */
     abort();
   }
 
   if(!eeprom_file) {
     eeprom_init();
   }
 
   fseek(eeprom_file, addr, SEEK_SET);
 
   while(size--) {
     if(fputc(value, eeprom_file) != value) {
       perror("fputc() failed");
       exit(EXIT_FAILURE);
     }
   }
 }
 
 /*---------------------------------------------------------------------------*/
 void
 eeprom_init(void)
 {
   long length;
   char *eeprom_filename ="eeprom.bin"; //getenv("CONTIKI_EEPROM");
 
   if(eeprom_filename) {
     eeprom_file = fopen(eeprom_filename, "r+");
 
     if(!eeprom_file) {
       /* File does exist yet, so let's create it. */
       eeprom_file = fopen(eeprom_filename, "w+");
 
       if(!eeprom_file) {
         perror("Unable to create EEPROM file");
         exit(EXIT_FAILURE);
       }
     }
 
     fprintf(stderr, "eeprom_init: Using \"%s\".\n", eeprom_filename);
   } else {
     eeprom_file = tmpfile();
 
     if(!eeprom_file) {
       perror("Unable to create temporary EEPROM file");
       exit(EXIT_FAILURE);
     }
   }
 
   /* Make sure that the file is the correct size by seeking
    * to the end and checking the file position. If it is
    * less than what we expect, we pad out the rest of the file
    * with 0xFF, just like a real EEPROM. */
 
   fseek(eeprom_file, 0, SEEK_END);
   length = ftell(eeprom_file);
 
   if(length < 0) {
     perror("ftell failed");
     exit(EXIT_FAILURE);
   }
 
   if(length < EEPROM_END_ADDR) {
     /* Fill with 0xFF, just like a real EEPROM. */
     eeprom_fill(length, 0xFF, EEPROM_SIZE - length);
   }
 }
 
 /*---------------------------------------------------------------------------*/
 void
 eeprom_write(uint32_t addr, unsigned char *buf, int size)
 {
   if((addr > EEPROM_END_ADDR) || (addr+size > EEPROM_END_ADDR+1) || size < 0) {
     fprintf(stderr, "eeprom_write: Bad address and/or size (addr = %04x, size = %d)\n", addr, size);

     abort();
   }
 
   if(!eeprom_file) {
     eeprom_init();
   }
 
   fseek(eeprom_file, addr, SEEK_SET);
 
   if(fwrite(buf, 1, size, eeprom_file) != size) {
     perror("fwrite() failed");
     exit(EXIT_FAILURE);
   }
 }
 
 /*---------------------------------------------------------------------------*/
 void
 eeprom_read(uint32_t addr, unsigned char *buf, int size)
 {
   if((addr > EEPROM_END_ADDR) || (addr+size > EEPROM_END_ADDR+1) || size < 0) {
     fprintf(stderr, "eeprom_read: Bad address and/or size (addr = %04x, size = %d)\n", addr, size);
     abort();
   }
 
   if(!eeprom_file) {
     eeprom_init();
   }
 
   fseek(eeprom_file, addr, SEEK_SET);
 
   if(fread(buf, 1, size, eeprom_file) != size) {
     perror("fread() failed");
     exit(EXIT_FAILURE);
   }
 }
