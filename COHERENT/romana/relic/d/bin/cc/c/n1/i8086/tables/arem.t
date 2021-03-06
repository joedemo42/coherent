/////////
/
/ Assigned remainder. This is just the same as divide except that the
/ result comes back in a different place. There is no need to do doubles
/ since remainder is not defined on them in the language. Fields get done
/ in the same fashon as they do on divide.
/
/////////

AREM:
%	PEFFECT|PRVALUE|PLVALUE
	WORD		DXAX	*	*	DX
		ADR|LV		FS16
		ADR		WORD
			[ZMOV]	[LO R],[AL]
			[ZCWD]
			[ZIDIV]	[AR]
			[ZMOV]	[AL],[HI R]

%	PEFFECT|PRVALUE|PLVALUE
	WORD		DXAX	*	*	DX
		ADR|LV		UWORD
		ADR		WORD
			[ZMOV]	[LO R],[AL]
			[ZSUB]	[HI R],[HI R]
			[ZDIV]	[AR]
			[ZMOV]	[AL],[HI R]

%	PEFFECT|PRVALUE
	WORD		DXAX	*	*	DX
		ADR|LV		FFLD16
		ADR		WORD
			[ZMOV]	[LO R],[AL]
			[ZAND]	[LO R],[LO EMASK]
			[ZSUB]	[HI R],[HI R]
			[ZDIV]	[AR]
			[ZAND]	[HI R],[LO EMASK]
			[ZAND]	[AL],[LO CMASK]
			[ZOR]	[AL],[HI R]

%	PEFFECT|PRVALUE
	WORD		DXAX	*	*	AX
		ADR|LV		FS8
		ADR		WORD
			[ZMOVB]	[LO LO R],[AL]
			[ZCBW]
			[ZCWD]
			[ZIDIV]	[AR]
			[ZMOVB]	[AL],[LO HI R]
		[IFV]	[ZMOVB]	[LO LO R],[LO HI R]
		[IFV]	[ZCBW]

%	PEFFECT|PRVALUE
	WORD		DXAX	*	*	DX
		ADR|LV		FU8
		ADR		WORD
			[ZMOVB]	[LO LO R],[AL]
			[ZSUBB]	[HI LO R],[HI LO R]
			[ZCWD]
			[ZDIV]	[AR]
			[ZMOVB]	[AL],[LO HI R]
		[IFV]	[ZSUBB]	[HI HI R],[HI HI R]

%	PEFFECT|PRVALUE
	WORD		DXAX	*	*	DX
		ADR|LV		FFLD8
		ADR		WORD
			[ZMOVB]	[LO LO R],[AL]
			[ZAND]	[LO R],[LO EMASK]	/ Clear AH.
			[ZSUB]	[HI R],[HI R]
			[ZDIV]	[AR]
			[ZAND]	[HI R],[LO EMASK]
			[ZANDB]	[AL],[LO CMASK]
			[ZORB]	[AL],[LO HI R]		/ DH.

%	PEFFECT|PRVALUE
	LONG		DXAX	*	*	DXAX
		ADR|LV		FS32
		ADR		LONG
			[ZPUSH]	[HI AR]
			[ZPUSH]	[LO AR]
			[ZPUSH]	[HI AL]
			[ZPUSH]	[LO AL]
			[CALL]	[GID lrrem]
			[ZADD]	[REGNO SP],[CONST 8]
			[ZMOV]	[LO AL],[LO R]
			[ZMOV]	[HI AL],[HI R]

%	PEFFECT|PRVALUE
	LONG		DXAX	*	*	DXAX
		ADR|LV		FU32
		ADR		LONG
			[ZPUSH]	[HI AR]
			[ZPUSH]	[LO AR]
			[ZPUSH]	[HI AL]
			[ZPUSH]	[LO AL]
			[CALL]	[GID vrrem]
			[ZADD]	[REGNO SP],[CONST 8]
			[ZMOV]	[LO AL],[LO R]
			[ZMOV]	[HI AL],[HI R]
