/*
 * Print an almanac of daily events.  Program reads today's date,
 * constructs a date string, and then examines the files
 * /usr/games/lib/almanac.birth, /usr/games/lib/almanac.death, and
 * /usr/games/lib/almanac.event for strings that contain the date string.
 * Originally written in the shell.
 *
 * By fb, 7/12/88.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>

extern time_t time();

char *months[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
	
char *filenames[] = {
	"/usr/games/lib/almanac.birth",
	"/usr/games/lib/almanac.death",
	"/usr/games/lib/almanac.event"
};

char *slugs[] = {
	"BIRTHS:",
	"DEATHS:",
	"EVENTS:"
};

/* print an error message and exit */
fatal(message)
char *message;
{
	fprintf (stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}

findstring(date, fileptr)
char *date;
FILE *fileptr;
{
	char buffer[120];
	int length = strlen(date);

	while (fgets(buffer, 120, fileptr) != NULL) {
		if (strncmp(buffer, date, length) == 0)
			printf ("%s", buffer+length);
		else
			continue;
	}
}

main(argc, argv)
int argc; char *argv[];
{
	long rawtime;
	struct tm *brokentime;
	FILE *fileptr;
	char buffer[20];
	int i;

	if (argc == 3) {
		if (strlen(argv[1]) < 3)
			fatal("Usage: almanac [Mon date]");
		argv[1][3] = '\0';
		sprintf(buffer, "%3s %s ", argv[1], argv[2]);
	} else if (argc == 1) {
		/* build the time string */
		rawtime = time(NULL);
		brokentime = localtime(&rawtime);
		sprintf(buffer, "%s %d ", months[brokentime->tm_mon],
			brokentime->tm_mday);
	} else
		fatal("Usage: almanac [Mon date]");

	/* open almanac text files; call string printing routine */
	for (i = 0; i < 3; i++) {
		if ((fileptr = fopen(filenames[i], "r")) == NULL)
			fatal("Cannot open almanac file.");
		
		printf("%s\n", slugs[i]);

		findstring(buffer, fileptr);

		fclose (fileptr);
	}
	exit(EXIT_SUCCESS);
}
