#include <os.h>

struct  cmdent  {     /* Entry in command table */
  char  *cname;     /* Name of command    */
  bool cbuiltin;   /* Is this a builtin command? */
};

const struct  cmdent  cmdtab[] = {
  {"echo",  true},
  {"cat",   false},
  {"clear", true},
  {"date",  false},
  {"devdump", false},
  {"echo",  false},
  {"exit",  true},
  {"help",  false},
  {"kill",  true},
  {"memdump", false},
  {"memstat", true}, /* Make built-in */
  {"ps",    false},
  {"sleep", false},
  {"uptime",  false},
  {"?",   false},
  {"fopen",   false},
  {"fclose",   false},
  {"fcreat",   false},
  {"fread",   false},
  {"fwrite",   false},
  {"ls",   false},
  {"cd",   false},
  {"pwd",   false},
  {"cat",   false},
  {"rm",   false},
  {"mkdir",   false},
  {"touch",   false},
  {"mount",   false},
  {"writegpio",   false},
  {"readgpio",   false},
  
};

uint32_t  ncmd = sizeof(cmdtab) / sizeof(struct cmdent);

int terminal(int argc, char const *argv[])
{
    while(1){
         printf("%s",SHELL_PROMPT);
         while(!uart0_available());
          while(uart0_available()){
              len = readBytes(buf,sizeof(buf));
        }
        if (len == EOF) {
           break;
        }
        if (len <= 1) {
            continue;
        }

        buf[len] = SH_NEWLINE;  /* terminate line */

  

    ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);

    /* Handle parsing error */

    if (ntok == SYSERR) {
      printf("%s\n",SHELL_SYNERRMSG);
      continue;
    }

    /* If line is empty, go to next input line */

    if (ntok == 0) {
      printf( "\n");
      continue;
    }
    
    for (j = 0; j < ncmd; j++) {
      src = cmdtab[j].cname;
      cmp = tokbuf;
      diff = false;
      while (*src != NULLCH) {
        if (*cmp != *src) {
          diff = true;
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
        printf("command %s not found %d\n",tokbuf,len);
        continue;
    }else{
        printf("ok");
        printf(" %d ",j);
      for (int i=0; i<ntok; i++) {
          args[i] = &tokbuf[tok[i]];
          printf("%s",args[i]);
          printf(" ");
      }
      if(!strcmp(args[0],"kill")){
        systick_disable();
        PEND_SV();
      }else if(!strcmp(args[0],"exit")){
        //reset();
      }
      printf("\n");

        
    }

    }
	return 0;
}