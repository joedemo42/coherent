/* (-lgl
 * 	COHERENT Driver Kit Version x.x.x
 * 	Copyright (c) 1982, 1990 by Mark Williams Company.
 * 	All rights reserved. May not be copied without permission.
 *
 * $Log:	alx.c,v $
 * Revision 2.8  92/02/14  10:14:48  bin
 * update by hal (post 321?)
 * 
 * Revision 1.4  92/02/07  09:41:21  hal
 * Fix Wallenberg bug.
 * 
 * Revision 2.10  92/02/04  18:50:00  hal
 * Use EBUSY, not EDBUSY - merge for 386 code.
 * 
 * Revision 2.9  92/01/13  08:38:24  hal
 * Rearrange alxopen() to deal with ill-behaved daemons.
 * 
 * Revision 2.8  92/01/12  19:23:50  hal
 * Tracking nasty Kelly bug.
 * 
 * Revision 2.6  91/12/26  16:52:05  hal
 * Flags left in bad state if open r-device got killed.
 *
 * Revision 2.5  91/12/20  14:08:27  hal
 * Add static alx_send().
 * From alxstart(), don't toggle Tx interrupts - call alx_send().
 *
 * Revision 2.4  91/12/10  08:04:13  hal
 * Make interrupt routine clear all UART irq conditions if it gets an
 * interrupt without an argument telling which port interrupted.
 *
 * Revision 2.3  91/12/05  09:35:06  hal
 * Working 16550A code.  Nfg on GeeSee.
 *
 * Revision 2.2  91/12/02  19:21:45  hal
 * Last version before FIFO testing.
 *
 * Revision 2.1  91/11/21  18:17:44  hal
 * Same as V1.13 - used in 3.2.05k
 *
 -lgl) */
/*
 * Shared parts of IBM async port drivers.
 */
#include <sys/coherent.h>
#ifndef _I386
#include <sys/i8086.h>
#endif
#include <sys/al.h>
#include <sys/con.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/tty.h>
#include <sys/uproc.h>
#include <sys/timeout.h>
#include <sys/clist.h>
#include <sys/ins8250.h>
#include <sys/sched.h>

#ifdef _I386
#define	EEBUSY	EBUSY
#else
#define	EEBUSY	EDBUSY
#endif

#define ALPORT	(((COM_DDP *)(tp->t_ddp))->port)
#define AL_NUM	(((COM_DDP *)(tp->t_ddp))->com_num)

#define DTRTMOUT  3	/* DTR timeout interval in seconds for close */
#define	IENABLE	(IE_RxI+IE_TxI+IE_LSI+IE_MSI)

/*
 * For rawin silo (see ktty.h), use last element of si_buf to count the number
 * of characters in the silo.
 */
#define SILO_CHAR_COUNT	si_buf[SI_BUFSIZ-1]
#define SILO_HIGH_MARK	(SI_BUFSIZ-SI_BUFSIZ/4)
#define SILO_LOW_MARK	(SI_BUFSIZ/4)
#define MAX_SILO_INDEX	(SI_BUFSIZ-2)
#define MAX_SILO_CHARS	(SI_BUFSIZ-1)

/*
 * The following silo FLUSH macros are always called at high priority!
 */
#define RAWIN_FLUSH(tp)	{ tp->t_rawin.si_ox = tp->t_rawin.si_ix; \
			tp->t_rawin.SILO_CHAR_COUNT = 0; }
#define RAWOUT_FLUSH(tp) { tp->t_rawout.si_ox = tp->t_rawout.si_ix; }

int	al_sg_set = 0;
int	al_sg_clr = 0;
static	int poll_divisor;  /* set by set_poll_rate(), read by alxclk() */

/*
 * functions herein
 */
int	alxopen();
int	alxclose();
int	alxtimer();
int	alxioctl();
int	alxparam();
int	alxcycle();
int	alxstart();
int	alxbreak();
int	alxintr();
static	int alxclk();
static	set_poll_rate();
static	void alxpoll();
static	void alx_send();
static	int iocbaud[4];
static	char ioclcr[4];

/*
 * Baud rate table and polling rate table.
 * Indexed by ioctl bit rates.
 */
int albaud[] ={
	0,				/* 0 */
	2304,				/* 50 */
	1536,				/* 75 */
	1047,				/* 110 */
	857,				/* 134.5 */
	768,				/* 150 */
	576,				/* 200 */
	384,				/* 300 */
	192,				/* 600 */
	96,				/* 1200 */
	64,				/* 1800 */
	58,				/* 2000 */
	48,				/* 2400 */
	32,				/* 3600 */
	24,				/* 4800 */
	16,				/* 7200 */
	12,				/* 9600 */
	6,				/* 19200 */
	0,				/* EXTA */
	0				/* EXTB */
};

/*
 *	alp_rate[] is tied to albaud[] - it gives the minimum polling
 *	rate for the corresponding port speed; it must be a multiple
 *	of 100 (system clock Hz) and >= baud/6
 */
int alp_rate[] ={			/* baud/6 or zero */
	0,				/* 0 */
	1*HZ,				/* 50 */
	1*HZ,				/* 75 */
	1*HZ,				/* 110 */
	1*HZ,				/* 134.5 */
	1*HZ,				/* 150 */
	1*HZ,				/* 200 */
	1*HZ,				/* 300 */
	1*HZ,				/* 600 */
	2*HZ,				/* 1200 */
	3*HZ,				/* 1800 */
	4*HZ,				/* 2000 */
	4*HZ,				/* 2400 */
	6*HZ,				/* 3600 */
	8*HZ,				/* 4800 */
	12*HZ,				/* 7200 */
	16*HZ,				/* 9600 */
	32*HZ,				/* 19200 */
	0,				/* EXTA */
	0				/* EXTB */
};

/*
 *	the following is for debug only
 */
#if DEBUG
#define CDUMP(text, tp)	cdump(text, tp);

cdump(message, tp)
char *message;
TTY *tp;
{
	int i, b;
	char cmd[11];

	for (i = 0; i < NUM_AL_PORTS; i++) {
		b = ((COM_DDP *)(tp_table[i]->t_ddp))->port;
		printf("%x:%x:%x:%x ", i+1, b, inb(b+MCR), inb(b+IER));
	}
	for (i = 0; i < 10; i++) {
		cmd[i] = u.u_comm[i];
	}
	cmd[10] = '\0';
	printf("poll=%d cmd=%s pid=%d ", poll_rate, cmd, SELF->p_pid);
	printf("%s\n", message);
	if (tp) {
		printf("#%d f=%x op=%d ", AL_NUM, tp->t_flags, tp->t_open);
		printf("in_use=%d irq=%d has_irq=%d ",
		  com_usage[AL_NUM].in_use,
		  com_usage[AL_NUM].irq,
		  com_usage[AL_NUM].has_irq);
		printf("poll=%d hcls=%d ohlt=%d\n",
		  com_usage[AL_NUM].poll,
		  com_usage[AL_NUM].hcls,
		  com_usage[AL_NUM].ohlt);
	}
}
#else
#define CDUMP(text, tp)
#endif

/*
 * alxopen()
 */
alxopen(dev, mode, tp, irqtty)
dev_t	dev;
int	mode;
register TTY	*tp, **irqtty;
{
	int	s;
	int	b;
	int	minor_h;  /* minor device number including high bit */
	unsigned char	msr;

	minor_h = minor(dev);     /* complete minor number */
	b = ALPORT;

	if (com_usage[AL_NUM].uart_type == US_NONE) { /* chip not found */
		u.u_error = ENXIO;
		goto bad_open;
	}

	if ((tp->t_flags & T_EXCL) && !super()) {
		u.u_error = ENODEV;
		goto bad_open;
	}

	if (drvl[major(dev)].d_time != 0) {	/* Modem settling */
		u.u_error = EEBUSY;
		goto bad_open;
	}

	/*
	 * Can't open a polled port if another driver is using polling.
	 */
	if (dev & CPOLL && poll_owner & ~ POLL_AL) {
		u.u_error = EEBUSY;
		goto bad_open;
	}

	/*
	 * Can't have both com[13] or both com[24] IRQ at once.
	 */
	if ( !(dev & CPOLL)
	&& com_usage[AL_NUM^2].irq
	&& com_usage[AL_NUM^2].in_use) {
		u.u_error = EEBUSY;
		goto bad_open;
	}

	/*
	 * If port already in use, are new and old open modes compatible?
	 */
	if (com_usage[AL_NUM].in_use) {
		int oldmode = 0, newmode = 0; /* mctl:1 poll:2 flow:4 */

		if (tp->t_flags & T_MODC)
			oldmode += 1;
		if (com_usage[AL_NUM].irq == 0)
			oldmode += 2;
		if (tp->t_flags & T_CFLOW)
			oldmode += 4;
		if ((minor_h & NMODC) == 0)
			newmode += 1;
		if (dev & CPOLL)
			newmode += 2;
		if (minor_h & CFLOW)
			newmode += 4;
		if (oldmode != newmode) {
			u.u_error = EEBUSY;
			goto bad_open;
		}
	}

	/*
	 * Sleep here if another process is opening or closing the port.
	 * This can happen if:
	 *   another process is trying a first open and awaiting CD;
	 *   another process is closing the port after losing CD;
	 *   a remote process opened the port, spawned a daemon,
	 *     and disconnected, and the daemon ignored SIGHUP and is
	 *     improperly keeping the port open.
	 * Don't try to set tp->t_flags before this sleep!  During
	 *   the sleep, ttclose() may be called and clear the flags.
	 */
	while (com_usage[AL_NUM].in_use &&
	  (com_usage[AL_NUM].hcls ||
	  ((minor_h & NMODC) == 0 && (inb(b+MSR) & MS_RLSD) == 0))) {
CDUMP("slp lst cls", tp)
		sleep((char *)(&tp->t_open), CVTTOUT, IVTTOUT, SVTTOUT);
#if DEBUG
printf("x1 ");
#endif
		if (SELF->p_ssig && nondsig()) {  /* signal? */
			u.u_error = EINTR;
			goto bad_open;
		}
	}

	/*
	 * If port already in use, are new and old open modes compatible?
	 * If not in use, mark it as such.
	 */
	if (com_usage[AL_NUM].in_use) {
		int oldmode = 0, newmode = 0; /* mctl:1 poll:2 flow:4 */

		if (tp->t_flags & T_MODC)
			oldmode += 1;
		if (com_usage[AL_NUM].irq == 0)
			oldmode += 2;
		if (tp->t_flags & T_CFLOW)
			oldmode += 4;
		if ((minor_h & NMODC) == 0)
			newmode += 1;
		if (dev & CPOLL)
			newmode += 2;
		if (minor_h & CFLOW)
			newmode += 4;
		if (oldmode != newmode) {
			u.u_error = EEBUSY;
			goto bad_open;
		}
	} else {
		/*
		 * Save modes for this open attempt to avoid future conflicts.
		 * Then start alxcycle() for this port.
		 */
		if (dev & CPOLL)
			com_usage[AL_NUM].irq = 0;
		else
			com_usage[AL_NUM].irq = 1;
		if (minor_h & CFLOW)
			tp->t_flags |= T_CFLOW;
		else
			tp->t_flags &= ~T_CFLOW;
		if (minor_h & NMODC)
			tp->t_flags &= ~T_MODC;
		else
			tp->t_flags |= T_MODC;
	}
	com_usage[AL_NUM].in_use++;
	/*
	 * From here, error exit is bad_open_u.
	 */

	if (tp->t_open == 0) {        /* not already open */
		if (!(dev & CPOLL)) {
			*irqtty = tp_table[AL_NUM];
			com_usage[AL_NUM].has_irq = 1;
		}

		/*
		 * Need to start cycling to scan for CD.
		 */
		alxcycle(tp);

		s = sphi();
		/*
		 * Raise basic modem control lines even if modem
		 * control hasn't been specified.
		 * MC_OUT2 turns on NON-open-collector IRQ line from the UART.
		 * since we can't have two UART's on same IRQ with MC_OUT2 on
		 */
		if (dev & CPOLL) {
			outb(b+MCR, MC_RTS|MC_DTR);
		} else {
			outb(b+MCR, MC_RTS|MC_DTR|MC_OUT2);
			outb(b+IER, IENABLE);
		}

		if ((minor_h & NMODC) == 0) {	/* want modem control? */
			tp->t_flags |= T_HOPEN | T_STOP;
			for (;;) {	/* wait for carrier */
				msr = inb(b+MSR);
				/*
				 * If carrier detect present
				 *   if port not already open
				 *     break out of loop and finish first open
				 *   else
				 *     do second (or third, etc.) open
				 */
				if (msr & MS_RLSD)
					break;
CDUMP("slp 1st CD", tp)
	   	  		sleep((char *)(&tp->t_open), CVTTOUT, IVTTOUT,
					SVTTOUT);	/* wait for carrier */
#if DEBUG
printf("x2 ");
#endif
		 		if (SELF->p_ssig && nondsig()) {  /* signal? */
					outb(b+MCR, 0);
			    		outb(b+IER, 0);
					u.u_error = EINTR;
					tp->t_flags &= ~(T_HOPEN | T_STOP);
					spl(s);
					goto bad_open_u;
				}
			}

			/*
			 * Mark that we are no longer hanging in open.
			 * Allow output over the port unless hardware flow
			 * control says not to.
			 */
			tp->t_flags &= ~T_HOPEN;
			tp->t_flags &= ~T_STOP;
			if (!(tp->t_flags & T_CFLOW) || (msr & MS_CTS))
				com_usage[AL_NUM].ohlt = 0;
			else
				com_usage[AL_NUM].ohlt = 1;

			/*
			 * Awaken any other opens on same device.
			 */
			wakeup((char *)(&tp->t_open));
		}
		tp->t_flags |= T_CARR;
		ttopen(tp);				/* stty inits */

		/*
		 * Allow custom modification of defaults.
		 */
		tp->t_sgttyb.sg_flags |=  al_sg_set;
		tp->t_sgttyb.sg_flags &= ~al_sg_clr;

		alxparam(tp);
		spl(s);
	} /* end of first-open case */

	tp->t_open++;
	ttsetgrp(tp, dev);

	/*
	 * Turn on polling for the port.
	 */
	if (dev & CPOLL) {
		com_usage[AL_NUM].poll = 1;
		set_poll_rate();
	}

	CDUMP((dev&CPOLL)?"open polled":"open irq", tp)
	return;

bad_open_u:
	--com_usage[AL_NUM].in_use;
	wakeup((char *)(&tp->t_open));
bad_open:
	return;
}

/*
 * alxclose()
 *
 *	Called whenever kernel closes a com port.
 */
alxclose(dev, mode, tp)
dev_t	dev;
int	mode;
TTY	*tp;
{
	register int b;
	int maj;
	int flags;
	int s;

	if (--tp->t_open)
		goto closed;
	s = sphi();

	/*
	 * Called at high priority by alclose after al_buff is drained
	 */
	com_usage[AL_NUM].hcls = 1;	/* disallow reopen til done closing */
	flags = tp->t_flags;		/* save flags - ttclose zeroes them */
	ttclose(tp);
	b = ALPORT;

	/*
	 * Wait for output silo and uart xmit buffer to empty.
	 * Allow signal to break the sleep.
	 */
	while ((tp->t_rawout.si_ix != tp->t_rawout.si_ox)
	  || !(inb(b+LSR) & LS_TxIDLE)) {
CDUMP("slp cls", tp)
		sleep((char *)&tp->t_rawout, CVTTOUT, IVTTOUT, SVTTOUT);
#if DEBUG
printf("x3 ");
#endif
		if (SELF->p_ssig && nondsig()) {  /* signal? */
			RAWOUT_FLUSH(tp);
			break;
		}
	}

	/*
	 * If not hanging in open
	 */
	if ((flags & T_HOPEN) == 0) {
		/*
		 * Disable interrupts.
		 */
		outb(b+IER, 0);
		outb(b+MCR, inb(b+MCR)&(~MC_OUT2));
	}

	/*
	 * If hupcls
	 */
	if (flags & T_HPCL) {
		/*
		 * Hangup port - drop DTR and RTS.
		 */
		outb(b+MCR, inb(b+MCR)&MC_OUT2);

		/*
		 * Hold dtr low for timeout
		 */
		maj = major(dev);
		drvl[maj].d_time = 1;
CDUMP("slp DTR", tp)
		sleep((char *)&drvl[maj].d_time, CVTTOUT, IVTTOUT, SVTTOUT);
#if DEBUG
printf("x4 ");
#endif
		drvl[maj].d_time = 0;
	}
	com_usage[AL_NUM].poll = 0;
	set_poll_rate();
	RAWIN_FLUSH(tp);
	com_usage[AL_NUM].hcls = 0;	/* allow reopen - done closing */
	wakeup((char *)(&tp->t_open));
	spl(s);
closed:;
	--com_usage[AL_NUM].in_use;
	wakeup((char *)(&tp->t_open));
	CDUMP("closed", tp)
}

/*
 * Common c_timer routine for async ports.
 */
alxtimer(dev)
dev_t dev;
{
	if (++drvl[major(dev)].d_time > DTRTMOUT)
		wakeup((char *)&drvl[major(dev)].d_time);
}


/*
 * Common c_ioctl routine for async ports.
 */
alxioctl(dev, com, vec, tp)
dev_t	dev;
struct sgttyb *vec;
register TTY 	*tp;
{
	register int	s, b;
	int stat1, stat2;
	unsigned char	msr;
	unsigned char ier_save;

	s = sphi();
	b = ALPORT;
	ier_save=inb(b+IER);
	stat1 = inb(b+MCR);		/* get current MCR register status */
	stat2 = inb(b+LCR);		/* get current LCR register status */

	switch(com) {
	case TIOCSBRK:			/* set BREAK */
		outb(b+LCR, stat2|LC_SBRK);
		break;
	case TIOCCBRK:			/* clear BREAK */
		outb(b+LCR, stat2 & ~LC_SBRK);
		break;
	case TIOCSDTR:			/* set DTR */
		outb(b+MCR, stat1|MC_DTR);
		break;
	case TIOCCDTR:			/* clear DTR */
		outb(b+MCR, stat1 & ~MC_DTR);
		break;
	case TIOCSRTS:			/* set RTS */
		outb(b+MCR, stat1|MC_RTS);
		break;
	case TIOCCRTS:			/* clear RTS */
		outb(b+MCR, stat1 & ~MC_RTS);
		break;
	case TIOCRSPEED:		/* set "raw" I/O speed divisor */
		outb(b+LCR, stat2|LC_DLAB);  /* set speed latch bit */
		outb(b+DLL, (unsigned) vec);
		outb(b+DLH, (unsigned) vec >> 8);
		outb(b+LCR, stat2);       /* reset latch bit */
		break;
	case TIOCWORDL:		/* set word length and stop bits */
		outb(b+LCR, ((stat2&~0x7) | ((unsigned) vec & 0x7)));
		break;
	case TIOCRMSR:		/* get CTS/DSR/RI/RLSD (MSR) */
		msr = inb(b+MSR);
		stat1 = msr >> 4;
		kucopy(&stat1, (unsigned *) vec, sizeof(unsigned));
		break;
	case TIOCFLUSH:		/* Flush silos here, queues in tty.c */
		RAWIN_FLUSH(tp);
		RAWOUT_FLUSH(tp);
		/* fall through to default... */
	default:
		ttioctl(tp, com, vec);
	}
	outb(b+IER, ier_save);
	spl(s);
}

alxparam(tp)
TTY	*tp;
{
	register int	b;
	register int	baud;
	int s;
	char newlcr;
	int write_baud=1, write_lcr=1;
	int alnum;

	b = ALPORT;

	/*
	 * error if input speed not the same as output speed
	 */
	if (tp->t_sgttyb.sg_ispeed!=tp->t_sgttyb.sg_ospeed) {
		u.u_error = ENODEV;
		return;
 	}

	if ((baud = albaud[tp->t_sgttyb.sg_ispeed]) == 0) {
		if (tp->t_flags & T_MODC) {  /* modem control? */
			s = sphi();
			tp->t_flags &= ~T_CARR;  /* indicate no carrier */
			outb(b+MCR, inb(b+MCR) & MC_OUT2); /* hangup */
			spl(s);
		}
		write_baud = 0;
	}

	switch (tp->t_sgttyb.sg_flags & (EVENP|ODDP|RAW)) {
	case ODDP:
		newlcr = LC_CS7|LC_PARENB;
		break;
	case EVENP:
		newlcr = LC_CS7|LC_PARENB|LC_PAREVEN;
		break;
	default:
		newlcr = LC_CS8;
		break;
	}

	alnum = AL_NUM;
	if (alnum >= 0 && alnum < 4) {
		if (baud == iocbaud[alnum]) {
			write_baud = 0;
			if (newlcr == ioclcr[alnum]) {
				write_lcr = 0;
			}
		}
		iocbaud[alnum] = baud;
		ioclcr[alnum] = newlcr;
	}

	if (write_lcr) {
		unsigned char ier_save;
		s=sphi();
		ier_save=inb(b+IER);
		if (write_baud) {
			outb(b+LCR, LC_DLAB);
			outb(b+DLL, baud);
			outb(b+DLH, baud >> 8);
		}
		outb(b+LCR, newlcr);
		if (com_usage[AL_NUM].uart_type == US_16550A)
			outb(b+FCR, FC_ENABLE | FC_Rx_RST | FC_Rx_08);
		outb(b+IER, ier_save);
		spl(s);
	}

	set_poll_rate();
}

/*
 * Middle level processor.
 *
 *	Invoked 10 times per second.  (Once every ten clock ticks.)
 *	Tranfers rawin buffer [from intr level] to canonical input queue.
 *	Checks modem status for loss of carrier.
 *	Transfers output queue to rawout buffer [for intr level].
 */
alxcycle(tp)
register TTY * tp;
{
	register int b;
	register int n;
	unsigned char msr, mcr;
	int s;

	/*
	 * Check Carrier Detect (RLSD).
	 *
	 * Modem status interrupts were not enabled due to 8250 hardware bug.
	 * Enabling modem status and receive interrupts may cause lockup
	 * on older parts.
	 */
	if (tp->t_flags & T_MODC) {

		/*
		 * Get status
		 */
		msr = inb(ALPORT+MSR);

		/*
		 * Carrier changed.
		 */
		if ((msr & MS_RLSD) && !(tp->t_flags & T_CARR)) {
			/*
			 * Carrier is on - wakeup open.
			 */
			s = sphi();
			tp->t_flags |= T_CARR;
			spl(s);
			wakeup((char *)(&tp->t_open));
		}

		if (!(msr & MS_RLSD) && (tp->t_flags & T_CARR)) {
			s = sphi();
			RAWIN_FLUSH(tp);
			RAWOUT_FLUSH(tp);
			tp->t_flags &= ~T_CARR;
			spl(s);
			tthup(tp);
		}
	}

	/*
	 * Empty raw input buffer.
	 *
	 * The line discipline module (tty.c) will set T_ISTOP true when the
	 * tt input queue is nearly full (tp->t_iq.cq_cc >= IHILIM), and make
	 * T_ISTOP false when it's ready for more input.
	 *
	 * When T_ISTOP is true, ttin() simply discards the character passed.
	 */
	if (!(tp->t_flags & T_ISTOP)) {
		while (tp->t_rawin.SILO_CHAR_COUNT > 0) {
			s = sphi();
			ttin(tp, tp->t_rawin.si_buf[tp->t_rawin.si_ox]);
			if (tp->t_rawin.si_ox < MAX_SILO_INDEX)
				tp->t_rawin.si_ox++;
			else
				tp->t_rawin.si_ox = 0;
			tp->t_rawin.SILO_CHAR_COUNT--;
			spl(s);
		}
	}

	/*
	 * Hardware flow control.
	 *	Check CTS to see if we need to halt output.
	 *	(MS_INTR should have done this - repeat code here to be sure)
	 *	Check input silo to see if we need to raise RTS.
	 */
	if (tp->t_flags & T_CFLOW) {

		/*
		 * Get status
		 */
		msr = inb(ALPORT+MSR);

		s = sphi();
		if (msr & MS_CTS)
			com_usage[AL_NUM].ohlt = 0;
		else
			com_usage[AL_NUM].ohlt = 1;
		spl(s);
#if DEBUG
{static cts = 0;
if (!cts && (msr & MS_CTS)) {
	cts = 1;
	printf("[");
} else if (cts && !(msr & MS_CTS)) {
	cts = 0;
	printf("]");
}}
#endif

		/*
		 * If using hardware flow control, see if we need to drop RTS.
		 */
		if ( (tp->t_flags & T_CFLOW)
		&& (tp->t_rawin.SILO_CHAR_COUNT > SILO_HIGH_MARK)) {
			s = sphi();
			mcr = inb(ALPORT+MCR);
			if (mcr & MC_RTS) {
				outb(ALPORT+MCR, mcr & ~MC_RTS);
#if DEBUG
printf("-");
#endif
			}
			spl(s);
		}

		/*
		 * If input silo below low mark, assert RTS.
		 */
		if (tp->t_rawin.SILO_CHAR_COUNT <= SILO_LOW_MARK) {
			s = sphi();
			mcr = inb(ALPORT+MCR);
			if ((mcr & MC_RTS) == 0) {
				outb(ALPORT+MCR, mcr | MC_RTS);
#if DEBUG
printf("+");
#endif
			}
			spl(s);
		}
	}

	/*
	 * Calculate free output slot count.
	 */
	n  = sizeof(tp->t_rawout.si_buf) - 1;
	n += tp->t_rawout.si_ox - tp->t_rawout.si_ix;
	n %= sizeof(tp->t_rawout.si_buf);

	/*
	 * Fill raw output buffer.
	 */
	for (;;) {
		if (--n < 0)
			break;
		s = sphi();
		b = ttout(tp);
		spl(s);
		if (b < 0)
			break;

		s = sphi();
		tp->t_rawout.si_buf[tp->t_rawout.si_ix] = b;
		if (tp->t_rawout.si_ix >= sizeof(tp->t_rawout.si_buf) - 1)
			tp->t_rawout.si_ix = 0;
		else
			tp->t_rawout.si_ix++;
		spl(s);
	}

	/*
	 * (Re)start output, wake sleeping processes, etc.
	 */
	ttstart(tp);

	/*
	 * Schedule next cycle.
	 */
	if (com_usage[AL_NUM].in_use)
		timeout(&tp->t_rawtim, HZ/10, alxcycle, tp);
}

/*
 * Serial Transmit Start Routine.
 */
alxstart(tp)
register TTY * tp;
{
	int b;
	int s;
	extern alxbreak();
	int need_xmit = 1;	/* True if should start sending data now. */

	/*
	 * Read line status register AFTER disabling interrupts.
	 */
	s = sphi();
	b = inb(ALPORT+LSR);

	/*
	 * Process break indication.
	 * NOTE: Break indication cleared when line status register was read.
	 */
	if (b & LS_BREAK)
		defer(alxbreak, tp);

	/*
	 * If no output data, it may be time to finish closing the port;
	 * but won't need another xmit interrupt.
	 */
	if (tp->t_rawout.si_ix == tp->t_rawout.si_ox) {
		wakeup((char *)&tp->t_rawout);
		need_xmit = 0;
	}

	/*
	 * Do nothing if output is stopped.
	 */
	if (tp->t_flags & T_STOP)
		need_xmit = 0;
	if (com_usage[AL_NUM].ohlt)
		need_xmit = 0;

	/*
	 * Start data transmission by writing to UART xmit reg.
	 */
	if ((b & LS_TxRDY) && need_xmit) {
		int xmit_count;

		xmit_count = (com_usage[AL_NUM].uart_type == US_16550A)?16:1;
		alx_send(&(tp->t_rawout), ALPORT+DREG, xmit_count);
	}

	spl(s);
}

/*
 * Serial Received Break Handler.
 */
alxbreak(tp)
TTY * tp;
{
	int s;

	s = sphi();
	RAWIN_FLUSH(tp);
	RAWOUT_FLUSH(tp);
	spl(s);
	ttsignal(tp, SIGINT);
}

/*
 * Serial Interrupt Handler.
 */
alxintr(tp)
register TTY * tp;
{
	int c;
	register int port = ALPORT;
	unsigned char msr;
	int xmit_count;

	if (tp) {
rescan:
		switch (inb(port+IIR) & 0x07) {

		case LS_INTR:
			if (inb(port+LSR) & LS_BREAK)
				defer(alxbreak, tp);
			goto rescan;

		case Rx_INTR:
			c = inb(port+DREG);
			if (tp->t_open == 0)
				goto rescan;
			/*
			 * Must recognize XOFF quickly to avoid transmit overrun.
			 * Recognize XON here as well to avoid race conditions.
			 */
			if (!ISRIN) {
				/*
				 * XOFF.
				 */
				if (ISSTOP) {
					tp->t_flags |= T_STOP;
					goto rescan;
				}

				/*
				 * XON.
				 */
				if (ISSTART) {
					tp->t_flags &= ~T_STOP;
					goto rescan;
				}
			}

			/*
			 * Save char in raw input buffer.
			 */
			if (tp->t_rawin.SILO_CHAR_COUNT < MAX_SILO_CHARS) {
				tp->t_rawin.si_buf[tp->t_rawin.si_ix] = c;
				if (tp->t_rawin.si_ix < MAX_SILO_INDEX)
					tp->t_rawin.si_ix++;
				else
					tp->t_rawin.si_ix = 0;
				tp->t_rawin.SILO_CHAR_COUNT++;
			}

			/*
			 * If using hardware flow control, see if we need to drop RTS.
			 */
			if ( (tp->t_flags & T_CFLOW)
			&& (tp->t_rawin.SILO_CHAR_COUNT > SILO_HIGH_MARK)) {
				unsigned char mcr = inb(port+MCR);
				if (mcr & MC_RTS) {
					outb(port+MCR, mcr & ~MC_RTS);
				}
			}
			goto rescan;

		case Tx_INTR:
			/*
			 * Do nothing if output is stopped.
			 */
			if (tp->t_flags & T_STOP)
				goto rescan;
			if (com_usage[AL_NUM].ohlt)
				goto rescan;

			/*
			 * Transmit next char in raw output buffer.
			 */
			xmit_count =
			  (com_usage[AL_NUM].uart_type == US_16550A)?16:1;
			alx_send(&(tp->t_rawout), port+DREG, xmit_count);
			goto rescan;

		case MS_INTR:
			/*
			 * Get status (and clear interrupt).
			 */
			msr = inb(port+MSR);

			/*
			 * Hardware flow control.
			 *	Check CTS to see if we need to halt output.
			 */
			if (tp->t_flags & T_CFLOW) {
				if (msr & MS_CTS)
					com_usage[AL_NUM].ohlt = 0;
				else
					com_usage[AL_NUM].ohlt = 1;
			}

			goto rescan;
		} /* endswitch */
	} else {
		/*
		 * If tp is zero, an interrupt occurred before things
		 * are fully set up.  Just try to clear all pending
		 * interrupts from ANY serial ports.
		 */
		int com_num;
		for (com_num = 1; com_num < 4; com_num++) {
			switch(com_num) {
			case 1:
				port = 0x3F8;
				break;
			case 2:
				port = 0x2F8;
				break;
			case 3:
				port = 0x3E8;
				break;
			case 4:
				port = 0x2E8;
				break;
			}
			inb(port+IIR);
			inb(port+LSR);
			inb(port+MSR);
			inb(port+DREG);
		}
	}
}

/*
 * alxclk will be called every time T0 interrupts - if it returns 0,
 * the usual system timer interrupt stuff is done
 */
static int alxclk()
{
	static int count;
	int i;

	for (i = 0; i < NUM_AL_PORTS;  i++)
		if (com_usage[i].poll)
			alxpoll(tp_table[i]);
	count++;
	if (count >= poll_divisor)
		count = 0;
	return count;
}

/*
 * set_poll_rate is called when a port is opened or closed or changes speed
 * it sets the polling rate only as fast as needed, and shuts off polling
 * whenever possible
 */
static set_poll_rate()
{
	int port_num, max_rate, port_rate;

	/*
	 * If another driver has the polling clock, do nothing.
	 */
	if (poll_owner & ~ POLL_AL)
		return;

	/*
	 * Find highest valid polling rate in units of HZ/10.
	 * If using FIFO chip, can poll at 1/16 the usual rate.
	 */
	max_rate = 0;
	for (port_num = 0; port_num < NUM_AL_PORTS; port_num++) {
		if (com_usage[port_num].poll) {
			port_rate = alp_rate[(tp_table[port_num])->t_sgttyb.sg_ispeed];
			if (com_usage[port_num].uart_type == US_16550A) {
				port_rate /= 16;
				if (port_rate % HZ)
					port_rate += HZ - (port_rate % HZ);
			}
			if (max_rate < port_rate)
				max_rate = port_rate;
		}
	}
	/*
	 * if max_rate is not current rate, adjust the system clock
	 */
	if (max_rate != poll_rate) {
		poll_rate = max_rate;
		poll_divisor = poll_rate/HZ;  /* used in alxclk() */
		altclk_out();		/* stop previous polling */
		poll_owner &= ~ POLL_AL;
		if (max_rate) {	/* resume polling at new rate if needed */
			poll_owner |= POLL_AL;
			altclk_in(poll_rate, alxclk);
		}
		CDUMP("new rate", 0)
	}
}

/*
 * alxpoll()
 *
 * Serial polling handler.  Compare to alxintr().
 */
static void alxpoll(tp)
register TTY * tp;
{
	int c;
	int port = ALPORT;
	int xmit_count;

	/*
	 * Check for received break first.
	 * This status is wiped out on reading the LSR.
	 */
	if (inb(port+LSR) & LS_BREAK)
		defer(alxbreak, tp);

	/*
	 * Handle all incoming characters.
	 */
	while (inb(port+LSR) & LS_RxRDY) {
		c = inb(port+DREG);
		if (tp->t_open == 0)
			continue;
		/*
		 * Must recognize XOFF quickly to avoid transmit overrun.
		 * Recognize XON here as well to avoid race conditions.
		 */
		if (!ISRIN) {
			/*
			 * XOFF.
			 */
			if (ISSTOP) {
				tp->t_flags |= T_STOP;
				continue;
			}

			/*
			 * XON.
			 */
			if (ISSTART) {
				tp->t_flags &= ~T_STOP;
				continue;
			}
		}

		/*
		 * Save char in raw input buffer.
		 */
		if (tp->t_rawin.SILO_CHAR_COUNT < MAX_SILO_CHARS) {
			tp->t_rawin.si_buf[tp->t_rawin.si_ix] = c;
			if (tp->t_rawin.si_ix < MAX_SILO_INDEX)
				tp->t_rawin.si_ix++;
			else
				tp->t_rawin.si_ix = 0;
			tp->t_rawin.SILO_CHAR_COUNT++;
		}

		/*
		 * If using hardware flow control, see if we need to drop RTS.
		 */
		if ( (tp->t_flags & T_CFLOW)
		  && (tp->t_rawin.SILO_CHAR_COUNT > SILO_HIGH_MARK)) {
			unsigned char mcr = inb(port+MCR);
			if (mcr & MC_RTS) {
				outb(port+MCR, mcr & ~MC_RTS);
			}
		}
	}

	/*
	 * Handle outgoing characters.
	 * Do nothing if output is stopped.
	 */
	if ((inb(port+LSR) & LS_TxRDY)
	  && !(tp->t_flags & T_STOP)
	  && !(com_usage[AL_NUM].ohlt)) {
		/*
		 * Transmit next char in raw output buffer.
		 */
		xmit_count = (com_usage[AL_NUM].uart_type == US_16550A)?16:1;
		alx_send(&(tp->t_rawout), port+DREG, xmit_count);
	}

	/*
	 * Hardware flow control.
	 *	Check CTS to see if we need to halt output.
	 */
	if (tp->t_flags & T_CFLOW) {
		if (inb(port+MSR) & MS_CTS)
			com_usage[AL_NUM].ohlt = 0;
		else
			com_usage[AL_NUM].ohlt = 1;
	}
}

/*
 * alx_send()
 *
 * Write to xmit data register of the UART.
 * Assume all checking about whether it's time to send has been done already.
 * Called by time-critical IRQ and polling routines!
 *
 * "rawout" is the output silo for the TTY struct supplying data to the port.
 * "dreg" is the i/o address of the UART xmit data register.
 * "xmit_count" is the max number of chars we can write (16 for FIFO parts).
 */
static void alx_send(rawout, dreg, xmit_count)
register silo_t * rawout;
int dreg, xmit_count;
{
	/*
	 * Transmit next chars in raw output buffer.
	 */
	for (;(rawout->si_ix != rawout->si_ox) && xmit_count; xmit_count--) {
		outb(dreg, rawout->si_buf[rawout->si_ox]);
		/*
		 * Adjust raw output buffer output index.
		 */
		if (++rawout->si_ox >= sizeof(rawout->si_buf))
			rawout->si_ox = 0;
	}
}
