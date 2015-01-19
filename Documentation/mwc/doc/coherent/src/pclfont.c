/*
 * pclfont.c
 *
 * Prepare a font for downloading to a PCL printer via lp.
 *
 * Usage:  pclfont [-f n] font [... font] | lp -dhpfont
 *
 * where -f indicates to begin loading the fonts into memory beginning with
 * slot n
 *
 * by fwb, 8/13/93 (Friday the 13th)
 */
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *usage = "Usage: pclfont [-f n] font [... font]";
char buffer[600];

void fatal(message)
char * message;
{
	sprintf(buffer, "%s", message);
	write (STDOUT_FILENO, buffer, strlen(buffer));
	exit(EXIT_FAILURE);
}

void beginstring(slot)
int slot;
{
	sprintf(buffer, "\033*c%dD", slot);
	write (STDOUT_FILENO, buffer, strlen(buffer));
}

void endstring()
{
	sprintf(buffer, "\033*c5F");
	write(STDOUT_FILENO, buffer, strlen(buffer));
}

void copyfont (indev)
int indev;
{
	int numread;

	while (1) {
		numread = read(indev, buffer, 512);

		if (numread == -1)
			fatal("Error on reading font file");

		write(STDOUT_FILENO, buffer, numread);

		if (numread < 512)
			return;	
	}
}

main (argc, argv)
int argc; char *argv[1];
{
	int indev, i, baseslot;

	/* find the base slot */
	if (!strncmp(argv[1], "-f", 2)) {
		baseslot = atoi(argv[2]);
		if (baseslot < 1)
			fatal ("Font slot must be at least 1");
		i = 3;
	} else {
		i = 1;
		baseslot = 1;
	}

	if (i == argc) {
		beginstring (baseslot);
		copyfont(STDIN_FILENO);
		endstring();
		exit(EXIT_SUCCESS);
	}

	for ( ; i < argc; baseslot++, i++) {
		if ((indev = open(argv[i], O_RDONLY, 0)) == -1)
			fatal("Could not open font file");
		beginstring (baseslot);
		copyfont(indev);
		endstring();
	}
	exit(EXIT_SUCCESS);
}
