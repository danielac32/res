


#ifndef KMALLOC_H
#define KMALLOC_H

extern  char _kernel_end;  // linker-supplied address -- end of static kernel structures in RAM
extern  char _stack_limit;

#define KMALLOC_START  (void*)&_kernel_end
#define KMALLOC_LENGTH (size_t)(&_stack_limit) - (size_t)(&_kernel_end)

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


#endif