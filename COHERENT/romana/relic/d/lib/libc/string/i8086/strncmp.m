//////////
/ i8086 C string library.
/ strncmp()
/ ANSI 4.11.4.4.
//////////

//////////
/ strncmp(String1, String2, Count)
/ char *String1, *String2;
/ int Count;
/
/ Compare up to Count bytes of String1 and String2.
//////////

#include <larges.h>

String1	=	LEFTARG
String2	=	String1+DPL
Count	=	String2+DPL

	Enter(strncmp_)
	mov	dx, $0		/ Result to DX
	mov	cx, Count(bp)	/ Count to CX
	or	cx, cx
	je	4f		/ Zero length, return equal
	Lds	si, String2(bp)	/ String2 address to DS:SI
	Les	di, String1(bp)	/ String1 address to ES:DI
	cld

1:	lodsb			/ String2 character to AL
	scasb			/ and compare to String1 character
	jne	2f		/ Mismatch
	orb	al, al
	loopne	1b		/ Continue if CX!=0 && AL!=0
	jmp	4f		/ Match, return 0

2:	ja	3f
	inc	dx		/ String1 > String2, return 1
	jmp	4f

3:	dec	dx		/ String1 < String2, return -1

4:	mov	ax, dx		/ Result to AX
	Leave
