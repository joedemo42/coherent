/*
 * System V Compatible Semaphore Device Driver
 *
 *	This device driver provides System V compatible semaphore operations.
 *	Operations are performed through the semaphore device (/dev/sem).
 *	and are implemented as ioctl calls from semctl, semget, semop
 *	utilities.
 *
 *	Author: Allan Cornish.
 */

#include <sys/coherent.h>
#include <sys/types.h>
#include <sys/uproc.h>
#include <errno.h>
#include <sys/con.h>
#include <sys/sem.h>
#ifndef _I386
#define getusd(arg)		getuwd(arg)
#define putusd(arg1, arg2)	putuwd(arg1, arg2)
#endif

/*
 * Functions.
 */

int seminit();
int semioctl();
int nulldev();
int nonedev();

/*
 * Device Configuration.
 */

CON semcon = {
	DFCHR,			/* Flags			*/
	23,			/* Major Index			*/
	nulldev,		/* Open				*/
	nulldev,		/* Close			*/
	nonedev,		/* Block			*/
	nonedev,		/* Read				*/
	nonedev,		/* Write			*/
	semioctl,		/* Ioctl			*/
	nulldev,		/* Power fail			*/
	nulldev,		/* Timeout			*/
	seminit,		/* Load				*/
	nulldev			/* Unload			*/
};

/*
 * Semaphore Device Ioctl.
 */

static
semioctl( dev, com, vec )

dev_t dev;  /* the id of the semaphore as determined by a previous open() of /dev/sem */
short com;  /* command ofr use in switch() */
register short *vec;  /* if calling semctl or semget, this is a pointer to a struct of 5 ints
                       * if calling semop, this is a pointer to a struct of 4 ints.
                       */

{
/*	printf("semioctl() called: dev= %x (%d) cmd = %x (%d)  vec= %x (%d)\n", 
 *		dev, dev, com, com, vec, vec);
 *	printf("Vectors: [0] = %d ; key = %d ; key>>16 = %d ; nsems = %d ; semflg = 0%o\n",
 *		vec[0],vec[1],vec[2],vec[3],vec[4]);
 */
	switch ( com ) {

	case SEMCTL:
		putusd( vec+0,                       /* -1 passed from semop (286 libc) */
			usemctl(getusd( vec+1 ),     /* semaphore id */
				getusd( vec+2 ),     /* semaphore number */
				getusd( vec+3 ),     /* command passed to usemctl */
				getusd( vec+4 ) ));  /* address of struct semid_ds */
		return;

	case SEMGET:
/*		printf("semioctl: performing SEMGET\n");  */
		putusd( vec+0,                        /* -1 passed from semget (286 libc) */				
			usemget(getusd( vec+1 ),      /* bits 00 - 15 of semaphore key */
/*				getusd( vec+2 ), */   /* bits 16 - 31 of semaphore key */
				getusd( vec+3 ),      /* number of semaphores to make room for */
				getusd( vec+4 ) ));   /* semaphore flag (IPC_CREAT|0644) */
		return;

	case SEMOP:
		putusd( vec+0,                        /* -1 passed from semop (286 libc) */
			usemop(	getusd( vec+1 ),      /* semaphore id */
				getusd( vec+2 ),      /* address of struct sembuf (ops on value) */
				getusd( vec+3 ) ));   /* # of struct sembufs */
		return;

	default:
		printf("semioctl(): received invalid cmd!\n");
		u.u_error = EINVAL;
		return;
	}
}
