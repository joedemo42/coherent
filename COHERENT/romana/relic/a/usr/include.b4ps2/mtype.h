/*
 * Machine types.
 *
 * $Log:	mtype.h,v $
 * Revision 1.2  92/09/29  09:06:39  bin
 * updated with kernel 63 source
 * 
 * Revision 1.1	89/04/26  16:25:40 	root
 * Initial revision
 * 
 */

#ifndef	 MTYPE_H
#define	 MTYPE_H	MTYPE_H

/*
 * Type definitions.
 */
#define M_PDP11	1		/* PDP-11 */
#define M_VAX	2		/* VAX */
#define	M_360	3		/* IBM 360 */
#define M_Z8001	4		/* Zilog 8001 (segmented) */
#define M_Z8002	5		/* Zilog 8002 (unsegmented) */
#define M_8086	6		/* Intel 8086 */
#define M_8080	7		/* Intel 8080 */
#define M_6800	8		/* Motorola 6800 */
#define M_6809	9		/* Motorola 6809 */
#define M_68000	10		/* Motorola 68000 */
#define	M_386	11		/* Intel 386, 486 (32 bit mode) */

#endif
