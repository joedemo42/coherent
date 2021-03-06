//////////
/ n1/i386/tables/shift.t
//////////

/////////
/
/ Shift left (<<) and shift right (>>).
/ Short shifts are done inline.
/ Longer shifts use 'shift by CL'.
/ [OP0] is ZSAL or ZSAR.
/ [TL OP0] maps to a signed or unsigned shift of the appropriate length.
/ The result of a shift by more than 32 bits is undefined behavior.
/ This table simply generates a shift by the arg,
/ and the i386 uses the shift counts modulo 32.
/
/////////

SHL:
SHR:

/ Long op immediate.
%	PEFFECT|PVALUE|PREL|P_SLT
	DWORD		ANYR	ANYR	*	TEMP
		TREG		DWORD
		SHCNT|MMX	LONG
			[TL OP0]	[R],[AR]
		[IFR]	[REL0]		[LAB]

/ Long op long.
/ This uses node temp EAX rather than ANYR to avoid using ECX.
%	PEFFECT|PVALUE|P_SLT
	DWORD		EAX	EAX	ECX	EAX
		TREG		DWORD
		ADR		LONG
			[ZMOV]		[REGNO ECX],[AR]
			[TL OP0]	[R],[REGNO CL]

//////////
/ end of n1/i386/tables/shift.t
//////////
