#include <stdint.h>

#ifndef BASIC_H_
#define BASIC_H_

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
 char *prog;                  
 char p_buf[PROG_SIZE]; 
 long variables[26];

static char *code[]={
                        "print",
                        "println",
                        "input",
                        "if",
                        "then",
                        "goto",
                        "for",
                        "next",
                        "to",
                        "gosub",
                        "return",
                        "end",
                        "delay_ms",
                        "delay_us",
                        "pinmode",
                        "dwrite",
                        "dread",
                        "aread",
                        "awrite",
                        "toggle",
                        "else"
};

static char tokk[]={
          PRINT,
          PRINTLN,
          INPUT,
          IF,
          THEN,
          GOTO,
          FOR,
          NEXT,
          TO,
          GOSUB,
          RETURN,
          END,
          DELAY_MS,
          DELAY_US,
          PINMODE,
          DWRITE,
          DREAD,
          AREAD,
          AWRITE,
          TOGGLE,
          ELSE};


 char token[80];
 char token_type, tok;

 struct label {
  char name[LAB_LEN];
  char *p;  /* points to place to go in source file*/
}label_table[NUM_LAB];
//struct label label_table[NUM_LAB];

 struct for_stack {
  int var; /* counter variable */
  int target;  /* target value */
  char *loc;
} fstack[FOR_NEST]; /* stack for FOR/NEXT loop */

 struct for_stack fpop();
 char *gstack[SUB_NEST]; /* stack for gosub */
 int ftos;  /* index to top of FOR stack */
 int gtos;  /* index to top of GOSUB stack */
 int state;

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

#endif
