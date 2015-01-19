.\" /usr/lib/tmac.s 4/16/91
.de It		\" Done at initialization
.po 1i
.wh 0 Hd
.nr Ft 0-\\n(FM
.wh -\\n(FMu Fo
.wh -\\n(FMu Fx
.nr Ps \\n(PS
.ta 5n 10n 15n 20n 25n 30n 35n 40n 45n 50n 55n 60n 65n 70n 75n 80n 85n 90n
.ds Xx **GOK**
.if \n(mo=1 .ds Xx January
.if \n(mo=2 .ds Xx February
.if \n(mo=3 .ds Xx March
.if \n(mo=4 .ds Xx April
.if \n(mo=5 .ds Xx May
.if \n(mo=6 .ds Xx June
.if \n(mo=7 .ds Xx July
.if \n(mo=8 .ds Xx August
.if \n(mo=9 .ds Xx September
.if \n(mo=10 .ds Xx October
.if \n(mo=11 .ds Xx November
.if \n(mo=12 .ds Xx December
.ds Ds "\\*(Xx \\n(dy, 19\\n(yr"
..
.de Rt		\" Reset everything to normal state
.ce 0
.if \\n(Di \\{\\
.di
.nr Di 0
.nf
.Tl
. \\}
.fi
.ps \\n(Ps
.vs \\n(VSu
.ft R
.if \\n(Il \\{\\
.nr Il 0
.in -\\n(Iku
. \\}
.if \\n(Qn \\{\\
.nr Qn -1
.in -\\n(Qdu
.ll +\\n(Qdu
. \\}
..
.de Hd		\" Header trap
'nr PN \\n%
'sp \\n(HMu/2u-1v
'ft R
'PT
'ft P
'sp \\n(HMu/2u
'if \\n(Kf \\{\\
'nr Kf 0
'ev 1
'nf
'Kd
'rm Kd
'ev
. \\}
.nr Fn 0	\" Clear footnote count
.ch Fo -\\n(FMu
.if \\n(Fh .Fv	\" And put out overflow
..
.de Fo		\" Footer trap
.if \\n(Fn \\{\\
.\".ev 1
.nf
.Fn
.rm Fn
.fi
.\".ev
.if "\\n(.z"Fe" .di
.nr Fn 0
.if \\n(dn .nr Fh 1
. \\}
'sp \\n(FMu/2u
'ft R
'BT
'ft P
'bp
..
.de CD		\" Centred display
.ce 10000
..
.de LD		\" Left display
.nf
..
.de ID		\" Indented display
.in +.5i
.nf
.nr Bi 1
..
.de BD		\" Block centred display
.nr Bd 1
.nf
..
.de PP		\" Start new paragraph
.Rt
.sp \\n(PDu
.ne 5
.ti \\n(PIu
..
.de LP		\" Left paragraph
.Rt
.ne 5
.sp
..
.de IP
.LP
.if \\n(.$>1 .nr Ik \\$2n
.nr Il 1
.in +\\n(Iku
.ti -\\n(Iku
\\$1\h'\\n(Iku-\\w`\\$1`u'\\c
..
.de QS		\" Start quoted paragraph
.sp
.ft R
.in +\\n(PIu
.ll -\\n(PIu*2u
..
.de QP		\" Quoted paragraph
.PP
.nr Qn 1
..
.de QE		\" End quoted paragraph
.sp
.ft P
.in -\\n(PIu
.ll +\\n(PIu*2u
..
.de RS		\" Start relative indent
.in +\\n(Iku
..
.de RE		\" End relative indent
.in -\\n(Iku
..
.		\" Title page and headings
.de TL		\" Main title
.nr Di 1	\" Diverted
.di Tl
.sp
.ce 1000
.ps \\n(PS+2
.ft B
..
.de AU		\" Author
.if !\\n(Di .da Tl
.nr Di 1
.sp
.ce 1000
.ps \\n(PS
.ft R
..
.de AI		\" Author's institution
.if !\\n(Di .da Tl
.nr Di 1
.sp
.ce 1000
.ps \\n(PS
.ft R
..
.de AB		\" Begin abstract
.if \\n(Di \\{\\
.sp
.ce 0
.ps \\n(PS
.ft R
.di
.nf
.Tl
.fi
.nr Di 0
. \\}
.sp
.ce
\fIABSTRACT\fP
.ll -.5i
.in +.5i
..
.de AE		\" End abstract
.ll +.5i
.in -.5i
.sp .5i
.ce 1000
\\*(Ds
.ce 0
.sp .5i
.sp
..
.de ND		\" New (or No) Date
.ds Ds \\$1
..
.de DA		\" Supply date
.ds Ds \\$1
..
.de SH		\" Sub-heading
.Rt
.sp
.ft B
..
.de NH		\" Numbered section heading
.Rt
.sp
.ft B
.nr Xx 1
.if \\n(.$>0 .nr Xx \\$1
.if \\n(Xx<5 .nr @5 0
.if \\n(Xx<4 .nr @4 0
.if \\n(Xx<3 .nr @3 0
.if \\n(Xx<2 .nr @2 0
.if \\n(Xx<1 .nr @1 0
.if \\n(Xx=0 .nr Xx 1
.if \\n(Xx>5 .tm NH level too large
.nr @\\n(Xx +1
.ds Yy \\n(@1
.if \\n(Xx=1 .as Yy .
.if \\n(Xx>1 .as Yy .\\n(@2
.if \\n(Xx>2 .as Yy .\\n(@3
.if \\n(Xx>3 .as Yy .\\n(@4
.if \\n(Xx>4 .as Yy .\\n(@5
\\*(Yy
..
.de PT		\" Page title
'tl '\\*(LT'\\*(CT'\\*(RT'
..
.de BT		\" Bottom title
'tl '\\*(LF'\\*(CF'\\*(RF'
..
.		\" Sizes and fonts
.de NL		\" Normal size
.if \\n(.$>0 \s\\n(Ps\\$1\s0
.if \\n(.$=0 .nr Ps \\n(Ps
..
.de SM		\" Smaller size
.if \\n(.$>0 \s-2\\$1\s0
.if \\n(.$=0 .nr Ps \\n(Ps-2
..
.de LG		\" Larger size
.if \\n(.$>0 \s+2\\$1\s0
.if \\n(.$=0 .nr Ps \\n(Ps+2
..
.de UL		\" Underline
\\$1\l'|0\(ul'
..
.de B		\" Bold face font
.if \\n(.$>0 \\fB\\$1\\fP
.if \\n(.$=0 .ft B
..
.de I		\" Italic font
.if \\n(.$>0 \\fI\\$1\\fP
.if \\n(.$=0 .ft I
..
.de R		\" Roman font
.if \\n(.$>0 \\fR\\$1\\fP
.if \\n(.$=0 .ft R
..
.de BX		\" Draw a box
\(br\|\\$1\|\(br\l'|0\(rn'\l'|0\(ul'
..
.de DS		\" Display start
.if \\n(.$=0 .ds Xx LD
.if \\n(.$>0 .ds Xx \\$1D
.KS
.\\*(Xx
..
.de DE		\" Display end
.ce 0
.nr Xx 0u
.if \\n(Bi \\{\\
.nr Xx -.5i
.nr Bi 0
. \\}
.if \\n(Bd \\{\\
.nr Bd 0
.di
.nr Xx (\\n(.lu-\\n(dlu)/2u
.in +\\n(Xxu
.da Kd
. \\}
.KE
.in -\\n(Xxu
..
.de KS		\" Start keep
.nr Kf 0
.Ks
..
.de KF		\" Start floating keep
.nr Kf 1
.Ks
..
.de Ks		\" Internal version of KS/KF
.br
.ev 1
.fi
.da Kd
..
.de KE		\" End keep
.br
.di
.if !\\n(Kf \\{\\
.if \\n(dn>=\\n(.t .bp
.nf
.Kd
.rm Kd
. \\}
.ev
..
.de TS		\" Table start
.tm No tables yet.
..
.de TE		\" Table end
..
.de EQ		\" Equation
.tm No equations yet.
..
.		\" Footnotes
.de FS		\" Footnote start
.da Fn
.ev 1
.fi
.ll \\n(FLu
.if \\n+(Fn=1 .Fs
..
.de FE		\" Footnote end
.br
.ev
.di
.ie \\n(.t>(\\n(dn+\\\n(.v) .ch Fo -(\\n(FMu+\\n(dnu+\\n(.vu)
.el                         .ch Fo (\\n(nlu+\\n(.vu)
..
.de Fs		\" Put out separator
-----
.br
..
.\" create an empty macro for later macros to initialize.
.de Fe
..
.de Fv		\" Catch footnote overflow
.nr Fv 0
.FS
.nf
.Fe
.FE
..
.de Fx		\" Divert extra stuff
.if \\n(Fn .di Fe
..
.de TA		\" Set tabs in ens
.ta \\$1n \\$2n \\$3n \\$4n \\$5n \\$6n \\$7n \\$8n \\$9n
..
.nr PS 10u	\" Default pointsize
.nr VS 12p	\" Vertical spacing
.nr LL 6i	\" Line length
.nr LT 6i	\" Length of title
.nr PD \n(VSu*3/10	\" Paragraph spacing
.nr PD 1v
.if \n(PDu<\n(.V .nr PD \n(.V
.nr PI 5n	\" Paragraph indent
.nr FL 11u*\n(LLu/12u \" Footnote length
.nr CW 7u*\n(LLu/15u \" Column width
.nr GW \n(LLu/15u \" Gutter width
.nr PO 26i/27i	\" Page offset
.nr HM 1i	\" Top margin
.nr FM 1i	\" Bottom margin
.nr Qd	\n(PI	\" Quoted indent difference
.nr Ik 5n	\" .IP default indent
.nr Fn 0 1	\" Footnote count
.ds CF "- \\n(PN -"
.ds - \(mi
.ds ' \z\(aa
.ds ` \z\(ga
.ds : \z:
.ds ^ \z^
.ds ~ \z~
.ds C \z^
.ds , \z,
.It
.\" end of /usr/lib/tmac.s
