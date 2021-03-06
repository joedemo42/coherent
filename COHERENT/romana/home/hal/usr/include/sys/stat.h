/* (-lgl
 *	Coherent 386 release 4.2
 *	Copyright (c) 1982, 1993 by Mark Williams Company.
 *	All rights reserved. May not be copied without permission.
 *	For copying permission and licensing info, write licensing@mwc.com
 -lgl) */

#ifndef	__SYS_STAT_H__
#define	__SYS_STAT_H__

#include <common/feature.h>
#include <common/ccompat.h>
#include <common/__time.h>
#include <common/__off.h>
#include <common/_fsize.h>
#include <common/_uid.h>
#include <common/__stat.h>
#include <common/_imode.h>
#include <common/_timestr.h>

/*
 * Structure returned by stat () and fstat () system calls.
 *
 * Some configuration systems do some ill-advised things such as using 'grep'
 * to determine whether a system is capable of supporting the R4 version of
 * the "stat" structure, so we must hide the definition by default.
 */

#ifdef	__R4STAT_TAG

struct __R4STAT_TAG {
	n_dev_t		st_dev;		/* device */
	long		__pad1 [3];
	n_ino_t		st_ino;		/* inode number */
	n_mode_t	st_mode;	/* permission bits */
	n_nlink_t	st_nlink;	/* link count */
	n_uid_t		st_uid;		/* owner */
	n_gid_t		st_gid;		/* group */
	n_dev_t		__NON_POSIX (st_rdev);
					/* directory device */
	long		__pad2 [2];
	__off_t		st_size;	/* current size */
	long		__pad3;
	__timestruc_t	st_atim;	/* time of last access */
	__timestruc_t	st_mtim;	/* time of last modification */
	__timestruc_t	st_ctim;	/* time of creation */
	long		__NON_POSIX (st_blksize);
					/* preferred operation size */
	long		__NON_POSIX (st_blocks);
					/* total size in allocated blocks */
	char		__NON_POSIX (st_fstype) [_ST_FSTYPSZ];
					/* file-system type name */
	long		__pad4 [8];
};

#endif	/* ! defined (__R4STAT_TAG) */


struct __R3STAT_TAG {
	o_dev_t		st_dev;		/* Device */
	o_ino_t		st_ino;		/* Inode number	*/
	o_mode_t	st_mode;	/* Mode	*/
	o_nlink_t	st_nlink;	/* Link count */
	o_uid_t		st_uid;		/* User id */
	o_gid_t		st_gid;		/* Group id */
	o_dev_t	 	__NON_POSIX (st_rdev);
					/* Real device */
	__off_t		st_size;	/* Size */
	__time_t	st_atime;	/* Access time */
	__time_t	st_mtime;	/* Modify time */
	__time_t	st_ctime;	/* Change time */
};


#if	_SYSV4 && ! _DDI_DKI

#define	st_atime	st_atim.tv_sec
#define	st_mtime	st_mtim.tv_sec
#define	st_ctime	st_ctim.tv_sec

#endif


/*
 * Mode macros. Note that the following definitions are required to be
 * macros. Several packages also use them in #if-expressions, so we
 * cannot base them on the underlying versions in <common/_imode.h>.
 */

#define	S_IFMT		0xF000		/* Type	mask */
#define	S_IFDIR		0x4000		/* Directory */
#define	S_IFCHR		0x2000		/* Character special */
#define	S_IFPIP		0x1000		/* Pipe	*/
#define	S_IFIFO		0x1000		/* Pipe	*/
#define	S_IFBLK		0x6000		/* Block special */
#define	S_IFREG		0x8000		/* Regular */
#define	S_IFSOCK	0x1000		/* Socket */


#define	S_ISUID		0x0800		/* Set user id on execution	*/
#define	S_ISGID		0x0400		/* Set group id on execution	*/
#define	S_ISVTX		0x0200		/* Save swapped text even after use */

#define	S_IREAD		0x0100		/* Read permission, owner	*/
#define	S_IWRITE	0x0080		/* Write permission, owner	*/
#define	S_IEXEC		0x0040		/* Execute/search permission, owner */
#define	S_IRWXU		0x01C0		/* RWX permission, owner	*/
#define	S_IRUSR		0x0100		/* Read permission, owner	*/
#define	S_IWUSR		0x0080		/* Write permission, owner	*/
#define	S_IXUSR		0x0040		/* Execute/search permission, owner */
#define	S_IRWXG		0x0038		/* RWX permission, group	*/
#define	S_IRGRP		0x0020		/* Read permission, group	*/
#define	S_IWGRP		0x0010		/* Write permission, group	*/
#define	S_IXGRP		0x0008		/* Execute/search permission, group */
#define	S_IRWXO		0x0007		/* RWX permission, other	*/
#define	S_IROTH		0x0004		/* Read permission, other	*/
#define	S_IWOTH		0x0002		/* Write permission, other	*/
#define	S_IXOTH		0x0001		/* Execute/search permission, other */

/* Mode test macros. */

#define	S_ISFIFO(mode)	(((mode) & S_IFMT) == S_IFIFO)
#define	S_ISSOCK(mode)	(((mode) & S_IFMT) == S_IFSOCK)
#define	S_ISCHR(mode)	(((mode) & S_IFMT) == S_IFCHR)
#define	S_ISDIR(mode)	(((mode) & S_IFMT) == S_IFDIR)
#define	S_ISBLK(mode)	(((mode) & S_IFMT) == S_IFBLK)
#define	S_ISREG(mode)	(((mode) & S_IFMT) == S_IFREG)


#if	! (_DDI_DKI || _KERNEL)

__EXTERN_C_BEGIN__

int		chmod		__PROTO ((__CONST__ char * _path,
					  __mode_t _mode));
int		fstat		__PROTO ((int _fildes, struct stat * _buf));
int		mkdir		__PROTO ((__CONST__ char * _path,
					  __mode_t _mode));
int		mkfifo		__PROTO ((__CONST__ char * _path,
					  __mode_t _mode));
int		stat		__PROTO ((__CONST__ char * _path,
					  struct stat * _buf));
__mode_t	umask		__PROTO ((__mode_t _mask));

__EXTERN_C_END__

#endif	/* ! (_DDI_DKI || _KERNEL) */

#if	! _DDI_DKI

/*
 * Nonexistent device.
 * Must compare correctly with dev_t, which is an unsigned short in the
 * internal COHERENT device-driver system.
 */

#ifndef	NODEV
# define NODEV		((__dev_t) -1)
#endif


/*
 * Functions.
 */
#define	major(dev)	(((dev) >> 8) & 0xFF)
#define minor(dev)	((dev) & 0xFF)
#define makedev(m1, m2)	((__dev_t) (((m1) << 8) | (m2)))

#endif	/* ! _DDI_DKI */

#endif	/* ! defined (__SYS_STAT_H__) */
