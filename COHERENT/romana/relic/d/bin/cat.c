

/*
 * cat -- concatinate and print
 */
#include	<errno.h>
#include	<stdio.h>
#include	<access.h>

int	uflag;				/* don't buffer */
char	*filename;			/* current input file name */

main( argc, argv)
char	*argv[];
{
	static char	obuf[BUFSIZ];
	register int	i = 0;	

	++argv;
	if (--argc>0 && strcmp( *argv, "-u")==0) {
		++argv;
		--argc;
		++uflag;
		setbuf( stdout, (char *)NULL);
	}
	else
		setbuf( stdout, obuf);

	if (argc <= 0)
		cat("-");
	else
	{	while (*argv)
		{   if (strcmp(*argv, "-") && access(*argv, AREAD))
		    {  fprintf(stderr, "cat: %s: %s\n", *argv,
			  sys_errlist[errno]);
		       *argv[0] = 0;  /* flag arg as invalid */
		    }
		    i++;     /* count the arg */
		    argv++;  /* advance to next arg */
		}
		
		argv -= i;  /* back to start of filename args to process */

		do {
			if (*argv[0] == 0)  /* arg to skip? */ 
			{  argv++;
			   continue;        /* yes */
			}
			cat(*argv++);
		} while (--argc);
	}
	exit(0);
}


cat(file)
char *file;
{
	register	c;
	register FILE	*f;
	extern char	*sys_errlist[];
	extern		errno;

	if (strcmp( file, "-") == 0) {
		filename = "<stdin>";
		f = stdin;
	}
	else {
		filename = file;
		f = fopen( file, "r");
		if (f == (FILE *)NULL) 
		{  fprintf(stderr, "cat: %s: %s\n", file, sys_errlist[errno]);
		   return;
		}
	}
	if (uflag)
		setbuf( f, (char *)NULL);

	while ((c=getc( f)) != EOF) {
		putchar( c);
		if (ferror( stdout))
			fatal( errno==EIO? "write error": sys_errlist[errno]);
	}

	if (ferror( f))
		fatal( errno==EIO? "read error": sys_errlist[errno]);
	if (f != stdin)
		fclose( f);
}


fatal( mesg)
char	*mesg;
{

	fflush( stdout);
	printerr( "cat: ", filename, ": ", mesg, "\n", (char *)0);
	exit(1);
}


printerr( arg0)
char	*arg0;
{
	register char	**pp;

	for (pp= &arg0; *pp; )
		fputs( *pp++, stderr);
}
