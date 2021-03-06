/*
 * This file contains the mini
 * constant expression folder used by the
 * parser. More of this is done in the
 * code generator. This one exists only to
 * allow constant expressions in case labels
 * and in array bounds.
 */
#ifdef   vax
#include "INC$LIB:cc0.h"
#else
#include "cc0.h"
#endif

/*
 * Try to fold.
 * Return pointer to a constant tree
 * node, or NULL if folding is impossible.
 */
TREE *
fold0(op, lp, rp)
TREE *lp, *rp;
{
	register TREE *tp;
	register lrf, zrf, sop;
	lval_t c1, c2, grablval();

	lrf = zrf = 0;
	sop = lp->t_op;
	if (sop == LCON)
		++lrf;
	else if (sop == ZCON)
		++zrf;
	else if (sop != ICON || lp->t_dp != NULL)
		return NULL;
	c1 = grablval(lp);
	if (op == QUEST) {
		if (rp->t_op != COLON)
			return (NULL);
		return (c1 ? rp->t_lp : rp->t_rp);
	}
	if (rp != NULL) {
		sop = rp->t_op;
		if (sop == LCON)
			++lrf;
		else if (sop == ZCON)
			++zrf;
		else if (sop != ICON || rp->t_dp != NULL)
			return NULL;
		c2 = grablval(rp);
	}
	switch (op) {

	case COM:
		c1 = ~c1;
		break;

	case NOT:
		c1 = !c1;
		break;

	case NEG:
		c1 = -c1;
		break;

	case ADD:
		c1 += c2;
		break;

	case SUB:
		c1 -= c2;
		break;

	case MUL:
		c1 *= c2;
		break;

	case DIV:
		if (c2 == 0) {
			cwarn("divide by zero");
			return (NULL);
		}
		c1 /= c2;
		break;

	case REM:
		if (c2 == 0) {
			cwarn("zero modulus");
			return (NULL);
		}
		c1 %= c2;
		break;

	case AND:
		c1 &= c2;
		break;

	case OR:
		c1 |= c2;
		break;

	case XOR:
		c1 ^= c2;
		break;

	case SHL:
		c1 <<= c2;
		break;

	case SHR:
		c1 >>= c2;
		break;

	case EQ:
		c1 = c1==c2;
		break;

	case NE:
		c1 = c1!=c2;
		break;

	case LT:
		c1 = c1<c2;
		break;

	case LE:
		c1 = c1<=c2;
		break;

	case GT:
		c1 = c1>c2;
		break;

	case GE:
		c1 = c1>=c2;
		break;

	case ANDAND:
		c1 = (c1 && c2);
		lrf = 0;
		break;

	case OROR:
		c1 = (c1 || c2);
		lrf = 0;
		break;

	default:
		return (NULL);
	}
	tp = talloc();
	if (lrf) {
		tp->t_op   = LCON;
		tp->t_type = T_LONG;
		tp->t_lval = c1;
		return (tp);
	}
	if (zrf) {
		tp->t_op  = ZCON;
		tp->t_type = T_UINT;
		tp->t_lval = c1;
		return (tp);
	}
	tp->t_op   = ICON;
	tp->t_type = T_INT;
	tp->t_ival = c1;
	return (tp);
}

/*
 * Grab the value of a
 * constant (ICON or LCON) node.
 * Look at the type of the node if ICON
 * and 0 extend if necessary.
 */
lval_t
grablval(tp)
register TREE *tp;
{
	register op, t;

	if ((op=tp->t_op) == ICON) {
		if (tp->t_dp != NULL
		|| (t=tp->t_type) == T_UCHAR
		||  t == T_USHORT
		||  t == T_UINT)
			return ((unsigned) tp->t_ival);
		return (tp->t_ival);
	}
	if (op == ZCON)
		return (tp->t_zval);
	if (op == LCON)
		return (tp->t_lval);
	cbotch("grablval");
}
