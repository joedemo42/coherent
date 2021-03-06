/* $Header: /kernel/kersrc/i286/RCS/ld.c,v 1.1 92/07/17 15:21:30 bin Exp Locker: bin $
 *
 *	The  information  contained herein  is a trade secret  of INETCO
 *	Systems, and is confidential information.   It is provided under
 *	a license agreement,  and may be copied or disclosed  only under
 *	the terms of that agreement.   Any reproduction or disclosure of
 *	this  material  without  the express  written  authorization  of
 *	INETCO Systems or persuant to the license agreement is unlawful.
 *
 *	Copyright (c) 1986
 *	An unpublished work by INETCO Systems, Ltd.
 *	All rights reserved.
 */

/*
 * Pseudo-Device Interface to Loadable Drivers.
 *
 * $Log:	ld.c,v $
 * Revision 1.1  92/07/17  15:21:30  bin
 * Initial revision
 * 
 * Revision 1.1	88/03/24  17:39:36	src
 * Initial revision
 * 
 * 87/12/08	Allan Cornish	/usr/src/sys/i8086/src/ld.c
 * Block device interface added to loadable drivers.
 *
 * 87/10/25	Allan Cornish		/usr/src/sys/i8086/drv/ld.c
 * Initial version.
 */

#include	<sys/coherent.h>
#include 	<sys/fdisk.h>
#include	<sys/buf.h>
#include	<sys/con.h>
#include	<sys/stat.h>
#include	<errno.h>

/*
 * Driver configuration.
 */
void	ld_open();
void	ld_close();
void	ld_read();
void	ld_write();
void	ld_block();
int	ld_ioctl();
void	ld_power();
void	ld_time();
int	ld_poll();
void	nulldev();
void	nonedev();

/*
 * Loadable driver: Pseudeo-device configuration.
 */
CON ldrvpsy = {
	DFCHR|DFBLK|DFPOL,		/* Flags */
	0,				/* Major index */
	ld_open,			/* Open */
	ld_close,			/* Close */
	ld_block,			/* Block */
	ld_read,			/* Read */
	ld_write,			/* Write */
	ld_ioctl,			/* Ioctl */
	ld_power,			/* Powerfail */
	ld_time,			/* Timeout */
	nulldev,			/* Load */
	nulldev,			/* Unload */
	ld_poll				/* Poll */
};

/*
 * Loadable driver: Code selectors.
 */
saddr_t ldrvsel[NDRV];

/*
 * Loadable driver: Pointers to driver configuration table.
 */
CON * ldrvcon[NDRV];

/*
 * Loadable driver: Selector referencing interrupt handler's code segment.
 */
saddr_t ldrvics[16];

/*
 * Loadable driver: Pointers to interrupt handlers within the loadable driver.
 */
void (*ldrvipc[16])();
