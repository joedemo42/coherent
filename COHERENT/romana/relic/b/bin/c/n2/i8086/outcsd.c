/*
 * This file writes writes Intel iAPX-86 OMF with csd debugging information.
 * Based on outomf.c, but with MWC CSD-format debug info (as in outrel.c)
 * instead of Intel format.
 * The 'out...' routines write to a temporary file during code assembly.
 * At the end of assembly, 'copycode' uses the 'put...' routines
 * to write the object file.
 * This is necessary: SEGDEF records must be written before
 * LEDATA and FIXUPP records, but each SEGDEF includes the size
 * of the segment in bytes.
 * However, the size of the DEBUG segment is fixed up ex post facto.
 */

/*
 * Reference: "8086 Relocatable Object Formats,"
 * Intel 121748-001 (C) Intel 1981.
 */

#ifdef	vax
#include "INC$LIB:cc2.h"
#include "INC$LIB:debug.h"
#else
#include "cc2.h"
#include "debug.h"
#endif

#define	NTXT	(512+3)			/* Text buffer size */
#define	NREL	256			/* Relocation buffer size */

#ifndef __LINE__
#define	__LINE__	0
#endif

#if	!TINY
#define	dbrpt(x,y)	_dbrpt(x,y)
#else
#define	dbrpt(x,y)	/* _dbrpt(x,y) */
#endif

/*
 * Scratch file opcodes.
 * Flags are or'ed in to TBYTE and TWORD opcodes only.
 */
#define	TTYPE	0x07			/* Type */
#define	TPCR	0x08			/* PC rel flag */
#define	TSYM	0x10			/* Symbol based flag */

#define	TEND	0x00			/* End marker */
#define	TENTER	0x01			/* Enter new area */
#define	TBYTE	0x02			/* Byte data */
#define	TWORD	0x03			/* Word data */
#define	TCODE	0x04			/* Code segment base */
#define	TDATA	0x05			/* Data segment base */
#define	TBASE	0x06			/* External segment base */
#define	TDLAB	0x07			/* Debug label */
#define	TDLOC	0x08			/* Debug location */

#if	EMUFIXUPS
/*
 * If EMUFIXUPS is defined at compile time, the generated output writer
 * targets 8087 instructions with magic "M:..." fixups.
 * The linker can then create objects which use either 8087 hardware
 * or software floating point emulation; in the latter case, the
 * linker changes the 8087 instructions into traps to the emulator.
 */
#define	T8087	0xE0			/* 8087 flags */
#define	TWT	0x20			/* M:_WT  */
#define	TWST	0x40			/* M:_WST */
#define	TWES	0x60			/* M:_WES */
#define	TWCS	0x80			/* M:_WCS */
#define	TWSS	0xA0			/* M:_WSS */
#define	TWDS	0xC0			/* M:_WDS */
#endif

/*
 * OMF types.
 */
#define	THEADR	0x80			/* Translator module header */
#define	COMENT	0x88			/* Comment */
#define	MODEND	0x8A			/* Module end */
#define	EXTDEF	0x8C			/* External definition (reference) */
#define	TYPDEF	0x8E			/* Type definition */
#define	PUBDEF	0x90			/* Public definition (definition) */
#define	LNAMES	0x96			/* List of names */
#define	SEGDEF	0x98			/* Segment definition */
#define	GRPDEF	0x9A			/* Group definition */
#define	FIXUPP	0x9C			/* Fixups */
#define	LEDATA	0xA0			/* Enumerated data */
#define	LIDATA	0xA2			/* Iterated data */

/*
 * Some OMF fields and flags.
 */
#define	BYTE	(0x01<<5)		/* Byte alignment */
#define	PARA	(0x03<<5)		/* Paragraph alignment */
#define	CMEM	(0x01<<2)		/* C field, memory */
#define	CCON	(0x02<<2)		/* C field, concatenate */
#define	CSTACK	(0x05<<2)		/* C field, stack */

#define	LOCAT	0x80			/* Locat flag bit */
#define	M	0x40			/* M bit; 0=self rel, 1=seg rel */
#define	OFFSET	(0x01<<2)		/* Locat */
#define	BASE	(0x02<<2)		/* Locat */
#define	POINTER	(0x03<<2)		/* Locat */

#define	SECWAY	0x04			/* Secondary way */
#define	FD8087	0x36			/* FIXDAT for 8087 fixup */
#define	FSI	(0<<4)			/* Frame: segment index */
#define	FGI	(1<<4)			/* Frame: group index */
#define	FEI	(2<<4)			/* Frame: external index */
#define	FTARGET	(5<<4)			/* Frame: target */
#define	TSI	0			/* Target: segment index */
#define	TGI	1			/* Target: group index */
#define	TEI	2			/* Target: external index */
#define	TFN	3			/* Target: frame */
#define	F	0x80			/* Frame thread used */
#define	FT0	0x00			/* 0 */
#define	FT1	0x10			/* 1 */

/*
 * Structure to contain debug items on this pass.
 * Debug items are kept in a linked list rooted at 'dbase',
 * with 'dnext' pointing at the most recent item.
 */
typedef struct dbgt {
	struct dbgt	*d_dp;		/* Link */
	int		d_ref;		/* Index number */
	unsigned char	d_class;	/* Storage class */
	unsigned char	d_flag;		/* Flags */
	unsigned char	d_level;	/* Bracket level */
	unsigned char	d_seg;		/* Reloc-style segment */
	unsigned char	d_cseg;		/* C Segment */
	ADDRESS		d_value;	/* Offset in segment */
	int		d_size;		/* Size of d_data */
	char		d_data[];	/* Name and type */
} DBGT;
/* Flag bits. */
#define	D_GBL	 1
#define	D_LCL	 2
#define	D_TAG	 4
#define	D_NAM	 8
#define	D_LAB	16
#define	D_ENU	32

/*
 * Globals for output writer.
 */
static	int	islarge;		/* True iff isvariant(LARGE) */
static	int	isvcsd;			/* True iff -VCSD */
static	int	sx_code;		/* Segment index of SCODE */
static	int	sx_data;		/* Segment index of SDATA */
static	int	sx_debug;		/* Segment index of debug info */
static	char *	segtab;			/* Segment index conversion table */
static	char	txt[NTXT];		/* Text buffer */
static	char	rel[NREL];		/* Relocation buffer */
static	char *	txtp = txt;		/* Text pointer */
static	char *	relp = rel;		/* Relocation pointer */
static	DBGT *	dbase;			/* Root debug list item */
static	DBGT *	dnext;			/* Last debug list item */
static	int	level = 0;		/* Debug brace level */
static	int	dline = 0;		/* Most recent line number entry */

#if	EMUFIXUPS
static	int	has8087;		/* Has 8087 code flag */
static	SYM *	wsp[6];			/* Links to emulator symbols */
#endif

#if	VAXFMT
/*
 * The Intel cross software packs the OMF
 * data into 512 byte blocks as 510 byte variable length
 * records, with the last record short.
 */
#define	NVDATA	510			/* Blocksize - 2 */
static	char	vaxbuf[NVDATA];		/* Buffer */
static	char *	vaxptr = vaxbuf;	/* Pointer */
#endif

/*
 * Function declarations.
 */
extern	char *	calloc();
extern	long	ftell();
extern	DBGT *	getdbgt();
extern	char *	getmembs();
extern	char *	malloc();
extern	char *	movename();
extern	DBGT *	newdbgt();
extern	long	putsegdef();
extern	char *	setindex();
extern	char *	setsymid();
extern	char *	strcpy();
extern	char *	strncpy();

/*
 * Logical name indices.
 * These must correspond to the order defined by the LNAMES
 * record written by putlnames().
 */
#define	LN_EMPTY	1	/* LARGE or SMALL ""	*/
#define	LN_L_M_CODE	2	/* LARGE "module_CODE"	*/
#define	LN_L_M_DATA	3	/* LARGE "module_DATA"	*/
#define	LN_L_CODE	4	/* LARGE "CODE"		*/
#define	LN_L_DATA	5	/* LARGE "DATA"		*/
#define	LN_L_DEBUG	6	/* LARGE "DEBUG"	*/
#define	LN_L_M_DEBUG	7	/* LARGE "module_DEBUG"	*/
#define	LN_S_CODE	2	/* SMALL "CODE"		*/
#define	LN_S_DATA	3	/* SMALL "DATA"		*/
#define	LN_S_CONST	4	/* SMALL "CONST"	*/
#define	LN_S_STACK	5	/* SMALL "STACK"	*/
#define	LN_S_MEMORY	6	/* SMALL "MEMORY"	*/
#define	LN_S_CGROUP	7	/* SMALL "CGROUP"	*/
#define	LN_S_DGROUP	8	/* SMALL "DGROUP"	*/
#define	LN_S_DEBUG	9	/* SMALL "DEBUG"	*/
#define	LN_S_M_DEBUG	10	/* SMALL "module_DEBUG"	*/

/*
 * Segment indices.
 * These must correspond to the order of the SEGDEF records
 * written by copycode().
 */
#define	SX_L_M_CODE	1	/* LARGE module_CODE	*/
#define	SX_L_M_DATA	2	/* LARGE module_DATA	*/
#define	SX_L_M_DEBUG	3	/* LARGE module_DEBUG	*/
#define	SX_S_CODE	1	/* SMALL CODE		*/
#define	SX_S_CONST	2	/* SMALL CONST		*/
#define	SX_S_DATA	3	/* SMALL DATA		*/
#define	SX_S_STACK	4	/* SMALL STACK		*/
#define	SX_S_MEMORY	5	/* SMALL MEMORY		*/
#define	SX_S_M_DEBUG	6	/* SMALL module_DEBUG	*/

/*
 * Group indices, SMALL model only.
 * These must correspond to the order of the GRPDEF records
 * written by copycode().
 */
#define	GX_CGROUP	1	/* CGROUP */
#define	GX_DGROUP	2	/* DGROUP */

/* Type index for NIL type. */
#define	TYPENIL		1

/*
 * getsegindex() uses the following tables,
 * indexed by a compiler segment name (SCODE et al.)
 * to get an OMF segment index.
 * These tables understand the way the SSTRN segment moves
 * around when the ROM option is given.
 */
static	char	lnonseg[] = {
	SX_L_M_CODE,			/* SCODE => module_CODE */
	SX_L_M_CODE,			/* SLINK => module_CODE */
	SX_L_M_CODE,			/* SPURE => module_CODE */
	SX_L_M_DATA,			/* SSTRN => module_DATA */
	SX_L_M_DATA,			/* SDATA => module_DATA */
	SX_L_M_DATA			/* SBSS  => module_DATA */
};

static	char	lromseg[] = {
	SX_L_M_CODE,			/* SCODE => module_CODE */
	SX_L_M_CODE,			/* SLINK => module_CODE */
	SX_L_M_CODE,			/* SPURE => module_CODE */
	SX_L_M_CODE,			/* SSTRN => module_CODE */
	SX_L_M_DATA,			/* SDATA => module_DATA */
	SX_L_M_DATA			/* SBSS  => module_DATA */
};

static	char	lramseg[] = {
	SX_L_M_CODE,			/* SCODE => module_CODE */
	SX_L_M_CODE,			/* SLINK => module_CODE */
	SX_L_M_DATA,			/* SPURE => module_DATA */
	SX_L_M_DATA,			/* SSTRN => module_DATA */
	SX_L_M_DATA,			/* SDATA => module_DATA */
	SX_L_M_DATA			/* SBSS  => module_DATA */
};

static	char	snonseg[] = {
	SX_S_CODE,			/* SCODE => CODE */
	SX_S_CODE,			/* SLINK => CODE */
	SX_S_CONST,			/* SPURE => CONST */
	SX_S_DATA,			/* SSTRN => DATA */
	SX_S_DATA,			/* SDATA => DATA */
	SX_S_DATA			/* SBSS  => DATA */
};

static	char	sromseg[] = {
	SX_S_CODE,			/* SCODE => CODE */
	SX_S_CODE,			/* SLINK => CODE */
	SX_S_CONST,			/* SPURE => CONST */
	SX_S_CONST,			/* SSTRN => CONST */
	SX_S_DATA,			/* SDATA => DATA */
	SX_S_DATA			/* SBSS  => DATA */
};

static	char	sramseg[] = {
	SX_S_CODE,			/* SCODE => CODE */
	SX_S_CODE,			/* SLINK => CODE */
	SX_S_DATA,			/* SPURE => DATA */
	SX_S_DATA,			/* SSTRN => DATA */
	SX_S_DATA,			/* SDATA => DATA */
	SX_S_DATA			/* SBSS  => DATA */
};

/*
 * Same game, but for group index codes.
 * getgrpindex() uses this, in SMALL model only.
 */
static	char	grpindex[] = {
	GX_CGROUP,			/* SCODE => CGROUP */
	GX_CGROUP,			/* SLINK => CGROUP */
	GX_DGROUP,			/* SPURE => DGROUP */
	GX_DGROUP,			/* SSTRN => DGROUP */
	GX_DGROUP,			/* SDATA => DGROUP */
	GX_DGROUP			/* SBSS  => DGROUP */
};

/*
 * Some prefabricated OMF items.
 * These are just blasted out via calls to putrecord() in copycode().
 */
static	char	lthred[] = {
				0x40, SX_L_M_CODE,	/* frame0 -> module_CODE */
				0x41, SX_L_M_DATA	/* frame1 -> module_DATA */
			   };
static	char	gdef1[]  = {
				LN_S_CGROUP,
				0xFF, SX_S_CODE
			   };
static	char	gdef2[]  = {
				LN_S_DGROUP,
				0xFF, SX_S_CONST,
				0xFF, SX_S_DATA,
				0xFF, SX_S_STACK,
				0xFF, SX_S_MEMORY
			   };
static	char	sthred[] = {
				0x44, GX_CGROUP,	/* frame0 -> CGROUP */
				0x45, GX_DGROUP		/* frame1 -> DGROUP */
			   };
static	char	typdef[] = {	0, 0, 0x80 };		/* TYPDEF [1] NIL */
static	char	modend[] = {	0x00 };

/*
** First pass routines.
** Output items to the temporary file.
*/
/*
 * Initialize the code writer.
 */
outinit()
{
#if	MONOLITHIC
#if	EMUFIXUPS
	register int	i;

	has8087 = 0;
	for (i=0; i<6; ++i)
		wsp[i] = NULL;
#endif
	txtp = &txt[0];
	relp = &rel[0];
	dbase = dnext = NULL;
#if	VAXFMT
	vaxptr = &vaxbuf[0];
#endif
#endif
	/* Initialize global data which depends on variant. */
	islarge = isvariant(VLARGE);
	isvcsd = isvariant(VTYPES) || isvariant(VDSYMB);
	if (islarge) {
		sx_code  = SX_L_M_CODE;
		sx_data  = SX_L_M_DATA;
		sx_debug = SX_L_M_DEBUG;
		segtab =   isvariant(VROM) ? lromseg
			: notvariant(VRAM) ? lnonseg
					   : lramseg;
	} else {
		sx_code  = SX_S_CODE;
		sx_data  = SX_S_DATA;
		sx_debug = SX_S_M_DEBUG;
		segtab =   isvariant(VROM) ? sromseg
			: notvariant(VRAM) ? snonseg
					   : sramseg;
	}
}

/*
 * Output a segment switch.
 */
outseg(s)
{
	bput(TENTER);
	bput(s);
}

/*
 * Output an absolute byte.
 */
outab(b)
{
	bput(TBYTE);
	bput(b);
	++dot;
}

/*
 * Output an absolute word.
 */
outaw(w)
{
	bput(TWORD);
	iput(w);
	dot += 2;
}

/*
 * Output a full byte.
 */
outxb(sp, b, pcrf)
register SYM *sp;
{
	register opcode;

	opcode = TBYTE;
	if (sp != NULL)
		opcode |= TSYM;
	if (pcrf)
		opcode |= TPCR;
	bput(opcode);
	bput(b);
	if (sp != NULL)
		pput(sp);
	++dot;
}

/*
 * Output a full word.
 */
outxw(sp, w, pcrf)
register SYM *sp;
{
	register opcode;

	opcode = TWORD;
	if (sp != NULL)
		opcode |= TSYM;
	if (pcrf)
		opcode |= TPCR;
	bput(opcode);
	iput(w);
	if (sp != NULL)
		pput(sp);
	dot += 2;
}

#if	EMUFIXUPS
/*
 * Output a one byte opcode for the 8087.
 */
outfb(b)
{
	has8087 = 1;
	bput(TBYTE|TWT);
	bput(b);
	++dot;
}

/*
 * Output a two byte opcode for the 8087.
 */
outfw(w, prefix)
{
	has8087 = 1;
	bput(TWORD|((prefix>>3)+TWST));
	iput(w);
	dot += 2;
}
#endif

/*
 * Output a 1 word object containing
 * the base address of the external symbol pointed to by 'sp'.
 */
outsb(sp)
SYM *sp;
{
	bput(TBASE);
	pput(sp);
	dot += 2;
}

/*
 * Copy a dlabel record from ifp to ofp.
 * Flag the symbol table entry if appropriate.
 * Recursive for DX_MEMBS member lists.
 */
outdlab(l,class)
{
	register int val;
	register SYM *sp;
	int n;

	if (l == 0)
		bput(TDLAB);
	bput(class);

	/* Get line number. */
	iput(iget());

	/* Get value */
	if (class < DC_AUTO)
		;
	else if (class < DC_MOS)
		iput(val = iget());
	else {
		bput(bget());
		bput(bget());
		iput(iget());
	}

	/* Get name */
	sget(id, NCSYMB);

	/* Check for symbol table entry */
	if (class==DC_GDEF || class==DC_SEX || class==DC_GREF) {
		sp = glookup(id, 0);
		sp->s_flag |= S_PUT;
	} else if (class==DC_SIN) {
		sp = llookup(val, 0);
		sp->s_flag |= S_PUT;
	}

	/* Put name */
	sput(id);

	/* Get type */
	for (;;) {
		switch (bput(bget())) {
		case DT_NONE:
		case DT_CHAR:
		case DT_UCHAR:
		case DT_SHORT:
		case DT_USHORT:
		case DT_INT:
		case DT_UINT:
		case DT_LONG:
		case DT_ULONG:
		case DT_FLOAT:
		case DT_DOUBLE:
		case DT_VOID:
			iput(iget());
			break;
		case DT_STRUCT:
		case DT_UNION:
		case DT_ENUM:
		case DD_PTR:
		case DD_FUNC:
		case DD_ARRAY:
			iput(iget());
			continue;
			break;
		case DX_MEMBS:
			bput(n = iget());
			for ( ; n > 0; n-=1) {
				outdlab(1, bget());
			}
			break;
		case DX_NAME:
			iget();
			sget(id, NCSYMB);
			sput(id);
			break;
		default:
			cbotch("outdlab: %d", n);
		}
		break;
	}
}

/*
 * Output a debug relocation record.
 * This 'n' is an index into the list of debug records written.
 */
outdloc(n)
{
	bput(TDLOC);
	iput(n);
}

/*
 * Finish up the code.
 */
outdone()
{
	if (isvariant(VASM))
		return;

#if	EMUFIXUPS
	/*
	 * Put magic names into the symbol table
	 * if any 8087 code was generated.
	 */
	if (has8087) {
		wsp[0] = glookup("M:_WT",  0);
		wsp[1] = glookup("M:_WST", 0);
		wsp[2] = glookup("M:_WES", 0);
		wsp[3] = glookup("M:_WCS", 0);
		wsp[4] = glookup("M:_WSS", 0);
		wsp[5] = glookup("M:_WDS", 0);
	}
#endif
	bput(TEND);
}

/*
** Second pass routines.
** Read the temporary file and write the output.
*/
/*
 * Finish up.
 * Figure out the sizes and final values of everything.
 * Copy the code from the scratch file back to the output file.
 */
copycode()
{
	register int	op;
	register SYM	*sp;
	register int	i;
	int		fixup;
	int		nd;
	int		data;
	long		debugdef;
	ADDRESS		code_size;
	ADDRESS		data_size;
	ADDRESS		const_size;
	ADDRESS		bss_size;

	/*
	 * Assign base addresses to the compiler's logical segments, based on
	 * the compilation model and the settings of the VROM and VRAM flags.
	 */
	dot = 0;
	newseg(SCODE);
	newseg(SLINK);
	if (!islarge || isvariant(VRAM)) {
		code_size = dot;
		dot = 0;
	}
	newseg(SPURE);
	if (notvariant(VROM) && notvariant(VRAM)) {
		if (islarge)
			code_size  = dot;
		else
			const_size = dot;
		dot = 0;
	}
	newseg(SSTRN);
	if (isvariant(VROM)) {
		if (islarge)
			code_size  = dot;
		else
			const_size = dot;
		dot = 0;
	}
	newseg(SDATA);
	newseg(SBSS);
	bss_size  = seg[SBSS].s_dot;
	data_size = dot;

	/*
	 * Put out the preamble.
	 */
	putrecord(THEADR, txt, movename(txt, module, 0) - txt);	/* THEADR */
	txt[0] = txt[1] = '\0';
	sprintf(&txt[2], "%s %s %s %s",
#ifdef vax
		"VAX",
#else
#ifdef UDI
		"UDI",
#else
#ifdef MSDOS
		"MS-DOS",
#else
		"COHERENT",
#endif
#endif
#endif
		"MWC86", VERSMWC,
		(islarge) ? "LARGE" : "SMALL");
	putrecord(COMENT, txt, strlen(&txt[2])+2);	/* COMENT */
	putlnames();					/* LNAMES */
	/*
	 * The order of putsegdef() calls below must correspond to
 	 * the definitions of SX_... macros above.
	 * Debug segment sizes are fixed up ex post facto.
	 */
	if (islarge) {
/* [1] */	putsegdef(PARA|CCON, code_size, LN_L_M_CODE, LN_L_CODE);
/* [2] */	putsegdef(PARA|CCON, data_size, LN_L_M_DATA, LN_L_DATA);
		if (isvcsd)
/* [3] */		debugdef = putsegdef(BYTE|CCON, 0, LN_L_M_DEBUG, LN_L_DEBUG);
		putrecord(FIXUPP, lthred, sizeof(lthred));	/* Threads */
	} else {
/* [1] */	putsegdef(BYTE|CCON, code_size,  LN_S_CODE,   LN_S_CODE);
/* [2] */	putsegdef(BYTE|CCON, const_size, LN_S_CONST,  LN_S_CONST);
/* [3] */	putsegdef(BYTE|CCON, data_size,  LN_S_DATA,   LN_S_DATA);
/* [4] */	putsegdef(PARA|CSTACK,       0,  LN_S_STACK,  LN_S_STACK);
/* [5] */	putsegdef(PARA|CMEM,         0,  LN_S_MEMORY, LN_S_MEMORY);
		if (isvcsd)
/* [6] */		debugdef = putsegdef(BYTE|CCON, 0, LN_S_M_DEBUG, LN_S_DEBUG);
		/*
		 * The order of putrecord() calls below must correspond to
		 * the definitions of GX_... macros above.
		 */
/* [1] */	putrecord(GRPDEF, gdef1, sizeof(gdef1));
/* [2] */	putrecord(GRPDEF, gdef2, sizeof(gdef2));
		putrecord(FIXUPP, sthred, sizeof(sthred));	/* Threads */
	}
	putrecord(TYPDEF, typdef, sizeof(typdef));	/* TYPDEF */
	putsymdef();					/* PUBDEF, EXTDEF */

	/*
	 * Copy out code.
	 */
	for (i=0; i<NSEG; ++i)
		seg[i].s_dot = seg[i].s_mseek;
	dotseg = SCODE;
	dot = seg[SCODE].s_dot;
	while ((op = bget()) != TEND) {
		switch (op) {

		case TENTER:
			notenuf();
			enter(bget());
			continue;
		case TDLAB:
			getdlab();
			continue;
		case TDLOC:
			getdloc();
			continue;
		case TBASE:
			if (!islarge)
				cbotch("base");
			sp = pget();
			enuf(2, 5);
			fixup = txtp - (txt+3);
			*txtp++ = 0;
			*txtp++ = 0;
			dot += 2;
			*relp++ = LOCAT|M|BASE|(fixup>>8);
			*relp++ = fixup;
			if ((sp->s_flag&S_DEF) != 0) {
				*relp++ = FTARGET|SECWAY|TSI;
				*relp++ = getsegindex(sp->s_seg);
			} else {
				*relp++ = FTARGET|SECWAY|TEI;
				relp = setindex(relp, sp->s_ref);
			}
			continue;
		}
		if ((op&TTYPE) == TBYTE) {
			nd = 1;
			data = bget();
		} else {
			nd = 2;
			data = iget();
		}
		/* Assure enough space for text and relocation written below. */
#if	EMUFIXUPS
		enuf(nd, 7);
#else
		enuf(nd, 6);
#endif
		dot += nd;
		fixup = (txtp - (txt+3)) | (LOCAT << 8);

#if	EMUFIXUPS
		/*
		 * 8087 opcodes.
		 * Apply magic relocation for the emulator.
		 */
		if ((op&T8087) != 0) {
			sp = wsp[((op&T8087)-TWT)>>5];
			*txtp++ = data;
			if (nd != 1) {
				*txtp++ = data >> 8;
				fixup |= OFFSET << 8;	/* Locat */
			}
			fixup |= M << 8;		/* Seg rel */
			*relp++ = fixup>>8;
			*relp++ = fixup;
			*relp++ = FD8087;		/* Symbol based */
			*relp++ = 0;			/* Frame # */
			*relp++ = 0;
			relp = setindex(relp, sp->s_ref);
			continue;
		}
#endif
		/*
		 * Absolute.
		 */
		if ((op&(TSYM|TPCR)) == 0) {
			*txtp++ = data;
			if (nd != 1)
				*txtp++ = data >> 8;
			continue;
		}
		/*
		 * Absolute PC rel.
		 */
		if ((op&TSYM) == 0) {
			*txtp++ = 0;
			if (nd != 1) {
				*txtp++ = 0;
				fixup |= OFFSET << 8;
			}
			*relp++ = fixup>>8;
			*relp++ = fixup;
			*relp++ = FTARGET|TFN;		/* Absolute frame */
			*relp++ = 0;			/* Frame number */
			*relp++ = data;			/* Disp */
			*relp++ = data >> 8;
			continue;
		}
		/*
		 * Symbol based.
		 * I would like to always do this
		 * in the primary way. However, LINK-86
		 * doesn't do 3 byte offsets, so I cannot
		 * do a negative one.
		 */
		sp = pget();
		if ((sp->s_flag&S_DEF) != 0) {
			data += sp->s_value;
			/* Absolute */
			if ((op&TPCR)!=0 && sp->s_seg==dotseg) {
				data -= dot;
				*txtp++ = data;
				if (nd != 1)
					*txtp++ = data >> 8;
				continue;
			}
			/* Segment relative */
			*txtp++ = 0;
			if (nd != 1) {
				*txtp++ = 0;
				fixup |= OFFSET << 8;	/* Loc */
			}
			if ((op&TPCR) == 0)
				fixup |= M << 8;	/* Seg rel */
			*relp++ = fixup>>8;
			*relp++ = fixup;
			if ((i = getsegindex(sp->s_seg)) == sx_code)
				*relp++ = F|FT0|TSI;	/* F code, seg ind */
			else
				*relp++ = F|FT1|TSI;	/* F data */
			*relp++ = i;
			*relp++ = data;			/* Disp */
			*relp++ = data >> 8;
			continue;
		}
		/*
		 * Symbol relative.
		 */
		*txtp++ = data;
		if (nd != 1) {
			*txtp++ = data >> 8;
			fixup |= OFFSET << 8;		/* Loc */
		}
		if ((op&TPCR) == 0)
			fixup |= M << 8;		/* Seg rel */
		*relp++ = fixup>>8;
		*relp++ = fixup;
		*relp++ = FTARGET|SECWAY|TEI;		/* Fix dat */
		relp = setindex(relp, sp->s_ref);
	}
	if (isvariant(VLINES))
		outbrace('}');
	if (notvariant(VLIB))
		dump();
	if (isvcsd)
		oglobals();				/* Dump globals */
	enter(SDATA);					/* flush */
	/*
	 * If the size of the SBSS segment is nonzero,
	 * use an LIDATA item to get it zeroed when the program is run.
	 */
	if (bss_size != 0) {
		txtp = &txt[0];
		*txtp++ = sx_data;
		*txtp++ = seg[SBSS].s_mseek;		/* Offset */
		*txtp++ = seg[SBSS].s_mseek >> 8;
		*txtp++ = bss_size;			/* Repeat count */
		*txtp++ = bss_size >> 8;
		*txtp++ = 0;				/* Block count */
		*txtp++ = 0;
		*txtp++ = 1;				/* Count */
		*txtp++ = 0;				/* Data */
		putrecord(LIDATA, txt, 9);		/* LIDATA */
	}
	putrecord(MODEND, modend, sizeof(modend));	/* MODEND */
#if	VAXFMT
	vaxflush();
#endif
	if (isvcsd) {
		if (fseek(ofp, debugdef, 0) == -1)
			cbotch("fixsegdef seek failed");
		if (islarge)
			putsegdef(BYTE|CCON, seg[SDBG].s_dot, LN_L_M_DEBUG, LN_L_DEBUG);
		else
			putsegdef(BYTE|CCON, seg[SDBG].s_dot, LN_S_M_DEBUG, LN_S_DEBUG);
	}
}

/*
 * Assign a base address to logical segment s.
 * Round up the segment size and bump dot.
 */
newseg(s)
register s;
{
	register ADDRESS n;

	if (((n = seg[s].s_dot)&1) == 1)
		seg[s].s_dot = ++n;	/* round up segment size to word */
	seg[s].s_mseek = dot;
	dot += n;
}

/*
** Header writing routines.
*/
/*
 * Write the list of names record.
 * The LN_... macros defined above must correspond to the order defined here.
 * For SMALL model, the names are:
 *	[1]  ""
 *	[2]  "CODE"
 *	[3]  "DATA"
 *	[4]  "CONST"
 *	[5]  "STACK"
 *	[6]  "MEMORY"
 *	[7]  "CGROUP"
 *	[8]  "DGROUP"
 *	[9]  "DEBUG"		(if -VCSD)
 *	[10] "module_DEBUG"	(if -VCSD)
 * For LARGE model, the names are:
 *	[1]  ""
 *	[2]  "module_CODE"
 *	[3]  "module_DATA"
 *	[4]  "CODE"
 *	[5]  "DATA"
 *	[6]  "DEBUG"		(if -VCSD)
 *	[7]  "module_DEBUG"	(if -VCSD)
 */
putlnames()
{
	register char *p1;

	p1 = &txt[0];
	*p1++ = 0;					/* "" */
	if (islarge) {
		p1 = movename(p1, "CODE", 1);
		p1 = movename(p1, "DATA", 1);
		p1 = movename(p1, "CODE", 0);
		p1 = movename(p1, "DATA", 0);
	} else {
		p1 = movename(p1, "CODE",   0);
		p1 = movename(p1, "DATA",   0);
		p1 = movename(p1, "CONST",  0);
		p1 = movename(p1, "STACK",  0);
		p1 = movename(p1, "MEMORY", 0);
		p1 = movename(p1, "CGROUP", 0);
		p1 = movename(p1, "DGROUP", 0);
	}
	if (isvcsd) {
		p1 = movename(p1, "DEBUG", 0);
		p1 = movename(p1, "DEBUG", 1);
	}
	putrecord(LNAMES, txt, p1-txt);
}

/*
 * If flag==0, move p2 to the supplied buffer p1.
 * If flag==1, move module_p2 to p1.
 */
char *
movename(p1, p2, flag)
register char *p1;
char *p2;
{
	register int n, m;

	n = strlen(p2);
	if (flag) {
		m = strlen(module);
		*p1++ = m + 1 + n;		/* length of "module_p2" */
		strcpy(p1, module);		/* copy module name */
		p1 += m;
		*p1++ = '_';			/* append '_' */
	} else
		*p1++ = n;			/* length of p2 */
	strcpy(p1, p2);				/* copy p2 */
	return(p1 + n);
}

/*
 * This routine formats and outputs a SEGDEF item.
 * There is no checking for BIG segments (segments that are 64k in size).
 * The overlay index points to a null name.
 * Return the location of the first byte of the record.
 */
long
putsegdef(flag, size, seg, class)
{
	long l;
	register char *bp;

	l = ftell(ofp);
	bp = &txt[0];
	*bp++ = flag;
	*bp++ = size;
	*bp++ = size >> 8;
	*bp++ = seg;
	*bp++ = class;
	*bp = LN_EMPTY;
	putrecord(SEGDEF, txt, 6);
	return(l);
}

/*
 * Sweep through the symbol table:
 *	1) Make symbols LSEG relative.
 *	2) Assign reference numbers.
 *	3) Output EXTDEF or PUBDEF.
 */
putsymdef()
{
	register SYM *sp;
	register int refnum;
	register int i;

	refnum = 1;
	for (i=0; i<NSHASH; ++i) {
		for (sp = hash2[i]; sp != NULL; sp = sp->s_fp ) {
			if ((sp->s_flag&S_DEF) != 0)
				sp->s_value += seg[sp->s_seg].s_mseek;
			if ((sp->s_flag&S_GBL)!=0 || (sp->s_flag&S_DEF)==0) {
				sp->s_ref = 0;
				if ((sp->s_flag&S_DEF) != 0) {
					putpubdef(sp);
				} else {
					sp->s_ref = refnum++;
					putextdef(sp);
				}
			}
		}
	}
}

/*
 * Put out a PUBDEF item for symbol 'sp'.
 * All defs are tagged with the group
 * so that a FTARGET,EI fixup can determine the correct frame.
 */
putpubdef(sp)
register SYM *sp;
{
	register char *bp, *cp;

	cp = bp = &txt[0];
	*bp++ = getgrpindex(sp->s_seg);
	*bp++ = getsegindex(sp->s_seg);
	bp = setsymid(bp, sp->s_id);
	*bp++ = sp->s_value;
	*bp++ = sp->s_value >> 8;
	*bp++ = TYPENIL;
	putrecord(PUBDEF, cp, bp-cp);
}

/*
 * Put out an EXTDEF item for symbol 'sp'.
 */
putextdef(sp)
SYM *sp;
{
	register char *bp, *cp;

	cp = &txt[0];
	bp = setsymid(cp, sp->s_id);
	*bp++ = TYPENIL;
	putrecord(EXTDEF, cp, bp-cp);
}

/*
 * Translate compiler segment codes
 * to OMF segment index value, using tables.
 * The table is selected by the model and the
 * setting of the VROM and VRAM variants.
 */
getsegindex(s)
register int	s;
{
	if (s > SBSS) {
		if (s == SDBG)
			return(sx_debug);
		else
			cbotch("getsegindex, seg=%d", s);
	}
	return(segtab[s]);
}

/*
 * Like getsegindex(), but returns a reloc-type segment name for CSD.
 */
segindex(s) register int s;
{
	register int seg;

	if ((seg = getsegindex(s)) == sx_code)
		return(PURCODE);
	else if (seg == sx_data || (!islarge && seg == SX_S_CONST))
		return(PURDATA);
	cbotch("segindex, s=%x, seg=%x\n", s, seg);
}
	
/*
 * Translate compiler segment codes
 * to OMF group index value, using table and model.
 */
getgrpindex(s)
register int s;
{
 	return (islarge ? 0 : grpindex[s]);
}

/*
** Debug information processors.
*/
/*
 * Read debug table item(s) into memory.
 */
getdlab()
{
	register DBGT *dp;
	static int refnum = 0;

	dp = getdbgt();

	/* This should be done in cc0 */

	if (isvariant(VLIB)) {
		switch (dp->d_class) {
			case DC_TYPE:
			case DC_STAG:
			case DC_UTAG:
			case DC_ETAG:
				return;
		}
	}
	if (dp->d_class == DC_LINE || dp->d_class == DC_LAB)
		dp->d_ref = refnum;

	refnum += 1; 
	if (dbase  == NULL)
		dbase = dp;
	if (dnext != NULL)
		dnext->d_dp = dp;
	dbrpt("enter", dp);
	while (dp->d_dp != NULL) {
		dp = dp->d_dp;
		dbrpt("enter", dp);
	}
	dnext = dp;
	if (dp->d_class == DC_FILE)
		outbrace('{');	
}

/*
 * Read in debug items translating into loader type syntax.
 * Member definitions are fetched recursively.
 * If a tag or type definition, the member definitions are appended to the
 * node surrounded by "{" and "}" nodes.
 * If not a tag definition, the member definitions are discarded.
 * In either case, the member types are appended to the type of the
 * parent node.
 */
#define	DSZ 512
DBGT *
getdbgt()
{
	register int n;
	register char *dptr;
	DBGT d;
	char data[DSZ];
	int value, width, offs;
	int typet, subt, membs;
	char *sdp;

	d.d_dp = NULL;
	d.d_ref = -1;
	d.d_flag = D_LCL;
	d.d_level = level;
	d.d_seg = LASTSEG;
	d.d_cseg = 0;
	d.d_value = 0;
	d.d_size = 0;
	width = membs = 0;

	/*
	 * Loop to read entire record:
	 *	class, optional value, name, type.
	 */
	dptr = data;
	for (typet = 0; ; typet += 1) {

		if (dptr >= data+DSZ) {
			dbrpt("1", &d);
			cbotch("getdbgt buffer");
		}

		n = bget();
		if (n < DX_MEMBS)
			value = iget() * NBPBYTE;
		else if (n < DC_SEX)
			;
		else {
			dline = iget();
			if (n < DC_AUTO)
				;
			else if (n < DC_MOS)
				value = iget();
			else {
				width = bget();
				offs = bget();
				value = iget();
			}
		}
		subt = 0;

		switch (n) {

		/*
		 * Classes: fetch values and set flags.
		 */
		case DC_SEX:
			d.d_flag |= D_NAM;
			goto setclass;

		case DC_GDEF:
		case DC_GREF:
			d.d_flag = D_GBL|D_NAM;
			goto setclass;

		case DC_ETAG:
			d.d_flag |= D_ENU;
			strcpy(dptr, "enum ");
			dptr += 5;
			goto tag;
		case DC_STAG:
			strcpy(dptr, "struct ");
			dptr += 7;
			goto tag;
		case DC_UTAG:
			strcpy(dptr, "union ");
			dptr += 6;
		case DC_TYPE:
			tag:
				d.d_flag = (level == 0) ? D_TAG|D_GBL
							: D_TAG|D_LCL;
				d.d_seg = TYPESYM;
				goto setclass;

		case DC_FILE:
			d.d_seg = PURCODE;
			goto setclass;

		case DC_AUTO:
			d.d_seg = L_STACK;
			d.d_value = value;
			goto setclass;

		case DC_SIN:
			d.d_flag |= D_LAB;
			goto setclass;

		case DC_LAB:
			goto setclass;

		case DC_REG:
			d.d_seg = L_REG;
			d.d_value = value;
			goto setclass;

		case DC_PREG:
			if (level == 0) {
				level++;
				outbrace('{');
			}
			d.d_seg = L_REG;
			d.d_value = value;
			goto setclass;

		case DC_MOE:
			d.d_seg = ABSLUTE;
			d.d_value = value;
			goto setclass;

		case DC_PAUTO:
 			if (level  == 0) {
				level++;
				outbrace('{');
			}
			d.d_seg = P_STACK;
			d.d_value = value;
			goto setclass;

		case DC_LINE:
			goto setclass;

		case DC_MOS:
		case DC_MOU:
			d.d_seg = FLD_OFF;
			d.d_value = value * NBPBYTE + offs;
			goto setclass;

		/*
		 * Set the class field, read the name;
		 * Lookup globals and statics.
		 */
		setclass:
			d.d_class = n;
			while (*dptr++ = bget())
				;
			if ((d.d_flag&(D_NAM|D_LAB)) != 0) {
				register SYM *sp;

				if ((d.d_flag&D_NAM) != 0)
					sp = glookup(data, 0);
				else
					sp = llookup(value, 0);
				if ((sp->s_flag&S_DEF) != 0) {
					d.d_cseg = sp->s_seg;
					d.d_seg = segindex(sp->s_seg);
					d.d_value = sp->s_value;
				}
#if	COMMON
				else if (sp->s_value != 0) {
					d.d_seg = BLDATA|Cb;
					d.d_value = sp->s_value;
				}
#endif
				if ((d.d_flag&D_GBL) != 0)
					d.d_ref = sp->s_ref;
			}
			continue;

		/*
		 * Types: write type and prepare for trailing data.
		 * For files and lines, write a phony type.
		 */
		case DT_NONE:
			if (d.d_class == DC_FILE) {
				register char *p;

				*dptr++ = STRINGt;
				p = "source file";
				while (*dptr++ = *p++)
					;
			}
			if (d.d_class == DC_LINE) {
				*dptr++ = 6;	/* Line */
				*dptr++ = dline;
				*dptr++ = dline >> 8;
				*dptr++ = 0;	/* Column */
				*dptr++ = 0;	/* Flag */
				*dptr++ = 0;	/* Saved instruction */
				*dptr++ = 0;
			}
			break;

		case DT_CHAR:
		case DT_SHORT:
		case DT_INT:
		case DT_LONG:
			n = Integer;
			goto getwidth;

		case DT_UCHAR:
		case DT_USHORT:
		case DT_UINT:
		case DT_ULONG:
			n = Natural;
			goto getwidth;

		case DT_FLOAT:
		case DT_DOUBLE:
			n = Rational;
			goto settype;

		case DT_VOID:
			n = Void;
			goto settype;

		case DT_STRUCT:
			membs = n;
			n = Record;
			sdp = dptr;
			goto settype;

		case DT_UNION:
			membs = n;
			n = Union;
			sdp = dptr;
			goto settype;

		case DT_ENUM:
			d.d_flag |= D_ENU;
			membs = n;
			n = Natural;
			sdp = dptr;
			goto settype;

		getwidth:
			if (width)
				value = width;

		settype:
			*dptr++ = 1;
			*dptr++ = n;
			if (n != Void) {
				if (value == 0)
					*dptr++ = ANYt;
				else if (value <= 0377) {
					*dptr++ = 1;
					*dptr++ = value;
				} else {
					*dptr++ = 2;
					*dptr++ = value;
					*dptr++ = value >> 8;
				}
			}
			if (subt) {
				*dptr++ = TYPEt;
				continue;
			}
			if (membs) {
				--typet;
				continue;
			}
			break;

		/*
		 * Dimensions: as for types, but flag for subtypes.
		 */
		case DD_PTR:
			n = Pointer;
			++subt;
			goto settype;

		case DD_FUNC:
			n = Proc;
			++subt;
			goto settype;

		case DD_ARRAY:
			n = Array;
			++subt;
			goto settype;

		/*
		 * Trailing struct/union/enum data.
		 */
		case DX_NAME:
			dptr = sdp;
			*dptr++ = STRINGt;
			switch (membs) {
				case DT_STRUCT:
					strcpy(dptr, "struct ");
					dptr += 7;
					break;
				case DT_UNION:
					strcpy(dptr, "union ");
					dptr += 6;
					break;
				case DT_ENUM:
					strcpy(dptr, "enum ");
					dptr += 5;
			}
			while (*dptr++ = bget())
				;
			membs = 0;
			break;

		case DX_MEMBS:
			level += 1;
			dptr = getmembs(&d, data, dptr);
			level -= 1;
			membs = 0;
			break;

		default:
			cbotch("getdbgt: %d", n);
		}
		/*
		 * Terminate types.
		 */
		if (dptr + typet >= data + DSZ) {
			dbrpt("2", &d);
			cbotch("getdbgt buffer");
		}
		while (--typet >= 0)
			*dptr++ = 0;
		break;
	}

	/*
	 * Check level change.
	 */
	if (data[0] == '{') {
		if (level++ == 0) {
			d.d_level = level++;
			outbrace('{');
		}
	}
	else if (data[0] == '}')
		level -= 1;

	/*
	 * Copy into dynamic storage.
	 */
	d.d_size = dptr - data;
	return (newdbgt(&d, data));
}

/*
 * Enter a new output segment.
 */
enter(newseg)
register int newseg;
{
	if (dotseg == newseg)
		return;			/* already in newseg */
	notenuf();			/* flush */
	seg[dotseg].s_dot = dot;	/* save current dot */
	dotseg = newseg;		/* reset dotseg */
	dot = seg[dotseg].s_dot;	/* and dot */
}

/*
 * Read a debug relocation item and patch current 'dot'
 * into the appropriate debug/symbol table item.
 * If the debug item is '}' and level 2, then dump the batch of locals.
 * If the debug item is '}' and level 1, then dump the type definitions.
 */
getdloc()
{
	register DBGT *dp;
	register int ref;

	ref = iget();
	for (dp = dbase; ; dp = dp->d_dp) {
		if (dp == NULL)
			cbotch("getdloc: %d", ref);
		if ((dp->d_flag&D_GBL) != 0)
			continue;
		if (dp->d_ref == ref)
			break;
	}
	dp->d_cseg = dotseg;
	dp->d_seg = segindex(dotseg);
	dp->d_value = dot;

	dbrpt("reloc", dp);
	if (dp->d_data[0] == '}' && dp->d_level <= 2) {
		outbrace('}');
 		level--;
		dump();
	}
}

/*
 * Dump the locals and type definitions.
 */
dump()
{
	register DBGT *dp, **dpp;
	int saveseg;

	saveseg = dotseg;
	enter(SDBG);
	dpp = &dbase;
	dnext = NULL;
	while ((dp = *dpp) != NULL) {
		if ((dp->d_flag&D_LCL) != 0) {
			dbrpt("write", dp);
			osymbol(dp, -1);
			dp->d_flag &= ~D_LCL;
		}
		if ((dp->d_flag&D_GBL) == 0) {
			dbrpt("free", dp);
			*dpp = dp->d_dp;
			free(dp);
		} else {
			dbrpt("save", dp);
			dnext = dp;
			dpp = &dp->d_dp;
		}
	}
	enter(saveseg);
}

/*
 * Write out global symbol definitions and references.
 */
oglobals()
{
	register DBGT *dp, *dp1;

	enter(SDBG);
	for (dp = dbase; dp != NULL; dp = dp1) {
		dp1 = dp->d_dp;
		dbrpt("write", dp);
		if (dp->d_seg == TYPESYM || dp->d_seg == ABSLUTE)
			osymbol(dp, 0);
		else
			osymbol(dp, dp->d_ref);
		dbrpt("free", dp);
		free(dp);
	}
	notenuf();
}

/*
 * Write out a symbol to the symbol or debug table.
 * TYPESYM and ABSLUTE symbols have refnum 0.
 * Debug table symbols have negative refnums and may require relocation.
 */
osymbol(dp, refnum)
register DBGT *dp;
{
	register char *cp;
	unsigned char seg;
	int nd, nr, fixup, fixseg;

	/* Write scope/refnum or seg/value+relocation expression */
	seg = dp->d_seg;
	nr = 0;
#if	0
printf("osymbol(%s, %x) seg=%x cseg=%x d_size=%x\n",
dp->d_data, refnum, seg, dp->d_cseg, dp->d_size);
#endif
	if (refnum >= 0) {
		if (seg == LASTSEG) {
			for (cp=dp->d_data; *cp++; )
				;		/* skip the name */
			seg = (*cp++ == 1 && *cp++ == Proc) ? PURCODE : PURDATA;
			fixseg = -1;		/* external fixup */
		} else
			fixseg = getsegindex(dp->d_cseg);
		if (seg != TYPESYM && seg != ABSLUTE)
			nr = islarge ? 10 : 6;
	} else if (refnum < 0) {
		if (seg < SUMMARY && seg != TYPESYM && seg != ABSLUTE) {
			nr = islarge ? 10 : 6;
			fixseg = getsegindex(dp->d_cseg);
		}
		else
	 		fixseg = sx_code;
	}
	enuf(5, nr);
	dot += 5;			/* bump dot */
	*txtp++ = seg;			/* reloc-style segment */
	if (fixseg == -1) {		/* extern fixup */
		if (islarge) {
			fixup = txtp - (txt+3);
			*relp++ = LOCAT|M|BASE|(fixup>>8);
			*relp++ = fixup;
			*relp++ = FTARGET|SECWAY|TEI;
			relp = setindex(relp, refnum);
		}
		*txtp++ = 0;
		*txtp++ = 0;
		fixup = txtp - (txt+3);
		*relp++ = LOCAT|M|OFFSET|(fixup>>8);
		*relp++ = fixup;
		*relp++ = FTARGET|SECWAY|TEI;
		relp = setindex(relp, refnum);
		*txtp++ = 0;
		*txtp++ = 0;
	} else {
		if (nr != 0 && islarge) {	/* segment fixup */
			fixup = txtp - (txt+3);
			*relp++ = LOCAT|M|BASE|(fixup>>8);
			*relp++ = fixup;
			*relp++ = FTARGET|SECWAY|TSI;
			*relp++ = fixseg;
		}
		*txtp++ = 0;
		*txtp++ = 0;			/* segment text */
		if (nr != 0) {			/* offset text and fixup */
			fixup = txtp - (txt+3);
			*relp++ = LOCAT|M|OFFSET|(fixup>>8);
			*relp++ = fixup;
			if (fixseg == sx_code)
				*relp++ = F|FT0|TSI;	/* fixup F code, seg index */
			else
				*relp++ = F|FT1|TSI;	/* fixup F data, seg index */
			*relp++ = fixseg;
			*relp++ = dp->d_value;
			*relp++ = dp->d_value >> 8;
			*txtp++ = 0;
			*txtp++ = 0;
		} else {
			*txtp++ = dp->d_value;
			*txtp++ = dp->d_value >> 8;	/* nonrelocated offset */
		}
	}

	/* Write name */
	cp = dp->d_data;
	nd = dp->d_size;
	enuf(nd, 0);
	dot += nd;
	do {
		*txtp++ = *cp;
		--nd;
	} while (*cp++);

	/* Type */
	while (--nd >= 0)
		*txtp++ = *cp++;	/* type */
}

/*
 * Read a list of members of a union/struct/enum.
 * If struct/union, append member types to caller's buffer.
 * If making a debug table:
 * If struct/union tag definition, bracket member definitions.
 * If enum tag, make member type into "enum tag".
 */
char *
getmembs(dp0, bb, bp)
DBGT *dp0;
char *bb;
register char *bp;
{
	int nmembs, n, istag, isenu, isgbl;
	char *sbp;
	register DBGT *dp;
	DBGT **dpp;
	register char *p;
	static DBGT dbrace = {
		NULL,		/* d_dp		*/
		-1,		/* d_ref	*/
		DT_NONE,	/* d_class	*/
		D_LCL,		/* d_flag	*/
		0,		/* d_level	*/
		TYPESYM,	/* d_seg	*/
		0,		/* d_cseg	*/
		0,		/* d_value	*/
		3		/* d_size	*/
	};

	dp = dp0;
	nmembs = bget();
	istag = (dp->d_flag&D_TAG) != 0;
	isenu = (dp->d_flag&D_ENU) != 0;
	isgbl = (dp->d_flag&D_GBL) != 0;
	if (isenu) {
		sbp = bp;
		isenu = strlen(bb) + 1 + 1;
	}
	if ( ! isenu) {
#if	DO_STRUCT_TYPE
		/* Note that this blows up for nmembs > 255 anyway */
		*bp++ = 1;
		*bp++ = nmembs;
#else
		*bp++ = 1;
		*bp++ = 1;
		*bp++ = ANYt;
#endif
		dpp = &dp->d_dp;
		dp = *dpp = newdbgt(&dbrace, "{\0\0");
		if (isgbl)
			dp->d_flag ^= D_GBL|D_LCL;
	}
	dpp = &dp->d_dp;
	while (--nmembs >= 0) {
		/* Note that dp could be a tagless structure, ie a list */
		dp = *dpp = getdbgt();
		if (isgbl)
			dp->d_flag ^= D_GBL|D_LCL;
		n = dp->d_size;
		p = dp->d_data;
		if (isenu && istag && isvcsd) {
			/* Enumerators eschew member lists */
			dp->d_size += isenu;
			dp = newdbgt(dp, dp->d_data);
			bp = dp->d_data;
			while (*bp++)
				;
			*bp++ = STRINGt;
			p = bb;
			while (*bp++ = *p++)
				;
			*bp++ = 0;
			free(*dpp);
			*dpp = dp;
			bp = sbp;
		}
#if	DO_STRUCT_TYPE
		if ( ! isenu) {
			do {
				--n;
			} while (*p++);
			*bp++ = TYPEt;
			if (bp + n >= bb + DSZ) {
				dbrpt("3", dp);
				cbotch("getdbgt buffer");
			}
			do {
				*bp++ = *p++;
			} while (--n > 0);
		}
#endif
		if (istag && isvcsd)
			dpp = &dp->d_dp;
		else
			free(*dpp);
	}
	if ( ! isenu) {
		dp = *dpp = newdbgt(&dbrace, "}\0\0");
		dpp = &dp->d_dp;
		if (isgbl)
			dp->d_flag ^= D_GBL|D_LCL;
	}
	*dpp = NULL;
	return (bp);
}

DBGT *
newdbgt(dp, cp)
DBGT *dp;
register char *cp;
{
	register DBGT *ndp;
	register char *ncp;
	register int n;

	n = dp->d_size;
	if ((ndp = (DBGT *)malloc(sizeof(DBGT) + n)) == NULL)
		cnomem("newdbgt");
	*ndp = *dp;
	for (ncp = ndp->d_data; --n >= 0; *ncp++ = *cp++)
		;
	return (ndp);
}

/*
 * Output a debug brace.
 */
outbrace(c)
int c;
{
 	DBGT *dp;

	if ((dp = (DBGT *)calloc(1, sizeof(DBGT) + 10)) == NULL)
		cnomem("outbrace");
	dp->d_class = DC_LINE;
	dp->d_flag = D_LCL;
	dp->d_seg = segindex(SCODE);
	dp->d_value = ( dotseg == SCODE ? dot : seg[SCODE].s_dot);
	dp->d_size = 10;
	dp->d_data[0] = c;
	*(dp->d_data + 2) = 6;
	*(dp->d_data + 3) = dline;
	*(dp->d_data + 4) = dline >> 8;
	if (dnext != NULL)
		dnext->d_dp = dp;
	dnext = dp;
}

#if	!TINY
_dbrpt(p, dp)
char *p;
register DBGT *dp;
{
	if (xflag > 5)
		printf("%5s: %-10s %2x %2d %2x %2x %2x %2x %04x %2x\n",
			p,
			dp->d_data,
			dp->d_class,
			dp->d_ref,
			dp->d_flag,
			dp->d_level,
			dp->d_seg,
			dp->d_cseg,
			dp->d_value,
			dp->d_size
		);
}

#endif

/*
** Low level record formatting.
*/
/*
 * Put OMF index i at cp and return updated cp.
 */
char *
setindex(cp, i)
register char *cp;
register unsigned i;
{
	if (i >= 128)
		*cp++ = (i >> 8) | 0x80;
	*cp++ = i;
	return (cp);
}

/*
 * Move the name pointed to by 'cp' into an OMF record.
 * The 'bp' argument is the OMF buffer pointer.
 * Return the updated value of 'bp'.
 */
char *
setsymid(bp, cp)
register char *bp;
char *cp;
{
	register n;

	if ((n=strlen(cp)) > 39) {
		cwarn("symbol \"%s\" truncated to 39 characters", cp);
		n = 39;
	}
	*bp++ = n;
	strncpy(bp, cp, n);
	return (bp + n);
}

#if	VAXFMT
putbyte(b)
register b;
{
	if (vaxptr >= &vaxbuf[NVDATA]) {
		vaxflush();
		vaxptr = vaxbuf;
	}
	*vaxptr++ = b;
}
#else
#define	putbyte(b)	bput(b);
#endif

/*
 * Put a record.
 * The 'type' argument is the record type.
 * The body of the record starts at 'p' and is 'n' bytes long.
 * The checksum is appended.
 */
putrecord(type, p, n)
char *p;
int type, n;
{
	register int i, checksum;

	++n;				/* +1 for the checksum */
	checksum = type + n + (n>>8);	/* sum of first three bytes */
	putbyte(type);
	putbyte(n);
	putbyte(n>>8);
	--n;				/* restore actual byte count */
	while (n--) {
		putbyte(i = *p++);
		checksum += i;
	}
	putbyte(-checksum);
}

/*
** Output buffering.
*/
/*
 * Check to see if there is enough room in the text and relocation buffers
 * to hold 'nt' bytes of text and 'nr' bytes of relocation.
 * notenuf() flushes the buffers if necessary.
 */
enuf(nt, nr)
{
	if (txtp+nt>&txt[NTXT] || relp+nr>&rel[NREL]) {
		notenuf();
		if (nt > NTXT-3 || nr > NREL)
			cbotch("enuf(%d, %d)", nt, nr);
	}
	if (txtp == txt) {
		*txtp++ = getsegindex(dotseg);
		*txtp++ = dot;
		*txtp++ = dot >> 8;
	}
}

/*
 * Flush the buffer.
 */
notenuf()
{
	register n;

	if ((n = txtp-txt) > 3) {
		putrecord(LEDATA, txt, n);
		if ((n = relp-rel) != 0) {
			putrecord(FIXUPP, rel, n);
			relp = rel;
		}
	}
	txtp = txt;
}

#if	VAXFMT
/*
 * Flush out a record on the VAX. The record is a standard,
 * RMS variable length record. This routine checks for no bytes
 * in the buffer, so it can be unconditionally called at the
 * end to flush the buffer out.
 * The record is padded out to NVDATA bytes with zeros. This
 * only happens on the last block; the data beyond the end
 * is zeroed.
 */
vaxflush()
{
	register int	n;

	if ((n = vaxptr-vaxbuf) != 0) {
		if (fwrite(vaxbuf, n, 1, ofp) != 1
		||  fflush(ofp) == EOF)
			cfatal("output write error");
	}
}
#endif
