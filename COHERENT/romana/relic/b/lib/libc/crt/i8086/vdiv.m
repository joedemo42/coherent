//////////
/ i8086 C library runtime support.
/ vdiv()
/ Unsigned long division and remainder support.
//////////

#include "larges.h"

//////////
/ Returns  (dx ax) / (bx cx) in (si di)
/     and  (dx ax) % (bx cx) in (dx ax).
/ All are unsigned longs.
//////////

	.globl	vdiv

vdiv:	mov	si, bx
	or	si, cx
	mov	si, $-1		/ does not affect flags
	mov	di, si
	jne	0f
	sub	si, si
	mov	di, si		/ divisor = 0
	Gret

1:	shr	si, $1		/ "count" quotient bits
	rcr	di, $1
0:	cmp	dx, bx
	jb	1f
	shl	cx, $1		/ shift until big enough
	rcl	bx, $1
	jnc	1b
				/ oops!
3:	rcr	bx, $1		/ carry!=0  only if drop from above
	rcr	cx, $1
1:	sub	ax, cx
	sbb	dx, bx
	jnc	2f
	add	ax, cx
	adc	dx, bx		/ produces another carry
2:	rcl	di, $1		/ collect inverted quotient
	rcl	si, $1
	jnc	3b		/ use above "count"

	not	si
	not	di
	Gret

/ end of vdiv.m
