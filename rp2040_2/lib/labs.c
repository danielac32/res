#include <lib.h>


long	labs(
	  long		arg
	)
{
    if (arg < 0)
        arg = -arg;
    return (arg);
}
