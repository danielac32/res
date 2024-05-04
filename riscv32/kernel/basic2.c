#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#include "lib/timer.h"
#include "lib/lib.h"
#include "lib/serial.h"
#include "lib/syscall.h"
//#include "driver/terminal.h"
#include "lib/task.h"
/*#include "driver/cmd.h"*/


#define NUM_LAB 25
#define LAB_LEN 10
#define FOR_NEST 10
#define SUB_NEST 10
#define PROG_SIZE 1024

#define DELIMITER  1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING    5
#define QUOTE   6

#define PRINT 1
#define INPUT 2
#define IF    3
#define THEN  4
#define FOR   5
#define NEXT  6
#define TO    7
#define GOTO  8
#define EOL   9
#define FINISHED  10
#define GOSUB 11
#define RETURN 12
#define END 13
#define DELAY_MS 14
#define PINMODE 15
#define DWRITE 16
#define DREAD 17
#define AREAD 18
#define AWRITE 19
#define DELAY_US 20
#define PRINTLN 21
#define ELSE 22
#define TOGGLE 29

char *prog;
char token[80];
char p_buf[PROG_SIZE];


long variables[26] ={0};
 

struct commands { /* keyword lookup table */
  char command[30];
  char tok;
} table[] =  { /* Commands must be entered lowercase */
  "print", PRINT, /* in this table. */
  "println",PRINTLN,
  "input", INPUT,
  "if", IF,
  "then", THEN,
  "else", ELSE,
  "goto", GOTO,
  "for", FOR,
  "next", NEXT,
  "to", TO,
  "gosub", GOSUB,
  "return", RETURN,
  "end", END,
  "delay_ms",DELAY_MS,
  "delay_us",DELAY_US,
  "pinmode",PINMODE,
  "dwrite",DWRITE,
  "dread",DREAD,
  "aread",AREAD,
  "awrite",AWRITE,
  "", END 
};

char token_type, tok ;

struct label {
  char name[LAB_LEN];
  char *p;  /* points to place to go in source file*/
};
struct label label_table[NUM_LAB];


int state =0;

struct for_stack {
  int var; /* counter variable */
  int target;  /* target value */
  char *loc;
} fstack[FOR_NEST]; /* stack for FOR/NEXT loop */
struct for_stack fpop();

char *gstack[SUB_NEST]; /* stack for gosub */
int ftos =0;  /* index to top of FOR stack */
int gtos =0;  /* index to top of GOSUB stack */


/****************************************************************/
char *find_label(char *s), *gpop();
void assignment();
void __print(), scan_labels(), find_eol(), exec_goto();
void exec_if(), exec_else(),exec_for(), next(), fpush(struct for_stack i), input();
void gosub(), greturn(), gpush(char *s), label_init();
void serror(int), get_exp(int *result), putback();
void level2(int *result), level3(int *result), level4(int *result), level5(int *result), level6(int *result), primitive(int *result);
void unary(char o, int *r), arith(char o, int *r, int *h);
int load_program(char *p, char *fname), look_up(char *s);
int get_next_label(char *s), iswhite(char c), isdelim(char c);
int find_var(char *s), get_token();

void delay_ms();
void delay_us();
void pinmode();
void dwrite();
void dread();
void aread();
void awrite();
void println();
void toggle();


void serror(int error){
 char *e[]= {
    "syntax error",
    "unbalanced parentheses",
    "no expression present",
    "equals sign expected",
    "not a variable",
    "Label table full",
    "duplicate label",
    "undefined label",
    "THEN expected",
    "TO expected",
    "too many nested FOR loops",
    "NEXT without FOR",
    "too many nested GOSUBs",
    "RETURN without GOSUB"
  };
  myprintf("%s\n",e[error]);
}



unsigned long si;
char ch;
unsigned long l=0;
int i;

void main(){
 mode=0;
 enable_timer=1;

 i=openfile((uint32_t)"basic.bas");
  if(!i){
     myprintf("ok\n");
     si=filesize();
     if(si<=999){
        myprintf("size file: %i Bytes\n", si);
     }else if(si >=1000 && si<1000000){
        myprintf("size file: %i kb\n", si/1000);
     }else if(si>=1000000){
        myprintf("size file: %l mb\n", si/1000000);
    }
     myprintf("\n*******************************************************\n");
     l=0;
     while(l<si){
           ch=readfile();
           p_buf[l]=ch;
           l++;
     }
     l=0;
     myprintf("\n*******************************************************\n");
     closefile();
  }else{
    myprintf("fail file\n");
  }

 prog = p_buf;
 scan_labels(); 
 ftos = 0; 
 gtos = 0;
 state=0;
 myprintf("%s\n",prog); 

do {
      token_type = get_token();
      if(token_type==VARIABLE) {
         putback(); 
         assignment(); 
      }else{
           myprintf("%s\n",token);
           switch(tok) {
      	  	case PRINT:
                  __print();
                break;
                case PRINTLN:
                 println();
                break;
                case GOTO:
                    exec_goto();
                break;
                case IF:
                   exec_if();
                break;
                case ELSE:
                   exec_else();
                break;
                case FOR:
                   exec_for();
                break;
                case NEXT:
                   next();
                break;
                case GOSUB:
                   gosub();
                break;
                case RETURN:
                   greturn();
                break;
                case DELAY_MS:
                   //delay_ms();
                break;
                case DELAY_US:
                   //delay_us();
                break;
                case PINMODE:
                   //pinmode();
                break;   
                case DWRITE:
                   //dwrite();
                break;
                case DREAD:
                   //dread();
                break;
                case AREAD:
                   //aread();
                break;
                case AWRITE:
                   //awrite();
                break;
                case END:
                 myprintf("end program\n");
                break;
          }
      }
}while(*prog!='\0');      
 
 while(1); 
}

void println(){
	myprintf("\n");
}
void __print(){
int value=0;
int i=0;
int error_=0;
int var=0;
get_token();
if(token_type==QUOTE) { 
   myprintf("%s",token);
   get_token();
   if(*token!=';')error_=1;
}else{
      putback();
      get_exp(&value);
      get_token();
      if(*token!=';')error_=1;
      else myprintf("%i",value);
}
if(error_)serror(0);
}

void assignment(){
  int var, value;
  get_token();
  if(!isalpha(*token)) {
    serror(4);
  }
  var = toupper(*token)-'A';
  get_token();
  if(*token!='=') {
    serror(3);
  }
  get_exp(&value);
  variables[var] = value;
}



void label_init(){
  int t;
  for(t=0; t<NUM_LAB; ++t) label_table[t].name[0]='\0';
}
void scan_labels()
{
  int addr;
  char *temp;

  label_init();   
  temp = prog;   

 
  get_token();
  if(token_type==NUMBER) {
    strcpy(label_table[0].name,token);
    label_table[0].p=prog;
  }

  find_eol();
  do {
    get_token();
    if(token_type==NUMBER) {
      addr = get_next_label(token);
      if(addr==-1 || addr==-2) {
          (addr==-1) ?serror(5):serror(6);
      }
      strcpy(label_table[addr].name, token);
      label_table[addr].p = prog;  
    }
 
    if(tok!=EOL) find_eol();
  } while(tok!=FINISHED);
  prog = temp;   
}

int get_next_label(char *s)
{
   int t;

  for(t=0;t<NUM_LAB;++t) {
    if(label_table[t].name[0]==0) return t;
    if(!strcmp(label_table[t].name,s)) return -2;  
  }

  return -1;
}

 
void find_eol()
{
  while(*prog!='\n'  && *prog!='\0') ++prog;
  if(*prog) prog++;
}


char *find_label(char *s)
{
   int t;

  for(t=0; t<NUM_LAB; ++t)
    if(!strcmp(label_table[t].name,s)) return label_table[t].p;
  return '\0';  
}


int get_token()
{

   char *temp;

  token_type=0; tok=0;
  temp=token;

  if(*prog=='\0') {  
    *token=0;
    tok = FINISHED;
    return(token_type=DELIMITER);
  }

  while(iswhite(*prog)) ++prog;   

  if(*prog=='\r') { 
    ++prog; ++prog;
    tok = EOL; *token='\r';
    token[1]='\n'; token[2]=0;
    return (token_type = DELIMITER);
  }

  if(strchr("+-*^/%=;(),><", *prog)){
    *temp=*prog;
    prog++;
    temp++;
    *temp=0;
    return (token_type=DELIMITER);
  }

  if(*prog=='"') {  
    prog++;
    while(*prog!='"'&& *prog!='\r') *temp++=*prog++;
    if(*prog=='\r') serror(1);
    prog++;*temp=0;
    return(token_type=QUOTE);
  }

  if(isdigit(*prog)) { 
    while(!isdelim(*prog)) *temp++=*prog++;
    *temp = '\0';
    return(token_type = NUMBER);
  }

  if(isalpha(*prog)) {  
    while(!isdelim(*prog)) *temp++=*prog++;
    token_type=STRING;
  }

  *temp = '\0';

   
  if(token_type==STRING) {
    tok=look_up(token); 
    if(!tok){
       token_type = VARIABLE;
    }else token_type = COMMAND;  
  }
  return token_type ;
}

 
void putback()
{

  char *t;

  t = token;
  for(; *t; t++) prog--;
}


int look_up(char *s)
{
   int i;
  char *p;


  p = s;
  while(*p){ *p = tolower(*p); p++; }

 
  for(i=0; *table[i].command; i++)
      if(!strcmp(table[i].command, s)) return table[i].tok;
  return 0; 
}
 
int isdelim(char c)
{
  if(strchr(" ;,+-<>/*%^=()", c) || c==9 || c=='\r' || c==0)
    return 1;
  return 0;
}

 
int iswhite(char c)
{
  if(c==' ' || c=='\t') return 1;
  else return 0;
}


void get_exp(int *result)
{
  get_token();
  if(!*token) {
    serror(2);
    return;
  }
  level2(result);
  putback();  
}


void level2(int *result)
{
  char  op;
  int hold;

  level3(result);
  while((op = *token) == '+' || op == '-') {
    get_token();
    level3(&hold);
    arith(op, result, &hold);
  }
}


void level3(int *result)
{
   char  op;
  int hold;

  level4(result);
  while((op = *token) == '*' || op == '/' || op == '%') {
    get_token();
    level4(&hold);
    arith(op, result, &hold);
  }
}

 
void level4(int *result)
{
  int hold;

  level5(result);
  if(*token== '^') {
    get_token();
    level4(&hold);
    arith('^', result, &hold);
  }
}

 
void level5(int *result)
{
   char  op;

  op = 0;
  if((token_type==DELIMITER) && *token=='+' || *token=='-') {
    op = *token;
    get_token();
  }
  level6(result);
  if(op)
    unary(op, result);
}

 
void level6(int *result)
{
  if((*token == '(') && (token_type == DELIMITER)) {
    get_token();
    level2(result);
    if(*token != ')')
      serror(1);
    get_token();
  }
  else
    primitive(result);
}

 
void primitive(int *result)
{

  switch(token_type) {
  case VARIABLE:
    *result = find_var(token);
    get_token();
    return;
  case NUMBER:
    *result = atoi(token);
    get_token();
    return;
  default:
    serror(0);
  }
}
 
void arith(char o, int *r, int *h)
{
   int t, ex;

  switch(o) {
    case '-':
      *r = *r-*h;
      break;
    case '+':
      *r = *r+*h;
      break;
    case '*':
      *r = *r * *h;
      break;
    case '/':
      *r = (*r)/(*h);
      break;
    case '%':
      t = (*r)/(*h);
      *r = *r-(t*(*h));
      break;
    case '^':
      ex = *r;
      if(*h==0) {
        *r = 1;
        break;
      }
      for(t=*h-1; t>0; --t) *r = (*r) * ex;
      break;
  }
}

 
void unary(char o, int *r)

{
  if(o=='-') *r = -(*r);
}


int find_var(char *s)
{
  if(!isalpha(*s)){
    serror(4);
    return 0;
  }
  return variables[toupper(*token)-'A'];
}


void exec_goto()
{
  char *loc;

  get_token(); /* get label to go to */
  /* find the location of the label */
  loc = find_label(token);
  if(*loc=='\0')
    serror(7); /* label not defined */

  else prog=loc;  /* start program running at that loc */
}

void exec_if()
{
  int x , y, cond;
  char op;
  get_exp(&x); 
  //printf("%i\n",x);
  get_token(); 
  if(!strchr("=<>", *token)) {
    serror(0); 
    return;
  }
  op=*token;
  get_exp(&y); 
  cond = 0;
  switch(op) {
    case '<':
      if(x<y) cond=1;
      break;
    case '>':
      if(x>y) cond=1;
      break;
    case '=':
      if(x==y) cond=1;
      break;
  }
  if(cond) { 
    get_token();
    if(tok!=THEN) {
      serror(8);
      return;
    }
     state=0;
  }else{
     state=1;

     find_eol(); 
  }
}
void exec_else(){
  if(state){
     state=0;
  }else find_eol(); 
}

void exec_for()
{
  struct for_stack i;
  int value;
  int var=0;

  get_token(); /* read the control variable */
  if(!isalpha(*token)) {
    serror(4);
    return;
  }
  i.var=toupper(*token)-'A'; /* save its index */
  get_token(); /* read the equals sign */
  if(*token!='=') serror(3);
  get_exp(&value);//valor inicial
  variables[i.var]=value;
  get_token();
  if(tok!=TO) serror(9); /* read and discard the TO */

  get_token();
  if(isalpha(*token)){   //is char
     var = toupper(*token)-'A';
     i.target=variables[var];
  }else{
     putback();
     get_exp(&i.target);
  }
  /* if loop can execute at least once, push info on stack */
  if(value>=variables[i.var]) {
    i.loc = prog;
    fpush(i);
  }
  else  /* otherwise, skip loop code altogether */
    while(tok!=NEXT) get_token();
}


/* Execute a NEXT statement. */
void next()
{
  struct for_stack i;

  i = fpop(); /* read the loop info */

  variables[i.var]++; /* increment control variable */
  if(variables[i.var]>i.target) return;  /* all done */
  fpush(i);  /* otherwise, restore the info */
  prog = i.loc;  /* loop */
}

/* Push function for the FOR stack. */
void fpush(struct for_stack i)

{
   if(ftos>FOR_NEST){
      //serror(10);
      ftos=0;
    }

  fstack[ftos]=i;
  ftos++;
}

struct for_stack fpop()
{
  ftos--;
  if(ftos<0) serror(11);
  return(fstack[ftos]);
}


/* Execute a GOSUB command. */
void gosub()
{
  char *loc;

  get_token();
  /* find the label to call */
  loc = find_label(token);
  if(*loc=='\0')
    serror(7); /* label not defined */
  else {
    gpush(prog); /* save place to return to */
    prog = loc;  /* start program running at that loc */
  }
}

/* Return from GOSUB. */
void greturn()
{
   prog = gpop();
}

/* GOSUB stack push function. */
void gpush(char *s)

{
  gtos++;

  if(gtos==SUB_NEST) {
    //serror(12);
    gtos=0;
    return;
  }

  gstack[gtos]=s;

}

/* GOSUB stack pop function. */
char *gpop()
{
  if(gtos==0) {
    serror(13);
    return 0;
  }

  return(gstack[gtos--]);
}