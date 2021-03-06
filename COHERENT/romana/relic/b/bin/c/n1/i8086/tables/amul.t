/////////
/
/ Assigned multiply.
/ Not a lot of special cases. The fixed register requirements of
/ byte operations and the multiply instruction make it not very
/ necessary. Also handles fields.
/
/////////

AMUL:
%	PEFFECT|PRVALUE|PLVALUE|P80186
	WORD		ANYR	*	*	TEMP
		ADR|LV		WORD
		IMM|MMX		WORD
			[ZIMULI]	[R],[AL],[AR]
			[ZMOV]	[AL],[R]

%	PEFFECT|PRVALUE|PLVALUE
	WORD		DXAX	*	*	AX
		ADR|LV		FS16
		ADR		WORD
			[ZMOV]	[LO R],[AL]
			[ZIMUL]	[AR]
			[ZMOV]	[AL],[LO R]

%	PEFFECT|PRVALUE|PLVALUE
	WORD		DXAX	*	*	AX
		ADR|LV		UWORD
		ADR		WORD
			[ZMOV]	[LO R],[AL]
			[ZMUL]	[AR]
			[ZMOV]	[AL],[LO R]

%	PEFFECT|PRVALUE
	WORD		DXAX	*	*	AX
		ADR|LV		FFLD16
		ADR		WORD
			[ZMOV]	[LO R],[AL]
			[ZAND]	[LO R],[LO EMASK]
			[ZMUL]	[AR]
			[ZAND]	[LO R],[LO EMASK]
			[ZAND]	[AL],[LO CMASK]
			[ZOR]	[AL],[LO R]

%	PEFFECT|PRVALUE
	WORD		DXAX	*	*	AX
		ADR|LV		FS8
		ADR		WORD
			[ZMOVB]	[LO LO R],[AL]
			[ZCBW]
			[ZIMUL]	[AR]
			[ZMOVB]	[AL],[LO LO R]
		[IFV]	[ZCBW]

%	PEFFECT|PRVALUE
	WORD		DXAX	*	*	AX
		ADR|LV		FU8
		ADR		WORD
			[ZMOVB]	[LO LO R],[AL]
			[ZSUBB]	[HI LO R],[HI LO R]
			[ZMUL]	[AR]
			[ZMOVB]	[AL],[LO LO R]
		[IFV]	[ZSUBB]	[HI LO R],[HI LO R]

%	PEFFECT|PRVALUE
	WORD		DXAX	*	*	AX
		ADR|LV		FFLD8
		ADR		WORD
			[ZMOVB]	[LO LO R],[AL]
			[ZAND]	[LO R],[LO EMASK]	/ Clear AH too.
			[ZMUL]	[AR]
			[ZAND]	[LO R],[LO EMASK]
			[ZANDB]	[AL],[LO CMASK]
			[ZORB]	[AL],[LO LO R]

%	PEFFECT|PRVALUE
	LONG		DXAX	*	DXAX	DXAX
		ADR|LV		FS32
		TREG		LONG
			[ZPUSH]	[HI R]
			[ZPUSH]	[LO R]
			[ZPUSH]	[HI AL]
			[ZPUSH]	[LO AL]
			[CALL]	[GID lrmul]
			[ZADD]	[REGNO SP],[CONST 8]
			[ZMOV]	[LO AL],[LO R]
			[ZMOV]	[HI AL],[HI R]

%	PEFFECT|PRVALUE
	LONG		DXAX	*	DXAX	DXAX
		ADR|LV		FU32
		TREG	LONG
			[ZPUSH]	[HI R]
			[ZPUSH]	[LO R]
			[ZPUSH]	[HI AL]
			[ZPUSH]	[LO AL]
			[CALL]	[GID vrmul]
			[ZADD]	[REGNO SP],[CONST 8]
			[ZMOV]	[LO AL],[LO R]
			[ZMOV]	[HI AL],[HI R]

%	PEFFECT|PRVALUE
	LONG		DXAX	*	*	DXAX
		ADR|LV		FS32
		ADR		LONG
			[ZPUSH]	[HI AR]
			[ZPUSH]	[LO AR]
			[ZPUSH]	[HI AL]
			[ZPUSH]	[LO AL]
			[CALL]	[GID lrmul]
			[ZADD]	[REGNO SP],[CONST 8]
			[ZMOV]	[LO AL],[LO R]
			[ZMOV]	[HI AL],[HI R]

%	PEFFECT|PRVALUE
	LONG		DXAX	*	*	DXAX
		ADR|LV		FU32
		ADR	LONG
			[ZPUSH]	[HI AR]
			[ZPUSH]	[LO AR]
			[ZPUSH]	[HI AL]
			[ZPUSH]	[LO AL]
			[CALL]	[GID vrmul]
			[ZADD]	[REGNO SP],[CONST 8]
			[ZMOV]	[LO AL],[LO R]
			[ZMOV]	[HI AL],[HI R]

////////
/
/ Floating point, using the numeric
/ data coprocessor (8087).
/
////////

#ifdef NDPDEF
%	PEFFECT|PRVALUE
	FF32|FF64	FPAC	*	FPAC	FPAC
		ADR|LV		FF32
		TREG		FF64
			[ZFMULF] [AL]
		[IFV]	[ZFSTF] [AL]
		[IFE]	[ZFSTPF] [AL]

%	PEFFECT|PRVALUE
	FF32|FF64	FPAC	*	FPAC	FPAC
		ADR|LV		FF64
		TREG		FF64
			[ZFMULD] [AL]
		[IFV]	[ZFSTD]	[AL]
		[IFE]	[ZFSTPD] [AL]
#endif
