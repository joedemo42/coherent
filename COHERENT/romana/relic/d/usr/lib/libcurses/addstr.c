/*
 * Copyright (c) 1981 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef COHERENT
#ifndef lint
static uchar sccsid[] = "@(#)addstr.c	5.4 (Berkeley) 6/30/88";
#endif /* not lint */
#endif /* not COHERENT */

# include	"curses.ext"

/*
 *	This routine adds a string starting at (_cury,_curx)
 *
 */
waddstr(win,str)
reg WINDOW	*win; 
reg uchar	*str;
{
# ifdef DEBUG
	fprintf(outf, "WADDSTR(\"%s\")\n", str);
# endif
	return waddbytes(win, str, strlen(str));
}
