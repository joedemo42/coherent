/	module name ps
	.comm	fptr,	4
	.comm	lptr,	4
	.comm	aflag,	4
	.comm	dflag,	4
	.comm	dbflag,	4
	.comm	fflag,	4
	.comm	gflag,	4
	.comm	lflag,	4
	.comm	mflag,	4
	.comm	nflag,	4
	.comm	rflag,	4
	.comm	tflag,	4
	.comm	wflag,	4
	.comm	xflag,	4
	.comm	ttdev,	2
	.comm	nl,	108
	.comm	allp,	4
	.comm	kfile,	4
	.comm	nfile,	4
	.comm	mfile,	4
	.comm	dfile,	4
	.comm	argp,	512
	.comm	kfd,	4
	.comm	mfd,	4
	.comm	dfd,	4
	.comm	u,	536
	.comm	cutime,	4
	.comm	cprocq,	184
	.comm	callocp,	16
	.comm	sysmem,	312

	.text

	.globl main
main:
	push	%ebp
	movl	%ebp, %esp
	push	%esi
	push	%edi
	push	%ebx
	call	initialise
	movl	%ebx, $1
.L19:
	cmpl	%ebx, 8(%ebp)
	jl	.L20001
	jmp	.L17
.L20001:
	movl	%edi, %ebx
	sall	%edi, $2
	addl	%edi, 12(%ebp)
	movl	%esi, (%edi)
.L22:
	cmpb	(%esi), $0
	jne	.L20002
	jmp	.L20
.L20002:
	movsxb	%eax, (%esi)
	movl	%edx, $.L10002-4
	movl	%ecx, $15
.L10003:
	addl	%edx, $4
	cmpl	%eax, (%edx)
	loopne	.L10003
	je	.L20003
	jmp	.L41
.L20003:
	ijmp	60(%edx)
.L10002:
	.long	45
	.long	68
	.long	97
	.long	99
	.long	100
	.long	102
	.long	103
	.long	107
	.long	108
	.long	109
	.long	110
	.long	114
	.long	116
	.long	119
	.long	120
	.long	.L21
	.long	.L29
	.long	.L25
	.long	.L26
	.long	.L28
	.long	.L30
	.long	.L31
	.long	.L32
	.long	.L34
	.long	.L35
	.long	.L36
	.long	.L37
	.long	.L38
	.long	.L39
	.long	.L40
.L21:
	incl	%esi
	jmp	.L22
.L25:
	incl	aflag
	jmp	.L21
.L26:
	incl	%ebx
	movl	%eax, %ebx
	cmpl	%eax, 8(%ebp)
	jl	.L27
	call	usage
.L27:
	movl	%edi, %ebx
	sall	%edi, $2
	addl	%edi, 12(%ebp)
	movl	%eax, (%edi)
	movl	nfile, %eax
	jmp	.L21
.L28:
	incl	dflag
	jmp	.L21
.L29:
	incl	dbflag
	jmp	.L21
.L30:
	incl	fflag
	jmp	.L21
.L31:
	incl	gflag
	jmp	.L21
.L32:
	incl	%ebx
	movl	%eax, %ebx
	cmpl	%eax, 8(%ebp)
	jl	.L33
	call	usage
.L33:
	movl	%edi, %ebx
	sall	%edi, $2
	addl	%edi, 12(%ebp)
	movl	%eax, (%edi)
	movl	mfile, %eax
	jmp	.L21
.L34:
	incl	lflag
	jmp	.L21
.L35:
	incl	mflag
	jmp	.L21
.L36:
	incl	nflag
	jmp	.L21
.L37:
	incl	rflag
	jmp	.L21
.L38:
	incl	tflag
	jmp	.L21
.L39:
	incl	wflag
	jmp	.L21
.L40:
	incl	xflag
	jmp	.L21
.L41:
	call	usage
	jmp	.L21
.L20:
	incl	%ebx
	jmp	.L19
.L17:
	call	execute
	push	$0
	call	exit
	pop	%ecx
	pop	%ebx
	pop	%edi
	pop	%esi
	leave
	ret
	.align	4
	.globl initialise
initialise:

	.data

.L43:
	.byte	47
	.byte	100
	.byte	101
	.byte	118
	.byte	47
	.byte	107
	.byte	109
	.byte	101
	.byte	109
	.byte	0

	.text


	.data

.L44:
	.byte	47
	.byte	100
	.byte	101
	.byte	118
	.byte	47
	.byte	109
	.byte	101
	.byte	109
	.byte	0

	.text


	.data

.L45:
	.byte	47
	.byte	100
	.byte	101
	.byte	118
	.byte	47
	.byte	115
	.byte	119
	.byte	97
	.byte	112
	.byte	0

	.text


	.data

.L47:
	.byte	112
	.byte	114
	.byte	111
	.byte	99
	.byte	113
	.byte	0

	.text


	.data

.L48:
	.byte	117
	.byte	116
	.byte	105
	.byte	109
	.byte	101
	.byte	114
	.byte	0

	.text


	.data

.L49:
	.byte	115
	.byte	116
	.byte	105
	.byte	109
	.byte	101
	.byte	114
	.byte	0

	.text


	.data

.L50:
	.byte	97
	.byte	108
	.byte	108
	.byte	111
	.byte	99
	.byte	112
	.byte	0

	.text


	.data

.L51:
	.byte	101
	.byte	110
	.byte	100
	.byte	0

	.text


	.data

.L52:
	.byte	115
	.byte	121
	.byte	115
	.byte	109
	.byte	101
	.byte	109
	.byte	0

	.text

	push	%ebp
	movl	%ebp, %esp
	push	%ebx
	movl	aflag, $0
	movl	gflag, $0
	movl	lflag, $0
	movl	mflag, $0
	movl	nflag, $0
	movl	xflag, $0
	call	pick_nfile
	movl	nfile, %eax
	movl	kfile, $.L43
	movl	mfile, $.L44
	movl	dfile, $.L45
	push	$512
	call	malloc
	pop	%ecx
	movl	%ebx, %eax
	orl	%eax, %eax
	je	.L46
	push	%ebx
	push	$_stdout
	call	setbuf
	addl	%esp, $8
.L46:
	push	$.L47
	push	$nl
	call	strcpy
	addl	%esp, $8
	movw	nl+14, $-1
	push	$.L48
	push	$nl+18
	call	strcpy
	addl	%esp, $8
	movw	nl+32, $-1
	push	$.L49
	push	$nl+36
	call	strcpy
	addl	%esp, $8
	movw	nl+50, $-1
	push	$.L50
	push	$nl+54
	call	strcpy
	addl	%esp, $8
	movw	nl+68, $-1
	push	$.L51
	push	$nl+72
	call	strcpy
	addl	%esp, $8
	movw	nl+86, $-1
	push	$.L52
	push	$nl+90
	call	strcpy
	addl	%esp, $8
	movw	nl+104, $-1
	pop	%ebx
	leave
	ret
	.align	4
	.globl usage
usage:

	.data

.L54:
	.byte	85
	.byte	115
	.byte	97
	.byte	103
	.byte	101
	.byte	58
	.byte	32
	.byte	112
	.byte	115
	.byte	32
	.byte	91
	.byte	45
	.byte	93
	.byte	91
	.byte	97
	.byte	99
	.byte	100
	.byte	102
	.byte	103
	.byte	107
	.byte	108
	.byte	109
	.byte	110
	.byte	114
	.byte	116
	.byte	119
	.byte	120
	.byte	93
	.byte	0

	.text

	push	%ebp
	movl	%ebp, %esp
	push	$.L54
	call	panic
	pop	%ecx
	leave
	ret
	.align	4
	.globl pick_nfile
pick_nfile:

	.bss

	.align	1
.L57:
	.blkb	0x10

	.text


	.data

.L58:
	.byte	47
	.byte	100
	.byte	101
	.byte	118
	.byte	47
	.byte	98
	.byte	111
	.byte	111
	.byte	116
	.byte	95
	.byte	103
	.byte	105
	.byte	102
	.byte	116
	.byte	0

	.text


	.data

.L60:
	.byte	47
	.byte	99
	.byte	111
	.byte	104
	.byte	101
	.byte	114
	.byte	101
	.byte	110
	.byte	116
	.byte	0

	.text


	.data

.L62:
	.byte	47
	.byte	99
	.byte	111
	.byte	104
	.byte	101
	.byte	114
	.byte	101
	.byte	110
	.byte	116
	.byte	0

	.text


	.data

.L68:
	.byte	47
	.byte	97
	.byte	117
	.byte	116
	.byte	111
	.byte	98
	.byte	111
	.byte	111
	.byte	116
	.byte	0

	.text


	.data

.L70:
	.byte	47
	.byte	37
	.byte	46
	.byte	49
	.byte	52
	.byte	115
	.byte	0

	.text


	.data

.L72:
	.byte	47
	.byte	97
	.byte	117
	.byte	116
	.byte	111
	.byte	98
	.byte	111
	.byte	111
	.byte	116
	.byte	0

	.text

	enter	$528, $0
	push	%edi
	movl	-528(%ebp), $512
	movw	-524(%ebp), $257
	push	$0
	push	$.L58
	call	open
	addl	%esp, $8
	movl	-4(%ebp), %eax
	cmpl	%eax, $-1
	jne	.L59
	push	$.L60
	jmp	.L20014
.L59:
	push	$512
	lea	%eax, -528(%ebp)
	push	%eax
	push	-4(%ebp)
	call	read
	addl	%esp, $12
	orl	%eax, %eax
	jne	.L61
	push	-4(%ebp)
	call	close
	pop	%ecx
	push	$.L62
	jmp	.L20014
.L61:
	push	-4(%ebp)
	call	close
	pop	%ecx
	movl	-16(%ebp), $0
	push	$0
	lea	%eax, -528(%ebp)
	push	%eax
	call	fifo_open
	addl	%esp, $8
	movl	-8(%ebp), %eax
	orl	%eax, %eax
	jne	.L65
	jmp	.L63
.L65:
	cmpl	-16(%ebp), $0
	jne	.L64
	push	-8(%ebp)
	call	fifo_read
	pop	%ecx
	movl	-12(%ebp), %eax
	orl	%eax, %eax
	je	.L64
	movl	%edi, -12(%ebp)
	cmpw	4(%edi), $771
	jne	.L65
	movl	-16(%ebp), $1
.L64:
	push	-8(%ebp)
	call	fifo_close
	pop	%ecx
	cmpl	-16(%ebp), $0
	jne	.L67
	push	$.L68
	jmp	.L20014
.L67:
	movl	%edi, -12(%ebp)
	movw	4(%edi), $257
	push	$0
	push	%edi
	call	fifo_open
	addl	%esp, $8
	movl	-8(%ebp), %eax
	push	%eax
	call	fifo_read
	pop	%ecx
	movl	-12(%ebp), %eax
	addl	%eax, $6
	push	%eax
	push	$.L70
	push	$.L57
	call	sprintf
	addl	%esp, $12
	push	-8(%ebp)
	call	fifo_close
	pop	%ecx
	jmp	.L20005
.L63:
	push	$.L72
.L20014:
	push	$.L57
	call	strcpy
	addl	%esp, $8
.L20005:
	movl	%eax, $.L57
	pop	%edi
	leave
	ret
	.align	4
	.globl execute
execute:

	.data

.L74:
	.byte	0

	.text


	.data

.L76:
	.byte	66
	.byte	97
	.byte	100
	.byte	32
	.byte	110
	.byte	97
	.byte	109
	.byte	101
	.byte	108
	.byte	105
	.byte	115
	.byte	116
	.byte	32
	.byte	102
	.byte	105
	.byte	108
	.byte	101
	.byte	32
	.byte	37
	.byte	115
	.byte	0

	.text


	.data

.L78:
	.byte	67
	.byte	97
	.byte	110
	.byte	110
	.byte	111
	.byte	116
	.byte	32
	.byte	111
	.byte	112
	.byte	101
	.byte	110
	.byte	32
	.byte	37
	.byte	115
	.byte	0

	.text


	.data

.L80:
	.byte	67
	.byte	97
	.byte	110
	.byte	110
	.byte	111
	.byte	116
	.byte	32
	.byte	111
	.byte	112
	.byte	101
	.byte	110
	.byte	32
	.byte	37
	.byte	115
	.byte	0

	.text


	.data

.L82:
	.byte	79
	.byte	117
	.byte	116
	.byte	32
	.byte	111
	.byte	102
	.byte	32
	.byte	99
	.byte	111
	.byte	114
	.byte	101
	.byte	32
	.byte	111
	.byte	114
	.byte	32
	.byte	105
	.byte	110
	.byte	118
	.byte	97
	.byte	108
	.byte	105
	.byte	100
	.byte	32
	.byte	107
	.byte	101
	.byte	114
	.byte	110
	.byte	101
	.byte	108
	.byte	32
	.byte	115
	.byte	112
	.byte	101
	.byte	99
	.byte	105
	.byte	102
	.byte	105
	.byte	101
	.byte	100
	.byte	0

	.text


	.data

.L83:
	.byte	47
	.byte	100
	.byte	101
	.byte	118
	.byte	0

	.text


	.data

.L84:
	.byte	84
	.byte	84
	.byte	89
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	80
	.byte	73
	.byte	68
	.byte	0

	.text


	.data

.L86:
	.byte	48
	.byte	120
	.byte	102
	.byte	102
	.byte	102
	.byte	102
	.byte	102
	.byte	102
	.byte	102
	.byte	102
	.byte	32
	.byte	0

	.text


	.data

.L88:
	.byte	32
	.byte	71
	.byte	82
	.byte	79
	.byte	85
	.byte	80
	.byte	0

	.text


	.data

.L90:
	.byte	32
	.byte	32
	.byte	80
	.byte	80
	.byte	73
	.byte	68
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	85
	.byte	73
	.byte	68
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	75
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	70
	.byte	32
	.byte	83
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	69
	.byte	86
	.byte	69
	.byte	78
	.byte	84
	.byte	0

	.text


	.data

.L92:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	67
	.byte	86
	.byte	65
	.byte	76
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	83
	.byte	86
	.byte	65
	.byte	76
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	73
	.byte	86
	.byte	65
	.byte	76
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	82
	.byte	86
	.byte	65
	.byte	76
	.byte	0

	.text


	.data

.L94:
	.byte	32
	.byte	85
	.byte	84
	.byte	73
	.byte	77
	.byte	69
	.byte	32
	.byte	83
	.byte	84
	.byte	73
	.byte	77
	.byte	69
	.byte	0

	.text


	.data

.L97:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	0

	.text


	.data

.L98:
	.byte	84
	.byte	84
	.byte	89
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	80
	.byte	73
	.byte	68
	.byte	0

	.text


	.data

.L100:
	.byte	32
	.byte	71
	.byte	82
	.byte	79
	.byte	85
	.byte	80
	.byte	0

	.text


	.data

.L102:
	.byte	32
	.byte	32
	.byte	80
	.byte	80
	.byte	73
	.byte	68
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	85
	.byte	73
	.byte	68
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	75
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	70
	.byte	32
	.byte	83
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	69
	.byte	86
	.byte	69
	.byte	78
	.byte	84
	.byte	0

	.text


	.data

.L104:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	67
	.byte	86
	.byte	65
	.byte	76
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	83
	.byte	86
	.byte	65
	.byte	76
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	73
	.byte	86
	.byte	65
	.byte	76
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	82
	.byte	86
	.byte	65
	.byte	76
	.byte	0

	.text


	.data

.L106:
	.byte	32
	.byte	85
	.byte	84
	.byte	73
	.byte	77
	.byte	69
	.byte	32
	.byte	83
	.byte	84
	.byte	73
	.byte	77
	.byte	69
	.byte	0

	.text


	.data

.L110:
	.byte	70
	.byte	114
	.byte	97
	.byte	103
	.byte	109
	.byte	101
	.byte	110
	.byte	116
	.byte	101
	.byte	100
	.byte	32
	.byte	108
	.byte	105
	.byte	115
	.byte	116
	.byte	0

	.text


	.data

.L120:
	.byte	48
	.byte	120
	.byte	37
	.byte	56
	.byte	120
	.byte	32
	.byte	0

	.text


	.data

.L121:
	.byte	32
	.byte	37
	.byte	53
	.byte	100
	.byte	0

	.text


	.data

.L123:
	.byte	32
	.byte	37
	.byte	53
	.byte	100
	.byte	0

	.text


	.data

.L125:
	.byte	32
	.byte	37
	.byte	53
	.byte	100
	.byte	0

	.text


	.data

.L126:
	.byte	32
	.byte	37
	.byte	56
	.byte	46
	.byte	56
	.byte	115
	.byte	0

	.text


	.data

.L127:
	.byte	32
	.byte	37
	.byte	52
	.byte	111
	.byte	0

	.text


	.data

.L128:
	.byte	32
	.byte	37
	.byte	99
	.byte	0

	.text


	.data

.L130:
	.byte	32
	.byte	48
	.byte	120
	.byte	37
	.byte	56
	.byte	120
	.byte	0

	.text


	.data

.L133:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	45
	.byte	0

	.text


	.data

.L135:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	0

	.text


	.data

.L137:
	.byte	32
	.byte	37
	.byte	57
	.byte	117
	.byte	0

	.text


	.data

.L138:
	.byte	32
	.byte	37
	.byte	57
	.byte	117
	.byte	0

	.text


	.data

.L139:
	.byte	32
	.byte	37
	.byte	49
	.byte	48
	.byte	100
	.byte	0

	.text


	.data

.L140:
	.byte	32
	.byte	37
	.byte	49
	.byte	48
	.byte	100
	.byte	0

	.text


	.data

.L142:
	.byte	32
	.byte	32
	.byte	0

	.text

	enter	$8, $0
	push	%esi
	push	%edi
	push	%ebx
	push	$6
	push	$.L74
	push	$nl
	push	nfile
	call	coffnlist
	addl	%esp, $16
	movzx	%eax, nl+14
	cmpl	%eax, $-1
	jne	.L75
	push	nfile
	push	$.L76
	call	panic
	addl	%esp, $8
.L75:
	push	$0
	push	mfile
	call	open
	addl	%esp, $8
	movl	mfd, %eax
	orl	%eax, %eax
	jge	.L77
	push	mfile
	push	$.L78
	call	panic
	addl	%esp, $8
.L77:
	push	$0
	push	kfile
	call	open
	addl	%esp, $8
	movl	kfd, %eax
	orl	%eax, %eax
	jge	.L79
	push	kfile
	push	$.L80
	call	panic
	addl	%esp, $8
.L79:
	push	$0
	push	dfile
	call	open
	addl	%esp, $8
	movl	dfd, %eax
	push	$184
	push	$cprocq
	push	nl+8
	call	kread
	addl	%esp, $12
	push	$312
	push	$sysmem
	push	nl+98
	call	kread
	addl	%esp, $12
	push	$16
	push	$callocp
	push	nl+62
	call	kread
	addl	%esp, $12
	push	callocp+8
	call	malloc
	pop	%ecx
	movl	allp, %eax
	orl	%eax, %eax
	jne	.L81
	push	$.L82
	call	panic
	pop	%ecx
.L81:
	push	callocp+8
	push	allp
	push	callocp+4
	call	kread
	addl	%esp, $12
	push	$4
	push	$cutime
	push	nl+26
	call	kread
	addl	%esp, $12
	call	settdev
	push	$.L83
	call	fttys
	pop	%ecx
	push	$.L84
	call	strlen
	pop	%ecx
	movl	-8(%ebp), %eax
	cmpl	dbflag, $0
	je	.L85
	push	$.L86
	call	strlen
	pop	%ecx
	addl	-8(%ebp), %eax
.L85:
	cmpl	gflag, $0
	je	.L87
	push	$.L88
	call	strlen
	pop	%ecx
	addl	-8(%ebp), %eax
.L87:
	cmpl	lflag, $0
	je	.L89
	push	$.L90
	call	strlen
	pop	%ecx
	addl	-8(%ebp), %eax
.L89:
	cmpl	mflag, $0
	je	.L91
	push	$.L92
	call	strlen
	pop	%ecx
	addl	-8(%ebp), %eax
.L91:
	cmpl	tflag, $0
	je	.L93
	push	$.L94
	call	strlen
	pop	%ecx
	addl	-8(%ebp), %eax
.L93:
	cmpl	nflag, $0
	je	.L20020
	jmp	.L95
.L20020:
	cmpl	dbflag, $0
	je	.L96
	push	$.L97
	call	printf
	pop	%ecx
.L96:
	push	$.L98
	call	printf
	pop	%ecx
	cmpl	gflag, $0
	je	.L99
	push	$.L100
	call	printf
	pop	%ecx
.L99:
	cmpl	lflag, $0
	je	.L101
	push	$.L102
	call	printf
	pop	%ecx
.L101:
	cmpl	mflag, $0
	je	.L103
	push	$.L104
	call	printf
	pop	%ecx
.L103:
	cmpl	tflag, $0
	je	.L105
	push	$.L106
	call	printf
	pop	%ecx
.L105:
	decl	_stdout+12
	jge	.L10004
	incl	_stdout+12
	push	$_stdout
	push	$10
	icall	_stdout+20
	addl	%esp, $8
	jmp	.L10005
.L10004:
	movl	%edi, _stdout
	incl	_stdout
	movb	(%edi), $10
.L10005:
	push	$_stdout
	call	fflush
	pop	%ecx
.L95:
	movl	%ebx, $cprocq
.L108:
	movl	%eax, 12(%ebx)
	movl	%esi, %eax
	cmpl	%eax, nl+8
	jne	.L20021
	jmp	.L107
.L20021:
	push	%esi
	call	range
	pop	%ecx
	orl	%eax, %eax
	jne	.L109
	push	$.L110
	call	panic
	pop	%ecx
.L109:
	movl	%eax, %esi
	subl	%eax, callocp+4
	addl	%eax, allp
	movl	%ebx, %eax
	testl	60(%ebx), $128
	je	.L111
	cmpl	dflag, $0
.L20017:
	jne	.L113
	jmp	.L108
.L111:
	movw	%ax, 92(%ebx)
	cmpw	%ax, $-1
	jne	.L114
	cmpl	xflag, $0
	jmp	.L20017
.L114:
	movw	%ax, 92(%ebx)
	cmpw	%ax, ttdev
	je	.L113
	cmpl	aflag, $0
	je	.L108
.L113:
	cmpl	dbflag, $0
	je	.L119
	push	%esi
	push	$.L120
	call	printf
	addl	%esp, $8
.L119:
	push	%ebx
	call	ptty
	pop	%ecx
	push	36(%ebx)
	push	$.L121
	call	printf
	addl	%esp, $8
	cmpl	gflag, $0
	je	.L122
	push	88(%ebx)
	push	$.L123
	call	printf
	addl	%esp, $8
.L122:
	cmpl	lflag, $0
	jne	.L20022
	jmp	.L124
.L20022:
	push	40(%ebx)
	push	$.L125
	call	printf
	addl	%esp, $8
	push	$_stdout
	call	fflush
	pop	%ecx
	push	%ebx
	call	uname
	pop	%ecx
	push	%eax
	push	$.L126
	call	printf
	addl	%esp, $8
	push	$_stdout
	call	fflush
	pop	%ecx
	push	%ebx
	call	psize
	pop	%ecx
	push	$_stdout
	call	fflush
	pop	%ecx
	push	60(%ebx)
	push	$.L127
	call	printf
	addl	%esp, $8
	push	$_stdout
	call	fflush
	pop	%ecx
	push	%esi
	push	%ebx
	call	state
	addl	%esp, $8
	movl	-4(%ebp), %eax
	push	%eax
	push	$.L128
	call	printf
	addl	%esp, $8
	push	$_stdout
	call	fflush
	pop	%ecx
	movl	%eax, -4(%ebp)
	cmpl	%eax, $83
	jne	.L129
	push	80(%ebx)
	push	$.L130
	call	printf
	addl	%esp, $8
	push	$_stdout
	call	fflush
	jmp	.L20016
.L129:
	cmpl	fflag, $0
	je	.L132
	push	$.L133
	jmp	.L20019
.L132:
	push	$.L135
.L20019:
	call	printf
.L20016:
	pop	%ecx
	push	$_stdout
	call	fflush
	pop	%ecx
.L124:
	cmpl	mflag, $0
	je	.L136
	push	%esi
	push	%ebx
	call	cval
	addl	%esp, $8
	push	%eax
	push	$.L137
	call	printf
	addl	%esp, $8
	push	104(%ebx)
	push	$.L138
	call	printf
	addl	%esp, $8
	push	108(%ebx)
	push	$.L139
	call	printf
	addl	%esp, $8
	push	112(%ebx)
	push	$.L140
	call	printf
	addl	%esp, $8
.L136:
	cmpl	tflag, $0
	je	.L141
	push	120(%ebx)
	call	ptime
	pop	%ecx
	push	124(%ebx)
	call	ptime
	pop	%ecx
.L141:
	push	$.L142
	call	printf
	pop	%ecx
	cmpl	wflag, $0
	je	.L10006
	movl	%eax, $132
	jmp	.L10007
.L10006:
	movl	%eax, $80
.L10007:
	subl	%eax, -8(%ebp)
	decl	%eax
	push	%eax
	push	%ebx
	call	printl
	addl	%esp, $8
	decl	_stdout+12
	jge	.L10008
	incl	_stdout+12
	push	$_stdout
	push	$10
	icall	_stdout+20
	addl	%esp, $8
	jmp	.L10009
.L10008:
	movl	%edi, _stdout
	incl	_stdout
	movb	(%edi), $10
.L10009:
	push	$_stdout
	call	fflush
	pop	%ecx
	jmp	.L108
.L107:
	call	rttys
	pop	%ebx
	pop	%edi
	pop	%esi
	leave
	ret
	.align	4
	.globl settdev
settdev:
	push	%ebp
	movl	%ebp, %esp
	push	%esi
	push	%edi
	push	%ebx
	call	getpid
	movl	%edi, %eax
	movl	%ebx, $cprocq
.L145:
	movl	%eax, 8(%ebx)
	movl	%esi, %eax
	cmpl	%eax, nl+8
	je	.L144
	push	%esi
	call	range
	pop	%ecx
	orl	%eax, %eax
	je	.L144
	movl	%eax, %esi
	subl	%eax, callocp+4
	addl	%eax, allp
	movl	%ebx, %eax
	cmpl	%edi, 36(%ebx)
	jne	.L145
	movw	%ax, 92(%ebx)
	movw	ttdev, %ax
	jmp	.L143
.L144:
	movw	ttdev, $-1
.L143:
	pop	%ebx
	pop	%edi
	pop	%esi
	leave
	ret
	.align	4
	.globl fttys
fttys:

	.data

.L149:
	.byte	46
	.byte	0

	.text


	.data

.L151:
	.byte	67
	.byte	97
	.byte	110
	.byte	110
	.byte	111
	.byte	116
	.byte	32
	.byte	111
	.byte	112
	.byte	101
	.byte	110
	.byte	32
	.byte	39
	.byte	37
	.byte	115
	.byte	39
	.byte	32
	.byte	105
	.byte	110
	.byte	32
	.byte	47
	.byte	100
	.byte	101
	.byte	118
	.byte	10
	.byte	0

	.text


	.data

.L158:
	.byte	46
	.byte	46
	.byte	0

	.text

	enter	$52, $0
	push	8(%ebp)
	call	chdir
	pop	%ecx
	push	$0
	push	$.L149
	call	open
	addl	%esp, $8
	movl	-4(%ebp), %eax
	orl	%eax, %eax
	jge	.L150
	push	8(%ebp)
	push	$.L151
	push	$_stderr
	call	fprintf
	addl	%esp, $12
	push	$1
	call	exit
	pop	%ecx
.L150:
	push	$16
	lea	%eax, -52(%ebp)
	push	%eax
	push	-4(%ebp)
	call	read
	addl	%esp, $12
	push	$16
	lea	%eax, -52(%ebp)
	push	%eax
	push	-4(%ebp)
	call	read
	addl	%esp, $12
.L153:
	push	$16
	lea	%eax, -52(%ebp)
	push	%eax
	push	-4(%ebp)
	call	read
	addl	%esp, $12
	orl	%eax, %eax
	je	.L152
	lea	%eax, -36(%ebp)
	push	%eax
	lea	%eax, -50(%ebp)
	push	%eax
	call	stat
	addl	%esp, $8
	orl	%eax, %eax
	jl	.L153
	testw	-32(%ebp), $8192
	je	.L155
	movzx	%eax, -24(%ebp)
	push	%eax
	lea	%eax, -50(%ebp)
	push	%eax
	call	addname
	addl	%esp, $8
	jmp	.L153
.L155:
	testw	-32(%ebp), $16384
	je	.L153
	lea	%eax, -50(%ebp)
	push	%eax
	call	fttys
	pop	%ecx
	jmp	.L153
.L152:
	push	-4(%ebp)
	call	close
	pop	%ecx
	push	$.L158
	call	chdir
	pop	%ecx
	leave
	ret
	.align	4
	.globl addname
addname:

	.data

.L162:
	.byte	79
	.byte	117
	.byte	116
	.byte	32
	.byte	111
	.byte	102
	.byte	32
	.byte	109
	.byte	101
	.byte	109
	.byte	111
	.byte	114
	.byte	121
	.byte	10
	.byte	0

	.text


	.data

.L168:
	.byte	79
	.byte	117
	.byte	116
	.byte	32
	.byte	111
	.byte	102
	.byte	32
	.byte	109
	.byte	101
	.byte	109
	.byte	111
	.byte	114
	.byte	121
	.byte	10
	.byte	0

	.text

	push	%ebp
	movl	%ebp, %esp
	push	%edi
	cmpl	fptr, $0
	jne	.L160
	push	$20
	call	malloc
	pop	%ecx
	movl	fptr, %eax
	orl	%eax, %eax
	jne	.L161
	push	$.L162
	push	$_stderr
	call	fprintf
	addl	%esp, $8
	push	$1
	call	exit
	pop	%ecx
.L161:
	movl	%edi, fptr
	movl	16(%edi), $0
	push	8(%ebp)
	movl	%eax, fptr
	addl	%eax, $2
	push	%eax
	call	strcpy
	addl	%esp, $8
	movl	%eax, 12(%ebp)
	movl	%edi, fptr
	jmp	.L20023
.L160:
	movl	%eax, fptr
.L20024:
	movl	lptr, %eax
	movl	%edi, lptr
	cmpl	16(%edi), $0
	je	.L164
	movzx	%eax, (%edi)
	cmpl	%eax, 12(%ebp)
	je	.L164
	movl	%eax, 16(%edi)
	jmp	.L20024
.L164:
	movl	%edi, lptr
	movzx	%eax, (%edi)
	cmpl	%eax, 12(%ebp)
	je	.L159
	push	$20
	call	malloc
	pop	%ecx
	movl	%edi, lptr
	movl	16(%edi), %eax
	orl	%eax, %eax
	jne	.L167
	push	$.L168
	push	$_stderr
	call	fprintf
	addl	%esp, $8
	push	$1
	call	exit
	pop	%ecx
.L167:
	movl	%edi, lptr
	movl	%eax, 16(%edi)
	movl	lptr, %eax
	movl	%edi, lptr
	movl	16(%edi), $0
	push	8(%ebp)
	addl	%eax, $2
	push	%eax
	call	strcpy
	addl	%esp, $8
	movl	%eax, 12(%ebp)
	movl	%edi, lptr
.L20023:
	movw	(%edi), %ax
.L159:
	pop	%edi
	leave
	ret
	.align	4
	.globl rttys
rttys:
	push	%ebp
	movl	%ebp, %esp
	push	%edi
	movl	%eax, fptr
	movl	lptr, %eax
.L171:
	movl	%edi, lptr
	cmpl	16(%edi), $0
	je	.L170
	movl	%eax, lptr
	movl	fptr, %eax
	movl	%edi, fptr
	movl	%eax, 16(%edi)
	movl	lptr, %eax
	push	%edi
	call	free
	pop	%ecx
	jmp	.L171
.L170:
	push	lptr
	call	free
	pop	%ecx
	pop	%edi
	leave
	ret
	.align	4
	.globl ptty
ptty:

	.data

.L174:
	.byte	45
	.byte	45
	.byte	45
	.byte	45
	.byte	45
	.byte	45
	.byte	45
	.byte	0

	.text


	.data

.L175:
	.byte	40
	.byte	100
	.byte	32
	.byte	61
	.byte	32
	.byte	112
	.byte	112
	.byte	45
	.byte	62
	.byte	112
	.byte	95
	.byte	116
	.byte	116
	.byte	100
	.byte	101
	.byte	118
	.byte	41
	.byte	58
	.byte	32
	.byte	37
	.byte	120
	.byte	44
	.byte	32
	.byte	100
	.byte	58
	.byte	32
	.byte	37
	.byte	120
	.byte	44
	.byte	32
	.byte	112
	.byte	112
	.byte	45
	.byte	62
	.byte	112
	.byte	95
	.byte	116
	.byte	116
	.byte	100
	.byte	101
	.byte	118
	.byte	58
	.byte	32
	.byte	37
	.byte	120
	.byte	44
	.byte	32
	.byte	78
	.byte	79
	.byte	68
	.byte	69
	.byte	86
	.byte	58
	.byte	32
	.byte	37
	.byte	120
	.byte	10
	.byte	0

	.text


	.text

.L10010:
	.word	-1

	.text


	.data

.L179:
	.byte	37
	.byte	45
	.byte	55
	.byte	46
	.byte	55
	.byte	115
	.byte	0

	.text


	.data

.L181:
	.byte	37
	.byte	45
	.byte	55
	.byte	46
	.byte	55
	.byte	115
	.byte	0

	.text


	.data

.L182:
	.byte	37
	.byte	51
	.byte	100
	.byte	32
	.byte	37
	.byte	51
	.byte	100
	.byte	0

	.text

	enter	$4, $0
	push	%edi
	push	%ebx
	movl	%ebx, 8(%ebp)
	movw	%ax, 92(%ebx)
	movzx	%eax, %ax
	movw	-4(%ebp), %ax
	cmpl	%eax, $-1
	jne	.L173
	push	$.L174
	call	printf
	pop	%ecx
	jmp	.L172
.L173:
	movzx	%eax, .L10010
	push	%eax
	movzx	%eax, 92(%ebx)
	push	%eax
	movzx	%eax, -4(%ebp)
	push	%eax
	movw	%ax, 92(%ebx)
	movzx	%eax, %ax
	movw	-4(%ebp), %ax
	push	%eax
	push	$.L175
	call	printf
	addl	%esp, $20
	movl	%eax, fptr
.L20025:
	movl	lptr, %eax
	movl	%edi, lptr
	cmpl	16(%edi), $0
	je	.L176
	movw	%ax, (%edi)
	cmpw	%ax, -4(%ebp)
	jne	.L178
	movl	%eax, lptr
	addl	%eax, $2
	push	%eax
	push	$.L179
.L20027:
	call	printf
	addl	%esp, $8
	jmp	.L172
.L178:
	movl	%edi, lptr
	movl	%eax, 16(%edi)
	jmp	.L20025
.L176:
	movl	%edi, lptr
	movw	%ax, (%edi)
	cmpw	%ax, -4(%ebp)
	jne	.L180
	movl	%eax, lptr
	addl	%eax, $2
	push	%eax
	push	$.L181
	jmp	.L20027
.L180:
	movzx	%eax, -4(%ebp)
	andl	%eax, $255
	push	%eax
	movzx	%eax, -4(%ebp)
	shrl	%eax, $8
	andl	%eax, $255
	push	%eax
	push	$.L182
	call	printf
	addl	%esp, $12
.L172:
	pop	%ebx
	pop	%edi
	leave
	ret
	.align	4
	.globl uname
uname:

	.bss

	.align	1
.L184:
	.blkb	0x8

	.text


	.data

.L186:
	.byte	37
	.byte	100
	.byte	0

	.text

	push	%ebp
	movl	%ebp, %esp
	push	%esi
	push	%ebx
	movl	%ebx, 8(%ebp)
	push	48(%ebx)
	call	getpwuid
	pop	%ecx
	movl	%esi, %eax
	orl	%eax, %eax
	je	.L185
	movl	%eax, (%esi)
	jmp	.L183
.L185:
	push	48(%ebx)
	push	$.L186
	push	$.L184
	call	sprintf
	addl	%esp, $12
	movl	%eax, $.L184
.L183:
	pop	%ebx
	pop	%esi
	leave
	ret
	.align	4
	.globl cval
cval:
	enter	$4, $0
	push	%esi
	push	%edi
	push	%ebx
	movl	%ebx, 8(%ebp)
	movl	%eax, 56(%ebx)
	cmpl	%eax, $1
	jne	.L188
	movl	%eax, cutime
	subl	%eax, 116(%ebx)
	movl	-4(%ebp), %eax
	movl	%eax, 100(%ebx)
	addl	%eax, -4(%ebp)
	cmpl	%eax, 100(%ebx)
	jbe	.L189
	movl	%eax, -4(%ebp)
	addl	%eax, 100(%ebx)
	jmp	.L187
.L189:
	movl	%eax, $-1
	jmp	.L187
.L188:
	movl	-4(%ebp), $0
	movl	%esi, $cprocq
.L191:
	movl	%eax, (%esi)
	movl	%edi, %eax
	cmpl	%eax, nl+8
	je	.L190
	push	%edi
	call	range
	pop	%ecx
	orl	%eax, %eax
	je	.L190
	movl	%eax, %edi
	subl	%eax, callocp+4
	addl	%eax, allp
	movl	%esi, %eax
	movl	%eax, 100(%esi)
	subl	-4(%ebp), %eax
	cmpl	%edi, 12(%ebp)
	jne	.L191
	movl	%eax, -4(%ebp)
	jmp	.L187
.L190:
	call	getpid
	cmpl	%eax, 36(%ebx)
	jne	.L194
	movl	%eax, 100(%ebx)
	jmp	.L187
.L194:
	subl	%eax, %eax
.L187:
	pop	%ebx
	pop	%edi
	pop	%esi
	leave
	ret
	.align	4
	.globl psize
psize:

	.data

.L203:
	.byte	32
	.byte	32
	.byte	32
	.byte	63
	.byte	75
	.byte	0

	.text


	.data

.L205:
	.byte	37
	.byte	52
	.byte	108
	.byte	100
	.byte	75
	.byte	0

	.text


	.text

.L10012:
	.long	1024

	.text


	.data

.L208:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	45
	.byte	0

	.text


	.data

.L210:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	0

	.text

	enter	$4, $0
	push	%esi
	push	%edi
	push	%ebx
	movl	%ebx, 8(%ebp)
	movl	-4(%ebp), $0
	subl	%edi, %edi
.L198:
	cmpl	%edi, $5
	jge	.L196
	cmpl	rflag, $0
	jne	.L199
	orl	%edi, %edi
	je	.L197
	cmpl	%edi, $4
	jne	.L199
.L197:
	incl	%edi
	jmp	.L198
.L199:
	movl	%ecx, %edi
	sall	%ecx, $2
	addl	%ecx, %ebx
	movl	%eax, 16(%ecx)
	movl	%esi, %eax
	orl	%eax, %eax
	je	.L197
	push	%esi
	call	range
	pop	%ecx
	orl	%eax, %eax
	jne	.L202
	push	$.L203
	jmp	.L20031
.L202:
	movl	%eax, %esi
	subl	%eax, callocp+4
	addl	%eax, allp
	movl	%esi, %eax
	movl	%eax, 20(%esi)
	addl	-4(%ebp), %eax
	jmp	.L197
.L196:
	cmpl	-4(%ebp), $0
	je	.L204
	movl	%eax, -4(%ebp)
	cdq
	idivl	.L10012
	push	%eax
	push	$.L205
	call	printf
	addl	%esp, $8
	jmp	.L195
.L204:
	cmpl	fflag, $0
	je	.L207
	push	$.L208
	jmp	.L20031
.L207:
	push	$.L210
.L20031:
	call	printf
	pop	%ecx
.L195:
	pop	%ebx
	pop	%edi
	pop	%esi
	leave
	ret
	.align	4
	.globl state
state:
	push	%ebp
	movl	%ebp, %esp
	push	%esi
	push	%ebx
	movl	%ebx, 8(%ebp)
	movl	%esi, 56(%ebx)
	cmpl	%esi, $1
	jne	.L212
	movl	%eax, 80(%ebx)
	cmpl	%eax, 12(%ebp)
	jne	.L213
	movl	%eax, $87
	jmp	.L211
.L213:
	testl	60(%ebx), $32
	je	.L214
	movl	%eax, $84
	jmp	.L211
.L214:
	movl	%eax, $83
	jmp	.L211
.L212:
	cmpl	%esi, $2
	jne	.L215
	movl	%eax, $82
	jmp	.L211
.L215:
	cmpl	%esi, $3
	jne	.L216
	movl	%eax, $90
	jmp	.L211
.L216:
	movl	%eax, $63
.L211:
	pop	%ebx
	pop	%esi
	leave
	ret
	.align	4
	.globl ptime
ptime:

	.text

.L10013:
	.long	100

	.text


	.data

.L220:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	45
	.byte	0

	.text


	.data

.L222:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	0

	.text


	.text

.L10014:
	.long	60

	.text


	.data

.L224:
	.byte	37
	.byte	54
	.byte	100
	.byte	0

	.text


	.data

.L225:
	.byte	32
	.byte	37
	.byte	50
	.byte	100
	.byte	58
	.byte	37
	.byte	48
	.byte	50
	.byte	100
	.byte	0

	.text


	.text

.L10015:
	.long	60

	.text

	push	%ebp
	movl	%ebp, %esp
	push	%ebx
	movl	%eax, 8(%ebp)
	addl	%eax, $50
	cdq
	idivl	.L10013
	movl	8(%ebp), %eax
	orl	%eax, %eax
	jne	.L218
	cmpl	fflag, $0
	je	.L219
	push	$.L220
.L20033:
	call	printf
	pop	%ecx
	jmp	.L217
.L219:
	push	$.L222
	jmp	.L20033
.L218:
	movl	%eax, 8(%ebp)
	cdq
	idivl	.L10014
	movl	%ebx, %eax
	cmpl	%eax, $100
	jb	.L223
	push	%ebx
	push	$.L224
	call	printf
	addl	%esp, $8
	jmp	.L217
.L223:
	movl	%eax, 8(%ebp)
	subl	%edx, %edx
	divl	.L10015
	push	%edx
	push	%ebx
	push	$.L225
	call	printf
	addl	%esp, $12
.L217:
	pop	%ebx
	leave
	ret
	.align	4
	.globl printl
printl:

	.bss

	.align	4
.L227:
	.blkb	0x4

	.text


	.data

.L230:
	.byte	60
	.byte	105
	.byte	100
	.byte	108
	.byte	101
	.byte	62
	.byte	0

	.text


	.data

.L232:
	.byte	32
	.byte	37
	.byte	115
	.byte	32
	.byte	0

	.text


	.data

.L234:
	.byte	60
	.byte	37
	.byte	46
	.byte	42
	.byte	115
	.byte	62
	.byte	0

	.text


	.data

.L235:
	.byte	0

	.text


	.data

.L237:
	.byte	115
	.byte	101
	.byte	103
	.byte	114
	.byte	101
	.byte	97
	.byte	100
	.byte	32
	.byte	50
	.byte	32
	.byte	105
	.byte	110
	.byte	32
	.byte	112
	.byte	114
	.byte	105
	.byte	110
	.byte	116
	.byte	108
	.byte	10
	.byte	0

	.text


	.data

.L238:
	.byte	117
	.byte	46
	.byte	117
	.byte	95
	.byte	97
	.byte	114
	.byte	103
	.byte	112
	.byte	58
	.byte	32
	.byte	37
	.byte	120
	.byte	44
	.byte	32
	.byte	115
	.byte	114
	.byte	112
	.byte	45
	.byte	62
	.byte	115
	.byte	114
	.byte	95
	.byte	98
	.byte	97
	.byte	115
	.byte	101
	.byte	58
	.byte	32
	.byte	37
	.byte	120
	.byte	10
	.byte	0

	.text


	.data

.L239:
	.byte	117
	.byte	46
	.byte	117
	.byte	95
	.byte	97
	.byte	114
	.byte	103
	.byte	99
	.byte	58
	.byte	32
	.byte	37
	.byte	120
	.byte	32
	.byte	115
	.byte	114
	.byte	112
	.byte	45
	.byte	62
	.byte	115
	.byte	114
	.byte	95
	.byte	115
	.byte	105
	.byte	122
	.byte	101
	.byte	58
	.byte	32
	.byte	37
	.byte	120
	.byte	10
	.byte	0

	.text


	.data

.L241:
	.byte	66
	.byte	97
	.byte	100
	.byte	32
	.byte	115
	.byte	101
	.byte	103
	.byte	114
	.byte	101
	.byte	97
	.byte	100
	.byte	40
	.byte	41
	.byte	10
	.byte	0

	.text

	enter	$8, $0
	push	%esi
	push	%edi
	push	%ebx
	movl	%ebx, 8(%ebp)
	movl	%eax, 56(%ebx)
	cmpl	%eax, $3
	jne	.L20038
	jmp	.L226
.L20038:
	cmpl	36(%ebx), $0
	jne	.L229
	push	$.L230
	call	printf
	pop	%ecx
	jmp	.L226
.L229:
	push	$u
	push	16(%ebx)
	call	u_init
	addl	%esp, $8
	orl	%eax, %eax
	jne	.L20039
	jmp	.L226
.L20039:
	push	$u+468
	push	$.L232
	call	printf
	addl	%esp, $8
	jmp	.L226
.L243:
	movl	%eax, -4(%ebp)
	decl	-4(%ebp)
	orl	%eax, %eax
	jne	.L245
	jmp	.L226
.L245:
	movl	%ecx, %esi
	incl	%esi
	movsxb	%eax, (%ecx)
	movl	%edi, %eax
	orl	%eax, %eax
	je	.L244
	testl	%edi, $128
	jne	.L10018
	testb	_ctype+1(%edi), $87
	jne	.L246
.L10018:
	decl	_stdout+12
	jge	.L10019
	incl	_stdout+12
	push	$_stdout
	push	$46
.L20037:
	icall	_stdout+20
	addl	%esp, $8
	jmp	.L245
.L10019:
	movl	%ecx, _stdout
	incl	_stdout
	movb	(%ecx), $46
	jmp	.L245
.L246:
	movl	%eax, 12(%ebp)
	decl	12(%ebp)
	orl	%eax, %eax
	je	.L226
	decl	_stdout+12
	jge	.L10021
	incl	_stdout+12
	push	$_stdout
	push	%edi
	jmp	.L20037
.L10021:
	movl	%ecx, _stdout
	incl	_stdout
	movl	%eax, %edi
	movb	(%ecx), %al
	jmp	.L245
.L244:
	push	$_stdout
	call	fflush
	pop	%ecx
	cmpl	-4(%ebp), $0
	jne	.L20040
	jmp	.L243
.L20040:
	decl	_stdout+12
	jge	.L10023
	incl	_stdout+12
	push	$_stdout
	push	$32
	icall	_stdout+20
	addl	%esp, $8
	jmp	.L243
.L10023:
	movl	%ecx, _stdout
	incl	_stdout
	movb	(%ecx), $32
	jmp	.L243
.L226:
	pop	%ebx
	pop	%edi
	pop	%esi
	leave
	ret
	.align	4
	.globl u_init
u_init:
	push	%ebp
	movl	%ebp, %esp
	push	%ebx
	push	8(%ebp)
	call	range
	pop	%ecx
	orl	%eax, %eax
	je	.L20042
	movl	%eax, 8(%ebp)
	subl	%eax, callocp+4
	addl	%eax, allp
	movl	%ebx, %eax
	testw	12(%ebx), $1
	je	.L251
	push	$536
	push	12(%ebp)
	push	$0
	push	24(%ebx)
	call	pt_mread
	addl	%esp, $16
	orl	%eax, %eax
	je	.L20042
.L253:
	movl	%eax, $1
	jmp	.L249
.L251:
	push	$536
	push	12(%ebp)
	movl	%eax, 28(%ebx)
	sall	%eax, $9
	push	%eax
	call	dread
	addl	%esp, $12
	orl	%eax, %eax
	jge	.L253
.L20042:
	subl	%eax, %eax
.L249:
	pop	%ebx
	leave
	ret
	.align	4
	.globl segread
segread:
	enter	$4, $0
	push	%esi
	push	%edi
	push	%ebx
	movl	%edi, 8(%ebp)
	push	12(%edi)
	call	range
	pop	%ecx
	orl	%eax, %eax
	je	.L20045
	movl	%edi, 8(%ebp)
	movl	%eax, 12(%edi)
	subl	%eax, callocp+4
	addl	%eax, allp
	movl	%ebx, %eax
	testw	12(%ebx), $2
	jne	.L257
	movl	%eax, 12(%ebp)
	subl	%eax, 4(%edi)
	jmp	.L20044
.L257:
	movl	%ecx, 8(%ebp)
	movl	%esi, 8(%ebp)
	movl	%edi, 4(%esi)
	subl	%edi, 8(%ecx)
	movl	%eax, 12(%ebp)
	subl	%eax, %edi
.L20044:
	movl	-4(%ebp), %eax
	testw	12(%ebx), $1
	je	.L259
	push	20(%ebp)
	push	16(%ebp)
	push	%eax
	push	24(%ebx)
	call	pt_mread
	addl	%esp, $16
	orl	%eax, %eax
	je	.L20045
.L261:
	movl	%eax, $1
	jmp	.L255
.L259:
	push	20(%ebp)
	push	16(%ebp)
	movl	%eax, 28(%ebx)
	sall	%eax, $9
	addl	%eax, 12(%ebp)
	push	%eax
	call	dread
	addl	%esp, $12
	orl	%eax, %eax
	jge	.L261
.L20045:
	subl	%eax, %eax
.L255:
	pop	%ebx
	pop	%edi
	pop	%esi
	leave
	ret
	.align	4
	.globl kread
kread:

	.data

.L265:
	.byte	75
	.byte	101
	.byte	114
	.byte	110
	.byte	101
	.byte	108
	.byte	32
	.byte	109
	.byte	101
	.byte	109
	.byte	111
	.byte	114
	.byte	121
	.byte	32
	.byte	114
	.byte	101
	.byte	97
	.byte	100
	.byte	32
	.byte	101
	.byte	114
	.byte	114
	.byte	111
	.byte	114
	.byte	0

	.text

	push	%ebp
	movl	%ebp, %esp
	push	$0
	push	8(%ebp)
	push	kfd
	call	lseek
	addl	%esp, $12
	push	16(%ebp)
	push	12(%ebp)
	push	kfd
	call	read
	addl	%esp, $12
	cmpl	%eax, 16(%ebp)
	je	.L263
	push	$.L265
	call	panic
	pop	%ecx
.L263:
	leave
	ret
	.align	4
	.globl mread
mread:

	.data

.L268:
	.byte	77
	.byte	101
	.byte	109
	.byte	111
	.byte	114
	.byte	121
	.byte	32
	.byte	114
	.byte	101
	.byte	97
	.byte	100
	.byte	32
	.byte	101
	.byte	114
	.byte	114
	.byte	111
	.byte	114
	.byte	0

	.text

	push	%ebp
	movl	%ebp, %esp
	push	$0
	push	8(%ebp)
	push	mfd
	call	lseek
	addl	%esp, $12
	push	16(%ebp)
	push	12(%ebp)
	push	mfd
	call	read
	addl	%esp, $12
	cmpl	%eax, 16(%ebp)
	je	.L266
	push	$.L268
	call	panic
	pop	%ecx
.L266:
	leave
	ret
	.align	4
	.globl dread
dread:

	.data

.L272:
	.byte	83
	.byte	119
	.byte	97
	.byte	112
	.byte	32
	.byte	114
	.byte	101
	.byte	97
	.byte	100
	.byte	32
	.byte	101
	.byte	114
	.byte	114
	.byte	111
	.byte	114
	.byte	0

	.text

	push	%ebp
	movl	%ebp, %esp
	cmpl	dfd, $0
	jle	.L270
	push	$0
	push	8(%ebp)
	push	dfd
	call	lseek
	addl	%esp, $12
	push	16(%ebp)
	push	12(%ebp)
	push	dfd
	call	read
	addl	%esp, $12
	cmpl	%eax, 16(%ebp)
	je	.L271
	push	$.L272
	call	panic
	pop	%ecx
.L271:
	movl	%eax, $1
	jmp	.L269
.L270:
	subl	%eax, %eax
.L269:
	leave
	ret
	.align	4
	.globl panic
panic:

	.data

.L274:
	.byte	37
	.byte	114
	.byte	0

	.text


	.data

.L275:
	.byte	10
	.byte	0

	.text

	push	%ebp
	movl	%ebp, %esp
	push	$_stdout
	call	fflush
	pop	%ecx
	push	$2
	call	sleep
	pop	%ecx
	lea	%eax, 8(%ebp)
	push	%eax
	push	$.L274
	push	$_stderr
	call	fprintf
	addl	%esp, $12
	push	$.L275
	push	$_stderr
	call	fprintf
	addl	%esp, $8
	push	$_stderr
	call	fflush
	pop	%ecx
	push	$1
	call	exit
	pop	%ecx
	leave
	ret
	.align	4
	.globl range
range:
	push	%ebp
	movl	%ebp, %esp
	movl	%eax, 8(%ebp)
	cmpl	%eax, callocp+4
	jb	.L10025
	movl	%eax, callocp+8
	addl	%eax, callocp+4
	cmpl	%eax, 8(%ebp)
	jbe	.L10025
	movl	%eax, $1
	jmp	.L276
.L10025:
	subl	%eax, %eax
.L276:
	leave
	ret
	.align	4
	.globl pt_mread
pt_mread:
	enter	$12, $0
	movl	%eax, 12(%ebp)
	shrl	%eax, $12
	push	%eax
	push	8(%ebp)
	call	pt_index
	addl	%esp, $8
	movl	-12(%ebp), %eax
	andl	%eax, $1
	cmpl	%eax, $1
	je	.L278
.L20047:
	subl	%eax, %eax
	jmp	.L277
.L278:
	andl	-12(%ebp), $-4096
	movl	%eax, 12(%ebp)
	andl	%eax, $4095
	movl	-8(%ebp), %eax
	movl	%eax, $4096
	subl	%eax, -8(%ebp)
	cmpl	%eax, 20(%ebp)
	jbe	.L10027
	movl	%eax, 20(%ebp)
	jmp	.L20046
.L10027:
	movl	%eax, $4096
	subl	%eax, -8(%ebp)
.L20046:
	movl	-4(%ebp), %eax
	cmpl	20(%ebp), $0
	jle	.L279
	push	%eax
	push	16(%ebp)
	movl	%eax, -8(%ebp)
	addl	%eax, -12(%ebp)
	push	%eax
	call	mread
	addl	%esp, $12
	movl	%eax, -4(%ebp)
	subl	20(%ebp), %eax
	addl	12(%ebp), %eax
	addl	16(%ebp), %eax
	movl	%eax, 12(%ebp)
	shrl	%eax, $12
	push	%eax
	push	8(%ebp)
	call	pt_index
	addl	%esp, $8
	movl	-12(%ebp), %eax
	andl	%eax, $1
	cmpl	%eax, $1
	jne	.L20047
	andl	-12(%ebp), $-4096
	movl	%eax, 12(%ebp)
	andl	%eax, $4095
	movl	-8(%ebp), %eax
	movl	%eax, 20(%ebp)
	cmpl	%eax, $4096
	jl	.L20046
	movl	%eax, $4096
	jmp	.L20046
.L279:
	movl	%eax, $1
.L277:
	leave
	ret
	.align	4
	.globl pt_index
pt_index:
	enter	$4, $0
	push	$4
	lea	%eax, -4(%ebp)
	push	%eax
	movl	%eax, 12(%ebp)
	sall	%eax, $2
	addl	%eax, 8(%ebp)
	push	%eax
	call	kread
	addl	%esp, $12
	movl	%eax, -4(%ebp)
	leave
	ret
	.align	4
