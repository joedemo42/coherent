/*
 * execute.c -- Attempt to run a loadable file.
 * Returns only if it fails.
 * Takes the inode for the file to be loaded, and a command line vector
 * to pass in.
 */
#include <coff.h>
#include <canon.h>
#include <sys/typed.h>
#include <sys/inode.h>
#include <sys/dir.h>

#ifdef TEST
#define MAIN
#endif
#include "tboot.h"
#undef MAIN

void
execute( argv )
	char *argv[];
{
	extern uint16 myds;	/* My data segment, from Setup.s.  */
	char *cmd_name;		/* File to boot.  */
	char response[BLOCK];	/* Command typed at prompt.  */

	struct inode imageinode;  /* Inode structure for the boot image.  */
	ino_t imageinum;		/* inode number of the boot image.  */

	uint16 filemagic;		/* Magic number from file.  */
	struct load_segment imagetable[MAX_SEGS]; /* How to load a file.  */
	struct load_segment *cur_segment; /* Pointer for walking imagetable.  */ 

	uint16 data_seg;	/* Data segment register for image.  */

	uint16 boot_value;      /* Offset of boot_gift into
				 * load image data segment.
				 */

	uint16 cpu_type;	/* What kind of CPU are we running on?  */
	
	cpu_type = get_cpu_type();

	/*
	 * Extract the command name from the command line.
	 */
	cmd_name = argv[0];
	
	/*
	 * Try to find the requested file.
	 */
	if (0 == (imageinum = namei(cmd_name)) ) {
		puts("No such file: ");
		puts(cmd_name);
		puts("\r\n");
		return;
	}

	/*
	 * Try to open that inode.
	 */
	if (0 == iopen(&imageinode, imageinum)) {
		puts("Can't open ");
		puts(cmd_name);
		puts(".  Type \"dir\" for a list of files.\r\n");
		return;
	}

	/*
	 * Read the magic number.
	 */
	iread(&imageinode, &filemagic, (fsize_t) 0, (uint16) sizeof (uint16));
	canint(filemagic);	/* Harmless on 80386.  */

	/*
	 * COFF executables probably should not be run on anything less
	 * than a 386.
	 */
	if (ISCOFF(filemagic)) {
		if (!IS_I386(cpu_type)) {
			puts("This is not a 386 or 486--");
			puts("you probably can not run this file.\r\n");
			puts("Continue anyway?  ");
			gets(response, BLOCK);
			if ( 0 != strcmp(response, "y") &&
			     0 != strcmp(response, "Y") &&
			     0 != strcmp(response, "yes") &&
			     0 != strcmp(response, "YES") &&
			     0 != strcmp(response, "sure") ) {
				return;
			}
		}
	} /* Is it COFF?  */

	/*
	 * If we haven't explicitly set sys_base, default it based
	 * on the type of file we are loading.
	 */
	if (!sys_base_set) {
		VERBOSE( {puts("Assuming default sys_base.\r\n");} );
		sys_base = object_sys_base(filemagic);
	}

	/*
	 * Try to read the information we need to actually load this file.
	 */
	if (! object2load(filemagic, &imageinode, imagetable, &data_seg)) {
		puts("File ");
		puts(cmd_name);
		puts(" is not an executable.\r\n");
		puts(".\r\n");
		return;
	}

	/* ASSERTION: imageinode and imagetable describe a valid executable.  */

	VERBOSE( {
		puts("OK!  Loading ");
		puts(cmd_name);
		puts("...\r\n");
	} );

	/* Now actually load everything into memory.  */
	for (cur_segment = &imagetable[0]; cur_segment->valid; ++cur_segment) {
		puts(cur_segment->message);

		/*
		 * Make sure that this segment will not overwrite tboot.
		 */
		if (seg_too_high(cur_segment)) {
			puts("This segment would overwrite tboot.\r\n");
			puts("Aborting...\r\n");
			return;
		}
		ifread(&imageinode,
			cur_segment->load_toseg,
			cur_segment->load_tooffset,
			cur_segment->load_offset,
			cur_segment->load_length);
	}

	/* Does the program we just loaded want more info?  */

	/* Look up the variable "boot_gift" in the image.  */	
	puts("\r\nPlease wait...\r\n");
	boot_value = object_nlist(filemagic, cmd_name, "boot_gift");


	if (0 != boot_value) {
		VERBOSE( { puts("Preparing arguments.\r\n"); } );
		/* Yes, this program can accept more information.  */
		prepare_gift(data_seg, boot_value, argv);
	}

	VERBOSE( {
		puts("\r\nRunning ");
		puts(cmd_name);
		puts("...\r\n");
	} );

	if (want_monitor) {
		puts("The kernel is ready to run.\r\n");
		puts("SYS_START: ");
		print16(SYS_START);
		puts(" sys_base: ");
		print16(sys_base);
		puts(" data_seg: ");
		print16(data_seg);
		puts("\r\n");
		monitor();
	}

	/* Run the image (the kernel).  */
	gotoker(SYS_START, sys_base, data_seg);
} /* execute() */


/*
 * int
 * seg_too_high( struct load_segment *lseg )
 * Check to see if a segment will overlap tboot.
 * Returns TRUE if it would, FALSE otherwise.
 */
int
seg_too_high( lseg )
	struct load_segment *lseg;
{
	uint32 load_top;

	load_top =	(lseg->load_toseg << 4) +
			lseg->load_tooffset +
			lseg->load_length;
	
#if 0
	printf("%x:%x + %lx = %lx\n",
		lseg->load_toseg,
		lseg->load_tooffset,
		lseg->load_length, 
		load_top);
#endif /* 0 */	

	return ( load_top >= ( ((uint32) RBOOTS) << 4 ) );
} /* seg_too_high() */

#ifdef TEST
int boot_gift;	/* Lie.  */
main()
{
	struct load_segment lseg;

	lseg.load_toseg	= 0x200;
	lseg.load_tooffset	= 0;
	lseg.load_length	= 128L*1024L;

	if (seg_too_high(&lseg)) {
		printf("BAD: %x:%x + %lx > %lx.\n", 
			lseg.load_toseg,
			lseg.load_tooffset,
			lseg.load_length,
			(((unsigned long)RBOOTS) << 4) );

	} else {
		printf("OK: %x:%x + %lx < %lx.\n", 
			lseg.load_toseg,
			lseg.load_tooffset,
			lseg.load_length,
			(((unsigned long)RBOOTS) << 4) );

	}
} /* main() */
#endif /* TEST */
