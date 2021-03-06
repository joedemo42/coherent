/* (-lgl
 * 	COHERENT Version 4.0.3
 * 	Copyright (c) 1982, 1993 by Mark Williams Company.
 * 	All rights reserved. May not be copied without permission.
 -lgl) */

/*
 * Accounting structure.
 */
#ifndef	 __ACCT_H__
#define	 __ACCT_H__

#include <sys/types.h>
#include <common/_time.h>

typedef	unsigned short	comp_t;


struct acct {
	char	ac_comm[10];		/* Command name */
	comp_t	ac_utime;		/* User time */
	comp_t	ac_stime;		/* System time */
	comp_t	ac_etime;		/* Elapsed time */
	time_t	ac_btime;		/* Beginning time of process */
	short	ac_uid;			/* User id */
	short	ac_gid;			/* Group id */
	short	ac_mem;			/* Average memory usage */
	comp_t	ac_io;			/* Number of disk I/O blocks */
	dev_t	ac_tty;			/* Control typewriter */
	char	ac_flag;		/* Accounting flag */
};

/*
 * Flags (ac_flag).
 */
#define AFORK	01			/* Execute fork, but not exec */
#define ASU	02			/* Used super user privileges */

#endif
