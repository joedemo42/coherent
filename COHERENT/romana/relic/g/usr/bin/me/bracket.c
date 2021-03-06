/*
 * This routine gets a bracket character from where the dot lies
 * and searches for the corresponding bracket.
 */
#include	<stdio.h>
#include	"ed.h"

static int o_col, o_row, r_col, r_char, r_row = -1;

/*
 * This is called before each comand is executed.
 */
bracketoff()
{
	if (bind.bracket && (r_row >= 0)) {
		tmove(r_row, r_col);
		tputc(r_char);
		r_row = -1;
		tmove(o_row, o_col);
		return (TRUE);
	}
	return (FALSE);
}

/*
 * toggle bracket mode.
 */
bracketsw()
{
	bind.bracket ^= 1;
}

bracketsearch()
{
	bstart(0);	/* do full search */
}

/*
 * This is called after each screen update.
 */
bracketmode(row, col)
{
	r_row = -1;
	if (bind.bracket)
		bstart(1);
	movecursor(row, col);
	tmove(o_row = row, o_col = col);
}

static
bstart(modesw)
{
	register LINE	*clp;
	int	cbo;

	if (((o_col = cbo = curwp->w_doto) == llength(clp = curwp->w_dotp)))
		if (modesw)
			--cbo;
		else
			return (FALSE);

	switch (lgetc(clp, cbo)) {
	case '/':
		if ((++cbo == llength(clp)) || ('*' != lgetc(clp, cbo--)))
			break;
	case '[':
	case '{':
	case '(':
		return (brksearch(1, modesw, clp, cbo));
	case '*':
		if ((++cbo == llength(clp)) || ('/' != lgetc(clp, cbo)))
			break;
	case ']':
	case '}':
	case ')':
		return (brksearch(-1, modesw, clp, cbo));
	case '"':
	case '\'':
		if (modesw)
			break;
		if (mlyesno("Forward search"))
			return (brksearch(1, modesw, clp, cbo));
		return (brksearch(-1, modesw, clp, cbo));
	}
	if (!modesw)
		mlwrite("Place dot on \"'(){}[] /* or */");
	return (FALSE);
}

/* defines for states of forward and backward searches */
#define NORMAL		0	/* normal state */
#define SLASH		1	/* slash maybe comment */
#define COMMENT	2	/* comment */
#define STAR		3	/* star in comment */
#define DQUOTE		4	/* in string */
#define SQUOTE		5	/* in single quote */
#define BSL			6	/* backslash */
#define BADEXPR	7	/* bad expression encountered */
#define STARTB		8	/* starting search */

/*
 * scan for end of balanced c expression forward or backward.
 */
static
brksearch(dir, mode, clp, cbo)
int dir;	/* 1 forward -1 backward */
int mode;	/* 1 paren mode, 0 match paren command */
LINE *clp;	/* line to start search */
int cbo;	/* char to start search */
{
	register int c, state;
	LINE *end;
	int prev, pstate, bcnt, pcnt, ccnt, bscnt, ctrs;

	bcnt = ccnt = pcnt = bscnt = 0;
	state = STARTB;
	if (mode) {	/* end is top or bottom of window */
		if (dir < 0)
			end = lback(curwp->w_linep);
		else
			for (end = curwp->w_linep, c = curwp->w_ntrows;
			    (c-- > 0) && (curbp->b_linep != end);
			    end = lforw(end)) ;
	} else	/* end is buffer end */
		end = curbp->b_linep;

	while (clp != end) {
		if (dir > 0) { /* get next char */
			if (cbo == llength(clp)) {
				clp = lforw(clp);
				cbo = 0;
				c = '\n';
			} else
				c = lgetc(clp, cbo++);
		} else { /* get prev char. watch \ here. */
			c = prev;
			if (cbo < 0) {
				clp = lback(clp);
				cbo = llength(clp) - 1;
				prev = '\n';
			}
			else if (('\\' == (prev = lgetc(clp, cbo--))) &&
				(state != COMMENT) && (state != SLASH)) {
				bscnt ^= 1;	/* count \ */
				prev = c; /* don't discard char */
				continue;
			}
			if (bscnt) { /* odd \ discard c */
				bscnt = 0;
				continue;
			}
		}

		switch (state) {	/* do state processing */
		case SLASH:		/* / encountered */
			if (c == '*') {
				state = COMMENT;
				continue;
			}
			state = NORMAL;
		case NORMAL:		/* not in anything */
			break;
		case STAR:		/* * in comment */
			if (c == '/') {
				state = NORMAL;
				goto processed;
			}
			state = COMMENT;
		case COMMENT:		/* in comment */
			if (c == '*')
				state = STAR;
			continue;
		case SQUOTE:		/* in ' string */
		case DQUOTE:		/* in " string */
			switch (c) {
			case '\n':
				state = BADEXPR;
				goto processed;
			case '\\':	/* can't get here if dir < 0 */
				pstate = state;
				state = BSL;
				continue;
			case '"':
				if (state != DQUOTE)
					continue;
				state = NORMAL;
				goto processed;
			case '\'':
				if (state != SQUOTE)
					continue;
				state = NORMAL;
				goto processed;
			default:
				continue;
			}
		case BSL:	/* can't get here if dir < 0 */
			state = pstate;
			continue;
		case STARTB:
			state = NORMAL;
			if (dir < 0)
				continue;
		}	

		switch (c) { /* process chars in NORMAL state */
		case '[':
			bcnt += dir;
			break;
		case '{':
			ccnt += dir;
			if (pcnt | bcnt)
				state = BADEXPR;
			break;
		case ';':
			if (bcnt)
				state = BADEXPR;
			break;
		case '(':
			pcnt += dir;
			break;
		case '"':
			state = DQUOTE;
			continue;
		case '\'':
			state = SQUOTE;
			continue;
		case '\\':	/* can't get here if dir < 0 */
			pstate = state;
			state = BSL;
			continue;
		case '/':
			state = SLASH;
			continue;
		case ']':
			bcnt -= dir;
			break;
		case '}':
			ccnt -= dir;
			if (pcnt | bcnt)
				state = BADEXPR;
			break;
		case ')':
			pcnt -= dir;
		}

		/* done if BADEXPR or balanced expr or ) with no ( etc */
processed:	if (((ctrs = bcnt | ccnt | pcnt) <= 0) || (state == BADEXPR)) {
			if (dir > 0) {
				if (--cbo < 0) {
					clp = lback(clp);
					cbo = llength(clp);
				}
			} else { /* we are on prev so back up 2 */
				for (bscnt = 0; bscnt < 2; bscnt++) {
					if (++cbo > llength(clp)) {
						clp = lforw(clp);
						cbo = 0;
						break;
					}
				}
			}
			if (mode) { /* paren mode */
				if ((ctrs < 0) || (state == BADEXPR) ||
				   (0 > (r_row = locatecursor(clp))))
					return (FALSE);
				tmove(r_row, (r_col = truecol(clp, cbo)));
				tstand(1);
				tputc(r_char = c);
				tstand(0);
			} else { /* match paren command */
				curwp->w_dotp = clp;
				curwp->w_doto = cbo;
				curwp->w_flag |= WFMOVE;
				if ((ctrs < 0) || (state == BADEXPR)) {
					mlwrite("Bad expression");
					return (FALSE);
				}
			}
			return (TRUE);
		}
	}
	if (!mode)
		mlwrite("End not found");
	return (FALSE);
}
