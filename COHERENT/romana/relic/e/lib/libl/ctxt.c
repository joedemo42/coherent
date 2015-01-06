#include "lexlib.h"
char	illctx[] = "illegal argument to yyswitch";
yyctxt(new, size)
{
	static old = 0;
	register tmp;

	if (new < 0 || new >= size)
		error(illctx);
	ll_ctxt = yy_lextab + yy_ctxtab[new];
	tmp = old;
	old = new;
	return (tmp);
}
