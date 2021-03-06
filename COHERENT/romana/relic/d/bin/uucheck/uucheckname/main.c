/* uucheckname
 * Synopsis:
 * checkperms [-s] [-v]
 *  -s	silent
 *  -v	verbose
 * 
 * Return values:
 * 0 on no errors or warnings
 * 1 if there was at least one warning
 * 2 if there was an error
 *
 * $ uucheckname -errors -warnings 9
 * ...or equivalently...
 * $ uucheckname
 */

#define MAIN
#include "uucheck.h"
#include "uucheckname.h"
#undef MAIN

main(argc, argv)
	int argc;	
	char *argv[];
{
	warning = FALSE;
	error = FALSE;

	parse_args(argc, argv);
	
	uucheckname();

	if (!error && !warning) {
		VERBOSE("Site name is OK.\n");
	} /* if no errors or warnings  */

	/* Set the correct exit value.  */
	/* 2 on error, 1 on warning only, 0 on no failures */
	EXIT;
} /* main() */
