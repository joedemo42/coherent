/* sh/alloc.c */

#include "sh.h"

extern	char	*malloc();

/*
 * Externals.
 */
BUF	*bufap = NULL;			/* Head of allocation list */
BUF	*buffp = NULL;			/* Tail of free list */
BUF	**bufapp = &bufap;		/* Tail of allocation list */

/*
 * Given a string, return a pointer to a copy of it.  If 'f'
 * is set, don't add it onto the free list.
 */
char *
duplstr(cp0, f)
register char *cp0;
{
	register char *cp1, *cp2;

	cp1 = cp0;
	while (*cp1++)
		;
#if 0
	cp2 = (char *) f ? salloc (cp1 - cp0) : balloc (cp1 - cp0);
#else
	if ((char *) f)
		cp2 = salloc (cp1 - cp0);
	else	
		cp2 = balloc (cp1 - cp0);
#endif
	cp1 = cp2;
	while (*cp1++=*cp0++)
		;
	return (cp2);
}

/*
 * Create an argument list.
 */
char **
makargl()
{
	register char **app;

	app = (char **)balloc((1+IALSIZE)*sizeof(char *));
	app++;
	app[-1] = (char *)IALSIZE;
	app[0] = NULL;
	return (app);
}

/*
 * Add an element to an argument list.
 */
char **
addargl(app, sp)
char **app;
char *sp;
{
	char **napp;
	register char **rapp;
	register unsigned n, i;

	rapp = app;
	n = (int)rapp[-1];
	while (*rapp++ != NULL)
		;
	if (rapp-app >= n) {
		napp = (char **)balloc((1+n*2)*sizeof(char *));
		napp++;
		napp[-1] = (char *)(n*2);
		for (i=0; i<n; i++)
			napp[i] = app[i];
		napp[n-1] = sp;
		napp[n] = NULL;
		return (napp);
	}
	*rapp = NULL;
	*--rapp = sp;
	return (app);
}

/*
 * Save allocation position.
 */
BUF **
savebuf()
{
	return (bufapp);
}

/*
 * Free everything allocated since passed allocation position
 * was saved.
 */
freebuf(bpp)
register BUF **bpp;
{
	*bufapp = buffp;
	buffp = *bpp;
	*bpp = NULL;
	bufapp = bpp;
}

/*
 * Allocate a buffer 'n' bytes long.
 * Add it onto the allocated buffer list.
 */
char *
balloc(n)
{
	register BUF *bp, **bpp;

	bpp = &buffp;
	for (;;) {
		if ((bp=*bpp) == NULL) {
			bp = (BUF *) salloc(sizeof (BUF) + n);
			bp->b_size = n;
			break;
		}
		if (bp->b_size == n) {
			*bpp = bp->b_next;
			break;
		}
		bpp = &bp->b_next;
	}
	*bufapp = bp;
	bufapp = &bp->b_next;
	*bufapp = NULL;
	return ((char *) bp + sizeof (BUF));
}

/*
 * Allocate 'n' bytes.
 */
char *
salloc(n)
{
	register char *cp;
	char	*malloc();

	if ((cp=malloc(n)) == NULL) {
		prints("Out of memory\n");
		reset(RNOSBRK);
		NOTREACHED;
	}

	return (cp);
}

/*
 * Free something possibly allocated by 'salloc'.
 *	notmem is part of our customised malloc package which
 *	prevents freeing of static strings and automatic variables.
 */
sfree(cp)
char *cp;
{
#if 0
	if (cp != NULL && notmem(cp)) {
		fprintf (stderr, "freeing \"%s\" at %x\n", cp, cp);
		return;
	}
#endif
	free(cp);
}

#ifdef	ALLOC_DEBUG
/*
 * For allocation debugging.
 */

void alloc_alloc (debug) struct alloc_debug * debug; {
	if (++ debug->count > debug->maxcount) {
		debug->maxcount = debug->count;
		fprintf (stderr, "%s:%d\n", debug->name, debug->count);
	}
}

#endif

ALLOC_COUNT (vector)

/*
 * Deallocate a vector.
 */
vfree(vecp)
char **vecp;
{
	register char **vpp;

	if (notmem(vecp))
		return;
	for (vpp = vecp; *vpp!=NULL; vpp += 1)
		sfree(*vpp);
	sfree(vecp);
	ALLOC_FREE (vector)
}

char **
vdupl(vecp)
char **vecp;
{
	register char **vp, **nvp, **tvp;

	for (vp=vecp; *vp++ != NULL; );
	tvp = nvp = (char **)salloc((vp - vecp) * sizeof *vp);
	for (vp=vecp; *vp != NULL; )
		*tvp++ = duplstr(*vp++, 1);
	*tvp = NULL;
	ALLOC_ALLOC (vector);
	return (nvp);
}

/* end of sh/alloc.c */
