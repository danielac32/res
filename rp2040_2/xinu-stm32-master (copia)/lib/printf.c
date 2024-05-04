/* printf.c - printf*/

#include <xinu.h>
#include <stdio.h>
#include <stdarg.h>

#define MAXSTR  80
#define NULL    0
#define PRECISION 6

//static void _prtl10(long num, char *str);
static void _prtl8(long num, char *str);
static void _prtX16(long num, char *str);
static void _prtl16(long num, char *str);
static void _prtl2(long num, char *str);

//extern void _fdoprnt(char *, va_list, int (*)(did32, char), int);

//extern	void	_doprnt(char *, va_list, int (*)(int));
/*------------------------------------------------------------------------
 *  printf  -  standard C printf function
 *------------------------------------------------------------------------
 */


static void dec2str(char *str, int n) {
	int i = 0;
	int w = 0;
	int t;
	int d = 1000000000;

	if (n < 0) {
		n = -n;
		str[i] = '-';
		++i;
	}

	while (d) {
		t = __aeabi_idiv(n, d);
		str[i] = t + '0';

		if (str[i] != '0')
			w = 1;

		if (w)
			++i;

		n = __aeabi_idivmod(n, d);
		d = __aeabi_idiv(d, 10);
	}

	if (i == 0) {
		str[0] = '0';
		str[1] = 0;
	} else {
		str[i] = 0;
	}
}

 

static void xputc(char c){
    
   uart_write(0,&c,1);
}


int	printf(
	  const char		*fmt,
	  ...
	)
{
    va_list ap;
     int c;
    int i;
    int f;                      /* The format character (comes after %) */
    char *str;                  /* Running pointer in string            */
    char string[20];            /* The string str points to this output */

    /*  from number conversion              */
    int length;                 /* Length of string "str"               */
    char fill;                  /* Fill character (' ' or '0')          */
    int leftjust;               /* 0 = right-justified, else left-just  */
    int fmax, fmin;             /* Field specifications % MIN . MAX s   */
    int leading;                /* No. of leading/trailing fill chars   */
    char sign;                  /* Set to '-' for negative decimals     */
    char digit1;                /* Offset to add to first numeric digit */
    long larg;
    double darg;



    va_start(ap, fmt);
    //_doprnt((char *)fmt, ap, (int (*)(int))xputc);

for (;;)
    {
        /* Echo characters until '%' or end of fmt string */
        while ((c = *fmt++) != '%')
        {
            if (c == '\0')
            {
                return;
            }
            xputc (c);
        }
        /* Echo "...%%..." as '%' */
        if (*fmt == '%')
        {
            xputc (*fmt++);
            continue;
        }
        /* Check for "%-..." == Left-justified output */
        if ((leftjust = ((*fmt == '-')) ? 1 : 0))
        {
            fmt++;
        }
        /* Allow for zero-filled numeric outputs ("%0...") */
        fill = (*fmt == '0') ? *fmt++ : ' ';
        /* Allow for minimum field width specifier for %d,u,x,o,c,s */
        /* Also allow %* for variable width (%0* as well)       */
        fmin = 0;
        if (*fmt == '*')
        {
            fmin = va_arg(ap, int);

            ++fmt;
        }
        else
        {
            while ('0' <= *fmt && *fmt <= '9')
            {
                fmin = fmin * 10 + *fmt++ - '0';
            }
        }
        /* Allow for maximum string width for %s */
        fmax = 0;
        if (*fmt == '.')
        {
            if (*(++fmt) == '*')
            {
                fmax = va_arg(ap, int);
                ++fmt;
            }
            else
            {
                while ('0' <= *fmt && *fmt <= '9')
                {
                    fmax = fmax * 10 + *fmt++ - '0';
                }
            }
        }

        str = string;
        if ((f = *fmt++) == '\0')
        {
            xputc ('%');
            return;
        }
        sign = '\0';            /* sign == '-' for negative decimal */

        switch (f)
        {
        case 'c':
            string[0] = va_arg(ap, int);
            string[1] = '\0';
            fmax = 0;
            fill = ' ';
            break;

        case 's':
            str = va_arg(ap, char *);

            if (NULL == str)
            {
                str = "(null)";
            }
            fill = ' ';
            break;

        case 'd':
            larg = va_arg(ap, long);

            if (larg < 0)
            {
                sign = '-';
            }
            //_prtl10(larg, str);
            dec2str(str,larg);
            break;
            
        case 'f':
           // darg = va_arg(ap, double);

           // if (darg < 0)
           // {
           //     sign = '-';
           // }
           // _prtdbl(darg, PRECISION, str);
            break;

        case 'u':
            digit1 = '\0';
            /* "negative" longs in unsigned format  */
            /* can't be computed with long division */
            /* convert *args to "positive", digit1  */
            /* = how much to add back afterwards    */
            larg = va_arg(ap, long);

            while (larg < 0)
            {
                larg -= 1000000000L;
                ++digit1;
            }
             dec2str(str,larg);//_prtl10(larg, str);
            str[0] += digit1;
            fmax = 0;
            break;

        case 'o':
            larg = va_arg(ap, long);

            //_prtl8(larg, str);
            fmax = 0;
            break;

        case 'X':
            larg = va_arg(ap, long);

             _prtX16(larg, str);
            fmax = 0;
            break;

        case 'x':
            larg = va_arg(ap, long);

            _prtl16(larg, str);
            fmax = 0;
            break;

        case 'H':
            larg = va_arg(ap, long);

            _prtX16(larg, str);

            larg = va_arg(ap, long);

            _prtX16(larg, str + 8);

            fmax = 0;
            break;

        case 'h':
            larg = va_arg(ap, long);

            _prtl16(larg, str);

            larg = va_arg(ap, long);

            _prtl16(larg, str + 8);

            fmax = 0;
            break;

        case 'b':
            larg = va_arg(ap, long);

            _prtl2(larg, str);
            fmax = 0;
            break;

        default:
            xputc (f);
            break;
        }
        for (length = 0; str[length] != '\0'; length++)
        {;
        }
        if (fmin > MAXSTR || fmin < 0)
        {
            fmin = 0;
        }
        if (fmax > MAXSTR || fmax < 0)
        {
            fmax = 0;
        }
        leading = 0;
        if (fmax != 0 || fmin != 0)
        {
            if (fmax != 0)
            {
                if (length > fmax)
                {
                    length = fmax;
                }
            }
            if (fmin != 0)
            {
                leading = fmin - length;
            }
            if (sign == '-')
            {
                --leading;
            }
        }
        if (sign == '-' && fill == '0')
        {
            xputc (sign);
        }
        if (leftjust == 0)
        {
            for (i = 0; i < leading; i++)
            {
                xputc (fill);
            }
        }
        if (sign == '-' && fill == ' ')
        {
            xputc (sign);
        }
        for (i = 0; i < length; i++)
        {
            xputc (str[i]);
        }
        if (leftjust != 0)
        {
            for (i = 0; i < leading; i++)
                xputc (fill);
        }
    }
va_end(ap);
return 0;
}

/**
 * Prints
 * @param num
 * @param *str
 */
/*
static void _prtl10(long num, char *str)
{
    int i;
    char temp[11];

    temp[0] = '\0';
    for (i = 1; i <= 10; i++)
    {
        temp[i] = num % 10 + '0';
        num /= 10;
    }
    for (i = 10; temp[i] == '0'; i--);
    if (i == 0)
        i++;
    while (i >= 0)
        *str++ = temp[i--];
}*/

/**
 * Prints 
 * @param num
 * @param *str
 */
static void _prtl8(long num, char *str)
{
    int i;
    char temp[12];

    temp[0] = '\0';
    for (i = 1; i <= 11; i++)
    {
        temp[i] = (num & 07) + '0';
        num = num >> 3;
    }
    temp[11] &= '3';
    for (i = 11; temp[i] == '0'; i--);
    if (i == 0)
        i++;
    while (i >= 0)
        *str++ = temp[i--];
}

/**
 * Prints 
 * @param num
 * @param *str
 */
static void _prtl16(long num, char *str)
{
    int i;
    char temp[9];

    temp[0] = '\0';
    for (i = 1; i <= 8; i++)
    {
        temp[i] = "0123456789abcdef"[num & 0x0F];
        num = num >> 4;
    }
    for (i = 8; temp[i] == '0'; i--);
    if (i == 0)
        i++;
    while (i >= 0)
        *str++ = temp[i--];
}

/**
 * Prints 
 * @param num
 * @param *str
 */
static void _prtX16(long num, char *str)
{
    int i;
    char temp[9];

    temp[0] = '\0';
    for (i = 1; i <= 8; i++)
    {
        temp[i] = "0123456789ABCDEF"[num & 0x0F];
        num = num >> 4;
    }
    for (i = 8; temp[i] == '0'; i--);
    if (i == 0)
        i++;
    while (i >= 0)
        *str++ = temp[i--];
}

/**
 * Prints 
 * @param num
 * @param *str
 */
static void _prtl2(long num, char *str)
{
    int i;
    char temp[35];

    temp[0] = '\0';
    for (i = 1; i <= 32; i++)
    {
        temp[i] = ((num % 2) == 0) ? '0' : '1';
        num = num >> 1;
    }
    for (i = 32; temp[i] == '0'; i--);
    if (i == 0)
        i++;
    while (i >= 0)
        *str++ = temp[i--];
}
