//////////
/ rts87.s
/ cc386 80x87 runtime support.
/ N.B. Converted 6/29/92 by steve from 8087 .a86 source, not tested!
/ Conversion is incomplete!
/ Operand lengths should be explicit for each opcode!
//////////

	.text

	.globl	_cfcc
	.globl	_tstcc
	.globl	_tstccp
	.globl	_dp87
	.globl	_fdcvt
	.globl	_vdcvt
	.globl	_ldcvt
	.globl	_udcvt
	.globl	_idcvt
	.globl	_dfcvt
	.globl	_dvcvt
	.globl	_dlcvt
	.globl	_ducvt
	.globl	_dicvt
	.globl	ldexp
	.globl	frexp
	.globl	modf

/ The following should be unnecessary but...
	.globl	_fvcvt
	.globl	_flcvt
	.globl	_fucvt
	.globl	_ficvt

iw87	.word	0x0fbf			/ chop control word
cwdown	.word	0x07ff			/ round down control word
two	.word	2			/ constant 2

//////////
/ _tstcc()	Test and copy floating condition codes.
/ _tstccp()	Test and pop and copy floating condition codes.
/ _cfcc()	Copy floating condition codes.+
/
/ _tstcc() tests the 80x87 stacktop against 0 and then does a _cfcc().
/ _tstccp() tests the 80x87 stacktop against 0, pops, and then does a _cfcc.
/ _cfcc() copies the C0 and C3 status flags from the  80x87
/ into the C and Z flags of the 80x86.
/ This uses the otherwise useless opcode that copies the flags
/ from %AH in 8080 format.
/
/ Input:
/	80x87 status word.
/ Output:
/	80x86 flags.
//////////

_tstcc:
	ftst				/ test st against 0.
	jmp	_cfcc			/ and set the condition codes.
_tstccp:
	ftst				/ test st against 0,
	fstp	%st			/ pop the 80x87 stack
					/ and fall through...
_cfcc:
/ N.B. fstsw unimplemented in as386 of 7/1/92, so use fwait+fnstsw instead
/	fstsw	%ax			/ store the 80x87 status word to AX
	fwait
	fnstsw	%ax			/ store the 80x87 status word to AX
	fwait				/ and wait for it to finish up.
	sahf				/ load cf and zf correctly.
	ret				/ and return to caller.

//////////
/ _dp87()	Double push from 80x87 stack.
/
/ Move a double from the top of the 80x87 stack to the top of the system stack.
/ There is some magic to keep the return address in one piece.
/
/ Input:
/	Double to push in %st.
/ Outputs:
/	NDP stack popped.
/	Double on 80x86 stack.
//////////

_dp87:
	pop	%eax			/ capture return address.
	subl	%esp, $8		/ claim a double on stack and
	fstpl	(%esp)			/ pop 80x87 to memory.
	fwait				/ make sure pop is completed.
	ijmp	%eax			/ return to the caller.

//////////
/ _dicvt()	Convert integer to double.
/ _ducvt()	Convert unsigned integer to double.
/ _dlcvt()	Convert long integer to double.
/ _dvcvt()	Convert unsigned long integer to double.
/ _dfcvt()	Convert float to double.
/
/ These routines are called directly from compiled code for all
/ widen conversions to double.
/ Some of the simple widens can be
/ compiled inline, but the unsigned widens and the signed
/ widens when the object lacks an lvalue cannot be.
/
/ Each routine is passed the source operand as any C
/ argument would be passed.
/ Each returns the result on the top of the 80x87 stack.
//////////

_dicvt:
_ficvt:
	fildl	4(%esp)			/ load up the integer.
	ret				/ return.

_ducvt:
_fucvt:
	push	%ebp			/ save
	movl	%ebp, %esp		/ sequence
	push	%eax			/ claim a long int.

	mov	%eax, 4(%ebp)		/ fetch the unsigned int and
	mov	-4(%ebp), %eax		/ put in ls half.
	mov	-2(%ebp), $0		/ zero extend.
	fild	-4(%ebp)		/ load and convert to double.

	mov	%esp, %ebp		/ back up stack.
	pop	%ebp			/ standard
	ret				/ return sequence.

_dlcvt:
_flcvt:
	fild	4(%esp)			/ load and convert the long.
	ret				/ sequence.

_dvcvt;
_fvcvt:
	push	%ebp			/ save
	mov	%ebp, %esp		/ sequence
	sub	%esp, $8		/ claim a 64 bit integer.

	mov	%eax, 4(%ebp)		/ low half of long
	mov	-8(%ebp), %eax		/ put in 64 bit integer.
	mov	%eax, 6(%ebp)		/ high half of long.
	mov	-6(%ebp), %eax		/ put in 64 bit integer.
	mov	-4(%ebp), $0		/ zeros on the
	mov	-2(%ebp), $0		/ end.
	fild	-8(%ebp)		/ load and convert to double.

	mov	%esp, %ebp		/ standard
	pop	%ebp			/ c
	ret				/ sequence.

_dfcvt:
	push	%ebp			/ save
	mov	%ebp, %esp		/ sequence.
	push	%eax			/ claim a float.

	fld	4(%ebp)	/ grab double and
	fstp	-4(%ebp)		/ round to a single float.
	fld	-4(%ebp)		/ load result.

	mov	%esp, %ebp		/ full.
	pop	%ebp			/ standard
	ret				/ sequence.

//////////
/ _idcvt()	convert double to integer
/ _udcvt()	convert double to unsigned integer
/ _ldcvt()	convert double to long integer
/ _vdcvt()	convert double to unsigned long integer
/ _fdcvt()	convert double to float
/
/ this set of routines handle all shrinks from double. the task
/ is a little harder than would be expected. the 80x87 is always
/ shifted into chop mode, so that conversions work the way that
/ you expect them to work.
/
/ all of these routines take a double argument in the  standard
/ way, and return the result in the standard return location of
/ objects of that type (%eax, dx%eax, tos).
//////////

_idcvt:
	push	%ebp			/ save
	mov	%ebp, %esp		/ sequence.
	push	%eax			/ claim 2 words.

/	fstcw	-2(%ebp)		/ save old control word
	fwait
	fnstcw	-2(%ebp)		/ save old control word
	fldcw	iw87			/ load chopping control word
	fld	4(%ebp)			/ load up double and
	fistp	-4(%ebp)		/ convert to integer and
	fldcw	-2(%ebp)		/ put control word back.
	mov	%eax, -4(%ebp)		/ ax=result.

	mov	%esp, %ebp		/ standard
	pop	%ebp			/ c
	ret				/ sequence.

_udcvt:
	push	%ebp			/ save
	mov	%ebp, %esp		/ sequence.
	sub	%esp, $6		/ claim 3 words.

/	fstcw	-2(%ebp)		/ save old control word
	fwait
	fnstcw	-2(%ebp)		/ save old control word
	fldcw	iw87			/ load chopping control word
	fld	4(%ebp)			/ load up double and
	fistp	-6(%ebp)		/ convert to 32 bit integer and
	fldcw	-2(%ebp)		/ put control word back.
	mov	%eax, -6(%ebp)		/ ax=ls half of result.

	mov	%esp, %ebp		/ standard
	pop	%ebp			/ c
	ret				/ sequence.

_ldcvt:
	push	%ebp			/ save
	mov	%ebp, %esp		/ sequence.
	sub	%esp, $6		/ claim 3 words.

/	fstcw	-2(%ebp)		/ save old control word
	fwait
	fnstcw	-2(%ebp)		/ save old control word
	fldcw	iw87			/ load chopping control word
	fld	4(%ebp)			/ load up double and
	fistp	-6(%ebp)		/ convert to long and
	fldcw	-2(%ebp)		/ put control word back.
	mov	%eax, -6(%ebp)		/ dxax=the 32 bit
	mov	%edx, -4(%ebp)		/ result.

	mov	%esp, %ebp		/ standard
	pop	%ebp			/ c
	ret				/ sequence.

_vdcvt:
	push	%ebp			/ save
	mov	%ebp, %esp		/ sequence.
	sub	%esp, $10		/ claim 5 words.

/	fstcw	-2(%ebp)		/ save old control word
	fwait
	fnstcw	-2(%ebp)		/ save old control word
	fldcw	iw87			/ load chopping control word
	fldcw	iw87			/ load chopping control word
	fld	4(%ebp)			/ load up double and
	fistp	-10(%ebp)		/ convert to integer and
	fldcw	-2(%ebp)		/ put control word back.
	mov	%eax, -10(%ebp)		/ dxax=unsigned long
	mov	%edx, -8(%ebp)		/ result.

	mov	%esp, %ebp		/ standard
	pop	%ebp			/ c
	ret				/ sequence.

_fdcvt:
	push	%ebp			/ save
	mov	%ebp, %esp		/ sequence.
	push	%eax			/ claim 2 words.

	fld	4(%ebp)			/ load the double and
	fstp	-4(%ebp)		/ convert it to a float.
	fld	-4(%ebp)		/ load the float up.

	mov	%esp, %ebp		/ standard
	pop	%ebp			/ c
	ret				/ sequence.

//////////
/ ldexp()	make a double.
/
/ this routine assembles a double precision floating point number
/ given a fraction and an integer exponent.
/
/ calling sequence:
/ double
/ ldexp(fraction, exponent);
/ double	fraction;
/ int		exponent;
//////////

ldexp:
	push	%ebp			/ save
	mov	%ebp, %esp		/ sequence.

	fild	12(%ebp)		/ exponent
	fld	4(%ebp)			/ fraction
	fscale				/ put it all together
	fstp	%st(1)			/ and pop stack

	pop	%ebp			/ standard
	ret				/ return.

//////////
/ frexp()	split a double.
/
/ this routine splits a double float into its fraction and its
/ exponent. the "fxtract" instruction almost does the job. the
/ ieee standard says that the significand is not from 0.5 to 1
/ but from 1 to 2. the extracted significand and exponent must
/ be adjusted (if non zero).
/
/ calling sequence:
/ double
/ frexp(value, expp);
/ double	value;
/ int		*expp;
//////////

frexp:
	push	%ebp			/ save
	mov	%ebp, %esp		/ sequence

	fld	4(%ebp)			/ load up the value
	fxtract				/ split it
	call	_tstcc			/ check if the significand is zero.
	je	frexp0			/ jump if it is.
	fild	two			/ divide the significand
	fdivp	%st(1), %st		/ by two and
	fld1				/ add 1 to the
	faddp	%st(2), %st		/ exponent.

frexp0:	fxch				/ move exponent to %st
	mov	%ebx, 12(%ebp)		/ save the exponent
	fistp	(%ebx)			/ through the supplied pointer.
	fwait				/ make sure it is good.

	pop	%ebp			/ standard
	ret				/ return.

//////////
/ modf()	split a double (another way!)
/
/ calling sequence:
/ double
/ modf(value, intpart);
/ double	value;
/ double	*intpart;
//////////

modf:
	push	%ebp			/ function
	mov	%ebp, %esp		/ sequence.
	push	%eax			/ claim one word.

/	fstcw	-2(%ebp)		/ save the old control word and
	fwait
	fnstcw	-2(%ebp)		/ save the old control word and
	fldcw	cwdown			/ load a new one that rounds down.
	fld	4(%ebp)			/ pick up argument.
	fld	%st			/ %st=arg, %st(1)=arg.
	frndint				/ %st=intpart, %st(1)=arg.
	fsub	%st(1), %st		/ %st=intpart, %st(1)=fracpart.
	mov	%ebx, 12(%ebp)		/ load integer pointer.
	fstp	(%ebx)			/ store intpart through it and pop.
	fldcw	-2(%ebp)		/ put old control word back.

	mov	%esp, %ebp		/ standard
	pop	%ebp			/ c
	ret				/ code

/ end of rts87.s
