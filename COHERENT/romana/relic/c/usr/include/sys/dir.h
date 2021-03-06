/* (-lgl
 * 	COHERENT Version 3.0
 * 	Copyright (c) 1982, 1990 by Mark Williams Company.
 * 	All rights reserved. May not be copied without permission.
 -lgl) */
/*
 * Directory entry.
 */

#ifndef	 DIR_H
#define	 DIR_H	DIR_H

#include <sys/types.h>

#define DIRSIZ	14		/* Size of directory name */
#define	DIR	char *		/* Directory type */

/*
 * Directory entry structure.
 */
struct direct {
	ino_t	 d_ino;			/* Inode number */
	char	 d_name[DIRSIZ];	/* Name */
};

#endif
