/* (-lgl
 *	Coherent 386 release 4.2
 *	Copyright (c) 1982, 1993 by Mark Williams Company.
 *	All rights reserved. May not be copied without permission.
 *	For copying permission and licensing info, write licensing@mwc.com
 -lgl) */

#ifndef	 __SYS_MOUNT_H__
#define	 __SYS_MOUNT_H__

/*
 * Mount table.
 */

#include <common/feature.h>
#include <common/_uid.h>
#include <sys/filsys.h>
#include <sys/ksynch.h>

/*
 * Mount table structure.
 */

typedef struct mount {
	struct mount  *	m_next;		/* Pointer to next */
	struct inode  *	m_ip;		/* Associated inode */
	o_dev_t		m_dev;		/* Device */
	int	 	m_flag;		/* Flags */
	struct filsys	m_super;	/* Super block */

	__DUMB_GATE	__m_ilock;	/* Inode lock */
	__DUMB_GATE	__m_flock;	/* Free list lock */
} MOUNT;


/*
 * Flags.
 */

#define	MFRON	001			/* Read only file system */


#if	_KERNEL

#define	__INIT_MOUNT_LOCK(mp) \
		(__GATE_INIT ((mp)->__m_ilock, "free inode list", \
			      __GATE_COUNT), \
		 __GATE_INIT ((mp)->__m_flock, "free block list", \
			      __GATE_COUNT))
#define	__LOCK_FREE_INODE_LIST(mp, where) \
		(__GATE_LOCK ((mp)->__m_ilock, "lock : free inodes " where))
#define	__UNLOCK_FREE_INODE_LIST(mp) \
		(__GATE_UNLOCK ((mp)->__m_ilock))
#define	__LOCK_FREE_BLOCK_LIST(mp, where) \
		(__GATE_LOCK ((mp)->__m_flock, "lock : free blocks " where))
#define	__UNLOCK_FREE_BLOCK_LIST(mp) \
		(__GATE_UNLOCK ((mp)->__m_flock))

/*
 * Functions.
 */

MOUNT	*fsmount();			/* fs2.c */
MOUNT	*getment();			/* fs2.c */

/*
 * Global variables.
 */
extern	MOUNT	*mountp;		/* Mount table */

#endif	/* _KERNEL */

#endif	/* ! defined (__SYS_MOUNT_H__) */
