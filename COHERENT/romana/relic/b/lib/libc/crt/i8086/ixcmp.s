////////
/
/ Intel 8086 floating point.
/ Double compare.
/ SMALL model.
/
////////

	.globl	ircmp
	.globl	ilcmp
	.globl	urcmp
	.globl	ulcmp

////////
/
/ ** ircmp -- double compare (rvalue)
/ ** ilcmp -- double compare (lvalue)
/
/ these two routines perform floating point compares. both the rvalue
/ and the lvalue forms are provided for use by the compiler. ax  gets
/ set to a completion code; an or ax,ax sets the flags so that a jump
/ instruction may be generated.
/
/ compiler calling sequences:
/	push	<right double argument>
/	push	<left  double argument>
/	call	ircmp
/	add	sp,16
/
/	mov	ax,offset <right double argument>
/	push	ax
/	push	<left  double argument>
/	call	ilcmp
/	add	sp,10
/
/ outputs:
/	ax =-1 if l<r
/	ax = 0 if l=r
/	ax = 1 if l>r
/
/ uses:
/	ax, bx, cx, dx
/
////////

expon	=	0x7F80			/ DECVAX format exponent bits

l	=	8			/ left argument
r	=	16			/ right argument (rvalue)
rp	=	16			/ right argument (lvalue)

urcmp:
ircmp:	push	si			/ standard
	push	di			/ c
	push	bp			/ function
	mov	bp,sp			/ linkage
	lea	di,r(bp)		/ di=pointer to right op.
	jmp	L0			/ go to common code.

ulcmp:
ilcmp:	push	si			/ standard
	push	di			/ c
	push	bp			/ function
	mov	bp,sp			/ linkage.
	mov	di,rp(bp)		/ di=pointer to right op.

L0:	sub	ax,ax			/ default return equal.
	mov	bx,l+6(bp)		/ bx=sign/exp word of left op.
	mov	cx,6(di)		/ cx=sign/exp word of right op.
	test	bx,$expon		/ check if left is zero.
	jnz	L1			/ left nonzero.
	test	cx,$expon		/ check if right is zero.
	jz	L6			/ 0:0, return equal.
	orb	ch,ch			/ check sign of right.
	js	GT			/ 0:-, return greater.
LT:	dec	ax			/ 0:+, return less.
	jmp	L6			/ all done.

/ Left is nonzero.
L1:	orb	bh,bh			/ check left sign.
	js	L3			/ jump if negative.
	test	cx,$expon		/ check if right is zero.
	jnz	L2			/ jump if nonzero.
GT:	inc	ax			/ +:0, return greater.
	jmp	L6			/ all done.

/ Left is positive, right is nonzero.
L2:	orb	ch,ch			/ check sign of right.
	jns	L4			/ jump if both positive.
	jmp	GT			/ +:-, return greater.

/ Left is negative.
L3:	test	cx,$expon		/ check if right is zero.
	jz	LT			/ -:0, return less.
	orb	ch,ch			/ check sign of right op.
	jns	LT			/ -:+, return less.

/ Left and right both nonzero and both same sign.
L4:	add	di,$6			/ di=right op.
	lea	si,l+6(bp)		/ si=left  op.
	mov	cx,$4			/ cx=count (4 words)
	std				/ decrement.
	repe				/ do the
	cmpsw				/ long compare.
	cld				/ reset the increment flag.
	je	L6			/ matches, return equal.
	inc	ax			/ ax=1, cf=untouched.
	jae	L5			/ jump if correct assumption.
	neg	ax			/ less.

L5:	orb	bh,bh			/ were operands less than zero.
	jns	L6			/ jump if not.
	neg	ax			/ yes, flip sense.

L6:	pop	bp			/ standard
	pop	di			/ c
	pop	si			/ function
	ret				/ return

