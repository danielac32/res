/* abs.c - abs */
#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  abs  -  Return the absolute value of an integer.
 *------------------------------------------------------------------------
 */
int	abs(
	  int		arg
	)
{
    if (arg < 0)
        arg = -arg;
    return (arg);
}
