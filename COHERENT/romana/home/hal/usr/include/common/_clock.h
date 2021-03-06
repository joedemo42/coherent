/* (-lgl
 *	Coherent 386 release 4.2
 *	Copyright (c) 1982, 1993 by Mark Williams Company.
 *	All rights reserved. May not be copied without permission.
 *	For copying permission and licensing info, write licensing@mwc.com
 -lgl) */

#ifndef	__COMMON__CLOCK_H__
#define	__COMMON__CLOCK_H__

/*
 * This internal header file defines the ISO C data type "clock_t". To
 * support cross-compilation environments, it uses a variety of feature
 * tests to avoid conflict with definitions supplied by the translator headers.
 */

#include <common/feature.h>
#include <common/__clock.h>

#if	__BORLANDC__

# ifndef	_CLOCK_T
# define	_CLOCK_T
typedef	__clock_t	clock_t;

#include <common/_clktck.h>

# endif

#elif	__GNUDOS__

typedef __clock_t	clock_t;

#elif	__COHERENT__

typedef __clock_t	clock_t;

#else

# error	clock_t not known for this system

#endif

#endif	/* ! defined (__COMMON__CLOCK_H__) */
