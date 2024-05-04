
asm("j main");
 
#include "../include/serial.h"
#include <string.h>
 




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

static char token[80];
static char token_type=0, tok=0;

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
    //if(*prog=='\r') serror(1);
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

  return token_type;
}

/***********************************/
char counter=100;
char store=99;
/***********************************/
static char str4[]={"hola como estas"};//no got
static char ptr2[]={"1234567890"};     //no got
/***********************************/
char *str="veronica";
char *str3="daniel";
char *ptr="global offset table";
/***********************************/
static char *pointer;
static char *pointer2;
static char *p;
/***********************************/
char str2[]={"kjkjglkj"}; //got
/***********************************/
const static char cadena2[]={"ormary"};
const static char buffer[]={'b','e','b','e',' ',' ',' ','c','a','f','e'};//no got
static char cadena[]={"valentina"};
static int cache[100];
char array[]={"abcdefg"};
/***********************************/
static struct label {
  char c;
  int i; 
  int *p;
}label_table[10];
/***********************************/

void thread(char *p1){
  char *p2=NULL;
  char *p3=NULL;
  p2=p1;
  p3=p1;
  myprintf("%s,%s,%s\n",p1,p2,p3);
}



void encrypt(char password[],int key)
{
    unsigned int i;
    for(i=0;i<strlen(password);++i)
    {
        password[i] = password[i] - key;
    }
}

void decrypt(char password[],int key)
{
    unsigned int i;
    for(i=0;i<strlen(password);++i)
    {
        password[i] = password[i] + key;
    }
}

char *find(){
	return "daniel";
}

 
void main(){

	pointer=NULL;
	pointer2=NULL;

	label_table[0].c=240;
	cache[90]=88;

    p=str2;
    myprintf("%i,%s\n",label_table[0].c,p);
    myprintf("%i,%i\n",counter,store);

    pointer=str4;
    pointer2=ptr2;
    myprintf("%s,%s,%s\n",pointer,pointer2,str2);

    for (int i = 0; i < 10; ++i)
    {
    	label_table[i].c=i;
    	label_table[i].i=(i * 255) % 0xffff;
    	label_table[i].p=NULL;
    }
    for (int i = 0; i < 10; ++i)
    {
    	myprintf("%i,%i,%i\n",label_table[i].c,label_table[i].i,label_table[i].p);
    }
    myprintf("%s,%s,%s\n",str,str3,ptr);

    myprintf("%s\n",array);
    thread(array);

    char password[20] ;

    myprintf("Enter the password: \n ");
    strcpy(password,"daniel quintero");
    myprintf("Passwrod     = %s\n",password);

    encrypt(password,0xbebecafe);
    encrypt(password,0xcafebebe);
    encrypt(password,0xffaaafff);
    encrypt(password,0x12345678);
    myprintf("Encrypted value = %s\n",password);

    decrypt(password,0xbebecafe);
    decrypt(password,0xcafebebe);
    decrypt(password,0xffaaafff);
    decrypt(password,0x12345678);

    myprintf("Decrypted value = %s\n",password);
    char *r=find();
    myprintf("%s\n",r);

   strcpy(p_buf,"");
   strcat(p_buf,"10 print a;");
   strcat(p_buf,"15 println;");
   strcat(p_buf,"20 a=a+1;");
   strcat(p_buf,"30 goto 10;");
   encrypt(p_buf,0xbebecafe);
   myprintf("Encrypted value = %s\n",p_buf);
   decrypt(p_buf,0xbebecafe);
   myprintf("Decrypted value = %s\n",p_buf);

   prog = p_buf;
   

   for (int i = 0; *prog; ++i){
        get_token();
        myprintf("%s\n",token);
   }


}