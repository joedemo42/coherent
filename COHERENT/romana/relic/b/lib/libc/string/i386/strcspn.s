//////////
/ libc/string/i386/strcspn.s
/ i386 C string library.
/ ANSI 4.11.5.3.
//////////

//////////
/ size_t
/ strcspn(char *String1, char *String2)
/
/ Return the length of the initial segment of String1
/ which consists of characters ->not<- in String2.
//////////

String1	.equ	12
String2	.equ	String1+4

	.globl	strcspn

strcspn:
	push	%esi
	push	%edi

	movl	%edi, String1(%esp)	/ String1 address to EDI
	movl	%edx, %edi		/ and save for length calculation
	cld

?fetch1:
	movb	%ah, (%edi)		/ String1 character to AH
	incl	%edi
	orb	%ah, %ah
	jz	?done			/ End of String1, done
	movl	%esi, String2(%esp)	/ String2 address to ESI

?fetch2:
	lodsb				/ String2 character to AL
	orb	%al, %al
	jz	?fetch1			/ Done with String2, fetch next from String1
	cmpb	%ah, %al
	jne	?fetch2			/ Mismatched, try next in String2

?done:
	decl	%edi			/ Back up to last String1 char
	movl	%eax, %edi
	subl	%eax, %edx		/ Return &last - &String1

	pop	%edi
	pop	%esi
	ret

/ end of libc/string/i386/strcspn.s
