#ifndef	__KERNEL_DDI_CPU_H__
#define	__KERNEL_DDI_CPU_H__

/*
 * This internal header file defines structures and an access procedure for
 * DDI/DKI data that is global per CPU. In a multiprocessor system, each
 * CPU would have a distinct data region separate from all others.
 */

/*
 *-IMPORTS:
 *	<common/ccompat.h>
 *		__EXTERN_C_BEGIN__
 *		__EXTERN_C_END__
 *		__PROTO ()
 *	<common/__size.h>
 *		__size_t
 *	<common/_intmask.h>
 *		intmask_t
 *	<kernel/_lock.h>
 *		__lock_t
 *		__MAX_HIERARCHY__
 *		__MIN_HIERARCHY__
 *	<kernel/_cpuid.h>
 *		processorid_t
 *	<sys/debug.h>
 *		ASSERT ()
 */

#include <common/ccompat.h>
#include <common/__size.h>
#include <common/_intmask.h>
#include <kernel/_cpuid.h>
#include <kernel/_lock.h>
#include <kernel/ddi_data.h>
#include <sys/debug.h>

/*
 * Many DDI/DKI routines specify constraints on the circumstances in which
 * they may be called, for example.
 *	Base level only, eg SV_WAIT ().
 *	Pass interrupt priority level greater than the current level,
 *		eg LOCK ().
 *	Not called from within a streams service routine, eg putbq () with
 *		a high-priority STREAMS message.
 *
 * These assertions are important for maintaining system integrity, but it can
 * be difficult to detect when these constraints a violated under normal
 * circumstances, especially when the probability of failure is expected to
 * be low. Therefore, it is important for routines which specify such
 * constraints to be able to umambiguously detect constraint violations with
 * the highest level of probability possible.
 *
 * However, the testability of these assertions depends on cooperation from
 * a wide range of kernel facilities. In a highly multithreaded system with
 * either multiple processors or the ability to suspend interrupt contexts,
 * or both, it is necessary to be able to make detailed inquiries about the
 * system state to re-establish some of the certainty about system state that
 * is lost in the move to gain extra concurrency.
 */

/*
 * Returns true if we are at base level or if the system cannot distinguish
 * between base and interrupt level; if the system is definitely not at base
 * level, returns false.
 */

#define		IS_BASE_LEVEL()		(ddi_cpu_data ()->dc_int_level == 0)


/*
 * Returns true if we are at interrupt level or if the system cannot
 * distinguish between base and interrupt level; if the system is definitely
 * at base level, returns false.
 */

#define		IS_INTERRUPT_LEVEL()	(ddi_cpu_data ()->dc_int_level != 0)


/*
 * Most of the time we want to include the above in simple assertions.
 */

#define		ASSERT_BASE_LEVEL()	ASSERT (IS_BASE_LEVEL ())


/*
 * Here is a description of the per-CPU data we wish to record.
 *
 * In order to deal with lock-hierarchy assertions, we use a table of
 * counters instead of maintaining a list of held locks. We do this because
 * the nature of shared read/write locks permits even a single CPU to hold
 * the same lock multiple times, and because while TRYLOCK () and UNLOCK ()
 * allow out-of-order acquisition and release we want LOCK () to rigidly
 * check the hierarchy assertions. The array of counters is mostly O(1), a
 * desirable property.
 *
 * While a trace of held locks is not unreasonable, it is difficult to set a
 * fixed upper bound on the number of simultaneous locks. Even if we use a
 * counter for multiple acquisitions of shared locks, we can potentially need
 * as many trace records as allocated locks.
 */

struct ddi_cpu_data {
	/*
	 * Interrupt-related data is at the front of this structure for easy
	 * access by hand-coded assembly-language support routines. Same for
	 * the defer-table stuff.
	 */

	processorid_t	dc_cpuid;		/* who are we for? */

	intmask_t	dc_base_mask;		/* interrupt masks */
	unsigned char	dc_int_level;		/* processing interrupts */
	unsigned char	dc_user_level;		/* user level/kernel level */
	unsigned char	dc_ipl;			/* current ipl */

	defer_t		dc_defint;		/* interrupt-deferred ops */
	defer_t		dc_defproc;		/* process-deferred ops */

	__lkhier_t	dc_max_hierarchy;	/*
						 * For basic-lock hierarchy
						 * assertions
						 */
	__lkhier_t    *	dc_hierarchy_cnt;

	atomic_uchar_t	dc_run_timeouts;	/* run deferred timeouts */

	struct pollwait	*
			dc_pollwait;		/* task performing a poll */

	char	      *	dc_dynalloc;		/* for getting per-cpu data */
	char	      *	dc_dynend;		/* end of free per-cpu data */
};

typedef struct ddi_cpu_data	dcdata_t;



__EXTERN_C_BEGIN__

dcdata_t      *	ddi_cpu_data	__PROTO ((void));

#if	_DDI_DKI_IMPL
	/*
	 * Functions for the implementation only.
	 */
__VOID__      * ddi_cpu_alloc	__PROTO ((__size_t _size));
dcdata_t      *	ddi_cpu_ref	__PROTO ((processorid_t _cpu));
void		ddi_cpu_unref	__PROTO ((dcdata_t * _data));
processorid_t	ddi_cpu_id	__PROTO ((void));

#endif

__EXTERN_C_END__


extern dcdata_t		__ddi_cpu_data [];

#define	ddi_cpu_data()		(__ddi_cpu_data)

#endif	/* ! defined (__KERNEL_DDI_CPU_H__) */
