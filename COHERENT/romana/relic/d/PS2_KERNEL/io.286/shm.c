/* $Header: /kernel/kersrc/io.286/shm.c,v 1.1 92/07/17 15:24:46 bin Exp Locker: bin $
 *
 *	The  information  contained herein  is a trade secret  of INETCO
 *	Systems, and is confidential information.   It is provided under
 *	a license agreement,  and may be copied or disclosed  only under
 *	the terms of that agreement.   Any reproduction or disclosure of
 *	this  material  without  the express  written  authorization  of
 *	INETCO Systems or persuant to the license agreement is unlawful.
 *
 *	Copyright (c) 1985, 1984
 *	An unpublished work by INETCO Systems, Ltd.
 *	All rights reserved.
 */

/*
 * System V Compatible Shared Memory Device Driver
 *
 *	This device driver provides System V compatible shared memory operations.
 *	Operations are performed through the shared memory device (/dev/shm).
 *	and are implemented as ioctl calls from shmctl, shmget, shmat, shmdt
 *	utilities.
 *
 *			Author: Allan Cornish, INETCO Systems Ltd., Sep 1984
 *
 * $Log:	shm.c,v $
 * Revision 1.1  92/07/17  15:24:46  bin
 * Initial revision
 * 
 * Revision 2.1	88/09/03  13:12:17	src
 * *** empty log message ***
 * 
 * Revision 1.1	88/03/24  17:06:32	src
 * Initial revision
 * 
 * 85/10/16	Allan Cornish
 * Driver split into shmcon.c, shm.c [driver implementation, System V shm].
 *
 * 85/07/22	Allan Cornish
 * Shmget, shmctl now return immediately if u.u_error is set.
 *
 * 85/07/19	Allan Cornish
 * Separation of io_seek into shmid and off improved through type casting.
 * Errno set to EFAULT if fucopy() or ufcopy() report no bytes transferred.
 * This would occur if an user address fault occurred.
 *
 * 85/07/03	Allan Cornish
 * Replaced use of EDOM with EIDRM.
 * Replaced shmaccess() by calls to ipcaccess(), increasing shared ipc code.
 * Eliminated shmlock() and shmunlock(), as shared memory use is synchronous.
 */

#include <coherent.h>
#include <sched.h>
#include <types.h>
#include <errno.h>
#include <stat.h>
#include <con.h>
#include <seg.h>
#include <shm.h>

#ifndef	EIDRM
#define	EIDRM	EDOM
#endif


extern	unsigned NSHMID;
extern	struct shmid_ds *shmids;
extern	struct seg **shmsegs;

/*
 * Shmctl - Shared Memory Control Operations.
 */

ushmctl( shmid, cmd, buf )

unsigned shmid;
int cmd;
struct shmid_ds *buf;

{
	register struct shmid_ds *idp;
	int ret = 0;

	if ( u.u_error )
		return -1;

	if ( shmid >= NSHMID ) {
		u.u_error = EINVAL;
		return -1;
	}

	idp = &shmids[shmid];

	if ( (idp->shm_perm.mode & IPC_ALLOC) == 0 ) {
		u.u_error = EINVAL;
		return -1;
	}

	switch ( cmd ) {

	case IPC_STAT:
		if ( ( ipcaccess( &idp->shm_perm ) & SHM_R ) == 0 ) {
			u.u_error = EACCES;
			return -1;
		}
		kucopy( idp, buf, sizeof(struct shmid_ds) );
		ret = 0;
		break;

	case IPC_SET:
		if ( (u.u_uid != 0) && (u.u_uid != idp->shm_perm.uid) ) {
			u.u_error = EPERM;
			ret = -1;
			break;
		}
		idp->shm_perm.uid   = getuwd( &(buf->shm_perm.uid ) );
		idp->shm_perm.gid   = getuwd( &(buf->shm_perm.gid ) );
		idp->shm_perm.mode &= ~0777;
		idp->shm_perm.mode |= getuwd(&(buf->shm_perm.mode)) & 0777;
		ret = 0;
		break;

	case IPC_RMID:
		if ( (u.u_uid != 0) && (u.u_uid != idp->shm_perm.uid) ) {
			u.u_error = EPERM;
			ret = -1;
			break;
		}
		idp->shm_perm.seq++;
		sfree(shmsegs[shmid]);
		idp->shm_perm.mode = 0;
		ret = 0;
		break;

	default:
		u.u_error = EINVAL;
		ret = -1;
	}

	return ret;
}

/*
 * Shmget - Get Shared Memory Segment
 */

ushmget( skey, size, shmflg )

key_t skey;
unsigned size;
int shmflg;

{
	register struct shmid_ds *idp;
	struct shmid_ds *freeidp = 0;

	if ( u.u_error )
		return -1;

	for ( idp = &shmids[NSHMID]; --idp >= shmids; ) {

		if ( (idp->shm_perm.mode & IPC_ALLOC) == 0 ) {

			if ((freeidp == 0) ||
			    (freeidp->shm_ctime > idp->shm_ctime))
				freeidp = idp;
			continue;
		}

#ifdef	IPC_PRIVATE
		if (skey == IPC_PRIVATE)
			continue;
#endif

		if (skey == idp->shm_perm.key) {		/* found! */

			if ( (shmflg & IPC_CREAT) && (shmflg & IPC_EXCL) ) {

				u.u_error = EEXIST;
				return -1;
			}

			if ((idp->shm_perm.mode & shmflg) != (shmflg&0777)) {

				u.u_error = EACCES;
				return -1;
			}

			if ( idp->shm_segsz < size ) {

				u.u_error = EINVAL;
				return -1;
			}

			return idp - shmids;
		}
	}

	if ( !(shmflg & IPC_CREAT) ) {
		u.u_error = ENOENT;
		return -1;
	}

	if ( freeidp == 0 ) {
		u.u_error = ENOSPC;
		return -1;
	}

	idp = freeidp;

	if ((shmsegs[idp - shmids] = salloc((size_t) size, SFNSWP)) == NULL){
		u.u_error = ENOSPC;
		return -1;
	}

	idp->shm_segsz = size;
	idp->shm_atime = 0;
	idp->shm_dtime = 0;
	idp->shm_ctime = timer.t_time;
	idp->shm_cpid  = SELF->p_pid;
	idp->shm_perm.cuid = idp->shm_perm.uid = u.u_uid;
	idp->shm_perm.cgid = idp->shm_perm.gid = u.u_gid;
	idp->shm_perm.mode = (shmflg & 0777) | IPC_ALLOC;
	idp->shm_perm.key  = skey;

#ifdef	IPC_PRIVATE
	if ( skey == IPC_PRIVATE )
		idp->shm_perm.mode |= SHM_DEST;
#endif

	return idp - shmids;
}
