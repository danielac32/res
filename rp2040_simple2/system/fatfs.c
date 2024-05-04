#include "fatfs.h"
#include "uart.h"
#include <string.h>

uint8_t ffs_result;


bool beginFat(){
	static BYTE work[FF_MAX_SS];
    if ((ffs_result = f_mount(&FatFs, "0:", 1)) != FR_OK) {
        printf("mount error! res=%d\n", ffs_result);
        printf("format...\n");
        if ((ffs_result = f_mkfs("0:/", FM_ANY, 0, work, sizeof(work))) != FR_OK) {
             printf("mkfs error! res=%d\n", ffs_result);
             return false;
        }
        printf("mkfs successfully\n");
    }
    printf("mount successfully\n");
    return ffs_result == FR_OK;
}

int32_t capacity()
{
  return ( FatFs.n_fatent - 2 ) * FatFs.csize >> 1; // >> 11;
}

/*
bool  mkdir( const char * path )
{
  ffs_result = f_mkdir( path );
  return ffs_result == FR_OK; // || res == FR_EXIST;
}

// Remove a directory (must be empty)
//   path : absolute name of directory to remove
// Return true if ok

bool rmdir( const char * path )
{
  return remove( path );
}



bool remove( const char * path )
{
  ffs_result = f_unlink( path );
  return ffs_result == FR_OK;
}


bool rename( const char * oldName, const char * newName )
{
  // f_rename modify the value pointed by parameters oldName0 and newName0
  const char * oldName0 = oldName;
  const char * newName0 = newName;
  ffs_result = f_rename( oldName0, newName0 );
  return ffs_result == FR_OK;
}

// Return true if a file or directory exists
//   path : absolute name of file or directory

bool exists( const char * path )
{
  if( strcmp( path, "/" ) == 0 )
    return true;
  return f_stat( path, (void *)0 ) == FR_OK;
}

// Return true if a absolute name correspond to an existing directory

bool isDir( const char * path )
{
  if( strcmp( path, "/" ) == 0 )
    return true;
  
  FILINFO finfo;
  return ( f_stat( path, & finfo ) == FR_OK ) &&
         ( finfo.fattrib & AM_DIR );
}


bool openDir( char * dirPath )
{
  ffs_result = f_opendir( & dir, dirPath );
  return ffs_result == FR_OK;
}




bool open( char * fileName, uint8_t mode )
{
  ffs_result = f_open( & ffile, fileName, mode );
  return ffs_result == FR_OK;
}

uint32_t write( void * buf, uint32_t lbuf )
{
  uint32_t lb, nwrt0, nwrt = 0;
  
  ffs_result = FR_OK;
  while( nwrt < lbuf && ffs_result == FR_OK )
  {
    nwrt0 = 0;
    lb = lbuf - nwrt;
    if( lb > FF_MIN_SS )
      lb = FF_MIN_SS;
    ffs_result = f_write( & ffile, ( buf + nwrt ), lb, (UINT*) & nwrt0 );
    nwrt += nwrt0;
  }
  return nwrt;
}

uint32_t read( void * buf, uint32_t lbuf )
{
  uint32_t lb, nrd0, nrd;
  
  nrd = 0;
  do
  {
    nrd0 = 0;
    lb = lbuf - nrd;
    if( lb > FF_MIN_SS )
      lb = FF_MIN_SS;
    ffs_result = f_read( & ffile, ( buf + nrd ), lb, (UINT*) & nrd0 );
    nrd += nrd0;
  }
  while( nrd0 > 0 && nrd < lbuf && ffs_result == FR_OK );
  return nrd;
}

uint32_t curPosition()
{
  return f_tell( & ffile );
}

// Moves the file read/write pointer
//   cur : new file pointer value
// Return if ok
// In case cur is greater than file size and file is opened in write mode,
//   size of file is expanded

bool seekSet( uint32_t cur )
{
  ffs_result = f_lseek( & ffile, cur );
  return ffs_result == FR_OK;
}

// Return size of file

uint32_t fileSize()
{
  return f_size( & ffile );
}*/