/*
 * /usr/include/sys/mwc_coherent.h
 *
 * mwc_coherent.h is for kernel debugging definitions.
 *
 * Revised: Thu May 27 08:06:57 1993 CDT
 */
#ifndef __SYS_MWC_COHERENT_H__
#define __SYS_MWC_COHERENT_H__

/*
 * Control flags for tracing features.
 *
 * t_piggy bits:
 *	0x0001	pexit()
 *	0x0002	286 system calls
 *	0x0004	386 system calls
 *	0x0008	uioctl()
 *	0x0020	ssread()/sswrite()	in io.386/ss.c
 *	0x0200	uread()			in coh.386/sys3.c
 *	0x0400	ssblock()		in io.386/ss.c
 *	0x1000	XP_DATA_IN/OUT		in state machine in io.386/ss.c
 *	0x10000	open() bad exit values	in coh.386/sys3.c
 *
 * t_hal bits:
 *	0x0001	TCSETA/TCSETAF/TCSETAW ioctl's
 *	0x0002	TIOCSETP/TIOCSETN ioctl's
 *	0x0004	al[01] verbose mode (see alx.c); asy devmsgs
 *	0x0008	obrk()
 *	0x0010	pipe open/read/write
 *	0x0020	1= NO c_grow() calls;  alx overruns
 *	0x0040	monitor number of free clists
 *	0x0080	'+' when asleep waiting for free clist
 *	0x0100	trace c_grow() calls
 *	0x0200	run check_slot() on alloc() and ubrk()
 *	0x0400	async opens, closes, speed changes
 *	0x0800	asy, all interrupts
 *	0x1000	explanation of page faults
 *	0x2000
 *	0x4000	all traps
 *	0x8000	ubrk()
 *
 * 0x0001 0000
 * 0x0002 0000
 *
 * t_vlad bits:
 *	0x0001
 *	0x0002	fcntl
 *
 * t_errno - trace u.u_error settings
 * t_con - trace console events
 *
 * t_msgq bits:
 *	0x0001	memory alloc
 *	0x0002	permissions
 */


/*
 * T_PIGGY() is for piggy controlled tracing.
 */

extern unsigned t_piggy;
#define T_PIGGY(flag, expr)	((t_piggy & (flag)) != 0 ? (void) (expr) : \
							   (void) 0)

/*
 * T_HAL() is for hal controlled tracing.
 */

extern unsigned t_hal;
#define T_HAL(flag, expr)	((t_hal & (flag)) != 0 ? (void) (expr) : \
							 (void) 0)

/*
 * T_VLAD() is for vlad controlled tracing.
 */
extern unsigned t_vlad;
#define T_VLAD(flag, expr)	((t_vlad & (flag)) != 0 ? (void) (expr) : \
							  (void) 0)

/*
 * T_MSGQ() is for message queue controlled tracing.
 */
extern unsigned t_msgq;
#define T_MSGQ(flag, expr)	((t_msgq & (flag)) != 0 ? (void) (expr) : \
							  (void) 0)

/*
 * SET_U_ERROR() is for error tracing.
 */
extern unsigned t_errno;
#define SET_U_ERROR(errno, msg)	\
	(u.u_error = (errno), \
	 t_errno ? (void) printf ("u_error: %d: %s:\n", (errno), (msg)) : \
		   (void) 0)

/*
 * T_CON() is for video & keyboard tracing.
 */
extern unsigned t_con;
#define T_CON(flag, expr)	((t_con & (flag)) != 0 ? (void) (expr) : \
							 (void) 0)

/* DV(variable) displays: variable=.... */
#define DISP_VAR(v)	(strchirp ("  " #v "="), print32(v))
#define DV(v)		T_PIGGY(0x400, DISP_VAR(v))

#endif
