/*
 * C compiler.
 * Intel iAPX-86 et al.
 * Prologs, etc.
 */
#ifdef   vax
#include "INC$LIB:cc2.h"
#else
#include "cc2.h"
#endif

int		framesize;
int		hasfloat;
static	SYM	*countp;

/*
 * Generate code for the ALIGN operation.
 * The 'align' argument is ignored because there
 * is only one style of alignment on the 8086:
 * simple word alignment.
 */
genalign(align)
{
	if ((dot&01) == 0)
		return;
	if (isvariant(VASM)) {
		bput(ALIGN); iput(align);
	} else if (dotseg == SBSS)
		++dot;
	else
		outab(0);
}

/*
 * Generate code for a function prologue.
 */
genprolog()
{
	register int	lab;
	register SYM	*labp;

	hasfloat = 0;
	if (isvariant(VASM)) {
		genone(ZPUSH, A_RSI);
		genone(ZPUSH, A_RDI);
		if (notvariant(V80186) || (framesize == 0) || isvariant(VPROF)) {
			genone(ZPUSH, A_RBP);
			gentwo(ZMOV, A_RBP, A_RSP);
		}
		if (isvariant(VPROF)) {
			genseg(SBSS);
			bput(LLABEL); iput(lab=newlab());
			genblock((sizeof_t)8);
			genseg(SCODE);
			gentwo(ZMOV, A_RBX, A_IMM|A_LID, lab);
			genone(ZCALL, A_GID|A_DIR, "scount");
		}
		if (framesize != 0) {
			if (isvariant(V80186) && notvariant(VPROF))
				gentwo(ZENTER, A_IMM|A_OFFS, framesize, A_IMM|A_OFFS, 0);
			else {
				if (framesize <= 2)
					genone(ZPUSH, A_RAX);
				else
					gentwo(ZSUB, A_RSP, A_OFFS|A_IMM, framesize);
			}
		}
	} else {
		outab(0x56);				/* push si */
		outab(0x57);				/* push di */
		if (notvariant(V80186) || (framesize == 0) || isvariant(VPROF)) {
			outab(0x55);			/* push bp */
			outab(0x8B);			/* mov bp, sp */
			outab(0xEC);
		}
		if (isvariant(VPROF)) {
			genseg(SBSS);
			labp = llookup(newlab(), 1);
			labp->s_seg = dotseg;
			labp->s_value  = dot;
			genblock((sizeof_t)8);
			genseg(SCODE);
			outab(0xBB);			/* mov bx, offset lab */
			outxw(labp, 0, 0);
			if (countp == NULL)
				countp = glookup("scount", 0);
			outab(0xE8);			/* near call */
			outxw(countp, 0, 1);
		}
		if (framesize != 0) {
			if (isvariant(V80186) && notvariant(VPROF)) {
				outab(0xC8);		/* enter framesize,0 */
				outaw(framesize);
				outab(0);
			} else {
				if (framesize <= 2)
					outab(0x50);		/* push ax */
				else {
					if (framesize >= 128)
						outab(0x81);	/* sub, sw=01 */
					else
						outab(0x83);	/* sub, sw=11 */
					outab(0xEC);		/* sp, */
					outab(framesize);	/* value */
					if (framesize >= 128)
						outab(framesize >> 8);
				}
			}
		}
	}
}

/*
 * Generate code for a function epilogue.
 * Note that the i8086 OMF and i80286 OMF output writers
 * know the length of the epilogue when they compute block length.
 * The variable 'hasfloat' keeps track of whether each function performs
 * floating point operations.
 * If it does, the epilogue includes an FWAIT to assure completion;
 * otherwise, a store into a local variable could complete after the exit,
 * when the allocated stack space has been reused for something else.
 */
genepilog()
{
	register INS	*ip;

	if (isvariant(VASM)) {
		if (hasfloat) {
			if (isvariant(VEMU87))
				genone(ZCALL, A_GID|A_DIR, "emu87");
			else {
				bput(CODE); bput(ZFWAIT);
			}
		}
		if (isvariant(V80186)) {
			bput(CODE); bput(ZLEAVE);
		} else {
			if (framesize != 0)
				gentwo(ZMOV, A_RSP, A_RBP);
			genone(ZPOP, A_RBP);
		}
		genone(ZPOP, A_RDI);
		genone(ZPOP, A_RSI);
		bput(CODE); bput(ZRET);
	} else {
		if (hasfloat) {
			if (isvariant(VEMU87))
				outemucall();		/* call emu87 */
			else
				outfb(0x9B);		/* fwait */
		}
		if (isvariant(V80186))
			outab(0xC9);			/* leave */
		else {
			if (framesize != 0) {
				outab(0x8B);		/* mov sp, bp */
				outab(0xE5);
			}
			outab(0x5D);			/* pop bp */
		}
		outab(0x5F);				/* pop di */
		outab(0x5E);				/* pop si */
#if !ONLYSMALL
		if (notvariant(VSMALL))
			outab(0xCB);			/* ret far */
		else
#endif
			outab(0xC3);			/* ret near */
	}
}

/*
 * Generate the code for a BLOCK item.
 * If in -S mode, output an assembly directive to get the space.
 * If generating binary, either bump 'dot' (if compiling
 * into BSS segment) or output a block of 0 bytes.
 */
genblock(n)
register sizeof_t n;
{
	if (isvariant(VASM)) {
		bput(BLOCK); zput(n);
	} else if (dotseg == SBSS)
		dot += n;
	else
#if	0	/* i8086 outnzb() not implemented yet */
		outnzb(n);
#else
		while (n--)
			outab(0);
#endif
}
