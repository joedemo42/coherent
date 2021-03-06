#ifndef	__COMMON__SIGNAL_H__
#define	__COMMON__SIGNAL_H__

#include <common/ccompat.h>
#include <common/_limits.h>
#include <common/_tricks.h>

/*
 * This header file contains a number of definitions for signal-related data
 * types which are variously used by kernel and user-level mechanisms to
 * support the variety of existing binary interfacs.
 */

/*
 * The following constant gives the number of signals for which kernel storage
 * is actually made available; this number is traditionally made available to
 * user code though the NSIG constant, although neither Standard C nor POSIX.1
 * applications can use this.
 */

#define	_SIGNAL_MAX		31


/*
 * Be aware that while ISO C and POSIX.1 specify the signature of a signal-
 * catching function as "void (*) (int sig)", extra parameters to the signal
 * functions are supported.
 */

typedef	void (*	__sigfunc_t)	__PROTO ((__ANY_ARGS__));


/*
 * For dealing with signal actions. For maximum efficiency, we deal with
 * signals as collections of bit-vectors; various user-level binary
 * compatibility standards specify the lengths of the vectors, while the
 * kernel internally allocates exactly as much as is really needed.
 */

typedef	unsigned long		__sigmask_t;

typedef	struct {
	__sigmask_t	_sigbits [4];
} n_sigset_t;

typedef	struct {
	__sigmask_t	_sigbits [1];
} o_sigset_t;

typedef struct {
	__sigmask_t	_sigbits [__DIVIDE_ROUNDUP_CONST (_SIGNAL_MAX,
				   __CHAR_BIT * sizeof (__sigmask_t))];
} __sigset_t;

/*
 * Signal-action flags that are relevant for all signals.
 */

enum {
	__SA_ONSTACK	= 1,
	__SA_RESETHAND	= 2,
	__SA_RESTART	= 4,
	__SA_SIGINFO	= 8,
	__SA_NODEFER	= 16
};
typedef	unsigned short	__sigactfl_t;


/*
 * Signal-action flags that are only relevant for particular signals.
 */

enum {
	__SF_NOCLDWAIT	= 1,
	__SF_NOCLDSTOP	= 2
};

typedef	unsigned short	__sigmiscfl_t;


/*
 * Some simple macros for dealing with the bit-set operations on sigset_t's.
 */

#define	__SIGSET_UNIT(ss,n)	\
	(sizeof ((ss)._sigbits) == sizeof (__sigmask_t) ? 0 : \
	 (unsigned) ((n) - 1) / (sizeof (__sigmask_t) * __CHAR_BIT))

#define	__SIGSET_EMPTY(ss)	memset ((ss)._sigbits, 0, sizeof (ss))
#define	__SIGSET_FILL(ss)	memset ((ss)._sigbits, 0xFF, sizeof (ss));

#define	__SIGSET_MASK(n)	(1UL << ((unsigned) ((n) - 1) & \
				  (sizeof (__sigmask_t) * __CHAR_BIT - 1)))

#define	__SIGSET_ADDBIT(ss,n)	\
		((ss)._sigbits [__SIGSET_UNIT (ss, n)] |= __SIGSET_MASK (n))
#define	__SIGSET_CLRBIT(ss,n)	\
		((ss)._sigbits [__SIGSET_UNIT (ss, n)] &= ~ __SIGSET_MASK (n))
#define	__SIGSET_TSTBIT(ss,n)	\
		(((ss)._sigbits [__SIGSET_UNIT (ss, n)] & \
		 __SIGSET_MASK (n)) != 0)
#define	__SIGSET_ADDMASK(ss,n,m)	\
		((ss)._sigbits [__SIGSET_UNIT (ss, n)] |= (m))
#define	__SIGSET_CLRMASK(ss,n,m)	\
		((ss)._sigbits [__SIGSET_UNIT (ss, n)] &= ~ (m))
#define	__SIGSET_TSTMASK(ss,n,m)	\
		(((ss)._sigbits [__SIGSET_UNIT (ss, n)] & (m)) != 0)
#define	__SIGSET_FIRSTBIT(mask)		(__LEAST_BIT_ULONG (mask) + 1)


#endif	/* ! defined (__COMMON__SIGNAL_H__) */
