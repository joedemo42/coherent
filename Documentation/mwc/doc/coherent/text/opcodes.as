37	\fBaaa\fR		Adjust after addition
D5 0A	\fBaad\fR		Adjust AX before division
D4 0A	\fBaam\fR		Adjust AX after multiply
3F	\fBaas\fR		Adjust AL after subtraction
.sp \n(pDu
	\fBadc\fR		Add with carry
83 /2	\fBadcl\fR	\fIimm8s,rm32\fR
83 /2	\fBadcw\fR	\fIimm8s,rm16\fR
14	\fBadcb\fR	\fIimm8,al\fR
15	\fBadcw\fR	\fIimm16,ax\fR
15	\fBadcl\fR	\fIimm32,eax\fR
15	\fBadcl\fR	\fIimm32\fR
80 /2	\fBadcb\fR	\fIimm8,rm8\fR
81 /2	\fBadcw\fR	\fIimm16,rm16\fR
81 /2	\fBadcl\fR	\fIimm32,rm32\fR
12 /r	\fBadcb\fR	\fIrm8,r8\fR
13 /r	\fBadcw\fR	\fIrm16,r16\fR
13 /r	\fBadcl\fR	\fIrm32,r32\fR
10 /r	\fBadcb\fR	\fIr8,rm8\fR
11 /r	\fBadcw\fR	\fIr16,rm16\fR
11 /r	\fBadcl\fR	\fIr32,rm32\fR
.sp \n(pDu
	\fBadd\fR		Add
83 /0	\fBaddl\fR	\fIimm8s,rm32\fR
83 /0	\fBaddw\fR	\fIimm8s,rm16\fR
04	\fBaddb\fR	\fIimm8,al\fR
05	\fBaddw\fR	\fIimm16,ax\fR
05	\fBaddl\fR	\fIimm32,eax\fR
05	\fBaddl\fR	\fIimm32\fR
80 /0	\fBaddb\fR	\fIimm8,rm8\fR
81 /0	\fBaddw\fR	\fIimm16,rm16\fR
81 /0	\fBaddl\fR	\fIimm32,rm32\fR
02 /r	\fBaddb\fR	\fIrm8,r8\fR
03 /r	\fBaddw\fR	\fIrm16,r16\fR
03 /r	\fBaddl\fR	\fIrm32,r32\fR
00 /r	\fBaddb\fR	\fIr8,rm8\fR
01 /r	\fBaddw\fR	\fIr16,rm16\fR
01 /r	\fBaddl\fR	\fIr32,rm32\fR
.sp \n(pDu
	\fBand\fR		Logical AND
83 /4	\fBandl\fR	\fIimm8s,rm32\fR
83 /4	\fBandw\fR	\fIimm8s,rm16\fR
24	\fBandb\fR	\fIimm8,al\fR
25	\fBandw\fR	\fIimm16,ax\fR
25	\fBandl\fR	\fIimm32,eax\fR
25	\fBandl\fR	\fIimm32\fR
80 /4	\fBandb\fR	\fIimm8,rm8\fR
81 /4	\fBandw\fR	\fIimm16,rm16\fR
81 /4	\fBandl\fR	\fIimm32,rm32\fR
22 /r	\fBandb\fR	\fIrm8,r8\fR
23 /r	\fBandw\fR	\fIrm16,r16\fR
23 /r	\fBandl\fR	\fIrm32,r32\fR
20 /r	\fBandb\fR	\fIr8,rm8\fR
21 /r	\fBandw\fR	\fIr16,rm16\fR
21 /r	\fBandl\fR	\fIr32,rm32\fR
.sp \n(pDu
63 /r	\fBarpl\fR	\fIr16,rm16\fR	Adjust RPL field of selector
.sp \n(pDu
	\fBbound\fR		Check if register is within bounds
62 /r	\fBboundw\fR	\fIm16,r16\fR
62 /r	\fBboundl\fR	\fIm32,r32\fR
.sp \n(pDu
	\fBbsf\fR		Bit scan forward
0F BC	\fBbsfw\fR	\fIrm16,r16\fR
0F BC	\fBbsfl\fR	\fIrm32,r32\fR
.sp \n(pDu
	\fBbsr\fR		Bit scan reverse
0F BD	\fBbsrw\fR	\fIrm16,r16\fR
0F BD	\fBbsrl\fR	\fIrm32,r32\fR
.sp \n(pDu
	\fBbt\fR		Save bit in carry flag
0F A3	\fBbtw\fR	\fIr16,rm16\fR
0F A3	\fBbtl\fR	\fIr32,rm32\fR
0F BA /4	\fBbtw\fR	\fIimm8,rm16\fR
0F BA /4	\fBbtl\fR	\fIimm8,rm32\fR
.sp \n(pDu
	\fBbtc\fR		Bit test and complement
0F BB	\fBbtcw\fR	\fIr16,rm16\fR
0F BB	\fBbtcl\fR	\fIr32,rm32\fR
0F BA /7	\fBbtcw\fR	\fIimm8,rm16\fR
0F BA /7	\fBbtcl\fR	\fIimm8,rm32\fR
.sp \n(pDu
	\fBbtr\fR		Bit test and reset
0F B3	\fBbtrw\fR	\fIr16,rm16\fR
0F B3	\fBbtrl\fR	\fIr32,rm32\fR
0F BA /6	\fBbtrw\fR	\fIimm8,rm16\fR
0F BA /6	\fBbtrl\fR	\fIimm8,rm32\fR
.sp \n(pDu
	\fBbts\fR		Bit test and set
0F AB	\fBbtsw\fR	\fIr16,rm16\fR
0F AB	\fBbtsl\fR	\fIr32,rm32\fR
0F BA /5	\fBbtsw\fR	\fIimm8,rm16\fR
0F BA /5	\fBbtsl\fR	\fIimm8,rm32\fR
.sp \n(pDu
E8	\fBcall\fR	\fIreli\fR	Call Procedure
98	\fBcbtw\fR		Sign extend AL
98	\fBcbw\fR		Sign extend AL
99	\fBcdq\fR		Double word to quad word
F8	\fBclc\fR		Clear carry
FC	\fBcld\fR		Clear direction Flag
FA	\fBcli\fR		Clear interrupt Flag
99	\fBcltd\fR		Double word to quad word
0F 06	\fBclts\fR		Clear task-switched flag in CR0
F5	\fBcmc\fR		Complement carry flag
.sp \n(pDu
	\fBcmp\fR		Compare
83 /7	\fBcmpl\fR	\fIimm8s,rm32\fR
83 /7	\fBcmpw\fR	\fIimm8s,rm16\fR
3C	\fBcmpb\fR	\fIimm8,al\fR
3D	\fBcmpw\fR	\fIimm16,ax\fR
3D	\fBcmpl\fR	\fIimm32,eax\fR
3D	\fBcmpl\fR	\fIimm32\fR
80 /7	\fBcmpb\fR	\fIimm8,rm8\fR
81 /7	\fBcmpw\fR	\fIimm16,rm16\fR
81 /7	\fBcmpl\fR	\fIimm32,rm32\fR
3A /r	\fBcmpb\fR	\fIrm8,r8\fR
3B /r	\fBcmpw\fR	\fIrm16,r16\fR
3B /r	\fBcmpl\fR	\fIrm32,r32\fR
38 /r	\fBcmpb\fR	\fIr8,rm8\fR
39 /r	\fBcmpw\fR	\fIr16,rm16\fR
39 /r	\fBcmpl\fR	\fIr32,rm32\fR
.sp \n(pDu
A6	\fBcmpsb\fR		Compare bytes
A7	\fBcmpsl\fR		Compare long
A7	\fBcmpsw\fR		Compare words
99	\fBcwd\fR		Word to double word
98	\fBcwde\fR		Sign extend AX
99	\fBcwtd\fR		Word to double word
98	\fBcwtl\fR		Sign extend AX
27	\fBdaa\fR		Decimal adjust after addition
2F	\fBdas\fR		Decimal adjust after subtraction
.sp \n(pDu
	\fBdec\fR		Decrement by 1
48 +r	\fBdecw\fR	\fIr16\fR
48 +r	\fBdecl\fR	\fIr32\fR
FE /1	\fBdecb\fR	\fIrm8\fR
FF /1	\fBdecw\fR	\fIrm16\fR
FF /1	\fBdecl\fR	\fIrm32\fR
.sp \n(pDu
	\fBdiv\fR		Unsigned divide
F6 /6	\fBdivb\fR	\fIrm8,al\fR
F6 /6	\fBdivb\fR	\fIrm8\fR
F7 /6	\fBdivw\fR	\fIrm16,ax\fR
F7 /6	\fBdivw\fR	\fIrm16\fR
F7 /6	\fBdivl\fR	\fIrm32,eax\fR
F7 /6	\fBdivl\fR	\fIrm32\fR
.sp \n(pDu
C8	\fBenter\fR	\fIimm8,imm16\fR	Make stack frame for procedure
D9 F0	\fBf2xm1\fR		ST = 2 ** ST - 1
D9 E1	\fBfabs\fR		ST = abs(ST)
.sp \n(pDu
	\fBfadd\fR		Floating add
D8 /0	\fBfadds\fR	\fIm32\fR
DC /0	\fBfaddl\fR	\fIm64\fR
D8 C0 +r	\fBfadd\fR	\fIfpreg,st0\fR
D8 C0 +r	\fBfadd\fR	\fIfpreg\fR
DE C1	\fBfadd\fR
DC C0 +r	\fBfadd\fR	\fIst0,fpreg\fR
.sp \n(pDu
	\fBfaddp\fR		Floating add and pop
DE C0 +r	\fBfaddp\fR	\fIst0,fpreg\fR
DE C0 +r	\fBfaddp\fR	\fIfpreg\fR
DE C1	\fBfaddp\fR
.sp \n(pDu
DF /4	\fBfbld\fR	\fIm80\fR	Load Binary Coded Decimal
DF /6	\fBfbstp\fR	\fIm80\fR	Store Binary Coded Decimal and Pop
D9 E0	\fBfchs\fR		Change Floating Sign
9B DB E2	\fBfclex\fR		Clear floating point exception flags
.sp \n(pDu
	\fBfcom\fR		Floating Compare
D8 /2	\fBfcoms\fR	\fIm32\fR
DC /2	\fBfcoml\fR	\fIm64\fR
D8 D0 +r	\fBfcom\fR	\fIfpreg,st0\fR
D8 D0 +r	\fBfcom\fR	\fIfpreg\fR
D8 D1	\fBfcom\fR
.sp \n(pDu
	\fBfcomp\fR		Floating Compare and Pop
D8 /3	\fBfcomps\fR	\fIm32\fR
DC /3	\fBfcompl\fR	\fIm64\fR
D8 D8 +r	\fBfcomp\fR	\fIfpreg\fR
D8 D9	\fBfcomp\fR
.sp \n(pDu
DE D9	\fBfcompp\fR		Floating Compare and pop twice
D9 FF	\fBfcos\fR		Cosine
D9 F6	\fBfdecstp\fR		Decrement Stack Top Pointer
.sp \n(pDu
	\fBfdiv\fR		Floating divide
D8 /6	\fBfdivs\fR	\fIm32\fR
DC /6	\fBfdivl\fR	\fIm64\fR
D8 F0 +r	\fBfdiv\fR	\fIfpreg,st0\fR
D8 F0 +r	\fBfdiv\fR	\fIfpreg\fR
DE F1	\fBfdiv\fR
DC F0 +r	\fBfdiv\fR	\fIst0,fpreg\fR
.sp \n(pDu
	\fBfdivp\fR		Floating divide and pop
DE F0 +r	\fBfdivp\fR	\fIst0,fpreg\fR
DE F0 +r	\fBfdivp\fR	\fIfpreg\fR
DE F1	\fBfdivp\fR
.sp \n(pDu
	\fBfdivr\fR		Reverse floating divide
D8 /7	\fBfdivrs\fR	\fIm32\fR
DC /7	\fBfdivrl\fR	\fIm64\fR
D8 F8 +r	\fBfdivr\fR	\fIfpreg,st0\fR
D8 F8 +r	\fBfdivr\fR	\fIfpreg\fR
DE F9	\fBfdivr\fR
DC F8 +r	\fBfdivr\fR	\fIst0,fpreg\fR
.sp \n(pDu
	\fBfdivrp\fR		Reverse floating divide and pop
DE F8 +r	\fBfdivrp\fR	\fIst0,fpreg\fR
DE F8 +r	\fBfdivrp\fR	\fIfpreg\fR
DE F9	\fBfdivrp\fR
.sp \n(pDu
DD C0 +r	\fBffree\fR	\fIfpreg\fR	Free Floating Point Register
.sp \n(pDu
	\fBfiadd\fR		Add integer to float
DA /0	\fBfiaddl\fR	\fIm32\fR
DE /0	\fBfiadds\fR	\fIm16\fR
.sp \n(pDu
	\fBficom\fR		Compare float to integer
DA /2	\fBficoml\fR	\fIm32\fR
DE /2	\fBficoms\fR	\fIm16\fR
.sp \n(pDu
	\fBficomp\fR		Compare float to integer and pop
DA /3	\fBficompl\fR	\fIm32\fR
DE /3	\fBficomps\fR	\fIm16\fR
.sp \n(pDu
	\fBfidiv\fR		Divide float by integer
DA /6	\fBfidivl\fR	\fIm32\fR
DE /6	\fBfidivs\fR	\fIm16\fR
.sp \n(pDu
	\fBfidivr\fR		Reverse divide integer by float
DA /7	\fBfidivrl\fR	\fIm32\fR
DE /7	\fBfidivrs\fR	\fIm16\fR
.sp \n(pDu
	\fBfild\fR		Load integer
DB /0	\fBfildl\fR	\fIm32\fR
DF /0	\fBfilds\fR	\fIm16\fR
DF /5	\fBfildll\fR	\fIm64\fR
.sp \n(pDu
	\fBfimul\fR		Multiply integer to float
DA /1	\fBfimull\fR	\fIm32\fR
DE /1	\fBfimuls\fR	\fIm16\fR
.sp \n(pDu
D9 F7	\fBfincstp\fR		Increment Stack Top Pointer
9B DB E3	\fBfinit\fR		Initialize Floating Point Unit
.sp \n(pDu
	\fBfist\fR		Store integer
DB /2	\fBfistl\fR	\fIm32\fR
DF /2	\fBfists\fR	\fIm16\fR
.sp \n(pDu
	\fBfistp\fR		Store integer and pop
DB /3	\fBfistpl\fR	\fIm32\fR
DF /3	\fBfistps\fR	\fIm16\fR
DF /7	\fBfistpll\fR	\fIm32\fR
.sp \n(pDu
	\fBfisub\fR		Subtract integer from float
DA /4	\fBfisubl\fR	\fIm32\fR
DE /4	\fBfisubs\fR	\fIm16\fR
.sp \n(pDu
	\fBfisubr\fR		Reverse subtract integer from float
DA /5	\fBfisubrl\fR	\fIm32\fR
DE /5	\fBfisubrs\fR	\fIm16\fR
.sp \n(pDu
	\fBfld\fR		Load Real
D9 C0 +r	\fBfld\fR	\fIfpreg\fR
D9 /0	\fBflds\fR	\fIm32\fR
DD /0	\fBfldl\fR	\fIm32\fR
DB /5	\fBfldt\fR	\fIm64\fR
.sp \n(pDu
D9 E8	\fBfld1\fR		Load Constant 1
D9 /5	\fBfldcw\fR	\fIm32\fR	Load Floating Point Control Word
D9 /4	\fBfldenv\fR	\fIm32\fR	Load FPU Environment
D9 EA	\fBfldl2e\fR		Load Constant log(e) base 2
D9 E9	\fBfldl2t\fR		Load Constant log(10) base 2
D9 EC	\fBfldlg2\fR		Load Constant log(2) base 10
D9 ED	\fBfldln2\fR		Load Constant log(2) base e
D9 EB	\fBfldpi\fR		Load Constant pi
D9 EE	\fBfldz\fR		Load Constant 0.0
.sp \n(pDu
	\fBfmul\fR		Floating multiply
D8 /1	\fBfmuls\fR	\fIm32\fR
DC /1	\fBfmull\fR	\fIm64\fR
D8 C8 +r	\fBfmul\fR	\fIfpreg,st0\fR
D8 C8 +r	\fBfmul\fR	\fIfpreg\fR
DE C9	\fBfmul\fR
DC C8 +r	\fBfmul\fR	\fIst0,fpreg\fR
.sp \n(pDu
	\fBfmulp\fR		Floating multiply and pop
DE C8 +r	\fBfmulp\fR	\fIst0,fpreg\fR
DE C8 +r	\fBfmulp\fR	\fIfpreg\fR
DE C9	\fBfmulp\fR
.sp \n(pDu
DB E2	\fBfnclex\fR		Clear floating point exception flags no wait
DB E3	\fBfninit\fR		Initialize Floating Point Unit no wait
D9 D0	\fBfnop\fR		No Operation
DD /6	\fBfnsave\fR	\fIm32\fR	Store FPU State no wait
D9 /7	\fBfnstcw\fR	\fIm32\fR	Store Control Word no wait
D9 /6	\fBfnstenv\fR	\fIm32\fR	Store FPU Environment no wait
.sp \n(pDu
	\fBfnstsw\fR		Store Status Word no wait
DD /7	\fBfnstsw\fR	\fIm16\fR
DF E0	\fBfnstsw\fR	\fIax\fR
.sp \n(pDu
D9 F3	\fBfpatan\fR		Partial Arctangent
D9 F8	\fBfprem\fR		Partial Remainder toward 0
D9 F5	\fBfprem1\fR		Partial Remainder < 1/2 modulus
D9 F2	\fBfptan\fR		Partial Tangent
D9 FC	\fBfrndint\fR		Round To Integer
DD /4	\fBfrstor\fR	\fIm32\fR	Resore FPU State
DB F4	\fBfrstpm\fR		set 287XL real mode (nop for 387/486)
9B DD /6	\fBfsave\fR	\fIm32\fR	Store FPU State
D9 FD	\fBfscale\fR		Scale
DB E4	\fBfsetpm\fR		set 287 protected mode (nop for 387/486)
D9 FE	\fBfsin\fR		Sine
D9 FB	\fBfsincos\fR		Sine and Cosine
D9 FA	\fBfsqrt\fR		Square Root
.sp \n(pDu
	\fBfst\fR		Store Real
DD D0 +r	\fBfst\fR	\fIfpreg\fR
D9 /2	\fBfsts\fR	\fIm32\fR
DD /2	\fBfstl\fR	\fIm64\fR
.sp \n(pDu
9B D9 /7	\fBfstcw\fR	\fIm32\fR	Store Control Word
9B D9 /6	\fBfstenv\fR	\fIm32\fR	Store FPU Environment
.sp \n(pDu
	\fBfstp\fR		Store Real and pop
DD D8 +r	\fBfstp\fR	\fIfpreg\fR
D9 /3	\fBfstps\fR	\fIm32\fR
DD /3	\fBfstpl\fR	\fIm64\fR
DB /7	\fBfstpt\fR	\fIm80\fR
.sp \n(pDu
	\fBfstsw\fR		Store Status Word
9B DD /7	\fBfstsw\fR	\fIm16\fR
9B DF E0	\fBfstsw\fR	\fIax\fR
.sp \n(pDu
	\fBfsub\fR		Floating subtract
D8 /4	\fBfsubs\fR	\fIm32\fR
DC /4	\fBfsubl\fR	\fIm64\fR
D8 E0 +r	\fBfsub\fR	\fIfpreg,st0\fR
D8 E0 +r	\fBfsub\fR	\fIfpreg\fR
DE E1	\fBfsub\fR
DC E0 +r	\fBfsub\fR	\fIst0,fpreg\fR
.sp \n(pDu
	\fBfsubp\fR		Floating subtract and pop
DE E0 +r	\fBfsubp\fR	\fIst0,fpreg\fR
DE E0 +r	\fBfsubp\fR	\fIfpreg\fR
DE E1	\fBfsubp\fR
.sp \n(pDu
	\fBfsubr\fR		Reverse floating subtract
D8 /5	\fBfsubrs\fR	\fIm32\fR
DC /5	\fBfsubrl\fR	\fIm64\fR
D8 E8 +r	\fBfsubr\fR	\fIfpreg,st0\fR
D8 E8 +r	\fBfsubr\fR	\fIfpreg\fR
DE E9	\fBfsubr\fR
DC E8 +r	\fBfsubr\fR	\fIst0,fpreg\fR
.sp \n(pDu
	\fBfsubrp\fR		Reverse floating subtract and pop
DE E8 +r	\fBfsubrp\fR	\fIst0,fpreg\fR
DE E8 +r	\fBfsubrp\fR	\fIfpreg\fR
DE E9	\fBfsubrp\fR
.sp \n(pDu
D9 E4	\fBftst\fR		Test
.sp \n(pDu
	\fBfucom\fR		Unordered compare real
DD E0 +r	\fBfucom\fR	\fIst0,fpreg\fR
DD E0 +r	\fBfucom\fR	\fIfpreg\fR
DD E1	\fBfucom\fR
.sp \n(pDu
	\fBfucomp\fR		Unordered compare real and pop
DD E8 +r	\fBfucomp\fR	\fIst0,fpreg\fR
DD E8 +r	\fBfucomp\fR	\fIfpreg\fR
DD E9	\fBfucomp\fR
.sp \n(pDu
DA E9	\fBfucompp\fR		Unordered compare %st %st1 and pop twice
9B	\fBfwait\fR		Wait for coprocessor
D9 E5	\fBfxam\fR		Examine
.sp \n(pDu
	\fBfxch\fR		Floating exchange
D9 C8 +r	\fBfxch\fR	\fIst0,fpreg\fR
D9 C8 +r	\fBfxch\fR	\fIfpreg,st0\fR
D9 C8 +r	\fBfxch\fR	\fIfpreg\fR
D9 C9	\fBfxch\fR
.sp \n(pDu
D9 F4	\fBfxtract\fR		Extract Exponent and Significand
D9 F1	\fBfyl2x\fR		%st1 * log(%st) base 2
D9 F9	\fBfyl2xp1\fR		%st1 * log(%st + 1.0) base 2
F4	\fBhlt\fR		Halt
FF /2	\fBicall\fR	\fIrm32\fR	Call indirect
.sp \n(pDu
	\fBidiv\fR		Signed divide
F6 /7	\fBidivb\fR	\fIrm8,al\fR
F6 /7	\fBidivb\fR	\fIrm8\fR
F7 /7	\fBidivw\fR	\fIrm16,ax\fR
F7 /7	\fBidivw\fR	\fIrm16\fR
F7 /7	\fBidivl\fR	\fIrm32,eax\fR
F7 /7	\fBidivl\fR	\fIrm32\fR
.sp \n(pDu
FF /4	\fBijmp\fR	\fIrm32\fR	Jump indirect
FF /3	\fBilcall\fR	\fIm32\fR	Long call indirect
FF /5	\fBiljmp\fR	\fIm32\fR	Long jump indirect
.sp \n(pDu
	\fBimul\fR		Signed multiply
F6 /5	\fBimulb\fR	\fIrm8,al\fR
F6 /5	\fBimulb\fR	\fIrm8\fR
F7 /5	\fBimulw\fR	\fIrm16,ax\fR
F7 /5	\fBimulw\fR	\fIrm16\fR
F7 /5	\fBimull\fR	\fIrm32,eax\fR
F7 /5	\fBimull\fR	\fIrm32\fR
0F AF /r	\fBimulw\fR	\fIrm16,r16\fR
0F AF /r	\fBimull\fR	\fIrm32,r32\fR
6B	\fBimulw\fR	\fIimm8s,rm16,r16\fR
6B	\fBimull\fR	\fIimm8s,rm32,r32\fR
6B /r	\fBimulw\fR	\fIimm8s,r16\fR
6B /r	\fBimull\fR	\fIimm8s,r32\fR
69	\fBimulw\fR	\fIimm16,rm16,r16\fR
69	\fBimull\fR	\fIimm32,rm32,r32\fR
69 /r	\fBimulw\fR	\fIimm16,r16\fR
69 /r	\fBimull\fR	\fIimm32,r32\fR
.sp \n(pDu
	\fBin\fR		Input from port
E4	\fBinb\fR	\fIimm8\fR
E5	\fBinw\fR	\fIimm8\fR
E5	\fBinl\fR	\fIimm8\fR
EC	\fBinb\fR	\fI(dx)\fR
ED	\fBinw\fR	\fI(dx)\fR
ED	\fBinl\fR	\fI(dx)\fR
.sp \n(pDu
	\fBinc\fR		Increment by one
40 +r	\fBincw\fR	\fIr16\fR
40 +r	\fBincl\fR	\fIr32\fR
FE /0	\fBincb\fR	\fIrm8\fR
FF /0	\fBincw\fR	\fIrm16\fR
FF /0	\fBincl\fR	\fIrm32\fR
.sp \n(pDu
6C	\fBinsb\fR		Input byte from port into ES:(E)DI
6C	\fBinsb\fR	\fI(dx)\fR	Input byte from port into ES:(E)DI
6D	\fBinsl\fR		Input long from port into ES:(E)DI
6D	\fBinsl\fR	\fI(dx)\fR	Input long from port into ES:(E)DI
6D	\fBinsw\fR		Input word from port into ES:(E)DI
6D	\fBinsw\fR	\fI(dx)\fR	Input word from port into ES:(E)DI
CC	\fBint\fR	\fIcon3\fR	Interrupt 3
CD	\fBint\fR	\fIimm8\fR	Interrupt
CE	\fBinto\fR		Int 4 if overflow is 1
CF	\fBiret\fR		Interrupt return
CF	\fBiretd\fR		Different mode different opcode ?
07	\fBja\fR	\fIreli\fR	Jump if above
	
03	\fBjae\fR	\fIreli\fR	Jump if above or equal
02	\fBjb\fR	\fIreli\fR	Jump if below
06	\fBjbe\fR	\fIreli\fR	Jump if below or equal
02	\fBjc\fR	\fIreli\fR	Jump if carry
E3	\fBjcxz\fR	\fIrel8\fR	Jump if CX is zero
04	\fBje\fR	\fIreli\fR	Jump if equal
E3	\fBjecxz\fR	\fIrel8\fR	Jump if CX is zero
	
0F	\fBjg\fR	\fIreli\fR	Jump if greater
0D	\fBjge\fR	\fIreli\fR	Jump if greater or equal
0C	\fBjl\fR	\fIreli\fR	Jump if less
0E	\fBjle\fR	\fIreli\fR	Jump if less or equal
E9	\fBjmp\fR	\fIreli\fR	Jump absolute
06	\fBjna\fR	\fIreli\fR	Jump if not above
02	\fBjnae\fR	\fIreli\fR	Jump if not above or equal
03	\fBjnb\fR	\fIreli\fR	Jump if not below
07	\fBjnbe\fR	\fIreli\fR	Jump if not below or equal
03	\fBjnc\fR	\fIreli\fR	Jump if no carry
05	\fBjne\fR	\fIreli\fR	Jump if not equal
0E	\fBjng\fR	\fIreli\fR	Jump if not greater
0C	\fBjnge\fR	\fIreli\fR	Jump if not greater or equal
0D	\fBjnl\fR	\fIreli\fR	Jump if not less
0F	\fBjnle\fR	\fIreli\fR	Jump if not less or equal
01	\fBjno\fR	\fIreli\fR	Jump if not overflow
0B	\fBjnp\fR	\fIreli\fR	Jump if not parity
09	\fBjns\fR	\fIreli\fR	Jump if not sign
05	\fBjnz\fR	\fIreli\fR	Jump if not zero
00	\fBjo\fR	\fIreli\fR	Jump if overflow
0A	\fBjp\fR	\fIreli\fR	Jump if parity
0A	\fBjpe\fR	\fIreli\fR	Jump if parity even
0B	\fBjpo\fR	\fIreli\fR	Jump if parity odd
08	\fBjs\fR	\fIreli\fR	Jump if sign
04	\fBjz\fR	\fIreli\fR	Jump if zero
04	\fBjz\fR	\fIreli\fR	Jump if zero
9F	\fBlahf\fR		Load flags into AH register
.sp \n(pDu
	\fBlar\fR		Load access rights byte
0F 02 /r	\fBlarw\fR	\fIrm16,r16\fR
0F 02 /r	\fBlarl\fR	\fIrm32,r32\fR
.sp \n(pDu
9A	\fBlcall\fR	\fIimm16x,imm32\fR	Long call
.sp \n(pDu
	\fBlds\fR		load full pointer DS:r16
C5 /r	\fBldsw\fR	\fIm16,r16\fR
C5 /r	\fBldsl\fR	\fIm32,r32\fR
.sp \n(pDu
	\fBlea\fR		Load effective address
8D /r	\fBleaw\fR	\fIm16,r16\fR
8D /r	\fBleal\fR	\fIm32,r32\fR
.sp \n(pDu
C9	\fBleave\fR		High level procedure exit
.sp \n(pDu
	\fBles\fR		Load full pointer ES:r16
C4 /r	\fBlesw\fR	\fIm16,r16\fR
C4 /r	\fBlesl\fR	\fIm32,r32\fR
.sp \n(pDu
	\fBlfs\fR		Load full pointer FS:r16
0F B4 /r	\fBlfsw\fR	\fIm16,r16\fR
0F B4 /r	\fBlfsl\fR	\fIm32,r32\fR
.sp \n(pDu
	\fBlgdt\fR		Load m into DGTR
0F 01 /2	\fBlgdtw\fR	\fIm16\fR
0F 01 /2	\fBlgdtl\fR	\fIm32\fR
.sp \n(pDu
	\fBlgs\fR		Load full pointer GS:r16
0F B5 /r	\fBlgsw\fR	\fIm16,r16\fR
0F B5 /r	\fBlgsl\fR	\fIm32,r32\fR
.sp \n(pDu
	\fBlidt\fR		Load m into IDTR
0F 01 /3	\fBlidtw\fR	\fIm16\fR
0F 01 /3	\fBlidtl\fR	\fIm32\fR
.sp \n(pDu
EA	\fBljmp\fR	\fIimm16x,imm32\fR	Long jump
0F 00 /2	\fBlldt\fR	\fIrm16\fR	Load local descriptor table register
0F 01 /6	\fBlmsw\fR	\fIrm16\fR	Load machine status word
F0	\fBlock\fR		Assert lock signal for next instruction
AC	\fBlodsb\fR		Load string operand byte
AD	\fBlodsl\fR		Load string operand long
AD	\fBlodsw\fR		Load string operand word
E2	\fBloop\fR	\fIrel8\fR	Dec count jmp if count <> 0
E1	\fBloope\fR	\fIrel8\fR	Dec count jmp if count <> 0 and ZF = 1
E0	\fBloopne\fR	\fIrel8\fR	Dec count jmp if count <> 0 and ZF = 0
E0	\fBloopnz\fR	\fIrel8\fR	Dec count jmp if count <> 0 and ZF = 0
E1	\fBloopz\fR	\fIrel8\fR	Dec count jmp if count <> 0 and ZF = 1
CB	\fBlret\fR		Far return
CA	\fBlret\fR	\fIimm16\fR	Far return pop imm16 bytes of parms
.sp \n(pDu
	\fBlsl\fR		Load segment limit
0F 03 /r	\fBlslw\fR	\fIrm16,r16\fR
0F 03 /r	\fBlsll\fR	\fIrm32,r32\fR
.sp \n(pDu
	\fBlss\fR		Load full pointer SS:r16
0F B2 /r	\fBlssw\fR	\fIm16,r16\fR
0F B2 /r	\fBlssl\fR	\fIm32,r32\fR
.sp \n(pDu
0F 00 /3	\fBltr\fR	\fIrm16\fR	Load task register
.sp \n(pDu
	\fBmov\fR		Move data
A0	\fBmovb\fR	\fImoffs,al\fR
A1	\fBmovw\fR	\fImoffs,ax\fR
A1	\fBmovl\fR	\fImoffs,eax\fR
A2	\fBmovb\fR	\fIal,moffs\fR
A3	\fBmovw\fR	\fIax,moffs\fR
A3	\fBmovl\fR	\fIeax,moffs\fR
8A /r	\fBmovb\fR	\fIrm8,r8\fR
8B /r	\fBmovw\fR	\fIrm16,r16\fR
8B /r	\fBmovl\fR	\fIrm32,r32\fR
88 /r	\fBmovb\fR	\fIr8,rm8\fR
89 /r	\fBmovw\fR	\fIr16,rm16\fR
89 /r	\fBmovl\fR	\fIr32,rm32\fR
8C /r	\fBmovw\fR	\fIsreg,rm16\fR
8E /r	\fBmovw\fR	\fIrm16,sreg\fR
B0 +r	\fBmovb\fR	\fIimm8,r8\fR
B8 +r	\fBmovw\fR	\fIimm16,r16\fR
B8 +r	\fBmovl\fR	\fIimm32,r32\fR
C6	\fBmovb\fR	\fIimm8,rm8\fR
C7	\fBmovw\fR	\fIimm16,rm16\fR
C7	\fBmovl\fR	\fIimm32,rm32\fR
0F 20 /r	\fBmovl\fR	\fIctlreg,r32\fR
0F 22 /r	\fBmovl\fR	\fIr32,ctlreg\fR
0F 21 /r	\fBmovl\fR	\fIdbreg,r32\fR
0F 23 /r	\fBmovl\fR	\fIr32,dbreg\fR
0F 24 /r	\fBmovl\fR	\fItreg,r32\fR
0F 26 /r	\fBmovl\fR	\fIr32,treg\fR
.sp \n(pDu
A4	\fBmovsb\fR		Move bytes
A5	\fBmovsl\fR		Move longs
A5	\fBmovsw\fR		Move words
.sp \n(pDu
	\fBmovsx\fR		Move with sign extend
0F BE /r	\fBmovsxb\fR	\fIrm8,r16\fR
0F BE /r	\fBmovsxb\fR	\fIrm8,r32\fR
0F BF /r	\fBmovsxw\fR	\fIrm16,r32\fR
0F BE /r	\fBmovsbw\fR	\fIrm8,r16\fR
0F BE /r	\fBmovsbl\fR	\fIrm8,r32\fR
0F BF /r	\fBmovswl\fR	\fIrm16,r32\fR
.sp \n(pDu
	\fBmovzx\fR		Move with zero extend
0F B6 /r	\fBmovzxb\fR	\fIrm8,r16\fR
0F B6 /r	\fBmovzxb\fR	\fIrm8,r32\fR
0F B7 /r	\fBmovzxw\fR	\fIrm16,r32\fR
0F B6 /r	\fBmovzbw\fR	\fIrm8,r16\fR
0F B6 /r	\fBmovzbl\fR	\fIrm8,r32\fR
0F B7 /r	\fBmovzwl\fR	\fIrm16,r32\fR
.sp \n(pDu
	\fBmul\fR		Unsigned multiply
F6 /4	\fBmulb\fR	\fIrm8,al\fR
F6 /4	\fBmulb\fR	\fIrm8\fR
F7 /4	\fBmulw\fR	\fIrm16,ax\fR
F7 /4	\fBmulw\fR	\fIrm16\fR
F7 /4	\fBmull\fR	\fIrm32,eax\fR
F7 /4	\fBmull\fR	\fIrm32\fR
.sp \n(pDu
	\fBneg\fR		Negate
F6 /3	\fBnegb\fR	\fIrm8\fR
F7 /3	\fBnegw\fR	\fIrm16\fR
F7 /3	\fBnegl\fR	\fIrm32\fR
.sp \n(pDu
90	\fBnop\fR		No operation
.sp \n(pDu
	\fBnot\fR		Invert bits
F6 /2	\fBnotb\fR	\fIrm8\fR
F7 /2	\fBnotw\fR	\fIrm16\fR
F7 /2	\fBnotl\fR	\fIrm32\fR
.sp \n(pDu
	\fBor\fR		Logical inclusive OR
83 /1	\fBorl\fR	\fIimm8s,rm32\fR
83 /1	\fBorw\fR	\fIimm8s,rm16\fR
0C	\fBorb\fR	\fIimm8,al\fR
0D	\fBorw\fR	\fIimm16,ax\fR
0D	\fBorl\fR	\fIimm32,eax\fR
0D	\fBorl\fR	\fIimm32\fR
80 /1	\fBorb\fR	\fIimm8,rm8\fR
81 /1	\fBorw\fR	\fIimm16,rm16\fR
81 /1	\fBorl\fR	\fIimm32,rm32\fR
0A /r	\fBorb\fR	\fIrm8,r8\fR
0B /r	\fBorw\fR	\fIrm16,r16\fR
0B /r	\fBorl\fR	\fIrm32,r32\fR
08 /r	\fBorb\fR	\fIr8,rm8\fR
09 /r	\fBorw\fR	\fIr16,rm16\fR
09 /r	\fBorl\fR	\fIr32,rm32\fR
.sp \n(pDu
	\fBout\fR		Output from port
E6	\fBoutb\fR	\fIimm8\fR
E7	\fBoutw\fR	\fIimm8\fR
E7	\fBoutl\fR	\fIimm8\fR
EE	\fBoutb\fR	\fI(dx)\fR
EF	\fBoutw\fR	\fI(dx)\fR
EF	\fBoutl\fR	\fI(dx)\fR
.sp \n(pDu
6E	\fBoutsb\fR		Output byte to port into ES:(E)DI
6F	\fBoutsl\fR		Output long to port into ES:(E)DI
6F	\fBoutsw\fR		Output word to port into ES:(E)DI
.sp \n(pDu
	\fBpop\fR		Pop a word from the stack
58 +r	\fBpopw\fR	\fIr16\fR
58 +r	\fBpopl\fR	\fIr32\fR
1F	\fBpopw\fR	\fIds\fR
07	\fBpopw\fR	\fIes\fR
17	\fBpopw\fR	\fIss\fR
0F A1	\fBpopw\fR	\fIfs\fR
0F A9	\fBpopw\fR	\fIgs\fR
8F /0	\fBpopw\fR	\fIm16\fR
8F /0	\fBpopl\fR	\fIm32\fR
.sp \n(pDu
	\fBpopa\fR		Pop all
61	\fBpopaw\fR
61	\fBpopal\fR
.sp \n(pDu
	\fBpopf\fR		Pop stack into flags
9D	\fBpopfw\fR
9D	\fBpopfl\fR
.sp \n(pDu
	\fBpush\fR		Push a word on the stack
50 +r	\fBpushw\fR	\fIr16\fR
50 +r	\fBpushl\fR	\fIr32\fR
6A	\fBpushb\fR	\fIimm8s\fR
68	\fBpushw\fR	\fIimm16\fR
68	\fBpushl\fR	\fIimm32\fR
0E	\fBpushw\fR	\fIcs\fR
1E	\fBpushw\fR	\fIds\fR
06	\fBpushw\fR	\fIes\fR
16	\fBpushw\fR	\fIss\fR
0F A0	\fBpushw\fR	\fIfs\fR
0F A8	\fBpushw\fR	\fIgs\fR
FF /6	\fBpushw\fR	\fIm16\fR
FF /6	\fBpushl\fR	\fIm32\fR
.sp \n(pDu
	\fBpusha\fR		Push all
60	\fBpushaw\fR
60	\fBpushal\fR
.sp \n(pDu
	\fBpushf\fR		Push flags
9C	\fBpushfw\fR
9C	\fBpushfl\fR
.sp \n(pDu
	\fBrcl\fR		Rotate carry left
D0 /2	\fBrclb\fR	\fIcon1,rm8\fR
D0 /2	\fBrclb\fR	\fIrm8\fR
D2 /2	\fBrclb\fR	\fIcl,rm8\fR
C0 /2	\fBrclb\fR	\fIimm8,rm8\fR
D1 /2	\fBrclw\fR	\fIcon1,rm16\fR
D1 /2	\fBrclw\fR	\fIrm16\fR
D3 /2	\fBrclw\fR	\fIcl,rm16\fR
C1 /2	\fBrclw\fR	\fIimm8,rm16\fR
D1 /2	\fBrcll\fR	\fIcon1,rm32\fR
D1 /2	\fBrcll\fR	\fIrm32\fR
D3 /2	\fBrcll\fR	\fIcl,rm32\fR
C1 /2	\fBrcll\fR	\fIimm8,rm32\fR
.sp \n(pDu
	\fBrcr\fR		Rotate carry right
D0 /3	\fBrcrb\fR	\fIcon1,rm8\fR
D0 /3	\fBrcrb\fR	\fIrm8\fR
D2 /3	\fBrcrb\fR	\fIcl,rm8\fR
C0 /3	\fBrcrb\fR	\fIimm8,rm8\fR
D1 /3	\fBrcrw\fR	\fIcon1,rm16\fR
D1 /3	\fBrcrw\fR	\fIrm16\fR
D3 /3	\fBrcrw\fR	\fIcl,rm16\fR
C1 /3	\fBrcrw\fR	\fIimm8,rm16\fR
D1 /3	\fBrcrl\fR	\fIcon1,rm32\fR
D1 /3	\fBrcrl\fR	\fIrm32\fR
D3 /3	\fBrcrl\fR	\fIcl,rm32\fR
C1 /3	\fBrcrl\fR	\fIimm8,rm32\fR
.sp \n(pDu
F3	\fBrep\fR		rep following instruction CX times
F3	\fBrepe\fR		repe following instruction CX times or eq
F2	\fBrepne\fR		repne following instruction CX times or ne
F2	\fBrepnz\fR		alternate name for repnz
F3	\fBrepz\fR		alternate name for repe
C3	\fBret\fR		Return
C2	\fBret\fR	\fIimm16\fR	Return pop imm16 bytes of parms
.sp \n(pDu
	\fBrol\fR		Rotate left
D0 /0	\fBrolb\fR	\fIcon1,rm8\fR
D0 /0	\fBrolb\fR	\fIrm8\fR
D2 /0	\fBrolb\fR	\fIcl,rm8\fR
C0 /0	\fBrolb\fR	\fIimm8,rm8\fR
D1 /0	\fBrolw\fR	\fIcon1,rm16\fR
D1 /0	\fBrolw\fR	\fIrm16\fR
D3 /0	\fBrolw\fR	\fIcl,rm16\fR
C1 /0	\fBrolw\fR	\fIimm8,rm16\fR
D1 /0	\fBroll\fR	\fIcon1,rm32\fR
D1 /0	\fBroll\fR	\fIrm32\fR
D3 /0	\fBroll\fR	\fIcl,rm32\fR
C1 /0	\fBroll\fR	\fIimm8,rm32\fR
.sp \n(pDu
	\fBror\fR		Rotate right
D0 /1	\fBrorb\fR	\fIcon1,rm8\fR
D0 /1	\fBrorb\fR	\fIrm8\fR
D2 /1	\fBrorb\fR	\fIcl,rm8\fR
C0 /1	\fBrorb\fR	\fIimm8,rm8\fR
D1 /1	\fBrorw\fR	\fIcon1,rm16\fR
D1 /1	\fBrorw\fR	\fIrm16\fR
D3 /1	\fBrorw\fR	\fIcl,rm16\fR
C1 /1	\fBrorw\fR	\fIimm8,rm16\fR
D1 /1	\fBrorl\fR	\fIcon1,rm32\fR
D1 /1	\fBrorl\fR	\fIrm32\fR
D3 /1	\fBrorl\fR	\fIcl,rm32\fR
C1 /1	\fBrorl\fR	\fIimm8,rm32\fR
.sp \n(pDu
9E	\fBsahf\fR		Store AH into flags
.sp \n(pDu
	\fBsal\fR		Shift arithmetic left
D0 /4	\fBsalb\fR	\fIcon1,rm8\fR
D0 /4	\fBsalb\fR	\fIrm8\fR
D2 /4	\fBsalb\fR	\fIcl,rm8\fR
C0 /4	\fBsalb\fR	\fIimm8,rm8\fR
D1 /4	\fBsalw\fR	\fIcon1,rm16\fR
D1 /4	\fBsalw\fR	\fIrm16\fR
D3 /4	\fBsalw\fR	\fIcl,rm16\fR
C1 /4	\fBsalw\fR	\fIimm8,rm16\fR
D1 /4	\fBsall\fR	\fIcon1,rm32\fR
D1 /4	\fBsall\fR	\fIrm32\fR
D3 /4	\fBsall\fR	\fIcl,rm32\fR
C1 /4	\fBsall\fR	\fIimm8,rm32\fR
.sp \n(pDu
	\fBsar\fR		Shift arithmetic right
D0 /7	\fBsarb\fR	\fIcon1,rm8\fR
D0 /7	\fBsarb\fR	\fIrm8\fR
D2 /7	\fBsarb\fR	\fIcl,rm8\fR
C0 /7	\fBsarb\fR	\fIimm8,rm8\fR
D1 /7	\fBsarw\fR	\fIcon1,rm16\fR
D1 /7	\fBsarw\fR	\fIrm16\fR
D3 /7	\fBsarw\fR	\fIcl,rm16\fR
C1 /7	\fBsarw\fR	\fIimm8,rm16\fR
D1 /7	\fBsarl\fR	\fIcon1,rm32\fR
D1 /7	\fBsarl\fR	\fIrm32\fR
D3 /7	\fBsarl\fR	\fIcl,rm32\fR
C1 /7	\fBsarl\fR	\fIimm8,rm32\fR
.sp \n(pDu
	\fBsbb\fR		Subtract with borrow
83 /3	\fBsbbl\fR	\fIimm8s,rm32\fR
83 /3	\fBsbbw\fR	\fIimm8s,rm16\fR
1C	\fBsbbb\fR	\fIimm8,al\fR
1D	\fBsbbw\fR	\fIimm16,ax\fR
1D	\fBsbbl\fR	\fIimm32,eax\fR
1D	\fBsbbl\fR	\fIimm32\fR
80 /3	\fBsbbb\fR	\fIimm8,rm8\fR
81 /3	\fBsbbw\fR	\fIimm16,rm16\fR
81 /3	\fBsbbl\fR	\fIimm32,rm32\fR
1A /r	\fBsbbb\fR	\fIrm8,r8\fR
1B /r	\fBsbbw\fR	\fIrm16,r16\fR
1B /r	\fBsbbl\fR	\fIrm32,r32\fR
18 /r	\fBsbbb\fR	\fIr8,rm8\fR
19 /r	\fBsbbw\fR	\fIr16,rm16\fR
19 /r	\fBsbbl\fR	\fIr32,rm32\fR
.sp \n(pDu
AE	\fBscasb\fR		Compare string bytes
AF	\fBscasl\fR		Compare string longs
AF	\fBscasw\fR		Compare string words
	
0F 97	\fBseta\fR	\fIrm8\fR	Set byte if above
0F 93	\fBsetae\fR	\fIrm8\fR	Set byte if above or equal
0F 92	\fBsetb\fR	\fIrm8\fR	Set byte if below
0F 96	\fBsetbe\fR	\fIrm8\fR	Set byte if below or equal
0F 92	\fBsetc\fR	\fIrm8\fR	Set byte if carry
0F 94	\fBsete\fR	\fIrm8\fR	Set byte if equal
0F 9F	\fBsetg\fR	\fIrm8\fR	Set byte if greater
0F 9D	\fBsetge\fR	\fIrm8\fR	Set byte if greater or equal
0F 9C	\fBsetl\fR	\fIrm8\fR	Set byte if less
0F 9E	\fBsetle\fR	\fIrm8\fR	Set byte if less or equal
0F 96	\fBsetna\fR	\fIrm8\fR	Set byte if not above
0F 92	\fBsetnae\fR	\fIrm8\fR	Set byte if not above or equal
0F 93	\fBsetnb\fR	\fIrm8\fR	Set byte if not below
0F 97	\fBsetnbe\fR	\fIrm8\fR	Set byte if not below or equal
0F 93	\fBsetnc\fR	\fIrm8\fR	Set byte if no carry
0F 95	\fBsetne\fR	\fIrm8\fR	Set byte if not equal
0F 9E	\fBsetng\fR	\fIrm8\fR	Set byte if not greater
0F 9C	\fBsetnge\fR	\fIrm8\fR	Set byte if not greater or equal
0F 9D	\fBsetnl\fR	\fIrm8\fR	Set byte if not less
0F 9F	\fBsetnle\fR	\fIrm8\fR	Set byte if not less or equal
0F 91	\fBsetno\fR	\fIrm8\fR	Set byte if not overflow
0F 9B	\fBsetnp\fR	\fIrm8\fR	Set byte if not parity
0F 99	\fBsetns\fR	\fIrm8\fR	Set byte if not sign
0F 95	\fBsetnz\fR	\fIrm8\fR	Set byte if not zero
0F 90	\fBseto\fR	\fIrm8\fR	Set byte if overflow
0F 9A	\fBsetp\fR	\fIrm8\fR	Set byte if parity
0F 9A	\fBsetpe\fR	\fIrm8\fR	Set byte if parity even
0F 9B	\fBsetpo\fR	\fIrm8\fR	Set byte if parity odd
0F 98	\fBsets\fR	\fIrm8\fR	Set byte if sign
0F 94	\fBsetz\fR	\fIrm8\fR	Set byte if zero
0F 94	\fBsetz\fR	\fIrm8\fR	Set byte if zero
	
0F 01 /0	\fBsgdt\fR	\fImem32\fR	Store gdtr
.sp \n(pDu
	\fBshl\fR		Shift arithmetic left
D0 /4	\fBshlb\fR	\fIcon1,rm8\fR
D0 /4	\fBshlb\fR	\fIrm8\fR
D2 /4	\fBshlb\fR	\fIcl,rm8\fR
C0 /4	\fBshlb\fR	\fIimm8,rm8\fR
D1 /4	\fBshlw\fR	\fIcon1,rm16\fR
D1 /4	\fBshlw\fR	\fIrm16\fR
D3 /4	\fBshlw\fR	\fIcl,rm16\fR
C1 /4	\fBshlw\fR	\fIimm8,rm16\fR
D1 /4	\fBshll\fR	\fIcon1,rm32\fR
D1 /4	\fBshll\fR	\fIrm32\fR
D3 /4	\fBshll\fR	\fIcl,rm32\fR
C1 /4	\fBshll\fR	\fIimm8,rm32\fR
.sp \n(pDu
	\fBshld\fR		Shift double precision left
0F A4	\fBshldw\fR	\fIimm8,r16,rm16\fR
0F A4	\fBshldl\fR	\fIimm8,r32,rm32\fR
0F A5	\fBshldw\fR	\fIcl,r16,rm16\fR
0F A5	\fBshldl\fR	\fIcl,r32,rm32\fR
.sp \n(pDu
	\fBshr\fR		Shift right
D0 /5	\fBshrb\fR	\fIcon1,rm8\fR
D0 /5	\fBshrb\fR	\fIrm8\fR
D2 /5	\fBshrb\fR	\fIcl,rm8\fR
C0 /5	\fBshrb\fR	\fIimm8,rm8\fR
D1 /5	\fBshrw\fR	\fIcon1,rm16\fR
D1 /5	\fBshrw\fR	\fIrm16\fR
D3 /5	\fBshrw\fR	\fIcl,rm16\fR
C1 /5	\fBshrw\fR	\fIimm8,rm16\fR
D1 /5	\fBshrl\fR	\fIcon1,rm32\fR
D1 /5	\fBshrl\fR	\fIrm32\fR
D3 /5	\fBshrl\fR	\fIcl,rm32\fR
C1 /5	\fBshrl\fR	\fIimm8,rm32\fR
.sp \n(pDu
	\fBshrd\fR		Shift double precision right
0F AC	\fBshrdw\fR	\fIimm8,r16,rm16\fR
0F AC	\fBshrdl\fR	\fIimm8,r32,rm32\fR
0F AD	\fBshrdw\fR	\fIcl,r16,rm16\fR
0F AD	\fBshrdl\fR	\fIcl,r32,rm32\fR
0F AD	\fBshrdw\fR	\fIr16,rm16\fR
0F AD	\fBshrdl\fR	\fIr32,rm32\fR
.sp \n(pDu
0F 01 /1	\fBsidt\fR	\fImem32\fR	Store idtr
0F 00 /0	\fBsldt\fR	\fIrm16\fR	Store ldtr to EA word
0F 01 /4	\fBsmsw\fR	\fIrm16\fR	Store machine status to EA word
F9	\fBstc\fR		Set carry flag
FD	\fBstd\fR		Clear direction flag
FB	\fBsti\fR		Set interrupt flag
AA	\fBstosb\fR		Store string byte
AB	\fBstosl\fR		Store string long
AB	\fBstosw\fR		Store string word
0F 00 /1	\fBstr\fR		Store task register
.sp \n(pDu
	\fBsub\fR		Subtract
83 /5	\fBsubl\fR	\fIimm8s,rm32\fR
83 /5	\fBsubw\fR	\fIimm8s,rm16\fR
2C	\fBsubb\fR	\fIimm8,al\fR
2D	\fBsubw\fR	\fIimm16,ax\fR
2D	\fBsubl\fR	\fIimm32,eax\fR
2D	\fBsubl\fR	\fIimm32\fR
80 /5	\fBsubb\fR	\fIimm8,rm8\fR
81 /5	\fBsubw\fR	\fIimm16,rm16\fR
81 /5	\fBsubl\fR	\fIimm32,rm32\fR
2A /r	\fBsubb\fR	\fIrm8,r8\fR
2B /r	\fBsubw\fR	\fIrm16,r16\fR
2B /r	\fBsubl\fR	\fIrm32,r32\fR
28 /r	\fBsubb\fR	\fIr8,rm8\fR
29 /r	\fBsubw\fR	\fIr16,rm16\fR
29 /r	\fBsubl\fR	\fIr32,rm32\fR
.sp \n(pDu
	\fBtest\fR		Logical compare
A8	\fBtestb\fR	\fIimm8,al\fR
A9	\fBtestw\fR	\fIimm16,ax\fR
A9	\fBtestl\fR	\fIimm32,eax\fR
A9	\fBtestl\fR	\fIimm32\fR
F6 /0	\fBtestb\fR	\fIimm8,rm8\fR
F7 /0	\fBtestw\fR	\fIimm16,rm16\fR
F7 /0	\fBtestl\fR	\fIimm32,rm32\fR
84 /r	\fBtestb\fR	\fIr8,rm8\fR
85 /r	\fBtestw\fR	\fIr16,rm16\fR
85 /r	\fBtestl\fR	\fIr32,rm32\fR
.sp \n(pDu
0F 00 /4	\fBverr\fR	\fIrm16\fR	Verify segment for read
0F 00 /5	\fBverw\fR	\fIrm16\fR	Verify segment for write
9B	\fBwait\fR		Wait for coprocessor
.sp \n(pDu
	\fBxchg\fR		Exchange register
90 +r	\fBxchgw\fR	\fIr16,ax\fR
90 +r	\fBxchgw\fR	\fIax,r16\fR
90 +r	\fBxchgl\fR	\fIr32,eax\fR
90 +r	\fBxchgl\fR	\fIr32\fR
90 +r	\fBxchgl\fR	\fIeax,r32\fR
86 /r	\fBxchgb\fR	\fIrm8,r8\fR
87 /r	\fBxchgw\fR	\fIrm16,r16\fR
87 /r	\fBxchgl\fR	\fIrm32,r32\fR
86 /r	\fBxchgb\fR	\fIr8,rm8\fR
87 /r	\fBxchgw\fR	\fIr16,rm16\fR
87 /r	\fBxchgl\fR	\fIr32,rm32\fR
.sp \n(pDu
D7	\fBxlat\fR		Table lookup translation
D7	\fBxlatb\fR		Table lookup translation
.sp \n(pDu
	\fBxor\fR		Logical exclusive OR
83 /6	\fBxorl\fR	\fIimm8s,rm32\fR
83 /6	\fBxorw\fR	\fIimm8s,rm16\fR
34	\fBxorb\fR	\fIimm8,al\fR
35	\fBxorw\fR	\fIimm16,ax\fR
35	\fBxorl\fR	\fIimm32,eax\fR
35	\fBxorl\fR	\fIimm32\fR
80 /6	\fBxorb\fR	\fIimm8,rm8\fR
81 /6	\fBxorw\fR	\fIimm16,rm16\fR
81 /6	\fBxorl\fR	\fIimm32,rm32\fR
32 /r	\fBxorb\fR	\fIrm8,r8\fR
33 /r	\fBxorw\fR	\fIrm16,r16\fR
33 /r	\fBxorl\fR	\fIrm32,r32\fR
30 /r	\fBxorb\fR	\fIr8,rm8\fR
31 /r	\fBxorw\fR	\fIr16,rm16\fR
31 /r	\fBxorl\fR	\fIr32,rm32\fR
