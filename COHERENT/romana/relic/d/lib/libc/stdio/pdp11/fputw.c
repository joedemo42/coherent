/*
 * Standard I/O Library
 * Put word (int) to file
 */

#include <stdio.h>

int
fputw(w, fp)
int	w;
register FILE	*fp;
{
	putc(w, fp);
	putc(w>>8, fp);
	return (w);
}
