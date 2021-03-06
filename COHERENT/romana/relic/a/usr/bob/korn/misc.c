/*
 * Miscellaneous functions
 */

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <access.h>
#include "sh.h"
#include "expand.h"
#include "table.h"

char ctypes [UCHAR_MAX];	/* type bits for unsigned char */

/*
 * Fast character classes
 */
void
setctypes(s, t)
	register Const char *s;
	register int t;
{
	register int i;

	if ((t&C_IFS)) {
		for (i = 0; i < UCHAR_MAX; i++)
			ctypes[i] &=~ C_IFS;
		ctypes[0] |= C_IFS; /* include \0 in C_IFS */
	}
	ctypes[(unsigned char) *s++] |= t;	/* allow leading \0 in string */
	while (*s != 0)
		ctypes[(unsigned char) *s++] |= t;
}

void
initctypes()
{
	register int c;

	for (c = 'a'; c <= 'z'; c++)
		ctypes[c] |= C_ALPHA;
	for (c = 'A'; c <= 'Z'; c++)
		ctypes[c] |= C_ALPHA;
	ctypes['_'] |= C_ALPHA;
	setctypes("0123456789", C_DIGIT);
	setctypes("\0 \t\n|&;<>()", C_LEX1);
	setctypes("*@#!$-?", C_VAR1);
	setctypes("=-+?#%", C_SUBOP);
}

/* convert unsigned long to base N string */

char *
ulton(n, base)
	register unsigned long n;
	int base;
{
	register char *p;
	static char buf [20];

	p = &buf[sizeof(buf)];
	*--p = '\0';
	do {
		*--p = "0123456789ABCDEF"[n%base];
		n /= base;
	} while (n != 0);
	return p;
}

char *
strsave(s, ap)
	register char *s;
	Area *ap;
{
	return strcpy((char*) alloc((size_t)strlen(s)+1, ap), s);
}

static struct option {
	char *name;
	int flag;
} options[] = {
	{"allexport",	FEXPORT},
	{"bgnice",	FBGNICE},
#if EDIT
	{"emacs",	FEMACS},
#endif
	{"errexit",	FERREXIT},
	{"hashall",	FHASHALL},
	{"ignoreeof",	FIGNEOF},
	{"interactive",	FTALKING},
	{"keyword",	FKEYWORD},
	{"markdirs",	FMARKDIRS},
	{"monitor",	FMONITOR},
	{"noexec",	FNOEXEC},
	{"noglob",	FNOGLOB},
	{"nounset",	FNOUNSET},
	{"privileged",	FPRIVILEGED},
	{"stdin",	FSTDIN},
	{"trackall",	FHASHALL},
	{"verbose",	FVERBOSE},
	{"xtrace",	FXTRACE},
	{NULL,		0}
};	

/*
 * translate -o option into F* constant
 */
int
option(n)
	Const char *n;
{
	register struct option *op;

	for (op = options; op->name != NULL; op++)
		if (strcmp(op->name, n) == 0)
			return op->flag;
	return 0;
}

char *
getoptions()
{
	register int c;
	char m [26+1];
	register char *cp = m;

	for (c = 'a'; c <= 'z'; c++)
		if (flag[FLAG(c)])
			*cp++ = (char) c;
	*cp = 0;
	return strsave(m, ATEMP);
}

void
printoptions()
{
	register struct option *op;

	for (op = options; op->name != NULL; op++)
		if (flag[op->flag])
			shellf("%s ", op->name);
	shellf("\n");
}
	
/* atoi with error detection */

getn(as)
	char *as;
{
	register char *s;
	register int n;

	s = as;
	if (*s == '-')
		s++;
	for (n = 0; digit(*s); s++)
		n = (n*10) + (*s-'0');
	if (*s)
		errorf("%s: bad number\n", as);
	return (*as == '-') ? -n : n;
}

/*
 * stripped down strerror for kill and exec
 */
char *
strerror(i)
	int i;
{
	switch (i) {
	  case EINVAL:
		return "Invalid argument";
	  case EACCES:
		return "Permission denied";
	  case ESRCH:
		return "No such process";
	  case EPERM:
		return "Not owner";
	  case ENOENT:
		return "No such file or directory";
	  case ENOTDIR:
		return "Not a directory";
	  case ENOEXEC:
		return "Exec format error";
	  case ENOMEM:
		return "Not enough memory";
	  case E2BIG:
		return "Argument list too long or program too large";
	  default:
		return "Unknown system error";
	}
}

xpexpand(xp)
	register XPtrV *xp;
{
	int n = XPsize(*xp);
	Void **vp;
	register Void **dvp, **svp;

	vp = alloc(sizeofN(Void*, n*2), ATEMP);
	for (svp = xp->beg, dvp = vp; svp < xp->cur; )
		*dvp++ = *svp++;
	afree((Void*) xp->beg, ATEMP);
	xp->beg = vp;
	xp->cur = vp + n;
	xp->end = vp + n*2;
}

/* -------- gmatch.c -------- */

/*
 * int gmatch(string, pattern)
 * char *string, *pattern;
 *
 * Match a pattern as in sh(1).
 * pattern character are prefixed with MAGIC by expand.
 */

#define	NOT	'!'	/* might use ^ */

static	char	*cclass ARGS((char *, int c));

int
gmatch(s, p)
	register unsigned char *s, *p;
{
	register int sc, pc;

	if (s == NULL || p == NULL)
		return 0;
	while ((pc = *p++) != 0) {
		sc = *s++;
		if (pc ==  MAGIC) {
			switch (*p++) {
			  case '[':
				if ((p = cclass(p, sc)) == NULL)
					return (0);
				break;

			  case '?':
				if (sc == 0)
					return (0);
				break;

			  case '*':
				s--;
				do {
					if (*p == '\0' || gmatch(s, p)) {
						return (1);
					}
				} while (*s++ != '\0');
				return (0);

			}
		} else
			if (sc != pc)
				return 0;
	}
	return (*s == 0);
}

static char *
cclass(p, sub)
	register char *p;
	register int sub;
{
	register int c, d, not, found = 0;

	if ((not = *p == NOT))
		p++;
	do {
		if (*p == '\0')
			return NULL;
		c = *p;
		if (p[1] == '-' && p[2] != ']') {
			d = p[2];
			p++;
		} else
			d = c;
		if (c == sub || c <= sub && sub <= d)
			found = 1;
	} while (*++p != ']');

	return (found != not) ? p+1 : NULL;
}

/* -------- qsort.c -------- */

/*
 * quick sort of array of generic pointers to objects.
 */

void
qsortp(base, n, f)
	Void **base;		/* base address */
	size_t n;		/* elements */
	int (*f)();		/* compare function */
{
	qsort1(base, base + n, f);
}

#define	swap2(a, b)	{\
	register Void *t; t = *(a); *(a) = *(b); *(b) = t;\
}
#define	swap3(a, b, c)	{\
	register Void *t; t = *(a); *(a) = *(c); *(c) = *(b); *(b) = t;\
}

qsort1(base, lim, f)
	Void **base, **lim;
	int (*f)();
{
	register Void **i, **j;
	register Void **lptr, **hptr;
	size_t n;
	int c;

  top:
	n = (lim - base) / 2;
	if (n == 0)
		return;
	hptr = lptr = base+n;
	i = base;
	j = lim - 1;

	for (;;) {
		if (i < lptr) {
			if ((c = (*f)(*i, *lptr)) == 0) {
				lptr --;
				swap2(i, lptr);
				continue;
			}
			if (c < 0) {
				i += 1;
				continue;
			}
		}

	  begin:
		if (j > hptr) {
			if ((c = (*f)(*hptr, *j)) == 0) {
				hptr ++;
				swap2(hptr, j);
				goto begin;
			}
			if (c > 0) {
				if (i == lptr) {
					hptr ++;
					swap3(i, hptr, j);
					i = lptr += 1;
					goto begin;
				}
				swap2(i, j);
				j -= 1;
				i += 1;
				continue;
			}
			j -= 1;
			goto begin;
		}

		if (i == lptr) {
			if (lptr-base >= lim-hptr) {
				qsort1(hptr+1, lim, f);
				lim = lptr;
			} else {
				qsort1(base, lptr, f);
				base = hptr+1;
			}
			goto top;
		}

		lptr -= 1;
		swap3(j, lptr, i);
		j = hptr -= 1;
	}
}

int
xstrcmp(p1, p2)
	Void *p1, *p2;
{
	return (strcmp((char *)p1, (char *)p2));
}

#if COHERENT
/*
 * check to see if new mail has arrived in the mailbox specified by $MAIL.
 * if so, issue a friendly message.
 */
void
mail()
{
	char	*mp;				  /* MAIL */
	char	*mcp;				  /* MAILCHECK */
	static	time_t	interval = MAILINTERVAL;  /* seconds between checks */
	static	time_t	lasttime;
	time_t		currtime, time();
	static	long	mailsize = -1;
	struct	stat	sbuf;

	if (((mp = strval(global("MAIL"))) == NULL) || *mp == '\0')
		return;
	if ((mcp = strval(global("MAILCHECK"))) != NULL && *mcp != '\0')
		if ((interval = (time_t)getn(mcp)) < 0)
			interval = MAILINTERVAL;

	currtime = time((time_t *)0);
	if ((currtime - lasttime) < interval)
		return;
	lasttime = currtime;

	if (stat(mp, &sbuf) < 0) {
		mailsize = 0;
	} else {
		if (sbuf.st_size != 0 && sbuf.st_size > mailsize) {
			if (mailsize == -1)
				printf("You have mail.\n");
			else
				printf("You have new mail.\n");
		}
		mailsize = sbuf.st_size;
	}
}

/*
 * modified version of access() which makes sure that anything
 * executable is really a file. This fixes a bug (misfeature) where
 * directories found along $PATH are treated as unexecutable executables.
 */
eaccess(name, mode)
char	*name;
int	mode;
{
	struct	stat	sbuf;
	int 	ret;

	if ((mode & AEXEC) == 0)
		return access(name, mode);
	if ((ret = access(name, mode)) != 0)
		return ret;
	if (stat(name, &sbuf) < 0)
		return 1;
	return ((sbuf.st_mode & S_IFMT) != S_IFREG);
}
#endif /* COHERENT */
