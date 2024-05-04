

#include <threads.h>
#include <timers.h>
#include <tools.h>
#include <debug.h>


struct data
{
    const char *name;
    u32_t count;
    u32_t interval;
};

static struct thread thread_1;
static u8_t stack_1[256];
static struct data data_1 = {"thread 1", 20, 200};

static struct thread thread_2;
static u8_t stack_2[256];
static struct data data_2 = {"thread 2", 10, 300};

static void blink(struct data *data)
{
    u32_t count = data->count;
    struct timer timer;

    debug("%s: hello\n", data->name);
    start_timer(&timer, data->interval);
    while (count--)
    {
        wait_timer(&timer);
        debug("%s: count {%d}\n", data->name, count);
    }

    stop_timer(&timer);
    debug("%s: bue\n", data->name);
}

void main()
{
     start_thread(&thread_1, (function_t)blink, &data_1, stack_1, sizeof(stack_1));
    start_thread(&thread_2, (function_t)blink, &data_2, stack_2, sizeof(stack_2));

    while (1);
}