/*
 * Relocatable object (from translator)
 *	header
 *	summary segment; contains sizes of loadable segments
 *	loadable segments (possibly fragmented)
 *	relocation fragment follows each relocatable fragment
 *	symbol segment (seek address is seg_off[SYMBOLS] in summary)
 *		(see below)
 */

#ifndef	RELOC_H
#define	RELOC_H	RELOC_H

#include <types.h>
typedef unsigned long uaddr_t;
#define canuaddr(x)	canlong(x)

/*
 * Object header
 */
#define	REL_OBJ	"(Reloc)"

typedef	struct	objhdr_t	{	/* object module header */
	char		obj_type[8],	/* REL_OBJ, BND_OBJ etc. */
			obj_hware[8],	/* target hardware */
			obj_sware[8],	/* program that produced object */
			obj_vers[8];	/* version of above */
} objhdr_t;

/*
 * Segment types
 */

/* vertically encoded */
#define	Xb	0x01	/* execute */
#define	Wb	0x02	/* write */
#define	Sb	0x04	/* shared */
#define	Bb	0x08	/* block */
#define	Cb	0x10	/* common */
#define	Db	Bb	/* expand downwards (output only) */
#define	Lb	Cb	/* loadable (output only) */

/* ordinary segments */
#define	PURCODE	(Xb)
#define	PURDATA	(0)
#define	SHRCODE	(Sb|Wb|Xb)	/* shared => writeable */
#define	SHRDATA	(Sb|Wb)
#define	OWNCODE	(Wb|Xb)
#define	OWNDATA	(Wb)
#define	PLCODE	(Bb|Sb|Wb|Xb)	/* pooled => shared block */
#define	PLDATA	(Bb|Sb|Wb)
#define	BLCODE	(Bb|Wb|Xb)
#define	BLDATA	(Bb|Wb)

/* meanings of otherwise meaningless bit combinations */
/* these take clever advantage of the order of the bits */
#define	STRINGS	(Sb)
#define	DEBUG	(Sb|Xb)
#define	SYMBOLS	(Bb)
#define	TYPESYM	(Bb|Xb)
#define	ABSLUTE	(Bb|Sb)
#define	RELOCN	(Bb|Sb|Xb)
#define	SUMMARY	(Cb)
#define	LASTSEG	(0xFF)

typedef	struct	summseg_t	{	/* summary segment */
	unsigned char	seg_type;	/* SUMMARY */
	fsize_t		seg_size;	/* sizeof (seg_off) */
	fsize_t		seg_off[SUMMARY];
} summseg_t;

typedef	struct	segment_t	{	/* general segment fragment */
	unsigned char	seg_type;
	fsize_t		seg_size;
	unsigned char	seg_data[];
} segment_t;

/*
 * A relocation fragment contains relocation items.
 * Relocation type specifies the byte layout of the relocated word.
 * The name defined for each layout is intended to suggest the number
 * of bytes in the word, and their significance in increasing order
 * when read in address order.
 */
#define	B_0	0
#define	B_01	((0<<2)|1)
#define	B_10	((1<<2)|0)
#define	B_012	((0<<4)|(1<<2)|2)
#define	B_210	((2<<4)|(1<<2)|0)
#define	B_0123	((0<<6)|(1<<4)|(2<<2)|3)
#define	B_3210	((3<<6)|(2<<4)|(1<<2)|0)
#define	B_2301	((2<<6)|(3<<4)|(0<<2)|1)
#define	B_1032	((1<<6)|(0<<4)|(3<<2)|2)

/* relocation flags (in rel_seg) */
#define	PCREL	0x40
#define	RELFLAGS (SIGNEDt|PCREL)

typedef	struct	reloctn_t	{	/* relocation item */
	unsigned char	rel_type,	/* byte layout of relocated word */
			rel_seg;	/* segment referenced by relocator */
	uaddr_t		rel_addr;	/* address of relocation */
/*	uaddr_t		rel_num;	   symbol num if rel_seg is SYMBOLS */
					/* bias if rel_seg is ABSLUTE */
} reloctn_t;

/*
 * Symbol segment contains a symbol count, followed by that many symbols,
 * numbered from 0, not necessarily in order.
 */

typedef	union	symbol_t	{	/* object symbol */
	struct	{			/* relocatable */
	unsigned char	sym_scope;	/* module, segment, load or ref */
	uaddr_t		sym_num;	/* symbol number */
	char		sym_name[];	/* NUL terminated */
	unsigned char	sym_expr[];	/* absent if reference */
	char		sym_type[];	/* always present */
	};
	struct	{			/* bound or debug symbol */
	unsigned char	sym_seg;	/* seg type */
	uaddr_t		sym_addr;	/* value */
	char		sym_name[];	/* NUL terminated */
	char		sym_type[];	/* always present */
	};
} symbol_t;

typedef	struct	symseg_t	{	/* symbol segment */
	unsigned char	seg_type;
	fsize_t		seg_size;
	uaddr_t		seg_nsym;
	symbol_t	seg_sym[];
} symseg_t;

/* Symbol scope */
#define	RFSCOPE	0	/* reference (undefined) */
#define	LDSCOPE	1	/* whole load */
#define	SGSCOPE	2	/* single segment */
#define	MDSCOPE	3	/* single module */

/*
 * Symbol names are arbitrary length NUL ('\0') terminated character strings.
 */
/*
 * Symbol expression is a prefix polish representation of a binary tree
 * i.e. (opr) (left expr) (right expr).
 * No expression exists for undefined symbols (scope == RFSCOPE).
 */

typedef	struct	symleaf_t	{	/* expression tree leaf */
	unsigned char	seg_type;
	uaddr_t		sym_addr;	/* relocatable address */
} symleaf_t;
/* If seg_type is SYMBOLS sym_addr is symbol number */

/* Operators */
#define	X_OP	0x80			/* distinguish opr from seg */
#define	X_ADD	(X_OP+1)
#define	X_SUB	(X_OP+2)
#define	X_MUL	(X_OP+3)
#define	X_DIV	(X_OP+4)
#define	X_MOD	(X_OP+5)
#define	X_SHL	(X_OP+6)
#define	X_SRL	(X_OP+7)		/* shift right logical */
#define	X_SRA	(X_OP+8)		/* shift right arithmetic */
#define	X_MSK	(X_OP+9)		/* a msk b  <=>  a&~b */
#define	X_AND	(X_OP+10)
#define	X_XOR	(X_OP+11)
#define	X_IOR	(X_OP+12)

typedef	unsigned char	symopr_t;

/*
 * Symbol type is tree represented as a NUL terminated string of leaves
 * where a leaf is a character string, an n-byte signed or unsigned number,
 * a type string, a type symbol, or special leaf ANYt.
 * A character string is introduced by STRINGt, and is NUL terminated.
 * A number consists of a byte count (or'd with SIGNEDt if signed),
 * followed by that many bytes of data in order of increasing significance,
 * with the sign (if any) at the top bit of the last byte.
 * A type string is introduced by TYPEt, and is recursively defined as above.
 * A type symbol is SYMBOLt followed by a symbol number (canonical uaddr_t)
 * referring to the symbol segment in which the type was found.
 */
#define	ANYt	(-2)
#define	STRINGt	(-3)
#define	TYPEt	(-4)
#define	SYMBOLt	(-5)
#define	SIGNEDt	(-1<<7)

#endif
