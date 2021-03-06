/*
 * Define useful symbols for use in the ps program.
 */
#ifndef PS_H	/* Rest of file.  */
#define PS_H

/*
 * These flags, ar_proc_flags, indicate which processes should be printed.
 */
#define	ALL_PROC	0x0001		/* All processes */
#define	DRIVER		0x0002		/* Print drivers */
#define	SPECIAL		0x0004		/* Get special processes */
#define	USER		0x0008		/* All processes for a given user  */
#define ATTY		0x0010		/* All processes for a given tty */

/*
 * These flags indicate which fields should be printed.  There are
 * two kinds of these--flags that indicate a format and flags that
 * indicate specific fields.  They are the ar_field_flags.
 */
#define	ALL_FIELDS	0x0001		/* Print out all fields */
#define	LONG_FMT	0x0002		/* Long format */
#define	WIDE_FMT	0x0004		/* Wide format */
#define PID_ONLY	0x0008		/* Print only PIDs */

#define	GROUPS		0x0100		/* Print out process groups */
#define	SCHED		0x0200		/* Print scheduling values */
#define	TIMES		0x0400		/* Print times */

/*
 * These are miscellaneous flags, ar_msc_flags.
 */
#define	REAL_SIZES	0x0001		/* Print out real sizes */
#define	NO_HEAD		0x0002		/* No header */
#define	DEBUG		0x0004		/* Debug flag */
#define	IGN_PRESENT	0x0008		/* UNDOCUMENTED: ignore present bit.  */


/*
 * Here we define some assorted handy values.
 */
typedef int boolean;
#define TRUE	1
#define FALSE	0

typedef struct DEVICES {
	struct DEVICES	*next;			/* Next entry */
	dev_t		dev;			/* Device number */
	char		pDevName[DIRSIZ];	/* Device name */
} stDevices;

#endif /* PS_H */
