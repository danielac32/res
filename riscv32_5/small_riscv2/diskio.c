/*-----------------------------------------------------------------------*/
/* MMCv3/SDv1/SDv2 (in SPI mode) control module                          */
/*-----------------------------------------------------------------------*/
/*
/  Copyright (C) 2014, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/
/* 29 Jul 2015                                                           */
/* Edit by mondul                                                        */
/* Edited for usage with the XC8 compiler and PIC18 microcontrollers.    */
/*                                                                       */
/* 9 Aug 2015                                                            */
/* Edit by mondul                                                        */
/* Added support for the XC16 compiler and PIC24 microcontrollers.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "reg.h"
 
 
/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

/* Definitions for MMC/SDC command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */


/* MMC card type flags (MMC_GET_TYPE) */
#define CT_MMC		0x01		/* MMC ver 3 */
#define CT_SD1		0x02		/* SD ver 1 */
#define CT_SD2		0x04		/* SD ver 2 */
#define CT_SDC		(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK	0x08		/* Block addressing */


static
DSTATUS Stat = STA_NOINIT;	/* Disk status */

static
BYTE CardType;			/* Card type flags */




void __delay_ms__(unsigned long t){               
unsigned long start_ms=0;             
   while(t>0){                     
      if(MS-start_ms>=1){            
         start_ms=MS;               
         t--;                      
      }                           
   }                               
}                         


void __delay_us__(unsigned long t) {              
unsigned long start_us=0;            
   while(t>0){                    
      if(US-start_us>=1){            
         start_us=US;                
         t--;                     
      }                           
   }                              
}    
/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv		/* Physical drive nmuber (0) */
)
{
asm volatile ("mv   a7, %0" : : "r" (SYSCALL_DISK_INITIALIZE));               
asm("ecall"); 
uint8_t x;
asm volatile ("mv   %0, a0" : "=r" (x));
return x;
}


/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0) */
)
{
asm volatile ("mv   a7, %0" : : "r" (SYSCALL_STATUS));               
asm("ecall"); 
uint8_t x;
asm volatile ("mv   %0, a0" : "=r" (x));
return x;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,			/* Physical drive nmuber (0) */
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	UINT count			/* Sector count (1..128) */
)
{
asm volatile ("mv   a7, %0" : : "r" (SYSCALL_READ_SECTOR));    
asm volatile ("mv   a0, %0" : : "r" ((uint32_t)buff));
asm volatile ("mv   a1, %0" : : "r" (sector));  
asm volatile ("mv   a2, %0" : : "r" (count));  
asm("ecall"); 
uint8_t x;
asm volatile ("mv   %0, a0" : "=r" (x));
return x;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0) */
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	UINT count			/* Sector count (1..128) */
)
{
asm volatile ("mv   a7, %0" : : "r" (SYSCALL_WRITE_SECTOR));    
asm volatile ("mv   a0, %0" : : "r" ((uint32_t)buff));
asm volatile ("mv   a1, %0" : : "r" (sector));  
asm volatile ("mv   a2, %0" : : "r" (count));  
asm("ecall"); 
uint8_t x;
asm volatile ("mv   %0, a0" : "=r" (x));
return x;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
asm volatile ("mv   a7, %0" : : "r" (SYSCALL_IOCTL));    
asm volatile ("mv   a0, %0" : : "r" (cmd));
asm volatile ("mv   a1, %0" : : "r" ((void*)buff));
asm("ecall"); 
uint8_t x;
asm volatile ("mv   %0, a0" : "=r" (x));
return x;
}
#endif
