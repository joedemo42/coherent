/*
There isn't such an animal except on mwc.  There we have a program called
ttytype which reads file /etc/ttytype to get terminal info.  We did this to
avoid people writing big shell case statements which took forever to run each
time a user logged into mwc.  Maybe we should document it and ship it with 
COH 4.1?  Let me know if you want details...

  -- norm --                                    norm@mwc.com, uunet!mwc!norm
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  "Where-oh-where has my SPARCstation gone? Oh where-oh-where can it be?"
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

/*
 * Print out the tty type of the current user
 * using the file /etc/ttytype
 * which contains lines of format
 * type name comments
 *
 * Examples:
 * ansipc    console    In the computer room
 * vt100     com2l      Accounting office
 */

#include <stdio.h>
#include <string.h>

extern char *ttyname();
char typefile[] = "/etc/ttytype";

#define	DEFAULT_TTYTYPE	"unknown\n"	/* if no entry found */

fatal(s)
char *s;
{
	fprintf(stderr, "ttytype: %r\n", &s);
	exit(1);
}

main()
{
	FILE *ifp;
	char line[80], type[20], name[20];
	char *tty;

	if (NULL == (tty = ttyname(fileno(stderr))) ||
	   NULL == (tty = strrchr(tty, '/')))
		fatal("Not a tty");
	tty++;
	if (NULL == (ifp = fopen(typefile, "r")))
		fatal("Cannot open %s", typefile);
	while (NULL != fgets(line, sizeof(line), ifp)) {
		sscanf(line, "%19s %19s", type, name);
		if (!strcmp(tty, name)) {
			printf("%s\n", type);
			exit(0);
		}
	}
	printf(DEFAULT_TTYTYPE);	/* the default */
	exit(0);
}
