/*
 * Tr translates characters from the standard input to the standard output.
 * It usage is as follows:
 *	tr [-c] [-d] [-s] [string1] [string2]
 * In the two strings, in addition to normal characters, one can include
 *	`x-y'	all characters between `x' and `y'
 *	\r	ascii carriage return
 *	\n	ascii line feed
 *	\b	ascii back space
 *	\t	ascii horizontal tab
 *	\f	ascii form feed
 *	\d, \dd or \ddd
 *		character with ascii code d (or dd or ddd) in octal
 *	\x	`x' for any `x' not listed above
 * In the absence of any options, tr simply converts any character appearing
 * in string1 to the character in the same position in string2.  If string2
 * is shorter then string1, then it is extended by replicating the last
 * character.
 * The `-c' option simply compliments string1 with respect to the character
 * set.  The resulting set of characters is used in increasing order.
 * The `-d' option causes characters in string1 to be deleted rather than
 * being translated.
 * The `-s' option causes characters in string2 (or string1 if there is no
 * string2 and the -d option is not specified) to be compressed on output.
 * This means that multiple occurances of the same character are compressed
 * to one occurance.
 */
#include <stdio.h>
#include <sys/mdata.h>


#define	bool	char			/* boolean type */
#define	not	!			/* logical negation operator */
#define	and	&&			/* logical conjunction */
#define	or	||			/* logical disjunction */
#define	TRUE	(0 == 0)
#define	FALSE	(not TRUE)
#define	EOS	'\0'			/* end-of-string char */
#define	CSIZE	(1 << NBCHAR)		/* character set size */
#define	MAXDIG	((NBCHAR+2) / 3)	/* max digits in character constant */


/*
 * List is used to hold a character list from which characters are begin
 * extrated.  It allows expansion of character ranges, backslash-protected
 * characters and character ranges.
 */
typedef struct	list {
	unsigned char	*l_next,	/* next character from string */
			*l_start;	/* start of string */
	bool		l_inr;		/* iff we are in a-b range */
	int		l_rnext,	/* next to give if inrange */
			l_rhi;		/* maximum to give if inrange */
}	list;


bool		cflag,			/* compliment string 1 */
		dflag,			/* delete chars in string 1 */
		sflag,			/* remove duplicates in string 2 */
		delete[CSIZE],		/* set of chars to delete on input */
		squeeze[CSIZE];		/* set of chars to squeeze on output */
unsigned char	map[CSIZE];		/* map to apply to characters */

int		nextchar();		/* get next char of list */
void		die(),			/* print error message and exit */
		usage(),		/* print usage message and exit */
		scan(),			/* actually do copy */
		startlist(),		/* start list from string */
		makeset(),		/* make set of chars in list */
		maketrans();		/* make transformation table */
unsigned char	*cstr();		/* compliment character list */
char		*alloc(),		/* unfailable malloc */
		*ralloc();		/* unfailable realloc */


int
main(argc, argv)
int		argc;
register char	*argv[];
{
	register unsigned char	*str1,
				*str2;

	for (str1=*++argv; str1 != NULL  &&  *str1 == '-'; str1=*++argv)
		while (*++str1 != EOS)
			switch (*str1) {
			case 'c':
				cflag = TRUE;
				break;
			case 'd':
				dflag = TRUE;
				break;
			case 's':
				sflag = TRUE;
				break;
			default:
				usage();
			}
	if (str1 == NULL)
		usage();
	if (cflag)
		str1 = cstr(str1);
	str2 = *++argv;
	if (str2 != NULL  &&  *++argv != NULL)
		usage();
	if (dflag)
		if (sflag) {		/* -d and -s */
			if (str2 == NULL)
				usage();
			makeset(str1, delete);
			makeset(str2, squeeze);
			maketrans(str1, str2, map);
		} else {		/* -d and no -s */
			if (str2 != NULL)
				usage();
			makeset(str1, delete);
			maketrans("", "", map);
		}
	else
		if (sflag)
			if (str2 == NULL) {	/* -s, no -d and one string */
				maketrans("", "", map);
				makeset(str1, squeeze);
			} else {		/* -s, no -d and two strings */
				maketrans(str1, str2, map);
				makeset(str2, squeeze);
			}
		else {			/* no -s, no -d */
			if (str2 == NULL)
				usage();
			maketrans(str1, str2, map);
		}
	scan();
	return (0);
}


/*
 * Die simply sends an error message to stderr and exits.
 */
void
die(str)
char	*str;
{
	fprintf(stderr, "%r\n", &str);
	exit(1);
}


/*
 * Usage gives a usage error message and exits.
 */
void
usage()
{
	die("usage: tr [-cds] [from_list [to_list]]");
}


/*
 * Scan does the acutal copying.  It deletes any input characters in
 * the set `delete'.  It then transforms input characters to output
 * characters useing the mapping `map'.  Finally, it changes multiple
 * occurances of output characters in the set `squeeze' to single
 * occurances.
 */
void
scan()
{
	register int	ch,
			lastch;

	lastch = EOF;
	while ((ch=getchar()) != EOF) {
		if (delete[ch])
			continue;
		ch = map[ch];
		if (ch == lastch  &&  squeeze[ch])
			continue;
		putchar(ch);
		lastch = ch;
	}
}


/*
 * Makeset sets the array of bools `set' (indexed by chars) such that
 * the i'th entry is TRUE iff character i appears in the character
 * list `str'.
 */
void
makeset(str, set)
unsigned char	*str;
register bool	*set;
{
	register bool	*rp;
	register int	n;
	list		cl;

	for (rp=set, n=CSIZE; --n >= 0;)
		*rp++ = FALSE;
	startlist(&cl, str);
	while ((n=nextchar(&cl)) != EOF)
		set[n] = TRUE;
	return;
}


/*
 * Maketrans sets the array of chars `map' (indexed by chars) to
 * the mapping which converts all characters in the character list
 * `str1' to the corresponding character in the character list `str2'.
 * If `str2' is short, the last character is duplicated.
 */
void
maketrans(str1, str2, map)
unsigned char	*str1,
		*str2,
		*map;
{
	register unsigned char	*rp;
	register int		n,
				m;
	list			l1,
				l2;
	bool			extra;

	startlist(&l1, str1);
	startlist(&l2, str2);
	for (rp=map, n=0; n < CSIZE; ++n)
		*rp++ = n;
	rp = map;
	n = nextchar(&l2);
	if (n == EOF) {
		if (nextchar(&l1) != EOF)
			die("Second string empty");
		return;
	}
	for (extra=FALSE; (m=nextchar(&l1)) != EOF;) {
		rp[m] = n;
		if (not extra) {
			m = nextchar(&l2);
			if (m != EOF)
				n = m;
			else
				extra = TRUE;
		}
	}
	if (not extra  &&  nextchar(&l2) != EOF)
		die("Extra characters in second string");
	return;
}


/*
 * Cstr returns a string which is the compliment of the string `str'.
 */
unsigned char	*
cstr(str)
unsigned char	*str;
{
	register unsigned char	*rp;
	register bool		*sp;
	register int		n;
	bool			set[CSIZE];
	unsigned char		*res;

	makeset(str, set);
	res = rp = (unsigned char *)alloc(3 + 2 + CSIZE + 1);
	sp = set;
	if (not *sp++) {		/* handle EOS specially */
		*rp++ = '\\';
		*rp++ = '0';
		*rp++ = '0';
		*rp++ = '0';
	}
	for (n=0; ++n < CSIZE;)
		if (not *sp++) {
			if (n == '\\'  ||  n == '-')
				*rp++ = '\\';
			*rp++ = n;
		}
	*rp++ = EOS;
	return ((unsigned char *)ralloc(res, rp - res));
}


/*
 * Startlist sets the list pointed to by `lp' to the string `str'.
 */
void
startlist(lp, str)
register list	*lp;
unsigned char	*str;
{
	lp->l_next = lp->l_start = str;
	lp->l_inr = FALSE;
}


/*
 * Nextchar returns the next character from the list pointed to by
 * `lp'.  This includes backslash protection and character ranges.
 * When there are no more characters, it returns EOF.
 */
int
nextchar(lp)
register list	*lp;
{
	register int	res;
	int		getprot();

	if (lp->l_inr) {
		res = lp->l_rnext;
		lp->l_inr = ++lp->l_rnext <= lp->l_rhi;
		return (res);
	}
	res = *lp->l_next++;
	if (res == EOS)
		return (EOF);
	if (res == '\\')
		res = getprot(lp);
	if (*lp->l_next != '-')
		return (res);
	++lp->l_next;
	lp->l_rnext = res;
	res = *lp->l_next++;
	if (res == EOS)
		die("Unexpected end of character list in `%s'", lp->l_start);
	if (res == '\\')
		res = getprot(lp);
	if (lp->l_rnext > res)
		die("Bad character range in `%s'", lp->l_start);
	lp->l_rhi = res;
	res = lp->l_rnext;
	lp->l_inr = ++lp->l_rnext <= lp->l_rhi;
	return (res);
}


/*
 * Getprot is used to get a backslash protected character from the
 * character list pointed to by `lp'.
 */
int
getprot(lp)
register list	*lp;
{
	register unsigned char	ch;
	register unsigned	n;
	int			m;

	ch = *lp->l_next++;
	switch (ch) {
	case EOS:
		die("Unexpected end of character list in `%s'", lp->l_start);
	case 'r':
		return ('\r');
	case 'b':
		return ('\b');
	case 't':
		return ('\t');
	case 'n':
		return ('\n');
	case 'f':
		return ('\f');
	default:
		break;
	}
	if ('0' > ch  ||  ch > '7')
		return (ch);
	n = ch - '0';
	ch = *lp->l_next;
	for (m=MAXDIG; --m > 0  &&  '0' <= ch  &&  ch <= '7'; ch=*++lp->l_next)
		n = n*8 + ch-'0';
	if (n >= CSIZE)
		die("Illegal character constant in `%s'", lp->l_start);
	return (n);
}


/*
 * Alloc is simply an interface to malloc which does not return if
 * there is no space.
 */
char	*
alloc(len)
unsigned	len;
{
	register char	*res;
	extern char	*malloc();

	res = malloc(len);
	if (res == NULL)
		die("Out of space");
	return (res);
}


/*
 * Ralloc is simply an interface to realloc which does not return if
 * there is no space.
 */
char	*
ralloc(cp, len)
char		*cp;
unsigned	len;
{
	register char	*res;
	extern char *realloc();

	res = realloc(cp, len);
	if (res == NULL)
		die("Out of space");
	return (res);
}
