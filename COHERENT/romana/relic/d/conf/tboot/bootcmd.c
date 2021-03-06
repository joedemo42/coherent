static char _version[]="boot_cmd version 1.0";
/*
 * This is a standalone program to extract the
 * command line from /dev/boot_gift.
 *
 * Prints the command line on stdout with spaces between arguments.
 * If there was not command line (i.e. tboot ran /autoboot),
 * there will be no output.
 *
 * An exit value of 0 is normal.
 * An exit value of 1 indicates that there was no command line.
 * An exit value of 2 indicates that there is an abnormal
 * condition--there will be an error message on stderr.
 */
#include <fcntl.h>
#define KERNEL
#include <sys/typed.h>

TYPED_SPACE(boot_gift, BG_LEN, T_FIFO_SIC);

#define fd_stdout	1
#define fd_stderr	2
void
main()
{
	int fd;
	FIFO *ffp;
	typed_space *tp;

	/*
	 * Read the boot gift from /dev/boot_gift.
	 */
	if (-1 == (fd = open("/dev/boot_gift", O_RDONLY))) {
		write(fd_stderr, "Can not open /dev/boot_gift\n", 
			strlen("Can not open /dev/boot_gift\n") );
		exit(2);
	}
	
	if (0 == read(fd, &boot_gift, BG_LEN)) {
		close(fd);
		write(fd_stderr, "Can not read /dev/boot_gift\n",
			strlen("Can not read /dev/boot_gift\n") );
		exit(2);
	}

	close(fd);

	/*
	 * Find the command line in the boot gift.
	 */
	ffp = fifo_open(&boot_gift, 0);

	if (F_NULL != ffp) {
		while (T_NULL != (tp = fifo_read(ffp))) {
			if (T_STR_ARGF == tp->ts_type) {
				break;
			}
		}
	}
	
	fifo_close(ffp);

	/*
	 * We've found the command line.  
	 */
	if (T_NULL != tp) {
		/* Recast the argument list, so we can read it.  */
		RETYPE(tp, T_FIFO_SIC);
		ffp = fifo_open(tp, 0);
		while (T_NULL != (tp = fifo_read(ffp))) {
			if (T_STR_STR == tp->ts_type) {
				/*
				 * Print out each command item, followed
				 * by a space.
				 */
				write(fd_stdout, tp->ts_data,
					strlen(tp->ts_data));
				write(fd_stdout, " ", strlen(" ") );
			} else {
				/*
				 * Illegal item found in the command line.
				 */
				write(fd_stderr, "/dev/bootgift is corrupt.\n",
				      strlen("/dev/bootgift is corrupt.\n") );
				fifo_close(ffp);
				exit(2);
			}
		} /* while there are more command arguments */
		fifo_close(ffp);
		write(1, "\n", 1);
		exit(0);
	} else {
		/*
		 * There was no command line.  Generate no output,
		 * but exit 1 to indicate this fact.
		 */
		exit(1);
	}
	write(fd_stdout, "Unreachable code in main().\n",
		strlen("Unreachable code in main().\n") );
} /* main() */
