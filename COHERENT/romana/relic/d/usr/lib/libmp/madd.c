#include "mprec.h"
#include <assert.h>


/*
 *	Madd adds the mints pointed to by "a" and "b" and puts the result
 *	into the mint pointed to by "c".  Note that it assumes that the
 *	contents of the mint pointed to by "c" are valid.  Also note that
 *	there is no assumption made on the distinctness of "a", "b" and "c".
 */
void
madd(a, b, c)
mint *a, *b, *c;
{
	register char *rp, *ap, *bp;
	char *limit;
	char asign, bsign;
	mint *temp;
	mint res;

	/* make a the bigger of the two */
	if (a->len < b->len) {
		temp = a;
		a = b;
		b = temp;
	}

	/* allocate new value */
	rp = (char *)mpalc(a->len + 1);
	res.val = rp;
	res.len = a->len + 1;

	ap = a->val;
	bp = b->val;

	/* add a and b over range of both */
	limit = bp + b->len;
	while (bp < limit)
		*rp++ = *ap++ + *bp++;

	/* add sign extended b to rest of a */
	bsign = (ispos(b) ? 00 : NEFL);
	limit = a->val + a->len;
	while (ap < limit)
		*rp++ = *ap++ + bsign;

	/* add sign of a and sign of b and put sum in lead digit of res */
	asign = (ispos(a) ? 00 : NEFL);
	*rp = asign + bsign;
	assert(rp == res.val + res.len - 1);

	norm(&res);
	mpfree(c->val);
	*c = res;
}


/*
 *	Msub subtracts the mint pointed to by "b" from that pointed at
 *	by "a" and stores the result in that pointed to by "c".  Note
 *	that there is no assumption made on the distinctness of "a", "b"
 *	and "c".
 */

void
msub(a, b, c)
mint *a, *b, *c;
{
	mint temp;

	minit(&temp);
	mneg(b, &temp);
	madd(a, &temp, c);
	mpfree(temp.val);
}
