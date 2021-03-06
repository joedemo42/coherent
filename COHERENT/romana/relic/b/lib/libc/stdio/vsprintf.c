/*
 * libc/stdio/vsprintf.c
 * ANSI-compliant C standard i/o library.
 * vsprintf()
 * ANSI 4.9.6.9.
 * Formatted print into given string.
 */

#include <stdio.h>
#include <stdarg.h>

int
vsprintf(s, format, args) char *s; const char *format; va_list args;
{
	register int	count;
	FILE		file;
	_FILE2		file2;

	file._f2p = &file2;
	_stropen(s, -1, &file);
	count = vfprintf(&file, format, args);
	putc('\0', &file);
	return count;
}

/* end of libc/stdio/vsprintf.c */
