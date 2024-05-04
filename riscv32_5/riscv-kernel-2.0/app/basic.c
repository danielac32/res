asm("j main");

#include <stdint.h>
#include <string.h>
//#include <ctype.h>
#include <stdlib.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/syscall.h"
#include "../include/ff.h"

//#include "../include/basic.h"

#define _U      01
#define _L      02
#define _N      04
#define _S      010
#define _P      020
#define _C      040
#define _X      0100
#define _B      0200

const static char _ctype_[1 + 256] = {
  0,
  _C, _C, _C, _C, _C, _C, _C, _C,
  _C, _C|_S,  _C|_S,  _C|_S,  _C|_S,  _C|_S,  _C, _C,
  _C, _C, _C, _C, _C, _C, _C, _C,
  _C, _C, _C, _C, _C, _C, _C, _C,
  _S|_B,  _P, _P, _P, _P, _P, _P, _P,
  _P, _P, _P, _P, _P, _P, _P, _P,
  _N, _N, _N, _N, _N, _N, _N, _N,
  _N, _N, _P, _P, _P, _P, _P, _P,
  _P, _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U,
  _U, _U, _U, _U, _U, _U, _U, _U,
  _U, _U, _U, _U, _U, _U, _U, _U,
  _U, _U, _U, _P, _P, _P, _P, _P,
  _P, _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L,
  _L, _L, _L, _L, _L, _L, _L, _L,
  _L, _L, _L, _L, _L, _L, _L, _L,
  _L, _L, _L, _P, _P, _P, _P, _C
};


#define isalpha(c)      ((_ctype_+1)[(int)(c)]&(_U|_L))
#define isupper(c)      ((_ctype_+1)[(int)(c)]&_U)
#define islower(c)      ((_ctype_+1)[(int)(c)]&_L)
#define isdigit(c)      ((_ctype_+1)[(int)(c)]&_N)
#define isxdigit(c)     ((_ctype_+1)[(int)(c)]&(_N|_X))
#define isspace(c)      ((_ctype_+1)[(int)(c)]&_S)
#define ispunct(c)      ((_ctype_+1)[(int)(c)]&_P)
#define isalnum(c)      ((_ctype_+1)[(int)(c)]&(_U|_L|_N))
#define isprint(c)      ((_ctype_+1)[(int)(c)]&(_P|_U|_L|_N|_B))
#define isgraph(c)      ((_ctype_+1)[(int)(c)]&(_P|_U|_L|_N))
#define iscntrl(c)      ((_ctype_+1)[(int)(c)]&_C)
#define isascii(c)      ((unsigned)(c)<=0177)
#define toupper(c)      ((c)-'a'+'A')
#define tolower(c)      ((c)-'A'+'a')
#define toascii(c)      ((c)&0177)

#define NUM_LAB 100
#define LAB_LEN 10
#define FOR_NEST 25
#define SUB_NEST 25
#define PROG_SIZE 2048

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
#define TOGGLE 22
#define ELSE 23
static char *prog;                  
static char p_buf[PROG_SIZE]; 
static long variables[26]={0};

static struct commands { /* keyword lookup table */
  char command[20];
  char tok;
} table[] = { /* Commands must be entered lowercase */
  "print", PRINT, /* in this table. */
  "input", INPUT,
  "if", IF,
  "else",ELSE,
  "then", THEN,
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
  "println",PRINTLN,
};

static char token[80];
static char token_type=0, tok=0;

static struct label {
  char name[LAB_LEN];
  char *p;  /* points to place to go in source file*/
}label_table[NUM_LAB];
//struct label label_table[NUM_LAB];

static struct for_stack {
  int var; /* counter variable */
  int target;  /* target value */
  char *loc;
} fstack[FOR_NEST]; /* stack for FOR/NEXT loop */

struct for_stack fpop();
static char *gstack[SUB_NEST]; /* stack for gosub */

static int ftos=0;  /* index to top of FOR stack */
static int gtos=0;  /* index to top of GOSUB stack */
static int state=0;

char *find_label(char *s), *gpop();
void assignment();
void _print(), scan_labels(), find_eol(), exec_goto();
void exec_if(), exec_for(),exec_else(), next(), fpush(struct for_stack i), input();
void gosub(), greturn(), gpush(char *s), label_init();
void serror(int), get_exp(int *result), putback();
void level2(int *result), level3(int *result), level4(int *result), level5(int *result), level6(int *result), primitive(int *result);
void unary(char o, int *r), arith(char o, int *r, int *h);
int load_program(char *p, char *fname), look_up(char *s);
int load_program2(char *p, char *fname), look_up(char *s);
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

  
int main(){
  
  static int counter=0;
  void *context;
  static int pid; 
  pid=get_id();
  static char *arg;
  arg=get_args(pid);
  
  strcpy(p_buf,"");

  strcat(p_buf,"5 for a=0 to 100;");
  strcat(p_buf,"10 print a;");
  strcat(p_buf,"15 println;");
  strcat(p_buf,"20 next;");
  strcat(p_buf,"30 end;");
  //strcat(p_buf,"20 a=a+1;");
  //strcat(p_buf,"40 goto 10;");


  /*while(1){
      myprintf("%i,%s\n",pid,arg);
      myprintf("%s\n",p_buf);
      context=get_context();
      k_yield(context);
  }*/

  
  /*myprintf("load basic...\n");
  if(!load_program(p_buf,arg)){
     myprintf("fatal error\n");
     while(1);
  }
 */
  myprintf("running...\n");

  prog = p_buf;
  scan_labels(); 
  ftos = 0; 
  gtos = 0;
  myprintf("%s\n",prog); 



      do {
              token_type = get_token();
              //printf("%d\n\r",token_type);
              if(token_type==VARIABLE) {
                 putback(); 
                 assignment();
              }else{
                  switch(tok) {
                        case PRINT:
                          _print();
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
                        case INPUT:
                          input();//printf("input\n\r");
                        break;
                        case GOSUB:
                          gosub(); 
                        break;
                        case RETURN:
                          greturn();
                        break;
                        case DELAY_MS:
                          delay_ms();//printf("delay_ms\n\r"); 
                        break;
                        case DELAY_US:
                          delay_us();//printf("delay_us\n\r");
                        break;
                        case PINMODE:
                          pinmode();//printf("pinmode\n\r"); 
                        break;   
                        case DWRITE:
                          dwrite();//printf("dwrite\n\r");
                        break;
                        case DREAD:
                          dread();//printf("dread\n\r");
                        break;
                        case AREAD:
                          aread();//printf("aread\n\r");
                        break;
                        case AWRITE:
                          awrite();///printf("awrite\n\r");
                        break;
                        case PRINTLN:
                          println();
                        break;
                        case TOGGLE:
                          myprintf("toggle\n\r");
                        break;
                        case END:
                          tok=FINISHED;
                        break;
                  }
                  //context=get_context();
                  //k_yield(context);

                  //if(variables[0]==100){
                   //  tok=FINISHED;
                  //}
              }
              //context=get_context();
              //k_yield(context);
         } while (tok != FINISHED);
         
         myprintf("\nexit(basic RUNNING)\n");
         /*while(1){
            myprintf("%i,%s\n",pid,arg);
            myprintf("%s\n",p_buf);

            context=get_context();
            k_yield(context);
         }*/
        return 0;
}



 
void scan_labels(){
  static int addr=0;
  static char *temp;

  label_init();  /* zero all labels */
  temp = prog;   /* save pointer to top of program */
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
      label_table[addr].p = prog;  /* current point in program */
    }
    /* if not on a blank line, find next line */
    if(tok!=EOL) find_eol();
  } while(tok!=FINISHED);
  prog = temp;  /* restore to original */
}

/* Find the start of the next line. */
void find_eol()
{
  while(*prog!='\n'  && *prog!='\0') ++prog;
  if(*prog) prog++;
}

/* Return index of next free position in label array.
   A -1 is returned if the array is full.
   A -2 is returned when duplicate label is found.
*/
int get_next_label(char *s)
{
   int t;

  for(t=0;t<NUM_LAB;++t) {
    if(label_table[t].name[0]==0) return t;
    if(!strcmp(label_table[t].name,s)) return -2; /* dup */
  }

  return -1;
}

char *find_label(char *s)
{
   int t;

  for(t=0; t<NUM_LAB; ++t)
    if(!strcmp(label_table[t].name,s)) return label_table[t].p;
  return '\0'; /* error condition */
}


void label_init()
{
   int t;

  for(t=0; t<NUM_LAB; ++t) label_table[t].name[0]='\0';
}


int get_token()
{

   char *temp;

  token_type=0; tok=0;
  temp=token;

  if(*prog=='\0') { /* end of file */
    *token=0;
    tok = FINISHED;
    return(token_type=DELIMITER);
  }

  while(iswhite(*prog)) ++prog;  /* skip over white space */

  if(*prog=='\r') { /* crlf */
    ++prog; ++prog;
    tok = EOL; *token='\r';
    token[1]='\n'; token[2]=0;
    return (token_type = DELIMITER);
  }

  if(strchr("+-*^/%=;(),><", *prog)){ /* delimiter */
    *temp=*prog;
    prog++; /* advance to next position */
    temp++;
    *temp=0;
    return (token_type=DELIMITER);
  }

  if(*prog=='"') { /* quoted string */
    prog++;
    while(*prog!='"'&& *prog!='\r') *temp++=*prog++;
    if(*prog=='\r') serror(1);
    prog++;*temp=0;
    return(token_type=QUOTE);
  }

  if(isdigit(*prog)) { // number 
    while(!isdelim(*prog)) *temp++=*prog++;
    *temp = '\0';
    return(token_type = NUMBER);
  }

  if(isalpha(*prog)) { //var or command 
    while(!isdelim(*prog)) *temp++=*prog++;
    token_type=STRING;
  }

  *temp = '\0';

  /* see if a string is a command or a variable */
  if(token_type==STRING) {
    tok=look_up(token); /* convert to internal rep */
    if(!tok) token_type = VARIABLE;
    else token_type = COMMAND; /* is a command */
  }
   
  return token_type;
}


 
/* Return true if c is a delimiter. */
int isdelim(char c)
{
  if(strchr(" ;,+-<>/*%^=()", c) || c==9 || c=='\r' || c==0)
    return 1;
  return 0;
}

/* Return 1 if c is space or tab. */
int iswhite(char c)
{
  if(c==' ' || c=='\t') return 1;
  else return 0;
}
 


void serror(int error){
struct error_list
{
  char cmd[30];
}table[] = {
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
  myprintf("%s\n", table[error].cmd);
  //longjmp(e_buf, 1); /* return to save point */
  tok=END;
}

/* Return a token to input stream. */
void putback()
{

  char *t;

  t = token;
  for(; *t; t++) prog--;
}

/* Look up a a token's internal representation in the
   token table.
*/
int look_up(char *s)
{
   int i;
  char *p;

  /* convert to lowercase */
 // p = s;
 // while(*p){ *p = tolower(*p); p++; }
  
  /*for(i=0;i<sizeof(tokk);i++){
      tok=tokk[i];
      if(!strcmp(code[i],s))return tok;
  }*/

 // return 0;
  /* see if token is in table */
  for(i=0; *table[i].command; i++)
      if(!strcmp(table[i].command, s)) return table[i].tok;
  return 0; /* unknown command */
}


void get_exp(int *result)
{
  get_token();
  if(!*token) {
    serror(2);
    return;
  }
  level2(result);
  putback(); /* return last token read to input stream */
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

/* Multiply or divide two factors. */
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

/* Process integer exponent. */
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

/* Is a unary + or -. */
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

/* Process parenthesized expression. */
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

/* Find value of number or variable. */
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

/* Perform the specified arithmetic. */
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

/* Reverse the sign. */
void unary(char o, int *r)
{
  if(o=='-') *r = -(*r);
}

/* Find the value of a variable. */
int find_var(char *s)
{
  if(!isalpha(*s)){
    serror(4); /* not a variable */
    return 0;
  }
  return variables[toupper(*token)-'A'];
} 



/**************************************************/
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



void input()
{
  char var;
  int i;

  get_token(); 
  /*if(token_type==QUOTE) {
    myprintf("%s",token);  
    get_token();
    if(*token!=',') serror(1);
    get_token();
  }
  else */myprintf("? ");  
  var = toupper(*token)-'A'; 
  char buff[10];
  int counter=0;
  while(1){
      if(ENRX){
         char c=RX;
         buff[counter]=c;
         if(c=='\n'){
            break;
         }
         counter++;
      }
  }
  
  i=atoi(buff);
  //scanf("%d", &i); 

  variables[var] = i;  
}
void delay_ms(){
 int value;
 get_exp(&value);
 get_token();
 if(*token!=';') {
    serror(0);
 }else{
    myprintf("delay_ms(%d)\n\r",value);
 }
}

void delay_us(){
 int value;
 get_exp(&value);
 get_token();
 if(*token!=';') {
    serror(0);
 }else{
    myprintf("delay_us(%d)\n\r",value);
 }
}

void pinmode(){
int pin=0;
int value=0;
int error_=0;
get_exp(&pin);
//printf("%d\n\r",value);
get_token();
if(*token!=',')error_=1;
//printf("%s\n\r",token);
get_exp(&value);
//printf("%d\n\r",value);
get_token();
if(*token!=';')error_=1;
//printf("%s\n\r",token);
if(error_){
  serror(0);
}else{
    myprintf("pinMode(%d,%d)\n\r",pin,value);
}
}


void dwrite(){
int pin=0;
int value=0;
int error_=0;
get_exp(&pin);
//printf("%d\n\r",value);
get_token();
if(*token!=',')error_=1;
//printf("%s\n\r",token);
get_exp(&value);
//printf("%d\n\r",value);
get_token();
if(*token!=';')error_=1;
//printf("%s\n\r",token);
if(error_){
  serror(0);
}else{
    myprintf("dwrite(%d,%d)\n\r",pin,value);
}
}


void awrite(){
int pin=0;
int value=0;
int error_=0;
get_exp(&pin);
//printf("%d\n\r",value);
get_token();
if(*token!=',')error_=1;
//printf("%s\n\r",token);
get_exp(&value);
//printf("%d\n\r",value);
get_token();
if(*token!=';')error_=1;
//printf("%s\n\r",token);
if(error_){
  serror(0);
}else{
    myprintf("awrite(%d,%d)\n\r",pin,value);
}
}

void dread(){
char var;
int i;
int pin; 
int error_=0;
get_exp(&pin);
//printf("%d\n\r",pin);
get_token(); 
if(*token!=',')error_=1;
//printf("%s\n\r",token);
get_token(); 
var = toupper(*token)-'A';
get_token(); 
if(*token!=';')error_=1;
if(error_){
   serror(0);
}else{
   i=2000;
   variables[var] = i; 
}
}

void aread(){
char var;
int i;
int pin; 
int error_=0;
get_exp(&pin);
//printf("%d\n\r",pin);
get_token(); 
if(*token!=',')error_=1;
//printf("%s\n\r",token);
get_token(); 
var = toupper(*token)-'A';
get_token(); 
if(*token!=';')error_=1;
if(error_){
   serror(0);
}else{
   i=1024;
   variables[var] = i; 
}
}
void println(){
get_token(); 
if(*token!=';'){
   serror(0);
}else{
   myprintf("\n\r");  
}
}
void _print(){
int value=0;
int i=0;
int error_=0;
get_token();
if(token_type==QUOTE) { 
   myprintf("%s",token);
   get_token();
   //printf("%s\n\r",token);
   if(*token==','){
      get_exp(&value);
      myprintf("%d",value);
      get_token();
      if(*token!=';')error_=1;
   }
}else{
   putback();
   get_exp(&value);
   get_token();
   if(*token!=';')error_=1;
   else myprintf("%d",value);
}
if(error_)serror(0);
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
  }else{
    find_eol();
  } 
}

void exec_for()
{
  struct for_stack i;
  int value;

  get_token(); /* read the control variable */
  if(!isalpha(*token)) {
    serror(4);
    return;
  }

  i.var=toupper(*token)-'A'; /* save its index */

  get_token(); /* read the equals sign */
  if(*token!='=') {
    serror(3);
    return;
  }

  get_exp(&value); /* get initial value */

  variables[i.var]=value;

  get_token();
  if(tok!=TO) serror(9); /* read and discard the TO */

  get_exp(&i.target); /* get target value */

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

void fpush(struct for_stack i)

{
   if(ftos>FOR_NEST)
    serror(10);

  fstack[ftos]=i;
  ftos++;
}

 struct for_stack fpop()
{
  ftos--;
  if(ftos<0) serror(11);
  return(fstack[ftos]);
}

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
    serror(12);
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





int load_program(char *p, char *fname){
  FIL *file;  
  UINT bw;
  FRESULT res; 
  uint32_t size=0;
  file=(FIL*)k_malloc(sizeof(FIL));
  res=exist(fname);
  if (res == FR_OK)
  {
     openFile(fname,file,FA_OPEN_EXISTING | FA_READ);
     size=file->fsize;
    // char *buff=(char *)k_malloc(size*sizeof(char));
     readbuff(file,p,size);
     close(file);
    // k_free(buff);
  }else{
     myprintf("file not found\n");
  }
  k_free(file);
}

 
