/* ramdisk.h - definitions for a ram disk (for testing) */

/* Ram disk block size */

// #define	RM_BLKSIZ	512		/* block size			*/
// #define	RM_BLKS		200		/* number of blocks		*/

#define	RM_BLKSIZ	4		/* block size			*/
#define	RM_BLKS		2		/* number of blocks		*/
/*
struct	ramdisk	{
	char	disk[RM_BLKSIZ * RM_BLKS];
	};

extern	struct	ramdisk	Ram;
*/
int eeprom_pos;
#define EEPROM_SIZE 1000