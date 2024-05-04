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
#define GPIO1                2	/* type gpio     */
#define GPIO2                3	/* type gpio     */
#define GPIO3                4	/* type gpio     */
#define GPIO4                5	/* type gpio     */
#define GPIO5                6	/* type gpio     */
#define GPIO6                7	/* type gpio     */
#define NULLDEV              8	/* type null     */
#define NAMESPACE            9	/* type nam      */
#define EEPROM0             10	/* type eeprom   */
#define VFS                 11	/* type eeprom   */
#define SDCARD              12	/* type eeprom   */
#define SRAM                13	/* type eeprom   */

/* Control block sizes */

#define	Nnull	1
#define	Ngpio	7
#define	Ntty	1
#define	Neeprom	4
#define	Nnam	1

#define NDEVS 14


/* Configuration and Size Constants */

#define	NPROC	     10		/* number of user processes		*/
#define	NSEM	     10		/* number of semaphores			*/
