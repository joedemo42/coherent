/*
 * File:	stty.c
 *
 * Purpose:	COH 386 stty command, based on termio
 *	This version is more S5 compliant than COH 286's was.
 *	It gets status from stdin, and writes to stdout.
 *	Using -g flag reports termio contents as hex dump
 *	Input can be a string of 12 hex numbers separated by colons,
 *	  same as output using -g flag.
 *
 * $Log:	stty.c,v $
 * Revision 1.8  93/09/08  08:52:20  bin
 * hal:report tabs/-tabs in brief output
 * 
 * Revision 1.4  92/08/11  15:59:27  root
 * COH 4.0.1
 * 
 */

/*
 * ----------------------------------------------------------------------
 * Includes.
 */
#include <stdio.h>
#include <termio.h>

/*
 * ----------------------------------------------------------------------
 * Definitions.
 *	Constants.
 *	Macros with argument lists.
 *	Typedefs.
 *	Enums.
 */
#define NAMELN		8
#define OPTNAMELEN	16

#define DEL	0x7F
#define NUL	0

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;
/*
 * For fields in c_[iocl]flag field in termio,
 *	optname is the name of the stty command switch
 *	mask has 1's where bits are modified
 *	val has the correct value at the bits in question
 */
typedef struct {
	char	optname[OPTNAMELEN];
	short	mask;
	short	val;
} FLAG_OPT;

/*
 * ----------------------------------------------------------------------
 * Functions.
 *	Import Functions.
 *	Export Functions.
 *	Local Functions.
 */
static void baudstr();
static int c_opt();
static int combo();
static void dump_all();
static void dump_brief();
static void dump_cc();
static void dump_ch();
static void dump_cflag();
static void dump_hex();
static void dump_iflag();
static void dump_lflag();
static void dump_oflag();
static int num_opt();
static void panic();
static int set_ch();
static int set_num();
static int set_speed();
static void set_hex();
static int simple();

/*
 * ----------------------------------------------------------------------
 * Global Data.
 *	Import Variables.
 *	Export Variables.
 *	Local Variables.
 */
static int gflag;
static int sflag;	/* true if setting termio rather than getting */
static struct termio t;

int	g_argc;
char	** g_argv;
char	lbuf[200];

FLAG_OPT i_list[] = {
	{ "ignbrk", IGNBRK, IGNBRK },
	{ "-ignbrk", IGNBRK, 0 },
	{ "brkint", BRKINT, BRKINT },
	{ "-brkint", BRKINT, 0 },
	{ "ignpar", IGNPAR, IGNPAR },
	{ "-ignpar", IGNPAR, 0 },
	{ "parmrk", PARMRK, PARMRK },
	{ "-parmrk", PARMRK, 0 },
	{ "inpck", INPCK, INPCK },
	{ "-inpck", INPCK, 0 },
	{ "istrip", ISTRIP, ISTRIP },
	{ "-istrip", ISTRIP, 0 },
	{ "inlcr", INLCR, INLCR },
	{ "-inlcr", INLCR, 0 },
	{ "igncr", IGNCR, IGNCR },
	{ "-igncr", IGNCR, 0 },
	{ "icrnl", ICRNL, ICRNL },
	{ "-icrnl", ICRNL, 0 },
	{ "iuclc", IUCLC, IUCLC },
	{ "-iuclc", IUCLC, 0 },
	{ "ixon", IXON, IXON },
	{ "-ixon", IXON, 0 },
	{ "ixany", IXANY, IXANY },
	{ "-ixany", IXANY, 0 },
	{ "ixoff", IXOFF, IXOFF },
	{ "-ixoff", IXOFF, 0 },
	{ "", 0, 0 }
};
FLAG_OPT o_list[] = {
	{ "opost", OPOST, OPOST },
	{ "-opost", OPOST, 0 },
	{ "olcuc", OLCUC, OLCUC },
	{ "-olcuc", OLCUC, 0 },
	{ "onlcr", ONLCR, ONLCR },
	{ "-onlcr", ONLCR, 0 },
	{ "ocrnl", OCRNL, OCRNL },
	{ "-ocrnl", OCRNL, 0 },
	{ "onocr", ONOCR, ONOCR },
	{ "-onocr", ONOCR, 0 },
	{ "onlret", ONLRET, ONLRET },
	{ "-onlret", ONLRET, 0 },
	{ "ofill", OFILL, OFILL },
	{ "-ofill", OFILL, 0 },
	{ "ofdel", OFDEL, OFDEL },
	{ "-ofdel", OFDEL, 0 },
	{ "nl0", NLDLY, NL0 },
	{ "nl1", NLDLY, NL1 },
	{ "cr0", CRDLY, CR0 },
	{ "cr1", CRDLY, CR1 },
	{ "cr2", CRDLY, CR2 },
	{ "cr3", CRDLY, CR3 },
	{ "tab0", TABDLY, TAB0 },
	{ "tab1", TABDLY, TAB1 },
	{ "tab2", TABDLY, TAB2 },
	{ "tab3", TABDLY, TAB3 },
	{ "bs0", BSDLY, BS0 },
	{ "bs1", BSDLY, BS1 },
	{ "vt0", VTDLY, VT0 },
	{ "vt1", VTDLY, VT1 },
	{ "ff0", FFDLY, FF0 },
	{ "ff1", FFDLY, FF1 },
	{ "", 0, 0 }
};
FLAG_OPT c_list[] = {
	{ "0", CBAUD, B0 },
	{ "50", CBAUD, B50 },
	{ "75", CBAUD, B75 },
	{ "110", CBAUD, B110 },
	{ "134", CBAUD, B134 },
	{ "150", CBAUD, B150 },
	{ "200", CBAUD, B200 },
	{ "300", CBAUD, B300 },
	{ "600", CBAUD, B600 },
	{ "1200", CBAUD, B1200 },
	{ "1800", CBAUD, B1800 },
	{ "2400", CBAUD, B2400 },
	{ "4800", CBAUD, B4800 },
	{ "9600", CBAUD, B9600 },
	{ "19200", CBAUD, B19200 },
	{ "38400", CBAUD, B38400 },
	{ "cs5", CSIZE, CS5 },
	{ "cs6", CSIZE, CS6 },
	{ "cs7", CSIZE, CS7 },
	{ "cs8", CSIZE, CS8 },
	{ "cstopb", CSTOPB, CSTOPB },
	{ "-cstopb", CSTOPB, 0 },
	{ "cread", CREAD, CREAD },
	{ "-cread", CREAD, 0 },
	{ "parenb", PARENB, PARENB },
	{ "-parenb", PARENB, 0 },
	{ "parodd", PARODD, PARODD },
	{ "-parodd", PARODD, 0 },
	{ "hupcl", HUPCL, HUPCL },
	{ "-hupcl", HUPCL, 0 },
	{ "hup", HUPCL, HUPCL },
	{ "-hup", HUPCL, 0 },
	{ "clocal", CLOCAL, CLOCAL },
	{ "-clocal", CLOCAL, 0 },
	{ "", 0, 0 }
};
FLAG_OPT l_list[] = {
	{ "isig", ISIG, ISIG },
	{ "-isig", ISIG, 0 },
	{ "icanon", ICANON, ICANON },
	{ "-icanon", ICANON, 0 },
	{ "xcase", XCASE, XCASE },
	{ "-xcase", XCASE, 0 },
	{ "echo", ECHO, ECHO },
	{ "-echo", ECHO, 0 },
	{ "echoe", ECHOE, ECHOE },
	{ "-echoe", ECHOE, 0 },
	{ "echok", ECHOK, ECHOK },
	{ "-echok", ECHOK, 0 },
	{ "echonl", ECHONL, ECHONL },
	{ "-echonl", ECHONL, 0 },
	{ "noflsh", NOFLSH, NOFLSH },
	{ "-noflsh", NOFLSH, 0 },
	{ "", 0, 0 }
};

/*
 * ----------------------------------------------------------------------
 * Code.
 */

main(argc, argv, envp)
int argc;
char ** argv, ** envp;
{
	int	argn;
	int	want_ch = -1;
	int	want_num = -1;
	int	want_line = 0;
	unsigned int	line;
	char	* arg;
	int	speed;

	g_argc = argc;
	g_argv = argv;

	if (ioctl(0, TCGETA, &t) == -1)
		panic("can't get terminal parameters for stdin");
	/*
	 * Decide which format the command line is:
	 *	stty
	 *	stty -a
	 *	stty -g
	 *	stty x:x:...:x  (12 hex values with colon separators)
	 *	stty arglist...
	 */
	if (argc == 1) {
		dump_brief();
		goto stty_done;
	}
	if (argc == 2) {
		if (strcmp(argv[1], "-a") == 0) {
			dump_all();
			goto stty_done;
		}
		if (strcmp(argv[1], "-g") == 0) {
			dump_hex();
			goto stty_done;
		}
		if (index(argv[1], ':')) {
			set_hex();
			goto stty_done;
		}
	}
	
	/*
	 * Process an argument list.
	 * Possible argument formats are:
	 *	nnnn		(decimal baud rate)
	 *	line nnnn	(line discipline spec)
	 *	{intr|quit|erase|kill|eof|eol} {c|^c|^?|^-|0xnn}
	 *	{min|time} {nnn}
	 *	simple_option
	 *	combined_option
	 */
	for (argn = 1; argn < argc; argn++) {
		arg = argv[argn];
		if (want_ch >= 0) {
			if (!set_ch(want_ch, arg))
				fprintf(stderr,
				  "stty: invalid option %s %s\n",
				  argv[argn-1], arg);
			want_ch = -1;
			continue;
		}
		if (want_num >= 0) {
			if (!set_num(want_num, arg))
				fprintf(stderr,
				  "stty: invalid option %s %s\n",
				  argv[argn-1], arg);
			want_num = -1;
			continue;
		}
		if (want_line) {
			if (sscanf(arg, "%d", &line) == 1 && line < 128)
				t.c_line = line;
			else
				fprintf(stderr,
				  "stty: invalid option line %s\n", arg);
			want_line = 0;
			continue;
		}
#if 0
		if (sscanf(arg, "%d", &speed) == 1) {
			if (!set_speed(speed))
				fprintf(stderr,
				  "stty: invalid speed %d\n", speed);
			continue;
		}
#endif
		if (strcmp(arg, "line") == 0) {
			want_line = 1;
			continue;
		}
		if ((want_ch = c_opt(arg)) >= 0)
			continue;
		if ((want_num = num_opt(arg)) >= 0)
			continue;
		if (simple(arg, i_list, &t.c_iflag))
			continue;
		if (simple(arg, o_list, &t.c_oflag))
			continue;
		if (simple(arg, c_list, &t.c_cflag))
			continue;
		if (simple(arg, l_list, &t.c_lflag))
			continue;
		if (combo(arg))
			continue;
		fprintf(stderr, "stty: invalid option %s\n", arg);
	}
	if (ioctl(0, TCSETA, &t) == -1)
		panic("can't set terminal parameters");
	if (want_ch >= 0 || want_line)
		fprintf(stderr, "stty: incomplete option %s\n", argv[argn-1]);
stty_done:
	exit(0);
}

/*
 * dump_all()
 *
 * Display all termio settings.
 */
void
dump_all()
{
	dump_iflag();
	dump_oflag();
	dump_cflag();
	dump_lflag();
	printf("line=%d\n", t.c_line);
	dump_cc();
}

void
dump_iflag()
{
	short f = t.c_iflag;

	lbuf[0] = '\0';
	strcat(lbuf, (f&IGNBRK)?"ignbrk\t":"-ignbrk\t");
	strcat(lbuf, (f&BRKINT)?"brkint\t":"-brkint\t");
	strcat(lbuf, (f&IGNPAR)?"ignpar\t":"-ignpar\t");
	strcat(lbuf, (f&PARMRK)?"parmrk\t":"-parmrk\t");
	strcat(lbuf, (f&INPCK)?"inpck\t":"-inpck\t");
	strcat(lbuf, (f&ISTRIP)?"istrip\t":"-istrip\t");
	strcat(lbuf, (f&INLCR)?"inlcr\t":"-inlcr\t");
	strcat(lbuf, (f&IGNCR)?"igncr\n":"-igncr\n");
	fputs(lbuf, stdout);

	lbuf[0] = '\0';
	strcat(lbuf, (f&ICRNL)?"icrnl\t":"-icrnl\t");
	strcat(lbuf, (f&IUCLC)?"iuclc\t":"-iuclc\t");
	strcat(lbuf, (f&IXON)?"ixon\t":"-ixon\t");
	strcat(lbuf, (f&IXANY)?"ixany\t":"-ixany\t");
	strcat(lbuf, (f&IXOFF)?"ixoff\n":"-ixoff\n");
	fputs(lbuf, stdout);
}

void
dump_oflag()
{
	short f = t.c_oflag;

	lbuf[0] = '\0';
	strcat(lbuf, (f&OPOST)?"opost\t":"-opost\t");
	strcat(lbuf, (f&OLCUC)?"olcuc\t":"-olcuc\t");
	strcat(lbuf, (f&ONLCR)?"onlcr\t":"-onlcr\t");
	strcat(lbuf, (f&OCRNL)?"ocrnl\t":"-ocrnl\t");
	strcat(lbuf, (f&ONOCR)?"onocr\t":"-onocr\t");
	strcat(lbuf, (f&ONLRET)?"onlret\t":"-onlret\t");
	strcat(lbuf, (f&OFILL)?"ofill\t":"-ofill\t");
	strcat(lbuf, (f&OFDEL)?"ofdel\n":"-ofdel\n");
	fputs(lbuf, stdout);

	lbuf[0] = '\0';
	strcat(lbuf, (f&NLDLY)?"nl1\t":"nl0\t");
	switch(f&CRDLY) {
	case 0x0000:  strcat(lbuf, "cr0\t");  break;
	case 0x0200:  strcat(lbuf, "cr1\t");  break;
	case 0x0400:  strcat(lbuf, "cr2\t");  break;
	case 0x0600:  strcat(lbuf, "cr3\t");  break;
	}
	switch(f&TABDLY) {
	case 0x0000:  strcat(lbuf, "tab0\t");  break;
	case 0x0800:  strcat(lbuf, "tab1\t");  break;
	case 0x1000:  strcat(lbuf, "tab2\t");  break;
	case 0x1800:  strcat(lbuf, "tab3\t");  break;
	}
	strcat(lbuf, (f&BSDLY)?"bs1\t":"bs0\t");
	strcat(lbuf, (f&VTDLY)?"vt1\t":"vt0\t");
	strcat(lbuf, (f&FFDLY)?"ff1\n":"ff0\n");
	fputs(lbuf, stdout);
}

void
dump_cflag()
{
	short f = t.c_cflag;

	lbuf[0] = '\0';
	baudstr(f);
	switch(f&CSIZE) {
	case 0x0000:  strcat(lbuf, "cs5\t");  break;
	case 0x0010:  strcat(lbuf, "cs6\t");  break;
	case 0x0020:  strcat(lbuf, "cs7\t");  break;
	case 0x0030:  strcat(lbuf, "cs8\t");  break;
	}
	strcat(lbuf, (f&CSTOPB)?"cstopb\t":"-cstopb\t");
	strcat(lbuf, (f&CREAD)?"cread\t":"-cread\t");
	strcat(lbuf, (f&PARENB)?"parenb\t":"-parenb\t");
	strcat(lbuf, (f&PARODD)?"parodd\t":"-parodd\t");
	strcat(lbuf, (f&HUPCL)?"hupcl\t":"-hupcl\t");
	strcat(lbuf, (f&CLOCAL)?"clocal\n":"-clocal\n");
	fputs(lbuf, stdout);
}

void
dump_lflag()
{
	short f = t.c_lflag;

	lbuf[0] = '\0';
	strcat(lbuf, (f&ISIG)?"isig\t":"-isig\t");
	strcat(lbuf, (f&ICANON)?"icanon\t":"-icanon\t");
	strcat(lbuf, (f&XCASE)?"xcase\t":"-xcase\t");
	strcat(lbuf, (f&ECHO)?"echo\t":"-echo\t");
	strcat(lbuf, (f&ECHOE)?"echoe\t":"-echoe\t");
	strcat(lbuf, (f&ECHOK)?"echok\t":"-echok\t");
	strcat(lbuf, (f&ECHONL)?"echonl\t":"-echonl\t");
	strcat(lbuf, (f&NOFLSH)?"noflsh\n":"-noflsh\n");
	fputs(lbuf, stdout);
}

void
dump_cc()
{
	dump_ch("intr", t.c_cc[VINTR]);
	dump_ch("quit", t.c_cc[VQUIT]);
	dump_ch("erase", t.c_cc[VERASE]);
	dump_ch("kill", t.c_cc[VKILL]);
	if (t.c_lflag & ICANON) {
		dump_ch("eof", t.c_cc[VEOF]);
		dump_ch("eol", t.c_cc[VEOL]);
		dump_ch("eol2", t.c_cc[VEOL2]);
	} else {
		printf("min=%d  time=%d  ", t.c_cc[VMIN], t.c_cc[VTIME]);
		dump_ch("swtch", t.c_cc[VSWTCH]);
	}
	putchar('\n');
}

void
dump_ch(tag, ch)
char	* tag;
unsigned char	ch;
{
	if (ch == '\0')
		printf("%s=<undef>  ", tag);
	else if (ch < 0x20)
		printf("%s=^%c  ", tag, ch|0x60);
#if 0
	else if (ch > 0x7f)
		printf("%s=%x  ", tag, ch);
#endif
	else if (ch != 0x7f)
		printf("%s=%c  ", tag, ch);
	else
		printf("%s=DEL  ", tag);
}

/*
 * dump_brief()
 *
 * Display names for selected settings.
 */
void
dump_brief()
{
	short f;

	f = t.c_iflag;
	lbuf[0] = '\0';
	strcat(lbuf, (f&BRKINT)?"brkint\t":"-brkint\t");
	strcat(lbuf, (f&INPCK)?"inpck\t":"-inpck\t");
	strcat(lbuf, (f&ICRNL)?"icrnl\t":"-icrnl\t");

	f = t.c_oflag;
	strcat(lbuf, (f&OPOST)?"opost\t":"-opost\t");
	strcat(lbuf, (f&ONLCR)?"onlcr\t":"-onlcr\t");
	switch(f&TABDLY) {
	case 0x0000:  strcat(lbuf, "tab0\n");  break;
	case 0x0800:  strcat(lbuf, "tab1\n");  break;
	case 0x1000:  strcat(lbuf, "tab2\n");  break;
	case 0x1800:  strcat(lbuf, "tab3\n");  break;
	}
	fputs(lbuf, stdout);


	f = t.c_cflag;
	lbuf[0] = '\0';
	baudstr(f);
	strcat(lbuf, (f&PARENB)?"parenb\t":"-parenb\t");
	strcat(lbuf, (f&PARODD)?"parodd\t":"-parodd\t");
	strcat(lbuf, (f&HUPCL)?"hupcl\n":"-hupcl\n");
	fputs(lbuf, stdout);

	f = t.c_lflag;
	lbuf[0] = '\0';
	strcat(lbuf, (f&ICANON)?"icanon\t":"-icanon\t");
	strcat(lbuf, (f&ECHO)?"echo\t":"-echo\t");
	strcat(lbuf, (f&ECHOE)?"echoe\t":"-echoe\t");
	strcat(lbuf, (f&ECHOK)?"echok\n":"-echok\n");
	fputs(lbuf, stdout);

	dump_ch("intr", t.c_cc[VINTR]);
	dump_ch("quit", t.c_cc[VQUIT]);
	dump_ch("erase", t.c_cc[VERASE]);
	dump_ch("kill", t.c_cc[VKILL]);
	putchar('\n');
}

/*
 * set_hex()
 *
 * Copy sequence of 12 hex input values to termio struct
 * and send to device.
 */
void
set_hex()
{
	int j[12];	/* need int size for %x input */

	/*
	 * Arg must be 12 hex numbers separated by colons.
	 */
	t.c_line = 0;
	if (12 == sscanf(g_argv[1],
	  "%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x",
	  j,j+1,j+2,j+3,j+4,j+5,
	  j+6,j+7,j+8,j+9,j+10,j+11)) { t.c_iflag = j[0];
		t.c_oflag = j[1];
		t.c_cflag = j[2];
		t.c_lflag = j[3];
		t.c_cc[0] = j[4];
		t.c_cc[1] = j[5];
		t.c_cc[2] = j[6];
		t.c_cc[3] = j[7];
		t.c_cc[4] = j[8];
		t.c_cc[5] = j[9];
		t.c_cc[6] = j[10];
		t.c_cc[7] = j[11];
		if (ioctl(0, TCSETA, &t) == -1)
			panic("can't set terminal parameters");
	} else
		panic("bad argument - expected 12 hex values");
}

/*
 * dump_hex()
 *
 * Dump hex contents of termio struct "t" to stdout.
 */
void
dump_hex()
{
	printf("%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x\n",
	  t.c_iflag, t.c_oflag, t.c_cflag, t.c_lflag,
	  t.c_cc[0], t.c_cc[1], t.c_cc[2], t.c_cc[3],
	  t.c_cc[4], t.c_cc[5], t.c_cc[6], t.c_cc[7]);
}

/*
 * panic()
 *
 * Display fatal error message and quit.
 */
void
panic(s)
char * s;
{
	fprintf(stderr, "stty: %s\n", s);
	exit(1);
}

/*
 * baudstr()
 *
 * Append to lbuf the ASCII text for a given baud rate in f.
 */
void
baudstr(f)
int f;
{
	switch(f&CBAUD) {
	case B0:  strcat(lbuf, "0\t");  break;
	case B50:  strcat(lbuf, "50\t");  break;
	case B75:  strcat(lbuf, "75\t");  break;
	case B110:  strcat(lbuf, "110\t");  break;
	case B134:  strcat(lbuf, "134\t");  break;
	case B150:  strcat(lbuf, "150\t");  break;
	case B200:  strcat(lbuf, "200\t");  break;
	case B300:  strcat(lbuf, "300\t");  break;
	case B600:  strcat(lbuf, "600\t");  break;
	case B1200:  strcat(lbuf, "1200\t");  break;
	case B1800:  strcat(lbuf, "1800\t");  break;
	case B2400:  strcat(lbuf, "2400\t");  break;
	case B4800:  strcat(lbuf, "4800\t");  break;
	case B9600:  strcat(lbuf, "9600\t");  break;
	case B19200:  strcat(lbuf, "19200\t");  break;
	case B38400:  strcat(lbuf, "38400\t");  break;
	}
}

int
simple(opt, list, flagp)
char		* opt;
FLAG_OPT	* list;
short		* flagp;
{
	int	ret = 0;
	int	i;

	for (i = 0; list[i].optname[0] && !ret; i++) {
		if (strcmp(opt, list[i].optname) == 0) {
			*flagp &= ~(list[i].mask);
			*flagp |= list[i].val;
			ret = 1;
		}
	}
	return ret;
}

int
combo(opt)
char * opt;
{
	int	ret = 1;

	if (strcmp(opt, "sane") == 0) {
		simple("icrnl", c_list, &t.c_iflag);
		simple("opost", o_list, &t.c_oflag);
		simple("onlcr", o_list, &t.c_oflag);
		simple("isig", l_list, &t.c_lflag);
		simple("icanon", l_list, &t.c_lflag);
		simple("-xcase", l_list, &t.c_lflag);
		simple("echo", l_list, &t.c_lflag);
		simple("echoe", l_list, &t.c_lflag);
		simple("echok", l_list, &t.c_lflag);
		t.c_cc[VERASE] = '\b';
		goto combo_done;
	}
	if (strcmp(opt, "evenp") == 0) {
		simple("parenb", c_list, &t.c_cflag);
		simple("-parodd", c_list, &t.c_cflag);
		simple("cs7", c_list, &t.c_cflag);
		goto combo_done;
	}
	if (strcmp(opt, "parity") == 0) {
		combo("evenp");
		goto combo_done;
	}
	if (strcmp(opt, "oddp") == 0) {
		simple("parenb", c_list, &t.c_cflag);
		simple("parodd", c_list, &t.c_cflag);
		simple("cs7", c_list, &t.c_cflag);
		goto combo_done;
	}
	if (strcmp(opt, "-parity") == 0) {
		simple("-parenb", c_list, &t.c_cflag);
		simple("cs8", c_list, &t.c_cflag);
		goto combo_done;
	}
	if (strcmp(opt, "-evenp") == 0) {
		simple("-parenb", c_list, &t.c_cflag);
		simple("cs8", c_list, &t.c_cflag);
		goto combo_done;
	}
	if (strcmp(opt, "-oddp") == 0) {
		simple("-parenb", c_list, &t.c_cflag);
		simple("cs8", c_list, &t.c_cflag);
		goto combo_done;
	}
	if (strcmp(opt, "raw") == 0) {
		t.c_iflag = 0;
		t.c_oflag &= ~OPOST;
		t.c_cflag &= ~(PARODD|PARENB);
		t.c_cflag |= (CS8|CREAD);
		t.c_lflag &= ~(ECHONL|ISIG|ICANON|XCASE);
		goto combo_done;
	}
	if (strcmp(opt, "-raw") == 0) {
		t.c_iflag |= BRKINT|IGNPAR|ISTRIP|ICRNL|IXON;
		t.c_oflag |= OPOST|ONLCR;
		t.c_cflag |= (PARENB|CS7|CREAD);
		t.c_iflag |= ISIG|ICANON;
		goto combo_done;
	}
	if (strcmp(opt, "cooked") == 0) {
		combo("-raw");
		goto combo_done;
	}
	if (strcmp(opt, "nl") == 0) {
		simple("-icrnl", i_list, &t.c_iflag);
		simple("-onlcr", o_list, &t.c_oflag);
		goto combo_done;
	}
	if (strcmp(opt, "-nl") == 0) {
		simple("icrnl", i_list, &t.c_iflag);
		simple("-inlcr", i_list, &t.c_iflag);
		simple("-igncr", i_list, &t.c_iflag);
		simple("onlcr", o_list, &t.c_oflag);
		simple("-ocrnl", o_list, &t.c_oflag);
		simple("-onlret", o_list, &t.c_oflag);
		goto combo_done;
	}
	if (strcmp(opt, "lcase") == 0) {
		t.c_lflag |= XCASE;
		t.c_iflag |= IUCLC;
		t.c_oflag |= OLCUC;
		goto combo_done;
	}
	if (strcmp(opt, "-lcase") == 0) {
		t.c_lflag &= ~XCASE;
		t.c_iflag &= ~IUCLC;
		t.c_oflag &= ~OLCUC;
		goto combo_done;
	}
	if (strcmp(opt, "LCASE") == 0) {
		combo("lcase");
		goto combo_done;
	}
	if (strcmp(opt, "-LCASE") == 0) {
		combo("-lcase");
		goto combo_done;
	}
	if (strcmp(opt, "tabs") == 0) {
		simple("tab0", o_list, &t.c_oflag);
		goto combo_done;
	}
	if (strcmp(opt, "-tabs") == 0) {
		simple("tab3", o_list, &t.c_oflag);
		goto combo_done;
	}
	if (strcmp(opt, "ek") == 0) {
		t.c_cc[VERASE] = '#';
		t.c_cc[VKILL] = '@';
		goto combo_done;
	}

	ret = 0;
combo_done:
	return ret;
}

/*
 * See if current argument specifies a character option.
 * If so, return the index into the c_cc array.
 * Else, return -1.
 */
int
c_opt(opt)
char * opt;
{
	int	ret = -1;
	int	i;
	static struct {
		char	name[NAMELN];
		int	index;
	} vlist[] = {
		{ "intr",	0},
		{ "quit",	1},
		{ "erase",	2},
		{ "kill",	3},
		{ "eof",	4},
		{ "eol",	5},
	};

	for (i = 0; i < sizeof(vlist)/sizeof(vlist[0]); i++) {
		if (strcmp(opt, vlist[i].name) == 0) {
			ret = vlist[i].index;
			break;
		}
	}
	return ret;
}

/*
 * See if current argument specifies a numeric option.
 * If so, return the index into the c_cc array.
 * Else, return -1.
 */
int
num_opt(opt)
char * opt;
{
	int	ret = -1;
	int	i;
	static struct {
		char	name[NAMELN];
		int	index;
	} vlist[] = {
		{ "min",	4},
		{ "time",	5}
	};

	for (i = 0; i < sizeof(vlist)/sizeof(vlist[0]); i++) {
		if (strcmp(opt, vlist[i].name) == 0) {
			ret = vlist[i].index;
			break;
		}
	}
	return ret;
}

/*
 * set_ch()
 *
 * Given index into c_cc[] array in termio and an argument string,
 * decode the string and store into specified array element.
 *
 * Return 1 if argument passed can be decoded, else return 0.
 * Valid argument strings are "0xnn", "c", "^c", "^?" denoting DEL,
 * and "^-" denoting NUL, which means unused.
 */
int
set_ch(want_ch, arg)
int	want_ch;
char	* arg;
{
	int	ret = 0;
	unsigned int n;

	if (strlen(arg) == 1) {
		n = arg[0];
		ret = 1;
		goto set_ch_end;
	}
	if (strlen(arg) == 2 && arg[0] == '^') {
		if (arg[1] == '?')
			n = DEL;
		else if (arg[1] == '-')
			n = NUL;
		else
			n = arg[1] & ~0x60;
		ret = 1;
		goto set_ch_end;
	}
	if (strncmp(arg, "0x", 2) == 0  && sscanf(arg+2, "%x", &n) == 1) {
		ret = 1;
		goto set_ch_end;
	}
set_ch_end:
	if (ret)
		t.c_cc[want_ch] = n;
	return ret;
}

/*
 * set_num()
 *
 * Given index into c_cc[] array in termio and an argument string,
 * decode the string and store into specified array element.
 *
 * Return 1 if argument passed can be decoded, else return 0.
 * Valid argument strings are "nnn", a decimal value.
 */
int
set_num(want_num, arg)
int	want_num;
char	* arg;
{
	int	ret = 0;
	unsigned int n;

	if (sscanf(arg, "%d", &n) == 1) {
		ret = 1;
		t.c_cc[want_num] = n;
	}
	return ret;
}

#if 0
/*
 * set_speed()
 *
 * Given baud rate, set speed in termio struct.
 * Return 1 if valid baud rate, else return 0.
 */
int
set_speed(rate)
int rate;
{
	int	ret = 1;
	int	x;

	switch (rate) {
	case 0:  x = B0;  break;
	case 50:  x = B50;  break;
	case 75:  x = B75;  break;
	case 110:  x = B110;  break;
	case 134:  x = B134;  break;
	case 150:  x = B150;  break;
	case 200:  x = B200;  break;
	case 300:  x = B300;  break;
	case 600:  x = B600;  break;
	case 1200:  x = B1200;  break;
	case 1800:  x = B1800;  break;
	case 2400:  x = B2400;  break;
	case 4800:  x = B4800;  break;
	case 9600:  x = B9600;  break;
	case 19200:  x = B19200;  break;
	case 38400:  x = B38400;  break;
	default:  ret = 0;
	}
	if (ret) {
		t.c_cflag &= ~CBAUD;
		t.c_cflag |= x;
	}
	return ret;
}
#endif
