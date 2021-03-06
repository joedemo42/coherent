/* (-lgl
 *	Coherent 386 release 4.2
 *	Copyright (c) 1982, 1993 by Mark Williams Company.
 *	All rights reserved. May not be copied without permission.
 *	For copying permission and licensing info, write licensing@mwc.com
 -lgl) */

#ifndef __SYS_SYSI86_H__
#define __SYS_SYSI86_H__

#include <common/ccompat.h>

#define SI86FPHW	40

#define FP_NO	0	/* no ndp, no emulation */
#define FP_SW	1	/* no ndp, emulation */
#define FP_HW	2	/* ndp chip present (bit) */
#define FP_287	2	/* 80287 */
#define FP_387	3	/* 80387 or 80486DX */

__EXTERN_C_BEGIN__

int		sysi86		__PROTO ((int _cmd, ...));

__EXTERN_C_END__

#endif	/* ! defined (__SYS_SYSI86_H__) */
