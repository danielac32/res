#ifndef FATFS_H
#define FATFS_H
#include <stdint.h>
#include <stdbool.h>
#include "ff.h"
#include "diskio.h"


FATFS    FatFs;
FILINFO  finfo;
DIR      dir;
FIL      ffile;


#define FILE            FIL




int32_t  capacity();
int32_t  free();
uint8_t  error();
  

bool     beginFat();

bool     mkdir( const char * path );
bool     rmdir( const char * path );
bool     remove( const char * path );
bool     rename( const char * oldName, const char * newName );
bool     exists( const char * path );
bool     isDir( const char * path );

bool     openDir( char * dirPath );
bool open( char * fileName, uint8_t mode );

bool     close();
bool     nextFile();
bool     rewind();
bool     isDir();
char *   fileName();
uint32_t fileSize();

uint32_t read( void * buf, uint32_t lbuf );
uint32_t write( void * buf, uint32_t lbuf );


uint32_t curPosition();
bool     seekSet( uint32_t cur );



#endif