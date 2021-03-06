/*
 * Kernel portion of typewriter structure.
 */
#ifndef	 KTTY_H
#define	 KTTY_H
#include <sys/types.h>
#include <sys/poll.h>
#include <sys/clist.h>
#include <sgtty.h>
#ifdef _I386
#include <termio.h>
#endif
#include <sys/timeout.h>

#define	NCIB	256		/* Input buffer */
#define	OHILIM	128		/* Output buffer hi water mark */
#define	OLOLIM	40		/* Output buffer lo water mark */
#define	IHILIM	512		/* Input buffer hi water mark */
#define	ILOLIM	40		/* Input buffer lo water mark */
#define	ITSLIM	(IHILIM-(IHILIM/4))	/* Input buffer tandem stop mark */
#define	ESC	'\\'		/* Some characters */

typedef struct tty {
	CQUEUE	t_oq;		/* Output queue */
	CQUEUE	t_iq;		/* Input queue */
	char	*t_ddp;		/* Device specific */
	int	(*t_start)();	/* Start function */
	int	(*t_param)();	/* Load parameters function */
	char	t_dispeed;	/* Default input speed */
	char	t_dospeed;	/* Default output speed */
	int	t_open;		/* Open count */
	int	t_flags;	/* Flags */
	char	t_nfill;	/* Number of fill characters */
	char	t_fillb;	/* The fill character */
	int	t_ibx;		/* Input buffer index */
	char	t_ib[NCIB];	/* Input buffer */
	int	t_hpos;		/* Horizontal position */
	int	t_opos;		/* Original horizontal position */
	struct	sgttyb t_sgttyb;/* Stty/gtty information */
	struct	tchars t_tchars;/* Tchars information */
#ifdef _I386
	struct	termio t_termio;
#endif
	int	t_group;	/* Process group */
	int	t_escape;	/* Pending escape count */
	event_t t_ipolls;	/* List of input polls enabled on device */
	event_t t_opolls;	/* List of output polls enabled on device */
	TIM	t_rawtim;	/* Raw timing struct */
	int	t_cs_sel;	/* 0 for resident drivers, CS for loadable */
} TTY;

/*
 * Test macros.
 * Assume `tp' holds a TTY pointer.
 *	  `c'  a character.
 * Be very careful if you work on the
 * tty driver that this is true.
 */
#define	ISINTR	(tp->t_tchars.t_intrc  == c)
#define	ISQUIT	(tp->t_tchars.t_quitc  == c)
#define	ISEOF	(tp->t_tchars.t_eofc   == c)
#define	ISBRK	(tp->t_tchars.t_brkc   == c)
#define	ISSTART	(tp->t_tchars.t_startc == c)
#define	ISSTOP	(tp->t_tchars.t_stopc  == c)
#define	ISCRMOD	((tp->t_sgttyb.sg_flags&CRMOD) != 0)
#define	ISXTABS	((tp->t_sgttyb.sg_flags&XTABS) != 0)
#define	ISECHO	((tp->t_sgttyb.sg_flags&ECHO)  != 0)
#define	ISERASE	(tp->t_sgttyb.sg_erase == c)
#define	ISKILL	(tp->t_sgttyb.sg_kill  == c)
#define	stopc	(tp->t_tchars.t_stopc)
#define	startc	(tp->t_tchars.t_startc)

/*
 * The following are not part of S5 sgtty.
 */
#define	ISRIN	((tp->t_sgttyb.sg_flags&RAWIN) != 0)
#define	ISROUT	((tp->t_sgttyb.sg_flags&RAWOUT)!= 0)
#define	ISCRT	((tp->t_sgttyb.sg_flags&CRT)   != 0)
#define	ISCBRK	((tp->t_sgttyb.sg_flags&CBREAK)!= 0)
#define	ISTAND	((tp->t_sgttyb.sg_flags&TANDEM)!= 0)
#define	ISBBYB	((tp->t_sgttyb.sg_flags&(RAWIN|CBREAK)) != 0)

#endif
