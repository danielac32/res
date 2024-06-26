/* conf.h (GENERATED FILE; DO NOT EDIT) */

/* Device switch table declarations */

/* Device table entry */
struct	dentry	{
	int32   dvnum;
	int32   dvminor;
	char    dvname[10];
	devcall (*dvinit) (struct dentry *);
	devcall (*dvopen) (struct dentry *, char *, char *);
	devcall (*dvclose)(struct dentry *);
	devcall (*dvread) (struct dentry *, void *, uint32);
	devcall (*dvwrite)(struct dentry *, void *, uint32);
	devcall (*dvseek) (struct dentry *, int32);
	devcall (*dvgetc) (struct dentry *);
	devcall (*dvputc) (struct dentry *, char);
	devcall (*dvcntl) (struct dentry *, int32, int32, int32);
	void    *dvcsr;
	void    (*dvintr)(void);
	byte    dvirq;
};

extern	 struct dentry	devtab[] ; /* one entry per device */

/* Device name definitions */

#define CONSOLE              0	/* type tty      */
#define GPIO0                1	/* type gpio     */
#define NULLDEV              2	/* type null     */
#define NAMESPACE            3	/* type nam      */
#define EEPROM0              4	/* type eeprom   */

/* Control block sizes */

#define	Nnull	1
#define	Ngpio	1
#define	Ntty	1
#define	Neeprom	1
#define	Nnam	1

#define NDEVS 5


/* Configuration and Size Constants */

#define	NPROC	     9		/* number of user processes		*/
#define	NSEM	     9		/* number of semaphores			*/
