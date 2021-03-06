/ (lgl-
/ 	COHERENT Driver Kit Version 1.1.0
/ 	Copyright (c) 1982, 1990 by Mark Williams Company.
/ 	All rights reserved. May not be copied without permission.
/ -lgl)
////////
/
/ Raw Serial Device Driver - Assembler Support
/
////////

////////
/
/ Locally defined global symbols
/
////////

	.globl	rsin_
	.globl	rsout_

////////
/
/ Offsets to fields in the IRING and ORING data structures.
/
////////

	Q_MASK	= 0
	Q_IX	= 2
	Q_OX	= 4
	Q_CC	= 6

////////
/
/ Offsets to fields in the IO data structure.
/
////////

	IO_IOC	= 2
	IO_BASE	= 8

////////
/
/ Rsin ( rawqp, iop )	-- transfer data from input ring to user
/ IRING *rawqp;
/ IO *iop;
/
////////

rsin_:
	push	si				/ Save SI, DI, BP, ES
	push	di
	push	bp
	mov	bp, sp
	push	es

	mov	bx, 10(bp)			/ User destination --> ES:DI
	mov	di, IO_BASE(bx)
	mov	es, uds_

	cld					/ Auto Increment
	mov	cx, IO_IOC(bx)			/ Byte count --> CX
	jcxz	1f

	mov	bx, 8(bp)			/ rawqp --> BX

	mov	si, Q_OX(bx)
	cmp	si, Q_IX(bx)			/ Input data available?
	je	1f

0:	movb	al, Q_CC(bx,si)			/ Yes,	read one character
	inc	si				/	update index
	and	si, Q_MASK(bx)			/	(wrap if necessary)
	stosb					/	write to user space
	cmp	si, Q_IX(bx)			/	More input data?
	loopne	0b				/

	mov	Q_OX(bx), si			/ Save revised index
	mov	bx, 10(bp)			/ Update io parameters
	mov	IO_BASE(bx), di
	mov	IO_IOC(bx), cx

1:	pop	es				/ Restore ES, BP, DI, SI.
	pop	bp
	pop	di
	pop	si
	ret

////////
/
/ Rsout( outqp, iop )	-- transfer data from user to output ring
/ ORING *outqp;
/ IO *iop;
/
////////

rsout_:
	push	si				/ Save SI, DI, BP, ES
	push	di
	push	bp
	mov	bp, sp
	push	es

	mov	bx, 10(bp)			/ User source --> ES:DI
	mov	di, IO_BASE(bx)
	mov	es, uds_

	mov	cx, IO_IOC(bx)			/ Byte count --> CX
	jcxz	2f

	mov	bx, 8(bp)			/ outqp --> BX
	mov	si, Q_IX(bx)

0:	movb	al, es:(di)
	inc	di
	movb	Q_CC(bx,si), al
	inc	si
	and	si, Q_MASK(bx)
	cmp	si, Q_OX(bx)
	loopne	0b

	jne	1f				/ If can't save last char
	dec	di				/	Undo changes.
	dec	si
	and	si, Q_MASK(bx)
	inc	cx

1:	mov	Q_IX(bx), si			/ Save revised index
	mov	bx, 10(bp)
	mov	IO_BASE(bx), di			/ Update io parameters
	mov	IO_IOC(bx), cx

2:	pop	es				/ Restore ES, BP, DI, SI.
	pop	bp
	pop	di
	pop	si
	ret
