/*
 * Expandable strings
 *
 * Usage
 *	XString xs;
 *	char *xp;
 *
 *	Xinit(xs, xp, 128);
 *	while ((c = generate()) {
 *		Xcheck(xs, xp);
 *		Xput(xs, xp, c);
 *	}
 *	return Xclose(xs, xp);
 */

typedef struct XString {
	char   *end, *beg;	/* end, begin of string */
#if 1
	char   *oth, *old;	/* togo, adjust */
#endif
	size_t	len;		/* length */
} XString;

typedef char * XStringP;

/* initialize expandable string */
#define	Xinit(xs, xp, length) { \
			(xs).len = length; \
			(xs).beg = alloc((xs).len + 4, ATEMP); \
			(xs).end = (xs).beg + (xs).len; \
			xp = (xs).beg; \
		}

/* stuff char into string */
#define	Xput(xs, xp, c)	*xp++ = (c)

/* check for overflow, expand string */
#define	Xcheck(xs, xp) if (xp >= (xs).end) { \
			char *old_beg = (xs).beg; \
			(xs).len += (xs).len; \
			(xs).beg = alloc((xs).len + 4, ATEMP); \
			(xs).end = (xs).beg + (xs).len; \
			memcpy((xs).beg, old_beg, (size_t)(xp - old_beg)); \
			afree((Void*) old_beg, ATEMP); \
			xp = (xs).beg + (xp - old_beg); \
		}

/* free string */
#define	Xfree(xs, xp)	afree((Void*) (xs).beg, ATEMP)

/* close, return string */
#define	Xclose(xs, xp)	(char*) aresize((Void*)(xs).beg, \
					(size_t)(xp - (xs).beg), ATEMP)
/* begin of string */
#define	Xstring(xs, xp)	((xs).beg)

#define	Xsavepos(xs, xp) (xp - (xs).beg)
#define	Xrestpos(xs, xp, n) ((xs).beg + (n))

/*
 * expandable vector of generic pointers
 */

typedef struct XPtrV {
	Void  **cur;		/* next avail ptr */
	Void  **beg, **end;	/* begin, end of vector */
} XPtrV;

#define	XPinit(x, n) { \
			register Void **vp; \
			vp = (Void**) alloc(sizeofN(Void*, n), ATEMP); \
			(x).cur = (x).beg = vp; \
			(x).end = vp + n; \
			}

#define	XPput(x, p) { \
			if ((x).cur >= (x).end) \
				xpexpand(&(x)); \
			*(x).cur++ = (p); \
			}

#define	XPptrv(x)	((x).beg)
#define	XPsize(x)	((x).cur - (x).beg)

#define	XPclose(x)	(Void**) aresize((Void*)(x).beg, \
					 sizeofN(Void*, XPsize(x)), ATEMP)

#define	XPfree(x)	afree((Void*) (x).beg, ATEMP)

