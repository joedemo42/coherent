/*
 *  uulog.c
 *
 *  Dump the UUCP uucico or uuxqt log files.
 */

#include "dcp.h"

#define LOGSDIR	 ".Log"

char	logdir[LOGFLEN];
char	command[BUFSIZ];
char	*sysname = NULL;
char	*process = "uucico";
int	fflag = 0;
int	tailnum = 0;

extern	int optind;
extern	int optopt;
extern	char *optarg;

main(argc, argv)
int argc;
char *argv[];
{
	int ch, exstat=0;

	while ( (ch=getopt(argc, argv, "f:n:vVx")) != EOF ) {
		switch (ch) {
		case 'f':
			fflag = 1;
			sysname = optarg;
			break;
		case 'n':
			tailnum = atoi(optarg);
			break;
		case 'x':
			process = "uuxqt";
			break;
		case 'v':
		case 'V':
			fatal("uulog: Version %s", VERSION);
		case '?':
		default:
			usage("Improper option usage: %c", optopt);
		}
	}
	sprintf(logdir, "%s/%s/%s", SPOOLDIR, LOGSDIR, process);
	if ( fflag ) {
		if ( optind < argc )
			usage("only one system with -f flag");
		if ( tailnum > 0 )
			sprintf(command, "tail -%df %s/%s", tailnum,
							logdir, sysname);
		else
			sprintf(command, "tail -f %s/%s", logdir, sysname);
		exit( system(command) );
	}

	if ( optind >= argc ) {
		if ( tailnum > 0 )
			sprintf(command, "for zz in `uuname`; do \
					  tail -%d %s/$zz; done",
							tailnum, logdir);
		else
			sprintf(command, "for zz in `uuname`; do \
					  cat %s/$zz; done", logdir);
		exit( system(command) );
	}

	for (;optind<argc; optind++) {
		if ( tailnum > 0 )
			sprintf(command, "tail -%d %s/%s", tailnum,
						logdir, argv[optind]);
		else
			sprintf(command, "cat %s/%s", logdir, argv[optind]);
		exstat |= system(command);
	}
	exit(exstat);
}

usage(x)
{
	fatal("\n%r\n\
Usage:	uulog [ -n <tail-numlines> ] [ -x ] [ -f <sys> ]\n\
	uulog [ -n <tail-numlines> ] [ -x ] [ system ... ]\n\
", &x);
}
