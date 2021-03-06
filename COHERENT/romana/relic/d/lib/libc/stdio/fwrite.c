/*
 * Standard I/O Library
 * Write nitems of size to file fp
 */

#include <stdio.h>

int
fwrite(bp, size, nitems, fp)
register unsigned char	*bp;
unsigned int	size, nitems;
register FILE	*fp;
{
	register unsigned int	nb = size*nitems;

	if (fp->_ff&_FUNGOT)
		(*fp->_gt)(fp);
	if (fp->_bp!=NULL || !(fp->_ff&_FSTBUF)) {
		for (; nb && putc(*bp++, fp)!=EOF; nb--)
			;
		if (nb)
			return (0);
		else
			return (nitems);
	} else if (write(fileno(fp), bp, nb) != nb) {
		fp->_ff |= _FERR;
		return (0);
	} else
		return (nitems);
}
