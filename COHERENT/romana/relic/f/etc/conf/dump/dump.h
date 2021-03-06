#ifndef	__DUMP_H__
#define	__DUMP_H__

/*
 * Information for the DUMP module.
 *
 * This header and the dump module itself are taken from the code printed in
 * the STREAMS Programmer's Guide for System V Release 4 Multiprocessor, with
 * some minor editing to reflect local coding standards.
 */

enum {
	DUMPIOC		= ('Q' << 8),

	DUMP_VERB	= DUMPIOC | 1,
	DUMP_TERSE	= DUMPIOC | 2
};

#if	_DDI_DKI

/*
 * Kernel-level information.
 */


typedef enum {
	D_FREE		= 0,		/* Dump slot free */
	D_USED		= 1		/* dump slot in use */
} dm_use_t;

typedef enum {
	D_NONE		= 0,		/* Nothing happening */
	D_OUT		= 1,		/* Outgoing data */
	D_IN		= 2		/* Incoming data */
} dm_dir_t;

enum {
	D_VERB		= 0x01		/* Verbose option on */
};

struct dm_str {
	dm_use_t	dm_use;		/* is slot in use */
	dm_dir_t	dm_dir;		/* last data direction */
	unsigned char	dm_flags;
};


extern	int		dm_ucnt;	/* count of dm_users */
extern 	struct dm_str	dm_users [];	/* 1 dm_str per user */

#endif

#endif	/* ! defined (__DUMP_H__) */
