/*  main.c  - main */

/* shell.c  -  shell */


#include <xinu.h>
#include "shprototypes.h"

 
/************************************************************************/
/* Table of Xinu shell commands and the function associated with each	*/
/************************************************************************/
//  struct	cmdent	cmdtab[] = {



  
 
shellcmd xsh_clear(int nargs, char *args[]){
  if (nargs > 1) {
	kprintf("use is: %s\n", args[0]);
	return 1;
    }

    kprintf("\033[0m\033[2J\033[H\n");
    sys(XINU_YIELD,0,0,0);

}
 
shellcmd xsh_echo(int nargs, char *args[]){
	int32	i;			/* walks through args array	*/

	if (nargs > 1) {
		kprintf("%s", args[1]);

		for (i = 2; i < nargs; i++) {
			kprintf(" %s", args[i]);
		}
	}
	kprintf("\n");
 
	INITRET;	
}

shellcmd xsh_exit(int nargs, char *args[]){
	kprintf("xsh_exit ok\n");
	sys(XINU_YIELD,0,0,0);
}


shellcmd xsh_help(int nargs, char *args[]){
	for (int i = 0; i < 6; ++i)
	{
		 kprintf("->%s\n",cmdtab[i].cname);
	}
	sys(XINU_YIELD,0,0,0);
}


shellcmd xsh_kill(int nargs, char *args[]){
    int32	retval;			/* return value			*/
	pid32	pid;			/* ID of process to kill	*/
	char	ch;			/* next character of argument	*/
	char	*chptr;			/* walks along argument string	*/

	/* Check argument count */

	if (nargs != 2) {
		return SYSERR;
	}

	/* compute process ID from argument string */

	chptr = args[1];
	ch = *chptr++;
	pid = 0;
	while(ch != NULLCH) {
		if ( (ch < '0') || (ch > '9') ) {
			INITRET;//return 1;
		}
		pid = 10*pid + (ch - '0');
		ch = *chptr++;
	}
	if (pid == 0) {
		INITRET;
	}

	retval = kill(pid);
	if (retval == SYSERR) {
		kprintf("%s: cannot kill process %d\n",	args[0], pid);
		 
	}
 
 
	INITRET;	//
}



const  char ps_msg0[] = "\ntable of current processes\n";
const  char ps_msg1[] = "\nname\tid\tparent\tprio\tstate\tstklen\tsem waits\n--\n";
 
shellcmd xsh_ps(int nargs, char *args[]){
    int32	i;			/* walks through args array	*/
   struct	procent	*prptr;

	kprintf("%s", ps_msg0);
	kprintf("%s", ps_msg1);
        for (i = 0; i < NPROC; i++) {
        	prptr = &tasks[i];
		if (tasks[i].prstate == PR_FREE)
			continue;
		kprintf("%s\t%d", prptr->prname, i);
		kprintf("\t%d ", prptr->prparent);
		kprintf("\t%d ", prptr->prprio);
		kprintf("\t%d ", prptr->prstate);
		kprintf("\t%d ", prptr->prstkptr);
		kprintf("\t%d ", prptr->prsem);
		kprintf("\n");
	}
 
	kmalloc_debug();
	INITRET;	
	//return INITRET;

}
 
static void printnl(void)
{
	kprintf("\n\r");
}


/*------------------------------------------------------------------------
 * xsh_memdump - Print memory content
 *------------------------------------------------------------------------
 */

uint32 getn(char *chptr){
 pid32	retval;

    char	ch;

   
	ch = *chptr++;
	retval = 0;
	while(ch != NULLCH) {
		if ( (ch < '0') || (ch > '9') ) {
			INITRET;//return 1;
		}
		retval = 10*retval + (ch - '0');
		ch = *chptr++;
	}

	return retval;
}


extern uint32 _istack;
extern uint32 _estack;
shellcmd xsh_memdump(int nargs, char *args[])
{

	 int from = (uint32)&_istack;
	 int to = (uint32)&_estack;		/* AVR atmega328p top RAM address */
   

    kprintf("%x,%x\n",_istack,_estack);
	if (nargs == 3) {
		from= getn(args[1]);
        to = getn(args[2]);
	}

	long i;
	int j=0;
	char * c = 0;

	for (i = from; i < to ; i++) {
		c = (char *)i;
		if (j==0) {
			kprintf("\n");
			kprintf ("0x%x  ", c);
		}
		j++;
		if (j==16) j=0;
		if (*c < 33)
			kprintf("-");
		else
			kprintf("%c", c);
 
	}
	printnl();

	INITRET;	
}


shellcmd xsh_devdump (
	 int	nargs,			/* number of arguments		*/
	 char	*args[]			/* list of arguments		*/
 	)
{
	struct	dentry	*devptr;	/* pointer to device entry	*/
	int32	i;			/* walks through device table	*/

	/* No arguments are expected */

	if (nargs > 1) {
		kprintf("No arguments allowed\n");
		INITRET;	
	}

	/* Walk through device table */

	kprintf("\nDevice\tName\tMinor\n");
	kprintf("-----------------------\n");

	for (i = 0; i < NDEVS; i++) {
		devptr = &devtab[i];
		kprintf("%d", i);
		kprintf("\t%s",devptr->dvname);
		kprintf("\t%d",devptr->dvminor);
		kprintf("\n");
	}

	INITRET;	
}



 
const	struct	cmdent	cmdtab[] = {
	{"clear",	TRUE,	xsh_clear},
	{"echo",	FALSE,	xsh_echo},
	{"dump",	FALSE,	xsh_memdump},
	{"help",	TRUE,	xsh_help},
	{"kill",	FALSE,	xsh_kill},
	{"ps",		FALSE,	xsh_ps},
	{"?",		TRUE,	xsh_help},
	{"dev",     FALSE,  xsh_devdump}
};

uint32	ncmd = sizeof(cmdtab) / sizeof(struct cmdent);

void	prod2(int nargs, char *args[]);
void	cons2(int nargs, char *args[]);

int32	n = 0;	


 
 
void	cons2(int nargs, char *args[])
{
	 
	int32	i=0;
    kprintf("test\n");
	while(1){
		kprintf("%i\n",i++ );
		sys(XINU_YIELD,0,0,0);
	}
	INITRET;	
}

process	main(void)
{
	 char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
	int32	len;			/* Length of line read		*/
	char	tokbuf[SHELL_BUFLEN +	/* Buffer to hold a set of	*/
			SHELL_MAXTOK];  /* Contiguous null-terminated	*/
	int32	tlen;			/* Current length of all data	*/
					/*   in array tokbuf		*/
	int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
	int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
	int32	ntok;			/* Number of tokens on line	*/
	pid32	child;			/* Process ID of spawned child	*/
	bool8	backgnd;		/* Run command in background?	*/
	char	*outname, *inname;	/* Pointers to strings for file	*/
					/*   names that follow > and <	*/
	did32	stdinput, stdoutput;	/* Descriptors for redirected	*/
					/*   input and output		*/
	int32	i;			/* Index into array of tokens	*/
	int32	j;			/* Index into array of commands	*/
	int32	msg;			/* Message from receive() for	*/
					/*   child termination		*/
	int32	tmparg;			/* Address of this var is used	*/
					/*   when first creating child	*/
					/*   process, but is replaced	*/
	char	*src, *cmp;		/* Pointers used during name	*/
					/*   comparison			*/
	bool8	diff;			/* Was difference found during	*/
	char	*args[SHELL_MAXTOK];	/* Argument vector passed to	*/
					/*   builtin commands		*/
	did32	dev = 0;		/* ID of tty device from which	*/

	char cname[8];
    
   
    //sys(XINU_CREATE,"cons",cons2,0);
 

	while (TRUE) {
           
         kprintf(SHELL_PROMPT);

         len = read(dev, buf, sizeof(buf));
            //kprintf("%s\n",buf);
 
           
		/* Exit gracefully on end-of-file */
        //sys(XINU_YIELD,0,0,0);
		if (len == EOF) {
			 //sys(XINU_YIELD,0,0,0);
			break;
		}

		/* If line contains only NEWLINE, go to next line */

		if (len <= 1) {
			//sys(XINU_YIELD,0,0,0);
			continue;
		}

		buf[len] = SH_NEWLINE;	/* terminate line */

		/* Parse input line and divide into tokens */

		ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);

		/* Handle parsing error */

		if (ntok == SYSERR) {
			kprintf("%s\n", SHELL_SYNERRMSG);
			//sys(XINU_YIELD,0,0,0);
			continue;
		}
        
        if (ntok == 0) {
			kprintf("\n");
			//sys(XINU_YIELD,0,0,0);
			continue;
		}

		/* If last token is '&', set background */

		if (toktyp[ntok-1] == SH_TOK_AMPER) {
			ntok-- ;
			tlen-= 2;
			backgnd = TRUE;
		} else {
			backgnd = FALSE;
		}

        outname = inname = NULL;
		if ( (ntok >=3) && ( (toktyp[ntok-2] == SH_TOK_LESS)
				   ||(toktyp[ntok-2] == SH_TOK_GREATER))){
			if (toktyp[ntok-1] != SH_TOK_OTHER) {
				kprintf("%s\n", SHELL_SYNERRMSG);
				//sys(XINU_YIELD,0,0,0);
				continue;
			}
			if (toktyp[ntok-2] == SH_TOK_LESS) {
				inname =  &tokbuf[tok[ntok-1]];
			} else {
				outname = &tokbuf[tok[ntok-1]];
			}
			ntok -= 2;
			tlen = tok[ntok];
		}


		if ( (ntok >=3) && ( (toktyp[ntok-2] == SH_TOK_LESS)
				   ||(toktyp[ntok-2] == SH_TOK_GREATER))){
			if (toktyp[ntok-1] != SH_TOK_OTHER) {
				kprintf("%s\n", SHELL_SYNERRMSG);
				//sys(XINU_YIELD,0,0,0);
				continue;
			}
			if (toktyp[ntok-2] == SH_TOK_LESS) {
				if (inname != NULL) {
				    kprintf("%s\n", SHELL_SYNERRMSG);
				    //sys(XINU_YIELD,0,0,0);
				    continue;
				}
				inname = &tokbuf[tok[ntok-1]];
			} else {
				if (outname != NULL) {
				    kprintf("%s\n", SHELL_SYNERRMSG);
				    //sys(XINU_YIELD,0,0,0);
				    continue;
				}
				outname = &tokbuf[tok[ntok-1]];
			}
			ntok -= 2;
			tlen = tok[ntok];
		}

		/* Verify remaining tokens are type "other" */

		for (i=0; i<ntok; i++) {
			if (toktyp[i] != SH_TOK_OTHER) {
				break;
			}
		}
		if ((ntok == 0) || (i < ntok)) {
			kprintf(SHELL_SYNERRMSG);
			//sys(XINU_YIELD,0,0,0);
			continue;
		}

		stdinput = stdoutput = dev;
        
        for (j = 0; j < ncmd; j++) {
			src = cmdtab[j].cname;
			cmp = tokbuf;
			diff = FALSE;
			while (*src != NULLCH) {
				if (*cmp != *src) {
					diff = TRUE;
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
			kprintf( "command %s not found\n", tokbuf);
			//sys(XINU_YIELD,0,0,0);
			continue;
		}
       // sys(XINU_YIELD,0,0,0);

       if (cmdtab[j].cbuiltin) { /* No background or redirect. */
       	  
			if (inname != NULL || outname != NULL || backgnd){
				kprintf(SHELL_BGERRMSG);
				continue;
			} else {
				/* Set up arg vector for call */

				for (i=0; i<ntok; i++) {
					args[i] = &tokbuf[tok[i]];
				}

				/* Call builtin shell function */

				if ((*cmdtab[j].cfunc)(ntok, args)
							== SHELL_EXIT) {
					break;
				}
			}
			continue;
		}else{ 
			 
             for (int i=0; i<ntok; i++) {
			      args[i] = &tokbuf[tok[i]];
		     }
		      
             child = sys(XINU_CREATE,cmdtab[j].cname,cmdtab[j].cfunc,ntok, args[0],args[1],args[2],args[3],args[4],args[5]);
           
             if ((child == SYSERR)) { 
			      kprintf(SHELL_CREATMSG);

			      continue;
		     }

		     tasks[child].prdesc[0] = stdinput;
		     tasks[child].prdesc[1] = stdoutput;
            
 
		}

     
   

	 
        sys(XINU_YIELD,0,0,0);
		    
    }

    /* Terminate the shell process by returning from the top level */

    // fprintf(dev,SHELL_EXITMSG);
    return OK;
}
