


#include <stdint.h>
#include <string.h>

#include "../include/reg.h"
#include "../include/serial.h"
//#include "../include/kmalloc.h"
#include "../include/diskio.h"
#include "../include/ff.h"
//#include "../include/timer.h"
#include "cocoos.h"

/***********************************************************/
char mem[30000];
#define KMALLOC_START  (void*)mem
#define KMALLOC_LENGTH sizeof(mem)
void *kmalloc(int length);
void kfree(void *ptr);

void kmalloc_init(void *start, int length);
void kmalloc_debug();
int kmalloc_test();


#define KUNIT sizeof(struct kmalloc_chunk)

#define KMALLOC_STATE_FREE 0xa1a1a1a1
#define KMALLOC_STATE_USED 0xbfbfbfbf

struct kmalloc_chunk {
	int state;
	int length;
	struct kmalloc_chunk *next;
	struct kmalloc_chunk *prev;
};

static struct kmalloc_chunk *head = 0;

/*
Initialize the linked list by creating a single chunk at
a given start address and length.  The chunk is initially
free and has no next or previous chunks.
*/

void kmalloc_init(void *start, int length)
{
	head = (struct kmalloc_chunk *) start;
	head->state = KMALLOC_STATE_FREE;
	head->length = length;
	head->next = 0;
	head->prev = 0;
}

/*
Split a large chunk into two, such that the current chunk
has the desired length, and the next chunk has the remainder.
*/

static void ksplit(struct kmalloc_chunk *c, int length)
{
	struct kmalloc_chunk *n = (struct kmalloc_chunk *) ((char *) c + length);

	n->state = KMALLOC_STATE_FREE;
	n->length = c->length - length;
	n->prev = c;
	n->next = c->next;

	if(c->next)
		c->next->prev = n;

	c->next = n;
	c->length = length;
}

/*
Allocate a chunk of memory of the given length.
To avoid fragmentation, round up the length to
a multiple of the chunk size.  Then, search fo
a chunk of the desired size, and split it if necessary.
*/

void *kmalloc(int length)
{
	// round up length to a multiple of KUNIT
	int extra = length % KUNIT;
	if(extra)
		length += (KUNIT - extra);

	// then add one more unit to accommodate the chunk header
	length += KUNIT;

	struct kmalloc_chunk *c = head;

	while(1) {
		if(!c) {
			myprintf("kmalloc: out of memory!\n");
			return NULL;
		}
		if(c->state == KMALLOC_STATE_FREE && c->length >= length)
			break;
		c = c->next;
	}

	// split the chunk if the remainder is greater than two units
	if(length - c->length > 2 * KUNIT) {
		ksplit(c, length);
	}

	c->state = KMALLOC_STATE_USED;

	// return a pointer to the memory following the chunk header
	return (c + 1);
}

/*
Attempt to merge a chunk with its successor,
if it exists and both are in the free state.
*/

static void kmerge(struct kmalloc_chunk *c)
{
	if(!c)
		return;

	if(c->state != KMALLOC_STATE_FREE)
		return;

	if(c->next && c->next->state == KMALLOC_STATE_FREE) {
		c->length += c->next->length;
		if(c->next->next) {
			c->next->next->prev = c;
		}
		c->next = c->next->next;
	}

}

/*
Free memory by marking the chunk as de-allocated,
then attempting to merge it with the predecessor and successor.
*/

void kfree(void *ptr)
{
	struct kmalloc_chunk *c = (struct kmalloc_chunk *) ptr;
	c--;

	if(c->state != KMALLOC_STATE_USED) {
		myprintf("invalid kfree(%x)\n", ptr);
		return;
	}

	c->state = KMALLOC_STATE_FREE;

	kmerge(c);
	kmerge(c->prev);
}

void kmalloc_debug()
{
	struct kmalloc_chunk *c;

	myprintf("state ptr      prev     next     length  ptr(%8x)\n",KMALLOC_START);

	for(c = head; c; c = c->next) {
		if(c->state == KMALLOC_STATE_FREE) {
			myprintf("Free");
		} else if(c->state == KMALLOC_STATE_USED) {
			myprintf("Used");
		} else {
			myprintf("kmalloc list corrupted at %x!\n", c);
			return;
		}
		myprintf("  %x       %x       %x       %l\n", c, c->prev, c->next, c->length);
	}
}

// Testing





/***********************************************************/

extern uint32_t stack_top;
static void TASK1(void);
static void TASK2(void);
static void TASK3(void);
long time;
void int_isr(){
    asm("nop");
    asm("nop");
     //if(US - time >=20){
       // time=US;
        os_tick();
     //}
}


void __attribute__ ((section(".interrupt"))) isr(){    
      //save_context;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    int_isr();
      //restore_context;
    asm("uret");  
}


 
static void TASK1(void) {
  task_open();
   static int i=0;
   static int j=0;
    for(;;) {
    	
        for ( i = 0; i < 100; ++i)
        {
        	 myprintf("task1 : %i\n",i);
        	 if(!j){
        	 	j=1;
        	 	myprintf("\nstart task2\n");
        	 	uint8_t id = task_create( TASK2, 2, NULL, 0, 0 );
        	 }
        	 
             task_wait( 1 );
       
        }

    }
  task_close();
}

static void TASK2(void) {
  task_open();
  static int i=0;
  static int j=0;
  

    for(;;) {
       for ( i = 0; i < 100; i+=3)
        { 
        	 myprintf("                task2 : %i\n",i);
        	 if(!j){
        	 	j=1;
        	 	myprintf("\nstart task3\n");
        	 	uint8_t id = task_create( TASK3, 3, NULL, 0, 0 );
        	 }
             task_wait( 2 );
        }
    }
  task_close();
}

static void TASK3(void) {
  task_open();
  static int i=0;
    for(;;) {
       for ( i = 0; i < 100; i+=10)
        {
        	 myprintf("                             task3 : %i\n",i);
             task_wait( 2 );
        }
    }
  task_close();
}

static void TASK4(void) {
  task_open();
  static int i=0;
  //static char j;
    for(;;) {
        if(ENRX){
           char j=RX;
           myprintf("%c",j);
        }
        task_wait(1);
         //myprintf("                                                task4:%i\n",j);
         //task_wait(100);
    }
  task_close();
}

static uint8_t taskId;
 

int main(){
   MODE=0;
   ENT0=0;
   myprintf("Iniciando kernel\n");
   myprintf("%l\n",&stack_top);
   	kmalloc_init((void *) KMALLOC_START, KMALLOC_LENGTH);


   	char *p1,*p2,*p3,*p4;
    
    p1=(char*)kmalloc(3000);
    p2=(char*)kmalloc(20000);
    p3=(char*)kmalloc(1000);
    p4=(char*)kmalloc(3000);
    
    myprintf("free\n");
    kmalloc_debug();
    kfree(p1);
    kmalloc_debug();
    kfree(p2);
    kmalloc_debug();
    kfree(p3);
    kmalloc_debug();
    kfree(p4);
    kmalloc_debug();


    //while(1);

   os_init();
   task_create( TASK1, 1, NULL, 0, 0 );
   //task_create( TASK2, 2, NULL, 0, 0 );
  // task_create( TASK3, 3, NULL, 0, 0 );
   task_create( TASK4, 4, NULL, 0, 0 );
   os_start();

   while(1);
   return 0;
}