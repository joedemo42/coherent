/* (-lgl
 * 	COHERENT Version 4.0
 * 	Copyright (c) 1982, 1992 by Mark Williams Company.
 * 	All rights reserved. May not be copied without permission.
 -lgl) */
/*
 * Buffer header.
 */
#ifndef  BUF_H
#define  BUF_H

#include <sys/types.h>

typedef struct buf {
	struct	 buf *b_actf;		/* First in queue */
	struct	 buf *b_actl;		/* Last in queue */
	GATE	 b_gate;		/* Gate */
	unsigned b_flag;		/* Flags */
	dev_t	 b_dev;			/* Device */
	daddr_t	 b_bno;			/* Block number */
	char	 b_req;			/* I/O type */
	char	 b_err;			/* Error */
	unsigned b_seqn;		/* Buffer sequence number */
	bold_t	 b_map;			/* Old map */
#ifndef _I386
	vaddr_t  b_count;		/* Size of I/O */
	vaddr_t  b_resid;		/* Driver returns count here */
	faddr_t	 b_faddr;		/* Far Virtual address */
	paddr_t	 b_paddr;		/* Physical address */
#else
	off_t	 b_count;		/* Size of I/O */
	off_t	 b_resid;		/* Driver returns count here */
	paddr_t	 b_paddr;		/* 	physical  address (bytes) */
	vaddr_t	 b_vaddr;		/* kernel virtual address (bytes) */
	unsigned long b_hashval;	/* used to index into hasharray[] */
	struct	 buf *b_LRUf;		/* Next (older) in LRU chain */
	struct	 buf *b_LRUb;		/* Previous (newer) in LRU chain */
	struct	 buf *b_hashf;		/* Next in this hash chain */
	struct	 buf *b_hashb;		/* Previous in this hash chain */
#endif /* _I386 */
} BUF;

/*
 * Flags (b_flags).
 */
#define BFNTP	0x0001			/* Buffer not valid */
#define BFREQ	0x0002			/* Buffer requested */
#define BFERR	0x0004			/* Error */
#define BFMOD	0x0008			/* Data has been modified */
#define BFASY	0x0010			/* Asynchrous */
#define BFMAP	0x0020			/* Buffer has been mapped */
#define BFRAW	0x0040			/* Request is raw */
#define BFTAP	0x0080			/* Request is to a tape */

#define BFBLK	0x4000			/* Aligned on a block boundary */
#define BFIOC	0x8000			/* Perform bounds checking */

/*
 * Requests.
 */
#define	BREAD	1			/* Read */
#define BWRITE	2			/* Write */

#define	BNULL	((BUF *)0)

#ifdef KERNEL
/*
 * Functions.
 */
extern	BUF	 *bread();		/* bio.c */
extern	BUF	 *bclaim();		/* bio.c */
extern	BUF	 *vread();		/* fs3.c */
extern	BUF	 *aread();		/* fs3.c */
extern	daddr_t	 vmap();		/* fs3.c */
extern	int	 *lmap();		/* fs3.c */

/*
 * Global variables.
 */
extern	unsigned bufseqn;		/* Buffer sequencer */
extern	int	 bufneed;		/* Buffer is needed */
extern	BUF	 swapbuf;		/* Buffer for swap I/O */
extern	BUF	 *bufl;			/* Buffer headers */
#endif /* KERNEL */

#endif /* BUF_H */
