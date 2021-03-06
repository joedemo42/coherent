/* 
 * Display a random line on a file. Suitable for automated
 * fortune cookies. Multi line fortunes such as poems should
 * have their lines separated by @'s.
 */
#include <sys/timeb.h>
#include <stdio.h>
#define RAND_MAX 32767

char *fortfile = "/usr/games/lib/fortunes";

void
main(argc, argv)
char *argv[];
{
	FILE  *ifp;
	double  randomAdj;
	int c;
	struct timeb tb;

	if(argc > 1)
		fortfile = argv[1];

	if(NULL == (ifp = fopen(fortfile, "r"))) {
		printf("Cannot open %s\n", fortfile);
		exit(1);
	}

	fseek(ifp, 0L, 2);
	randomAdj = (double)ftell(ifp) / ((double)RAND_MAX);

	ftime(&tb);
	srand((unsigned int)(tb.time ^ tb.millitm));
	/* a user running this several times in quick succession will
	 * be dissapointed if he keeps getting the same result.
	 * Running rand() 100 times distributes the low order bit
	 * changes in the seed to the high half returned by rand().
	 */
	for(c = 0; c < 100; c++)
		rand();

	fseek(ifp, (long)(randomAdj * (double)rand()), 0);
	while('\n' != (c = fgetc(ifp)) && EOF != c)
		;

	if(c == EOF) {
		printf("File does not end with nl\n");
		exit(1);
	}
		
	while('\n' != (c = fgetc(ifp))) {
		if(EOF == c) {
			fseek(ifp, 0L, 0);
			continue;
		}
		if('@' == c) /* to display multi line fortunes */
			c = '\n';
		putchar(c);
	}
	putchar('\n');
}
