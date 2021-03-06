/* (-lgl
 *	Coherent 386 release 4.2
 *	Copyright (c) 1982, 1993 by Mark Williams Company.
 *	All rights reserved. May not be copied without permission.
 *	For copying permission and licensing info, write licensing@mwc.com
 -lgl) */

#ifndef	__KERNEL_DDI_BASE_H__
#define	__KERNEL_DDI_BASE_H__

/*
 * This internal header file defines structures and an access procedure for
 * DDI/DKI process-level data that will not be needed outside the process
 * context.  This information corresponds to "U area" data in traditional
 * UNIX implementations.  Although it may in fact be implemented that way,
 * using an accessor macro guarantees source-level compatibility with systems
 * that do not or cannot implement a traditional U area.
 */

#include <common/feature.h>
#include <common/ccompat.h>
#include <sys/uio.h>


/*
 * DDI/DKI per-process data needed only at base level.
 *
 * Note that it might seem like a reasonable idea to record the list of sleep
 * locks held by a process for debugging.  However, this is not possible,
 * because the DDI/DKI specification defines SLEEP_TRYLOCK () and
 * SLEEP_UNLOCK () as being available at interrupt level, i.e., not requiring
 * user context, which makes this idea impossible.  (It is not clear whether
 * a process can in fact try to recursively lock a sleep lock if it has
 * arranged for some interrupt-level activity to unlock it, but it seems
 * reasonable.)
 */

struct ddi_base_data {
	char		_non_empty;		/* make structure non-empty */
};

typedef struct ddi_base_data	dbdata_t;


__EXTERN_C_BEGIN__

dbdata_t      *	ddi_base_data	__PROTO ((void));

#if	0
void		DESTROY_UIO	__PROTO ((uio_t * _uiop, IO * _iop));
uio_t	      *	MAKE_UIO	__PROTO ((uio_t * _uiop, iovec_t * _iovp,
					  int _mode, IO * _iop));
#endif

__EXTERN_C_END__

#endif	/* ! defined (__KERNEL_DDI_BASE_H__) */
