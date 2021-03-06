/*
 * Standard I/O Library
 * Get word (int)
 */

#include <stdio.h>

int
fgetw(fp)
register FILE	*fp;
{
	register int	c0, c1;

	if ((c0=getc(fp))==EOF)
		return (EOF);
	else if ((c1=getc(fp))==EOF) {
		fp->_ff |= _FERR;
		return (EOF);
	} else
		return (c0<<8|c1);
}
