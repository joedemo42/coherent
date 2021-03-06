#ifndef	__KERNEL_DEFER_H__
#define	__KERNEL_DEFER_H__

/*
 *-IMPORTS:
 *	<common/ccompat.h>
 *		__EXTERN_C_BEGIN__
 *		__EXTERN_C_END__
 *		__PROTO ()
 *	<common/_cpuid.h>
 *		processorid_t
 */

#include <common/ccompat.h>
#include <kernel/_cpuid.h>
#include <kernel/_defer.h>

/*
 * An interesting facility is used in the Coherent kernel; called defer (), it
 * requests that a function (with a single argument) be called before return
 * to user level. It's a useful facility, but need some work to be useful for
 * a multiprocessor system. At the same time, it seems like a good idea to
 * alter it a litte; unfortunately, the facility suffers severe abuse from (a)
 * being used to solve problems of poor design such as not having interrupts
 * prioritized, and (b) being used as a global "solution" for synchronization
 * issues by moving parts of interrupt code out into base level, where no
 * thought need be given to synchronization. In this system, (a) has been
 * dealt with properly, and (b) synchronization issues *must* be dealt with in
 * a multiprocessor system properly.
 *
 * The version presented here has two aims; as a general mechanism for
 * deferring activity for "return to user level" time to reduce interrupt
 * latency, and as a mechanism for binding activities to specific processors.
 * A benefit of using this approach is simplifying the complex set of "before
 * we return to user level" features.
 *
 * Note that this version does not permit callback functions to take any
 * parameters, thus avoiding type problems (see bufcall () for some of these
 * that AT&T have foisted on us) and also discourages using it in fashion (b)
 * above. This could be changed if we find a genuine reason.
 */


__EXTERN_C_BEGIN__

int		defer_int_cpu	__PROTO ((__deffuncp_t _funcp,
					  processorid_t _cpuid));
int		defer_int_any	__PROTO ((__deffuncp_t _funcp));
int		defer_int_here	__PROTO ((__deffuncp_t _funcp));

int		defer_proc_cpu	__PROTO ((__deffuncp_t _funcp,
					  processorid_t _cpuid));
int		defer_proc_any	__PROTO ((__deffuncp_t _funcp));
int		defer_proc_here	__PROTO ((__deffuncp_t _funcp));

__EXTERN_C_END__


#endif	/* ! defined (__KERNEL_DEFER_H__) */
