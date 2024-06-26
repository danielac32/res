/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
//#include "ramdisk.h"
#include <string.h>
#include "spi.h"

#include "timer.h"
#include "gpio.h"
#include "uart.h"




/* Definitions of physical drive number for each drive */
#define DEV_RAM		1	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		0	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */




#define CFG_RAMDISK_SIZE               (/*500 * 1024*/100000)
#define CFG_RAMDISK_SECTOR_SIZE        (512)
#define RAMDISK_SECTOR_TOTAL           ((CFG_RAMDISK_SIZE) / (CFG_RAMDISK_SECTOR_SIZE))

static unsigned char rambuf[RAMDISK_SECTOR_TOTAL][CFG_RAMDISK_SECTOR_SIZE];

int RAM_disk_status(void)
{
    return 0;
}

int RAM_disk_initialize(void)
{
    return 0;
}

int RAM_disk_read(unsigned char *buff, unsigned int sector, unsigned int count)
{
    memcpy(buff, &rambuf[sector], count * CFG_RAMDISK_SECTOR_SIZE);
    return 0;
}

int RAM_disk_write(const unsigned char *buff, unsigned int sector, unsigned int count)
{
    memcpy(&rambuf[sector], buff, count * CFG_RAMDISK_SECTOR_SIZE);
    return 0;
}

int RAM_disk_ioctl(unsigned char cmd, void *buff)
{
    int res;

    switch (cmd) {
    case CTRL_SYNC:
        res = RES_OK;
        break;
    case GET_SECTOR_SIZE:
        *(DWORD *)buff = CFG_RAMDISK_SECTOR_SIZE;
        res = RES_OK;
        break;
    case GET_BLOCK_SIZE:
        *(WORD *)buff = 1;
        res = RES_OK;
        break;
    case GET_SECTOR_COUNT:
        *(DWORD *)buff = RAMDISK_SECTOR_TOTAL;
        res = RES_OK;
        break;
    default:
        res = RES_PARERR;
        break;
    }

    return res;
}

DWORD get_fattime(void)
{
	return 0;
}



 
#define sd_tx(d)  transfer(d)
#define sd_rx()   transfer(0xff)

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
DSTATUS Stat = STA_NOINIT;	/* Disk status */


BYTE CardType;			/* Card type flags */



BYTE wait_ready (void)  /* 1:Ready, 0:Timeout */
{
  uint16_t tmr;


  for (tmr = 5000; tmr; tmr--) {  /* Wait for ready in timeout of 500ms */
    if (sd_rx() == 0xFF) break;
    delay(100);
  }

  return tmr ? 1 : 0;
}

void deselect (void)
{
  spi_cs(1);
  sd_rx();  /* Dummy clock (force DO hi-z for multiple slave SPI) */
}

BYTE select (void)  /* 1:Successful, 0:Timeout */
{
   spi_cs(0);
   sd_rx();  /* Dummy clock (force DO enabled) */
   if (wait_ready()) return 1; /* Wait for card ready */

   deselect();
   return 0; /* Timeout */
}

BYTE rcvr_datablock (
  BYTE *buff,     /* Data buffer to store received data */
  uint16_t btr      /* BYTE count (must be multiple of 4) */
)
{
  BYTE token;
  uint16_t tmr;
  uint16_t block=512;
 // uint32 mask=disable();
  for (tmr = 2000; tmr; tmr--) {  /* Wait for data packet in timeout of 200ms */
    token = sd_rx();
    if (token != 0xFF) break;
    delay(100);
  }
  if (token != 0xFE) {
     ////restore(mask);
     return 0;  /* If not valid data token, retutn with error */
  }
  do
    *buff++ = sd_rx();    /* Receive the data block into buffer */
  while (--btr);
  sd_rx();          /* Discard CRC */
  sd_rx();

  ////restore(mask);
  return 1;         /* Return with success */
}

BYTE xmit_datablock (
  BYTE *buff, /* 512 BYTE data block to be transmitted */
  BYTE token      /* Data/Stop token */
)
{
  BYTE resp;
  uint16_t i;
  uint16_t block=512;
 // uint32 mask=disable();

  if (!wait_ready()) {
    ////restore(mask);
    return 0;
  }

  sd_tx(token);     /* Xmit data token */
  if (token != 0xFD) {  /* Is data token */
    i = 512;
    do
      sd_tx(*buff++);       /* Xmit the data block  the MMC */
    while (--i);
    sd_rx();            /* CRC (Dummy) */
    sd_rx();
    resp = sd_rx();         /* Reveive data response */
    if ((resp & 0x1F) != 0x05){    /* If not accepted, return with error */
      ////restore(mask);
      return 0;
    }
  }

  ////restore(mask);
  return 1;
}


BYTE __send_cmd (    /* Returns R1 resp (bit7==1:Send failed) */
  BYTE cmd,   /* Command index */
  uint32_t arg   /* Argument */
)
{
  BYTE n, res;


  /* Select the card and wait for ready except to stop multiple block read */
  if (cmd != CMD12) {
    deselect();
    if (!select()) return 0xFF;
  }

  /* Send command packet */
  sd_tx(0x40 | cmd);        /* Start + Command index */
  sd_tx((BYTE)(arg >> 24));   /* Argument[31..24] */
  sd_tx((BYTE)(arg >> 16));   /* Argument[23..16] */
  sd_tx((BYTE)(arg >> 8));    /* Argument[15..8] */
  sd_tx((BYTE)arg);       /* Argument[7..0] */
  n = 0x01;           /* Dummy CRC + Stop */
  if (cmd == CMD0) n = 0x95;    /* Valid CRC for CMD0(0) + Stop */
  if (cmd == CMD8) n = 0x87;    /* Valid CRC for CMD8(0x1AA) Stop */
  sd_tx(n);

  /* Receive command response */
  if (cmd == CMD12) sd_rx();    /* Skip a stuff BYTE when stop reading */
  n = 10;               /* Wait for a valid response in timeout of 10 attempts */
  do
    res = sd_rx();
  while ((res & 0x80) && --n);

  return res;     /* Return with the response value */
}



BYTE send_cmd (    /* Returns R1 resp (bit7==1:Send failed) */
  BYTE cmd,   /* Command index */
  uint32_t arg   /* Argument */
)
{
  BYTE res;

  if (cmd & 0x80) { /* ACMD<n> is the command sequense of CMD55-CMD<n> */
      cmd &= 0x7F;
      res = __send_cmd(CMD55, 0);
      if (res > 1) return res;
  }
  return __send_cmd(cmd, arg);  /* Return with the response value */
}






DSTATUS mmc_initialize (){
BYTE n, cmd, ty, ocr[4];
  uint16_t tmr;
 	DSTATUS s;
 
  if (Stat & STA_NODISK){
 
     printf("error disk\n");
     return Stat; /* No card in the socket */
  }
    spi_init();

    delay(100);

  for (n = 10; n; n--) sd_rx(); /* 80 dummy clocks */

  ty = 0;
  if (send_cmd(CMD0, 0) == 1) {     /* Enter Idle state */
    if (send_cmd(CMD8, 0x1AA) == 1) { /* SDv2? */
      for (n = 0; n < 4; n++) ocr[n] = sd_rx(); /* Get trailing return value of R7 resp */
      if (ocr[2] == 0x01 && ocr[3] == 0xAA) {   /* The card can work at vdd range of 2.7-3.6V */
        for (tmr = 1000; tmr; tmr--) {      /* Wait for leaving idle state (ACMD41 with HCS bit) */
          if (send_cmd(ACMD41, 1UL << 30) == 0) break;
          delay(10);
        }
        if (tmr && send_cmd(CMD58, 0) == 0) {   /* Check CCS bit in the OCR */
          for (n = 0; n < 4; n++) ocr[n] = sd_rx();
          ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;  /* SDv2 */
        }
      }
    } else {              /* SDv1 or MMCv3 */
      if (send_cmd(ACMD41, 0) <= 1)   {
        ty = CT_SD1; cmd = ACMD41;  /* SDv1 */
      } else {
        ty = CT_MMC; cmd = CMD1;  /* MMCv3 */
      }
      for (tmr = 1000; tmr; tmr--) {      /* Wait for leaving idle state */
        if (send_cmd(cmd, 0) == 0) break;
        delay(10);
      }
      if (!tmr || send_cmd(CMD16, 512) != 0)  /* Set R/W block length to 512 */
        ty = 0;
    }
  }
  CardType = ty;
   s = ty ? 0 : STA_NOINIT;
Stat = s;

  deselect();

  return s;
}
DSTATUS mmc_status (){
return 0;
}
DRESULT mmc_read (BYTE* buff, DWORD sector, UINT count){
 BYTE cmd;


  if (Stat & STA_NOINIT) return RES_NOTRDY;
   
 
  if (!(CardType & CT_BLOCK)) sector *= 512;  /* Convert to BYTE address if needed */

  if (count == 1) { /* Single sector read */
    if ((send_cmd(CMD17, sector) == 0)  /* READ_SINGLE_BLOCK */
      && rcvr_datablock(buff, 512)) {
      count = 0;
    }
  }
  else {        /* Multiple sector read */
    if (send_cmd(CMD18, sector) == 0) { /* READ_MULTIPLE_BLOCK */
      do {
        if (!rcvr_datablock(buff, 512)) break;
        buff += 512;
      } while (--count);
      send_cmd(CMD12, 0);       /* STOP_TRANSMISSION */
    }
  }
  deselect();
 
  return count ? RES_ERROR : RES_OK;
}
DRESULT mmc_write (const BYTE* buff, DWORD sector, UINT count){
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  if (Stat & STA_PROTECT) return RES_WRPRT;
 
  if (!(CardType & CT_BLOCK)) sector *= 512;  /* Convert to BYTE address if needed */

  if (count == 1) { /* Single block write */
    if ((send_cmd(CMD24, sector) == 0)  /* WRITE_BLOCK */
      && xmit_datablock((BYTE*)buff, 0xFE))
      count = 0;
  }
  else {        /* Multiple block write */
    if (CardType & CT_SDC) send_cmd(ACMD23, count);
    if (send_cmd(CMD25, sector) == 0) { /* WRITE_MULTIPLE_BLOCK */
      do {
        if (!xmit_datablock((BYTE*)buff, 0xFC)) break;
        buff += 512;
      } while (--count);
      if (!xmit_datablock(0, 0xFD)) count = 1; /* STOP_TRAN token */
    }
  }
  deselect();

 
  return count ? RES_ERROR : RES_OK;
}
DRESULT mmc_ioctl (BYTE cmd, void* buff){
DRESULT res;
	BYTE n, csd[16];
	DWORD *dp, st, ed, csize;


	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	res = RES_ERROR;

	switch (cmd) {
	case CTRL_SYNC :		/* Wait for end of internal write process of the drive */
		if (select()) res = RES_OK;
		break;

	case GET_SECTOR_COUNT :	/* Get drive capacity in unit of sector (DWORD) */
		if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
			if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
				csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
				*(DWORD*)buff = csize << 10;
			} else {					/* SDC ver 1.XX or MMC ver 3 */
				n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
				csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
				*(DWORD*)buff = csize << (n - 9);
			}
			res = RES_OK;
		}
		break;

	case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
		if (CardType & CT_SD2) {	/* SDC ver 2.00 */
			if (send_cmd(ACMD13, 0) == 0) {	/* Read SD status */
				sd_tx(0xFF);
				if (rcvr_datablock(csd, 16)) {				/* Read partial block */
					for (n = 64 - 16; n; n--) sd_tx(0xFF);	/* Purge trailing data */
					*(DWORD*)buff = 16UL << (csd[10] >> 4);
					res = RES_OK;
				}
			}
		} else {					/* SDC ver 1.XX or MMC */
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {	/* Read CSD */
				if (CardType & CT_SD1) {	/* SDC ver 1.XX */
					*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
				} else {					/* MMC */
					*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
				}
				res = RES_OK;
			}
		}
		break;


	default:
		res = RES_PARERR;
	}

	deselect();

	return res;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
	case DEV_RAM :
	    stat = RAM_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		 stat = mmc_status(pdrv);
        
		// translate the reslut code here

		return 0;

 
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
	case DEV_RAM :
	    stat = RAM_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		 stat = mmc_initialize();

		// translate the reslut code here

		return stat;

 
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

	    res = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		res = mmc_read(buff, sector, count);

		// translate the reslut code here

		return res;

	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

	    res = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		res = mmc_write(buff, sector, count);

		// translate the reslut code here

		return res;
}
return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case DEV_RAM :
        res = RAM_disk_ioctl(cmd, buff);
		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :
        res = mmc_ioctl(cmd,buff);
		return res;
	}

	return RES_PARERR;
}


