/*  main.c  - main */

/* shell.c  -  shell */


#include <xinu.h>

 
void core1(int nargs, char *args[]){
    int i=0;
	while(1){
        kprintf("%d\n",i++);
        sleepms(10);
	}
}


void blink(int nargs, char *args[]){
    
	while(1){
 
		 gpio_arduino_write(13, 1);
         sleepms(50);
		 gpio_arduino_write(13, 0);
		 sleepms(50);
	}
}



const  char ps_msg0[] = "\ntable of current processes\n";
const  char ps_msg1[] = "\nname\tid\tparent\tprio\tstate\tstklen\tsem waits\n--\n";

shellcmd xsh_ps(int nargs, char *args[]){
int32	i;			/* walks through args array	*/
 

	kprintf("%s", ps_msg0);
	kprintf("%s", ps_msg1);
        for (i = 0; i < NPROC; i++) {
		if (proctab[i].prstate == PR_FREE)
			continue;
		kprintf("%s\t%d", proctab[i].prname, i);
		kprintf("\t%d ", proctab[i].prparent);
		kprintf("\t%d ", proctab[i].prprio);
		kprintf("\t%d ", proctab[i].prstate);
		kprintf("\t%d ", proctab[i].prstklen);
		kprintf("\t%d ", proctab[i].prsem);
		kprintf("\n");
	}


	return 0;

}

shellcmd xsh_help(int nargs, char *args[]){

return 0;
}

shellcmd xsh_devdump(int nargs, char *args[]){

struct	dentry	*devptr;	/* pointer to device entry	*/
	int32	i;			/* walks through device table	*/

	/* No arguments are expected */

	if (nargs > 1) {
		kprintf("No arguments allowed\n");
		return 1;
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

	return 0;
}



int printFreeList(void)
{
	struct memblk *block;

 

	kprintf("Free List:\n");
	kprintf("\nBlock\taddress\tLength\t(dec)\tLength\t(hex)\n");
	kprintf("---------------------------------------------\n");
	
	for (block = memlist.mnext; block != NULL; block = block->mnext) {
		//kprintf("  0x%08x    %9d     0x%08x\n", block,block->mlength, block->mlength);

		kprintf("0x%08x", block);
		kprintf("\t%9d",block->mlength);
		kprintf("\t0x%08x",block->mlength);
		kprintf("\n");

	}
	kprintf("\n");
	return 0;
}

const	struct	cmdent	cmdtab[] = {
	{"ps",		TRUE,	xsh_ps},
	//{"?",		TRUE,	xsh_help},
	{"dev",     TRUE,  xsh_devdump},
	//{"mem",     TRUE,  printFreeList}
};

uint32	ncmd = sizeof(cmdtab) / sizeof(struct cmdent);



status	addargs(
	  pid32		pid,		/* ID of process to use		*/
	  int32		ntok,		/* Count of arguments		*/
	  int32		tok[],		/* Index of tokens in tokbuf	*/
	  int32		tlen,		/* Length of data in tokbuf	*/
	  char		*tokbuf,	/* Array of null-term. tokens	*/
	  void 		*dummy		/* Dummy argument that was	*/
					/*   used at creation and must	*/
					/*   be replaced by a pointer	*/
					/*   to an argument vector	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	uint32	aloc;			/* Argument location in process	*/
					/*   stack as an integer	*/
	uint32	*argloc;		/* Location in process's stack	*/
					/*   to place args vector	*/
	char	*argstr;		/* Location in process's stack	*/
					/*   to place arg strings	*/
	uint32	*search;		/* pointer that searches for	*/
					/*   dummy argument on stack	*/
	uint32	*aptr;			/* Walks through args array	*/
	int32	i;			/* Index into tok array		*/

	//mask = disable();

	/* Check argument count and data length */

	if ( (ntok <= 0) || (tlen < 0) ) {
		// serial_put_char('M');
		//restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

 
	for (i=0 ; i < ntok; i++) {
 
		prptr->parg[i] = &tokbuf[tok[i]];

	}
//	*toarg = 0;
	prptr->pargs = ntok;
	prptr->parg[ntok] = 0;
 
	prptr->pregs[24] = lobyte((unsigned)prptr->pargs);	/*r24*/
	prptr->pregs[25] = hibyte((unsigned)prptr->pargs);
	

	//restore(mask);
	return OK;
//	return SYSERR;
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
 
	kprintf( "%s\n\n", SHELL_STRTMSG);

	/* Continually prompt the user, read input, and execute command	*/

	//printFreeList();

	while (TRUE) {

		/* Display prompt */
		kprintf( SHELL_PROMPT);


		len = read(dev, buf, sizeof(buf));

		/* Exit gracefully on end-of-file */

		if (len == EOF) {
			break;
		}

		/* If line contains only NEWLINE, go to next line */

		if (len <= 1) {
			continue;
		}

		buf[len] = SH_NEWLINE;	/* terminate line */

		/* Parse input line and divide into tokens */

		ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);

		/* Handle parsing error */

		if (ntok == SYSERR) {
			kprintf("%s\n", SHELL_SYNERRMSG);
			continue;
		}

		/* If line is empty, go to next input line */

		if (ntok == 0) {
			kprintf( "\n");
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


		/* Check for input/output redirection (default is none) */

		outname = inname = NULL;
		if ( (ntok >=3) && ( (toktyp[ntok-2] == SH_TOK_LESS)
				   ||(toktyp[ntok-2] == SH_TOK_GREATER))){
			if (toktyp[ntok-1] != SH_TOK_OTHER) {
				kprintf("%s\n", SHELL_SYNERRMSG);
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
				continue;
			}
			if (toktyp[ntok-2] == SH_TOK_LESS) {
				if (inname != NULL) {
				    kprintf("%s\n", SHELL_SYNERRMSG);
				    continue;
				}
				inname = &tokbuf[tok[ntok-1]];
			} else {
				if (outname != NULL) {
				    kprintf("%s\n", SHELL_SYNERRMSG);
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
			kprintf( SHELL_SYNERRMSG);
			continue;
		}

		stdinput = stdoutput = dev;

		/* Lookup first token in the command table */

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
			continue;
		}

		/* Handle built-in command */

		if (cmdtab[j].cbuiltin) { /* No background or redirect. */
			if (inname != NULL || outname != NULL || backgnd){
				kprintf( SHELL_BGERRMSG);
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
		}

		 
		/* Spawn child thread for non-built-in commands */

		child = create(cmdtab[j].cfunc,
			SHELL_CMDSTK, SHELL_CMDPRIO,
			cmdtab[j].cname, 2, ntok, &tmparg);

		/* If creation or argument copy fails, report error */

		if ((child == SYSERR) ||
		    (addargs(child, ntok, tok, tlen, tokbuf, &tmparg)
							== SYSERR) ) {
			kprintf( SHELL_CREATMSG);
			continue;
		}

		/* Set stdinput and stdoutput in child to redirect I/O */

		proctab[child].prdesc[0] = stdinput;
		proctab[child].prdesc[1] = stdoutput;

		msg = recvclr();
		resume(child);
		
		if (! backgnd) {
			msg = receive();
			while (msg != child) {
				msg = receive();
			}
		}
    }

    /* Terminate the shell process by returning from the top level */

    kprintf(SHELL_EXITMSG);
    return OK;
}
