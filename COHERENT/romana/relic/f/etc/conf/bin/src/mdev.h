#ifndef	MDEV_H
#define	MDEV_H

#include "devadm.h"


enum {
	MD_DISABLED,
	MD_INSTALLABLE,
	MD_ENABLED
};


/*
 * Structure of the data gathered by the mdev routines.
 */

typedef	unsigned int	maj_t;
typedef	unsigned int	min_t;

struct mdevice {
	mdev_t	      *	md_next;	/* global list of all entries */
	mdev_t	      *	md_chrlink;	/* for sorts */
	mdev_t	      *	md_blklink;	/* for sorts */
	mdev_t	      *	md_modlink;	/* for sorts */
	mdev_t	      *	md_cohlink;	/* for sorts */

	sdev_t	      *	md_sdevices;	/* thread of "sdevice" entries */
	int		md_configure;	/* See MD_... constants */
	int		md_interrupt;	/* has an enabled interrupt */

	symbol_t      *	md_devname;	/* internal module/device name */
	symbol_t      *	md_functions;	/* present driver entry points */
	symbol_t      *	md_flags;	/* driver characteristics */
	symbol_t      *	md_prefix;	/* driver prefix */
	maj_t		md_blk_maj [2];	/* block major number low, high */
	maj_t		md_chr_maj [2];	/* character major number low, high */
	min_t		md_minor_min;	/* minor number minimum */
	min_t		md_minor_max;	/* minor number maximum */

	/*
	 * The following fields are not used in the Coherent system as yet
	 * but are defined by mdevice (4) so we read them.
	 */

	int		md_dma_chan;	/* DMA channel number */
	int		md_cpu_id;	/* processor-id */
};


/*
 * Types suitable for use as selection and comparison predicates for the
 * mdev_sort () function.
 */

typedef	int  (*	msel_t)		PROTO ((mdev_t * _mdevp));
typedef int  (*	mcmp_t)		PROTO ((mdev_t * _left, mdev_t * _right));
typedef	void (*	miter_t)	PROTO ((VOID * _extra, mdev_t * _mdevp));


/*
 * For Coherent, the first 32 major numbers are reserved for the use of the
 * old-style Coherent device-driver system.
 */

enum {
	MAJOR_RESERVED		= 32,
	MAX_DEVNAME		= 8,
	MAX_PREFIX		= 4
};

enum {
	MDEV_OPEN		= 'o',
	MDEV_CLOSE		= 'c',
	MDEV_READ		= 'r',
	MDEV_WRITE		= 'w',
	MDEV_IOCTL		= 'i',
	MDEV_STARTUP		= 's',
	MDEV_EXIT		= 'x',
	MDEV_INIT		= 'I',
	MDEV_HALT		= 'h',
	MDEV_CHPOLL		= 'p',

	MDEV_MMAP		= 'M',	/* not defined in SVR4 mdevice (4) */
	MDEV_SIZE		= 'z'	/* not defined in SVR4 mdevice (4) */
};

#define	MDEV_FUNCS		"ocrwisxIhpMz"

enum {
	MDEV_INSTALLABLE	= 'i',
	MDEV_CHAR		= 'c',
	MDEV_BLOCK		= 'b',
	MDEV_STREAM		= 'S',
	MDEV_DDI_DDK		= 'f',
	MDEV_TTY		= 't',
	MDEV_ONE_SDEVICE	= 'o',
	MDEV_REQUIRED		= 'r',
	MDEV_SAME_MAJORS	= 'u',
	MDEV_HARDWARE		= 'H',
	MDEV_NO_INT		= 'G',
	MDEV_SHARE_DMA		= 'D',
	MDEV_MULTIPLE_MAJORS	= 'M',
	MDEV_ALLOW_IOA_OVERLAP	= 'O',

/*
 * The following flag specifies that we are dealing with an old-style Coherent
 * device driver. These drivers ignore any interrupt settings, doing their
 * work though the mtune/stune system instead, so the 'G' flag also applies.
 * The 'o' flag is also appropriate.
 */

	MDEV_COHERENT		= 'C'
};

#define	MDEV_FLAGS		"icbSftoruHGDMOC"


EXTERN_C_BEGIN

mdev_t	      *	find_mdev	PROTO ((symbol_t * _sym));
void		for_all_mdevices PROTO ((miter_t _iter, VOID * _extra));
int		mdev_func	PROTO ((mdev_t * _mdevp, char _func));
int		mdev_flag	PROTO ((mdev_t * _mdevp, char _flag));
void		read_mdev_file	PROTO ((CONST char * _inname,
					CONST char * _outname,
					VOID * _extra));
void		read_mdev_string PROTO ((CONST char * _string,
					 VOID * _extra));
int		mdev_sort	PROTO ((mdev_t ** _mlistp, mdev_t ** _mdendp,
					msel_t _selpred, mcmp_t _cmppred,
					size_t _ptroff));
void		write_mdevice	PROTO ((mdev_t * _mdevp, input_t * _input));

EXTERN_C_END

#endif	/* ! defined (MDEV_H) */
