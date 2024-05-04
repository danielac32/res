/* origin: FreeBSD /usr/src/lib/msun/src/e_cosh.c */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* cosh(x)
 * Method :
 * mathematically cosh(x) if defined to be (exp(x)+exp(-x))/2
 *      1. Replace x by |x| (cosh(x) = cosh(-x)).
 *      2.
 *                                                      [ exp(x) - 1 ]^2
 *          0        <= x <= ln2/2  :  cosh(x) := 1 + -------------------
 *                                                         2*exp(x)
 *
 *                                                exp(x) +  1/exp(x)
 *          ln2/2    <= x <= 22     :  cosh(x) := -------------------
 *                                                        2
 *          22       <= x <= lnovft :  cosh(x) := exp(x)/2
 *          lnovft   <= x <= ln2ovft:  cosh(x) := exp(x/2)/2 * exp(x/2)
 *          ln2ovft  <  x           :  cosh(x) := huge*huge (overflow)
 *
 * Special cases:
 *      cosh(x) is |x| if x is +INF, -INF, or NaN.
 *      only cosh(0)=1 is exact for finite x.
 */

#include <math.h>
#include "libm.h"

static const double huge = 1.0e300;

double cosh(double x)
{
	double t, w;
	int32_t ix;

	GET_HIGH_WORD(ix, x);
	ix &= 0x7fffffff;

	/* x is INF or NaN */
	if (ix >= 0x7ff00000)
		return x*x;

	/* |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) */
	if (ix < 0x3fd62e43) {
		t = expm1(fabs(x));
		w = 1.0+t;
		if (ix < 0x3c800000)
			return w;  /* cosh(tiny) = 1 */
		return 1.0 + (t*t)/(w+w);
	}

	/* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|))/2; */
	if (ix < 0x40360000) {
		t = exp(fabs(x));
		return 0.5*t + 0.5/t;
	}

	/* |x| in [22, log(maxdouble)] return 0.5*exp(|x|) */
	if (ix < 0x40862E42)
		return 0.5*exp(fabs(x));

	/* |x| in [log(maxdouble), overflowthresold] */
	if (ix <= 0x408633CE)
		return __expo2(fabs(x));

	/* |x| > overflowthresold, cosh(x) overflow */
	return huge*huge;
}
