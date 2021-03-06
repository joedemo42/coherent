/
/	The  information  contained herein  is a trade secret  of INETCO
/	Systems, and is confidential information.   It is provided under
/	a license agreement,  and may be copied or disclosed  only under
/	the terms of that agreement.   Any reproduction or disclosure of
/	this  material  without  the express  written  authorization  of
/	INETCO Systems or persuant to the license agreement is unlawful.
/
/	Copyright (c) 1987
/	An unpublished work by INETCO Systems, Ltd.
/	All rights reserved.
/

////////
/
/ iAPX-286 Processor Reboot function
/
////////

KBDATA	=	0x60			/ 8042 keyboard mpu data I/O port.
KBCTRL	=	0x64			/ 8042 keyboard mpu ctrl I/O port.

	.globl	reboot_
reboot_:
	cli				/ Disable interrupts.
					/
0:	sub	cx, cx			/
	inb	al, KBCTRL		/ Wait for 8042 input buffer to empty.
	testb	al, $2			/
	loopne	0b			/
	jmp	.+2		/ DELAY /
					/
	movb	al, $0xD1		/ Request next output byte to be
	outb	KBCTRL, al		/	sent to the 8042 output port.
					/
	sub	cx, cx			/
0:	inb	al, KBCTRL		/ Wait for 8042 input buffer to empty.
	testb	al, $2			/
	loopne	0b			/
	jmp	.+2		/ DELAY /
					/
	movb	al, $0xDF		/ Enable A20 address line.
	outb	KBDATA, al		/ See Page 1-44, IBM-AT Tech Ref.
					/
	sub	cx, cx			/
0:	inb	al, KBCTRL		/ Wait for 8042 input buffer to empty.
	testb	al, $2			/
	loopne	0b			/
	jmp	.+2		/ DELAY /
	jmp	.+2		/ DELAY /
					/
	movb	al, $0xFE		/ Issue a shutdown command
	outb	KBCTRL, al		/ to the 8042 control port.
					/
0:	hlt				/ Halt until processor reset occurs.
	jmp	0b
