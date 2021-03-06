/*
 * The routines in this file make up
 * a writer for the Coherent reloc.h object
 * file. During code assembly all the bits get
 * written to a temporary file. At the very end the
 * bits get copied back to the final object file.
 * The symbol table is constructed as the code is read,
 * each symbol is declared when defined. References are
 * tacked onto the end of the file.
 * Relocation instructions for debug and symbol table entries are
 * also passed through the scratch file.
 */
#ifdef vax
#include "INC$LIB:cc2.h"
#include "INC$LIB:canon.h"
#include "INC$LIB:reloc.h"
#include "INC$LIB:debug.h"
#else
#include "cc2.h"
#include <canon.h>
#include <reloc.h>
#include <debug.h>
#endif

#if !TINY
#define dbrpt(x,y)	_dbrpt(x,y)
#else
#define dbrpt(x,y)	/* dbrpt(x,y) */
#endif
#define	NTXT	512			/* Text buffer size */
#define	NREL	256			/* Relocation buffer size */
/*
 * Scratch file operators.
 */
#define	TTYPE	017			/* Type mask */
#define Tflag(x)	(((x)>>4)&7)	/* Get flag bits */
#define	TEND	000			/* End marker */
#define	TENTER	001			/* Enter new area */
#define	TBYTE	002			/* Byte data */
#define	TWORD	003			/* Word data */
#if 0
#define TTRIP	004			/* Triple byte data */
#define TLONG	005			/* Long data */
#endif
#define TDLAB	006			/* Debug item */
#define	TDLOC	007			/* Debug location */
#define	TCODE	010			/* Code segment base */
#define	TDATA	011			/* Data segment base */
#define	TBASE	012			/* External segment base */

#define	TPCR	020			/* PC rel flag, ored in */
#define	TSYM	040			/* Symbol based flag, ored in */
#define locrup(x)	(((x)+01)&~01)	/* memory alignment */
objhdr_t objhdr = {			/* object header */
		REL_OBJ,
		"i8086",
		"C",
		VERSMWC
};

summseg_t summseg = {			/* Summary segment */
		SUMMARY,
		sizeof (summseg.seg_off)
};

extern	char	*calloc();
extern	char	*malloc();
extern	char	*strcpy();

ADDRESS	SCODE_len;			/* Length of SCODE in bytes */
char	txt[NTXT];			/* Text buffer */
char	rel[NREL];			/* Relocation buffer */
char	*txtp = txt;			/* Text pointer */
char	*relp = rel;			/* Relocation pointer */
int	refnum = 0;			/* Symbol reference number */
static int level = 0;			/* Debug brace level */
static int dline = 0;			/* Most recent line number entry */

/*
 * This table converts a C compiler
 * segment index into a relocatable object segment type.
 */
char	segindex[] = {
	PURCODE,			/* SCODE */
	OWNCODE,			/* SLINK */
	PURDATA,			/* SPURE */
	STRINGS,			/* SSTRN */
	OWNDATA,			/* SDATA */
	BLDATA,				/* SBSS */
	-1,				/* SANY */
	-1,				/* SSTACK */
	-1,				/* SALIEN */
	DEBUG,				/* SDBG */
	SYMBOLS				/* SSYM */
};

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
	dot += 1;
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


#ifdef TLONG
/*
 * Output an absolute long.
 */
outal(l)
long l;
{
	bput(TLONG);
	lput(l);
	dot += 4;
}
#endif

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
	dot += 1;
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

#ifdef TLONG
/*
 * Output a full long.
 */
outxl(sp, l, pcrf)
register SYM *sp;
long l;
{
	register opcode;

	opcode = TLONG;
	if (sp != NULL)
		opcode |= TSYM;
	if (pcrf)
		opcode |= TPCR;
	bput(opcode);
	lput(lR);
	if (sp != NULL)
		pput(sp);
	dot += 4;
}
#endif

/*
 * Copy a dlabel record from ifp to ofp.
 * Flag the symbol table entry if appropriate.
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
 * Output a debug line record.
 */
outdlin(op)
{
	bput(TDLAB);
	bput(DC_LINE);
	iput(line);
	bput(op);
	bput(0);
	bput(DT_NONE);
	iput(0);
}

/*
 * Output a debug relocation record.
 */
outdloc(n)
{
	bput(TDLOC);
	iput(n);
}

/*
 * Abort illegal output.
 * reloc.h doesn't handle 8087 or Large model.
 */
outbad()
{
	cbotch("bad temp file object");
}

/*
 * Output a 1 word object containing
 * the base address of the current code
 * segment.
 */
outcb()
{
	return (outbad());
	bput(TCODE);
	dot += 2;
}

/*
 * Output a 1 word object containing
 * the base address of the current data
 * segment.
 */
outdb()
{
	return (outbad());
	bput(TDATA);
	dot += 2;
}

/*
 * Output a 1 word object containing
 * the base address of the external symbol
 * pointed to by `sp'.
 */
outsb(sp)
SYM *sp;
{
	return (outbad());
	bput(TBASE);
	pput(sp);
	dot += 2;
}

/*
 * Initialize the code writer.
 * This routine is called before anything is written to the scratch file.
 * A no op on coherent reloc.h format.
 */
outinit()
{
}

/*
 * Finish off the code writer.
 * This routine is called just before the scratch file is closed.
 * Write either the "end" to the output file
 * if generating assembler,
 * or a "TEND" code to stop the
 * "copycode" routine if writing binary.
 */
outdone()
{
	register SYM *sp;
	register int i;

	if (isvariant(VASM)) {
		return;
	}

	/*
	 * Assign refnums to symbols.
	 * And make dlabel items for internally generated symbols.
	 */
	for (refnum = i = 0;  i < NSHASH;  i++) {
		for (sp = hash2[i];  sp != NULL;  sp = sp->s_fp) {
			if ((sp->s_flag&S_GBL) != 0
			 || (sp->s_flag&S_DEF) == 0) {
				if ((sp->s_flag&S_LABNO) == 0)
					sp->s_ref = refnum++;
				if ((sp->s_flag&S_PUT) == 0) {
					bput(TDLAB);
					bput(DC_GREF);
					iput(0);
					sput(sp->s_id);
					bput(DT_NONE);
					iput(0);
				}
			}
		}
	}

	bput(TEND);
}

/*
 * Copy back.
 * Figure out the sizes and final values
 * of everything. Copy the code from the scratch
 * file back to the output file.
 */
copycode()
{
	int op, nd, nr;
	ADDRESS data;
	long	addr;
	fsize_t size, symsize;
	unsigned char reltype, segtype;
	 
	SCODE_len = seg[SCODE].s_dot;

	/*
	 * Initialize output.
	 */
	oheader(0);

	/*
 	 * Copy out code.
	 */
	dotseg = SCODE;
	dot = 0;
	while ((op=bget()) != TEND) {
		switch (op & TTYPE) {

		case TENTER:
			oenter(bget());
			continue;

		case TBYTE:
			nd = 1;
			reltype = B_0;
			data = bget();
			break;

		case TWORD:
			nd = 2;
			reltype = B_01;
			data = iget();
			break;

#ifdef TLONG
		case TLONG:
			nd = 4;
			reltype = B_0123;
			data = lget();
			break;
#endif

		case TDLAB:
			getdlab();
			continue;

		case TDLOC:
			getdloc();
			continue;

		default:
			cbotch("copycode: op=%d", op);

		}

		switch (Tflag(op)) {

		case 0:			/* Absolute */
			nr = 0;
			break;

		case Tflag(TPCR|TSYM):	/* Symbol based pc relative */
			data -= dot+nd;
			/* Fall through */

		case Tflag(TSYM):	/* Symbol based */
		{
			register SYM *sp;

			sp = pget();
			if ((sp->s_flag&S_DEF) != 0) {
				nr = 6;
				segtype = segindex[sp->s_seg];
				data += sp->s_value;
			} else {
				nr = 10;
				segtype = SYMBOLS;
				addr = sp->s_ref;
			}
			if ((op&TPCR) != 0)
				segtype |= PCREL;
			break;
		}

		case Tflag(TPCR):	/* Absolute pc relative */
			data -= dot+nd;
			segtype = ABSLUTE|PCREL;
			nr = 10;
			addr = 0;
			break;

		default:
			cbotch("copycode: flags=%o", op);
		}

		/*
		 * Write the current record.
		 */
		enuf(nd, nr);
		if (nr != 0) {
			rbbuf(reltype);
			rbbuf(segtype);
			rlbuf((long) dot);
			if (nr == 10)
				rlbuf(addr);
		}
		do {
			bbuf((int) data);
			data >>= 8;
		} while (--nd > 0);
	}

	if (isvariant(VLINES))
		outbrace('}');
	if (notvariant(VLIB))
		dump();			/* Dump the typedefs */

	/*
	 * Flush buffers.
	 */
	notenuf();

	/*
	 * Write symbol table.
	 */
	summseg.seg_off[SYMBOLS] = ftell(ofp);
	segtype = SYMBOLS;
	bput(segtype);
	size = symsize;
	cansize(size);
	owrite(&size, sizeof(size));
	addr = refnum;
	canuaddr(addr);
	owrite(&addr, sizeof(addr));
	oglobals();
	symsize = ftell(ofp) - summseg.seg_off[SYMBOLS] - sizeof(fsize_t) - 1;

	/*
	 * Put end marker.
	 */
	segtype = LASTSEG;
	bput(segtype);

	/*
	 * Now go back and patch the remaining holes.
	 */
	fseek(ofp, (fsize_t)0, 0);
	oheader(1);

	/*
	 * Symbol segment size and count.
	 */
	fseek(ofp, summseg.seg_off[SYMBOLS]+1, 0);
	size = symsize;
	cansize(size);
	owrite(&size, sizeof(size));
	addr = refnum;
	canuaddr(addr);
	owrite(&addr, sizeof(addr));
}

/*
 * Switch segments.
 */
oenter(newseg)
register int newseg;
{
	notenuf();
	if (dotseg != SSYM)
		seg[dotseg].s_dot = dot;
	dotseg = newseg;
	if (dotseg != SSYM)
		dot = seg[dotseg].s_dot;
	else
	dot = 0;
}

/*
 * This routine checks to see if
 * there is enough room in the text and relocation
 * buffers to hold `nt' bytes of text and `nr' bytes of
 * relocation.
 */
enuf(nt, nr)
{
	if (txtp+nt>&txt[NTXT] || relp+nr>&rel[NREL])
		notenuf();
}

/*
 * Flush buffers.
 */
notenuf()
{
	register n;
	unsigned char	segtype;
	fsize_t	size;

	if ((n=txtp-txt) != 0) {
		if (dotseg != SSYM) {
			segtype = segindex[dotseg];
			bput(segtype);
			size = n;
			cansize(size);
			owrite(&size, sizeof(size));
		}
		owrite(txt, n);
		if ((n=relp-rel) != 0) {
			segtype = RELOCN;
			bput(segtype);
			size = n;
			cansize(size);
			owrite(&size, sizeof(size));
			owrite(rel, n);
			relp = rel;
		}
	}
	txtp = txt;
}

/*
 * Write a byte or long to text or relocation buffers
 * in canonical ordering.
 */
bbuf(b)
{
	*txtp++ = b;
	dot += 1;
}

lbuf(l)
long l;
{
	*txtp++ = l >> 16;
	*txtp++ = l >> 24;
	*txtp++ = l;
	*txtp++ = l >> 8;
	dot += 4;
}

rbbuf(b)
{
	*relp++ = b;
}

rlbuf(l)
long l;
{
	*relp++ = l >> 16;
	*relp++ = l >> 24;
	*relp++ = l;
	*relp++ = l >> 8;
}

/*
 * Write a block of bytes to the output file.
 */
owrite(p, n)
register char *p;
register int n;
{
	while (--n >= 0) 
		bput(*p++);
}

/*
 * Write the header and summary segment.
 * Summary is complete on second call only.
 */
oheader(n)
{
	fsize_t offs;
	register int i;
	register struct seg *segp;

	/*
	 * Write header.
	 */
	owrite(&objhdr, sizeof(objhdr));

	/*
	 * Copy segment sizes into summary segment.
	 */
	for (segp = &seg[i=0];  i < NSEG;  segp++, i++) {
		if (segindex[i] < 0 || i == SSYM) continue;
		offs = segp->s_dot;
		if (i != SDBG)
			offs = locrup(offs);
		if (n == 0 || i >= SDBG)
			summseg.seg_off[segindex[i]] += offs;
		segp->s_dot = 0;
	}
	/*
	 * Write summary segment.
	 */
	bput(summseg.seg_type);
	offs = summseg.seg_size;
	cansize(offs);
	owrite(&offs, sizeof(offs));
	for (i=0; i<SUMMARY; i+=1) {
		offs = summseg.seg_off[i];
		cansize(offs);
		owrite(&offs, sizeof(offs));
	}
}

/*
 * Structure to contain debug items on this pass.
 */
struct dbgt {
	struct dbgt *d_dp;		/* Link */
	int d_ref;			/* Index number */
	unsigned char d_class;		/* Storage class */
	unsigned char d_flag;		/* Flags */
	unsigned char d_level;		/* Bracket level */
	unsigned char d_seg;		/* Segment */
	ADDRESS d_value;		/* Offset in segment */
	int d_size;			/* Size of d_data */
	char d_data[];			/* Name and type */
};
#define D_GBL	1
#define D_LCL	2
#define D_TAG	4
#define D_NAM	8
#define D_LAB	16
#define D_ENU	32

struct dbgt *newdbgt();
struct dbgt *getdbgt();
struct dbgt *dbase;
struct dbgt *dnext;
char *getmembs();

/*
 * Read debug table item(s) into memory.
 */
getdlab()
{
	register struct dbgt *dp;
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
 * Read a debug relocation item and patch current `dot'
 * into the appropriate debug/symbol table item.
 * If the debug item is '}' and level 2, then dump the batch of locals.
 * If the debug item is '}' and level 1, then dump the type definitions.
 */
getdloc()
{
	register struct dbgt *dp;
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
	dp->d_seg = segindex[dotseg];
	dp->d_value = dot;

	dbrpt("reloc", dp);
	if (dp->d_data[0] == '}' && dp->d_level <= 2) {
		outbrace('}');
 		level--;
		dump();
	}
}

/*
 * Dump the locals and type definitions
 */

dump()
{
	register struct dbgt *dp, **dpp;
	int saveseg;

	if ((saveseg = dotseg) != SDBG)
		oenter(SDBG);
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
	if (saveseg != SDBG)
		oenter(saveseg);
}

/*
 * Write out global symbol definitions and references.
 */
oglobals()
{
	register struct dbgt *dp, *dp1;
	extern int refnum;

	oenter(SSYM);
	for (dp = dbase; dp != NULL; dp = dp1) {
		dp1 = dp->d_dp;
		dbrpt("write", dp);
		if (dp->d_seg == TYPESYM || dp->d_seg == ABSLUTE)
			osymbol(dp, refnum++);
		else
			osymbol(dp, dp->d_ref);
		dbrpt("free", dp);
		free(dp);
	}
	notenuf();
}

/*
 * Write out a symbol to the symbol or debug table.
 * Debug table symbols have negative refnums,
 * and may require relocation.
 */
osymbol(dp, refnum)
register struct dbgt *dp;
{
	register char *cp;
	unsigned char segtype;
	uaddr_t segaddr;
	int nd, nr;

	/* Write scope/refnum or seg/value+relocation expression */
	nd = 5 + dp->d_size;
	nr = 0;
	if (refnum >= 0) {
		if (dp->d_seg == ABSLUTE) {
			segtype = MDSCOPE;
			nd += 5;
		} else if (dp->d_seg != LASTSEG) {
			segtype = LDSCOPE;
			nd += 5;
		} else
			segtype = RFSCOPE;
		segaddr = refnum;
	} else {
		segtype = dp->d_seg;
		segaddr = dp->d_value;
		if (segtype < SUMMARY
		 && segtype != TYPESYM
		 && segtype != ABSLUTE)
			nr = 6;
	}
	enuf(nd, nr);
	bbuf(segtype);
	if (nr != 0) {
		rbbuf(B_2301);
		rbbuf(dp->d_seg);
		rlbuf((long) dot);
	}
	lbuf(segaddr);

	/* Write name */
	cp = dp->d_data;
	nd = dp->d_size;
	do {
		bbuf(*cp);
		nd -= 1;
	} while (*cp++);

	/* Write seg/value expression */
	if (refnum >= 0 && (segtype == MDSCOPE || segtype == LDSCOPE)) {
		bbuf(dp->d_seg);
		lbuf((long) dp->d_value);
	}

	/* Type */
	while (--nd >= 0)
		bbuf(*cp++);
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
struct dbgt *
getdbgt()
{
	register int n;
	register char *dptr;
	struct dbgt d;
	char data[DSZ];
	int value, width, offs;
	int typet, subt, membs;
	char *sdp;

	d.d_dp = NULL;
	d.d_ref = -1;
	d.d_flag = D_LCL;
	d.d_level = level;
	d.d_seg = LASTSEG;
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
				d.d_flag = (level == 0) ? D_TAG|D_GBL : D_TAG|D_LCL;
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

		case DC_LAB:
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
					d.d_seg = segindex[sp->s_seg];
					d.d_value = sp->s_value;
#if COMMON
				} else if (sp->s_value != 0) {
					d.d_seg = BLDATA|Cb;
					d.d_value = sp->s_value;
#endif
				}
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
 * Read a list of members of a union/struct/enum.
 * If struct/union, append member types to caller's buffer.
 * If making a debug table:
 * If struct/union tag definition, bracket member definitions.
 * If enum tag, make member type into "enum tag".
 */
char *
getmembs(dp0, bb, bp)
struct dbgt *dp0;
char *bb;
register char *bp;
{
	int nmembs, n, istag, isenu, isgbl, isdbg;
	char *sbp;
	register struct dbgt *dp;
	struct dbgt **dpp;
	register char *p;
	static struct dbgt dbrace = {
		NULL,		/* d_dp */
		-1,		/* d_ref */
		DT_NONE,	/* d_class */
		D_LCL,		/* d_flag */
		0,		/* d_level */
		TYPESYM,	/* d_seg */
		0,		/* d_value */
		3		/* d_size */
	};

	dp = dp0;
	nmembs = bget();
	istag = (dp->d_flag&D_TAG) != 0;
	isenu = (dp->d_flag&D_ENU) != 0;
	isgbl = (dp->d_flag&D_GBL) != 0;
	isdbg = (isvariant(VDSYMB) && isvariant(VTYPES)) != 0;
	if (isenu) {
		sbp = bp;
		isenu = strlen(bb) + 1 + 1;
	}
	if ( ! isenu) {
		*bp++ = 1;
		*bp++ = nmembs;
	}
	dpp = &dp->d_dp;
	if (istag && isdbg && ! isenu) {
		dp = *dpp = newdbgt(&dbrace, "{\0\0");
		if (isgbl)
			dp->d_flag ^= D_GBL|D_LCL;
		dpp = &dp->d_dp;
	}
	while (--nmembs >= 0) {
		dp = *dpp = getdbgt();
		if (isgbl)
			dp->d_flag ^= D_GBL|D_LCL;
		n = dp->d_size;
		p = dp->d_data;
		if (isenu && istag && isdbg) {
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
		if (istag && isdbg)
			dpp = &dp->d_dp;
		else
			free(*dpp);
	}
	if (istag && isdbg && ! isenu) {
		dp = *dpp = newdbgt(&dbrace, "}\0\0");
		if (isgbl)
			dp->d_flag ^= D_GBL|D_LCL;
		dpp = &dp->d_dp;
	}
	*dpp = NULL;
	return (bp);
}

/*
 * Output a debug brace
 */
outbrace(c)
char c;
{
 	struct dbgt *dp;

	if ((dp = (struct dbgt *)calloc(1, sizeof(struct dbgt) + 10)) == NULL)
		cnomem("outbrace");
	dp->d_class = DC_LINE;
	dp->d_flag = D_LCL;
	dp->d_seg = segindex[SCODE];
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

struct dbgt *
newdbgt(dp, cp)
struct dbgt *dp;
register char *cp;
{
	register struct dbgt *ndp;
	register char *ncp;
	register int n;

	n = dp->d_size;
	if ((ndp = (struct dbgt *)malloc(sizeof(struct dbgt) + n)) == NULL)
		cnomem("newdbgt");
	*ndp = *dp;
	for (ncp = ndp->d_data; --n >= 0; *ncp++ = *cp++)
		;
	return (ndp);
}

#if !TINY
_dbrpt(p, dp)
char *p;
register struct dbgt *dp;
{
	if (xflag > 5) {
		printf("%5s: %-10s %2x %2d %2x %2x %2x %04x %2x\n",
			p,
			dp->d_data,
			dp->d_class,
			dp->d_ref,
			dp->d_flag,
			dp->d_level,
			dp->d_seg,
			dp->d_value,
			dp->d_size
			);
	}
}

dbtyp(dp)
struct dbgt *dp;
{
	register int n;
	register char *p;

	if (xflag > 20) {
		n = dp->d_size;
		p = dp->d_data;
		do {
			--n;
			putchar(*p);
		} while (*p++);
		while (--n >= 0) {
			if (*p == STRINGt) {
				putchar(' ');
				putchar('"');
				++p;
				do {
					--n;
				} while (putchar(*p++));
				putchar('"');
				continue;
			} else if (*p == TYPEt)
				printf(" TYPEt");
			else if (*p == ANYt)
				printf(" ANYt");
			else
				printf(" %o", *p&0377);
			++p;
		}
		putchar('\n');
	}
}
#endif
