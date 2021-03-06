/*
 *	Ram Pipe Device Driver
 */

#include <coherent.h>
#include <con.h>
#include <seg.h>
#include <stat.h>
#include <sched.h>
#include <termio.h>
#include <v7sgtty.h>
#include <errno.h>

#define	MAXNRP	30		/* Maximum number of ram pipes (must be < 32) */
#define	NCPQ	2048		/* Size of pipe in bytes */
#define	RPMAJOR 22		/* Major device for ram pipes  */

/*
 *	function definitions
 */
int	rpopen();
int	rpread();
int	rpwrite();
int	rpioctl();
int	rppoll();
void	rpuload();
int	nulldev();
int	nonedev();


/*
 *	configuration table
 */
CON rpcon = {
	DFCHR|DFPOL,			/* flags	*/
	RPMAJOR,			/* major index	*/
	rpopen,				/* open		*/
	nulldev,			/* close	*/
	nonedev,			/* block	*/
	rpread,				/* read		*/
	rpwrite,			/* write	*/
	rpioctl,			/* ioctl	*/
	nulldev,			/* power fail	*/
	nulldev,			/* timeout	*/
	nulldev,			/* load		*/
	rpuload,			/* unload	*/
	rppoll				/* poll		*/
};

/*
 *	Ram Pipe Headers
 */
static
struct ring {
	unsigned short	q_size;		/* Number of characters in queue    */
	unsigned short	q_mask;		/* Ring buffer Mask: NCPQ-1	    */
	faddr_t		q_ifaddr;	/* Input virtual address	    */
	faddr_t		q_ofaddr;	/* Output virtual address	    */
	GATE		q_igate;	/* Read lock			    */
	GATE		q_ogate;	/* Write lock			    */
	event_t		q_ipoll;	/* Input polls			    */
	event_t		q_opoll;	/* Output polls			    */

} rpq[MAXNRP];

static SEG * rpsegp;
unsigned NRP = MAXNRP;

/*
 * Initialization Routine
 */
static
rpinit()
{
	register struct ring *rp;
	faddr_t faddr;
	paddr_t paddr;

	/*
	 * Ensure valid number of ram pipes
	 */
	if ( NRP > MAXNRP )
		NRP = MAXNRP;
		
	/*
	 * Allocate ram pipe segment, initialize ram pipe queues
	 */
	 if ( NRP != 0 ) {
		rpsegp = salloc((fsize_t)NRP*NCPQ, SFSYST|SFHIGH|SFNSWP|SFNCLR);

		if ( rpsegp == NULL )
			return -1;

		paddr = rpsegp->s_paddr;

		for ( rp = &rpq[0]; rp < &rpq[NRP]; rp++, paddr += NCPQ ) {

			faddr = ptov( paddr, (fsize_t)NCPQ );
			rp->q_size = 0;
			rp->q_mask = NCPQ - 1;

			rp->q_ifaddr = faddr;
			rp->q_ofaddr = faddr;
		}
	}
	return 0;
}

/*
 * Unload Routine.
 */
static void
rpuload()
{
	register struct ring *rp;

	/*
	 * Release virtual address mappers.
	 */
	for ( rp = &rpq[0]; rp < &rpq[NRP]; rp++ ) {
		if ( rp->q_ifaddr )
			vrelse( rp->q_ifaddr );
	}

	/*
	 * Release ring buffer storage.
	 */
	if ( rpsegp != NULL ) {
		sfree( rpsegp );
		rpsegp = NULL;
	}

	/*
	 * Erase private data.
	 */
	memset( &rpq[0], 0, sizeof(rpq) );
}

/*
 * Open Routine
 */
static
rpopen( dev, mode )
dev_t dev;
{
	int s;

	s = sphi();
	if ( rpq[0].q_mask == 0 )
		if ( rpinit() < 0 )
			u.u_error = ENOSPC;
	spl( s );
	if ( minor(dev) >= NRP )
		u.u_error = ENXIO;
}

/*
 * Ioctl Routine
 */
static
rpioctl( dev, com, vec )
dev_t dev;
{
	switch ( com ) {

	case TIOCQUERY:
		putuwd( vec, rpq[ minor(dev) ].q_size );
		return;

	case TIOCOUTQ:
		putuwd( vec, rpq[ minor(dev) ].q_size );
		return;

	case TIOCFLUSH:
	case TCFLSH:
		rpflush( &rpq[minor(dev)] );
		return;

	default:
		u.u_error = EINVAL;
		return;
	}
}

/*
 * Read Routine
 */
static
rpread( dev, iop )
dev_t dev;
register IO *iop;
{
	register struct ring *rp;
	unsigned n;
	int s;

	rp = &rpq[ minor(dev) ];
	s  = sphi();

	/*
	 * Wait until read is unlocked, and there is data to read
	 */
	while ( (rp->q_igate[0] != 0) || ((n = rp->q_size) == 0) ) {

		/*
		 * Non-blocking reads.
		 */
		if ( iop->io_flag & IONDLY ) {
			u.u_error = EAGAIN;
			return;
		}

		++rp->q_igate[1];
		sleep( rp->q_igate, CVTTOUT, IVTTOUT, SVTTOUT );
		--rp->q_igate[1];

		if ( SELF->p_ssig && nondsig() ) {	/* signal received */

			spl( s );
			u.u_error = EINTR;
			return;
		}
	}
	rp->q_igate[0] = 1;			/* lock read gate	    */
	spl( s );

	if ( n > iop->io_ioc )			/* more data than requested */
		n = iop->io_ioc;

	rucopy( rp, iop->io_base, n );		/* copy data to user space  */
	iop->io_base += n;
	iop->io_ioc  -= n;

	if ( rp->q_ogate[1] != 0 )		/* someone waiting to write */
		wakeup( rp->q_ogate );
	if ( rp->q_opoll.e_procp )		/* someone polling to write */
		pollwake( &rp->q_opoll );

	rp->q_igate[0] = 0;			/* unlock read gate	    */

	if ( rp->q_igate[1] != 0 )		/* others waiting to read   */
		wakeup( rp->q_igate );
}

/*
 * Write Routine
 */
static
rpwrite( dev, iop )
dev_t dev;
register IO *iop;
{
	register struct ring *rp;
	unsigned n;
	int s;

	rp = &rpq[ minor(dev) ];

	do {
		s  = sphi();
		/*
		 * Wait until write is unlocked and 512 free slots exist
		 */

		while ((rp->q_ogate[0] != 0) || ((n = NCPQ-rp->q_size) < 512)) {

			/*
			 * Non-blocking writes.
			 */
			if ( iop->io_flag & IONDLY ) {
				u.u_error = EAGAIN;
				return;
			}

			++rp->q_ogate[1];
			sleep( rp->q_ogate, CVTTOUT, IVTTOUT, SVTTOUT );
			--rp->q_ogate[1];

			if (SELF->p_ssig && nondsig()) { /* received signal */

				spl( s );
				u.u_error = EINTR;
				return;
			}
		}
		rp->q_ogate[0] = 1;		/* lock write gate	     */
		spl( s );

		if ( n > iop->io_ioc )
			n = iop->io_ioc;

		urcopy( iop->io_base, rp, n );	/* copy data from user space */
		iop->io_base += n;
		iop->io_ioc  -= n;

		rp->q_ogate[0] = 0;		/* unlock write gate	     */

		if ( rp->q_igate[1] != 0 )	/* someone waiting to read   */
			wakeup( rp->q_igate );

		if ( rp->q_ipoll.e_procp )	/* someone polling to read */
			pollwake( &rp->q_ipoll );

	} while ( iop->io_ioc != 0 );		/* until all data copied     */

	if (rp->q_ogate[1] != 0)		/* someone waiting to write  */
		wakeup( rp->q_ogate );
}

/*
 * Poll.
 */
rppoll( dev, ev, msec )
dev_t dev;
int ev;
int msec;
{
	register struct ring *rp = &rpq[ minor(dev) ];

	ev &= ~POLLPRI;

	/*
	 * Input poll.
	 */
	if ( ev & POLLIN ) {

		/*
		 * Pipe empty.
		 */
		if ( FP_OFF(rp->q_ifaddr) == FP_OFF(rp->q_ofaddr) ) {
			if ( msec != 0 )
				pollopen( &rp->q_ipoll );
			ev &= ~POLLIN;
		}
	}

	/*
	 * Output poll.
	 */
	if ( ev & POLLOUT ) {

		/*
		 * Pipe not empty.
		 */
		if ( FP_OFF(rp->q_ifaddr) != FP_OFF(rp->q_ofaddr) ) {
			if ( msec != 0 )
				pollopen( &rp->q_opoll );
			ev &= ~POLLOUT;
		}
	}

	return ev;
}

/*
 * Flush queue
 */
static
rpflush( rp )
register struct ring *rp;
{
	register int s;

	s = sphi();

	/*
	 * Wait until read is unlocked, or nothing in queue
	 */

	while ((rp->q_size != 0) && (rp->q_igate[0] != 0) ) {

		++rp->q_igate[1];
		sleep( rp->q_igate, CVTTOUT, IVTTOUT, SVTTOUT );
		--rp->q_igate[1];

		if (SELF->p_ssig && nondsig()) { /* received signal	   */

			spl( s );
			u.u_error = EINTR;
			return;
		}
	}

	if (rp->q_size != 0) {			/* flush ram pipe	   */

		rp->q_ofaddr = rp->q_ifaddr;
		rp->q_size = 0;
	}
	spl( s );

	if (rp->q_ogate[1] != 0)		/* someone waiting to write */
		wakeup( rp->q_ogate );

	if ( rp->q_opoll.e_procp )		/* someone polling to write */
		pollwake( &rp->q_opoll );
}
