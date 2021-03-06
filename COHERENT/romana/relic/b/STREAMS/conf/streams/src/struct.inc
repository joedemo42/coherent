/ Coherent 'as' macro for simplifying some aspects of structure and data type
/ declaration. This facility uses the rich macro facilities of the Coherent
/ 'as' assembler to ease interfacing of assembly language and C or C++, with
/ some limitations. Do not expect this to work with any other assembler.

/ It would have been nice to actually have a notion of scope, but the macro
/ facility in Coherent 'as', while better than most, is still an ad hoc kludge.
/ Now, something more along Scheme lines...

/ Limitations:
/	Only one structure may be declared at a time. Nested structures must
/	be defined separately (and thus, must be given names).
/		C		as
/	  struct foo {		.struct	_internal
/	    struct {		.ends	_internal
/	    } a;		.struct foo
/	  };			.member	a,_internal
/				.ends	foo

/ Every C/C++ data item that we might want to declare an instance of has some
/ special attributes; a type name, a size in bytes, and an alignment. An
/ instance of a given type will also typically have an address.

.extern		.macro		name,type
		.globl		name
SIZE.\name	.equ		SIZE..\type
ALIGN.\name	.equ		ALIGN..\type
		.endm

.declare	.macro		name,type
		.comm		name,SIZE..\type
SIZE.\name	.equ		SIZE..\type
ALIGN.\name	.equ		ALIGN..\type
		.endm

.deftype	.macro		name,size
SIZE..\name	.equ		size
ALIGN..\name	.equ		size
		.endm

.typedef	.macro		name,type
SIZE..\name	.equ		SIZE..\type
ALIGN..\name	.equ		ALIGN..\type
		.endm


		.deftype	char,1
		.deftype	uchar,1
		.deftype	short,2
		.deftype	ushort,2
		.deftype	int,4
		.deftype	uint,4
		.deftype	long,4
		.deftype	ulong,4
		.deftype	ptr,4

DEPTH		.equ		0

.struct		.macro		name
		.if		DEPTH
		.fail		Structure declarations cannot be nested
		.endi
NAME		.define		..\name
SIZE\NAME	.equ		0
ALIGN\NAME	.equ		0
DEPTH\NAME	.equ		1
DEPTH		.equ		DEPTH + 1
		.endm

.member		.macro		name,type
		.if		DEPTH != 1
		.fail		.member needs to be inside .struct
		.endi
		.if		.defined name
		.fail		member tag already in use
		.endi
name		.equ		[SIZE\NAME + ALIGN..\type - 1] & - ALIGN..\type
SIZE\NAME	.equ		name + SIZE..\type
		.if		ALIGN\NAME < ALIGN..\type
ALIGN\NAME	.equ		ALIGN..\type
		.endi
		.endm

/ Implementation note; under Coherent 'as', .define symbols built within macros
/ can never be removed, because lines beginning with the .define-suppression
/ indicator '#' are actually never processed despite what the documentation
/ says. Maybe this is how Charles "fixed" as for GCC.
/ [ This has been reported, so maybe this will be changed. ]

.ends		.macro		name
		.if		DEPTH != 1 | DEPTH != DEPTH..\name
		.fail		.ends needs to match .struct
		.endi
DEPTH..\name	.equ		0
DEPTH		.equ		DEPTH - 1
		.endm

