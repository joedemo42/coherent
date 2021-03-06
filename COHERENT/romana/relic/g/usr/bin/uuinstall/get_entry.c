/* get_entry.c:	get a keystroke. Increment or decrement row/column
 *		values as indicated by the keystroke. Also test for
 *		<RETURN> or 's' being pressed. This will select the
 *		current highlighted entry for processing. If an entry
 *		is selected, the entry number within the file is returned.
 *		A -1 is returned is no selection is made (user pressed 'q').
 */

#include "uuinstall.h"

get_entry()
{
	short retflag = 0;	/* bogus, but needed for do...while loop. */
	char arrow;

	do{
		arrow = wgetch(selwin);		/* here's our keystroke */

		/* test for console arrow key. This will be represented
		 * by a 3 char escape sequence. The first char is esc,
		 * the second we don't care about. The third tells us
		 * what arrow key was pressed. We will translate this third
		 * character into a vi keystroke of h,j,k or l.
		 */

		if (arrow == 27){		/* escape. indicates arrow */
			wgetch(selwin);		/* skip 2nd char of esc seq. */
			arrow = wgetch(selwin);	/* this is the value we want */

			switch(arrow){
				case 68:	arrow = 'h';
						break;
				case 67:	arrow = 'l';
						break;
				case 66:	arrow = 'j';
						break;
				case 65:	arrow = 'k';
						break;
			}
		}

		switch(arrow){
			case 'h':	/* move left */
				newcol = prevcol - 15;
				if(newcol < 1)
					newcol = 61;
			/* if the next entry position is empty, go back to
			 * first position in this row.
			 */
				if(' ' == mvwinch(selwin,newrow,newcol))
					newcol = 1;
				break;


			case 'l':	/* move right */
				newcol = prevcol + 15;
				if(newcol > 61)
					newcol = 1;
			/* if the next entry position is empty, go back to
			 * first position in this row.
			 */
				if(' ' == mvwinch(selwin,newrow,newcol))
					newcol = 1;
				break;


			case 'j':	/* move down */
				newrow = prevrow +1;
			/* wrap to first row if we try to move down too far */
				if (newrow == 20)
					newrow = 0;
			/* if next position is empty, wrap */
				if(' ' ==mvwinch(selwin,newrow,newcol))
					newrow = 0;
				break;


			case 'k':	/* move up */
				newrow = prevrow -1;
			/* wrap to last row if we try to move down too far */
				if (newrow == -1)
					newrow = 19;
			/* if next position is empty, wrap */
				if(' ' ==mvwinch(selwin,newrow,newcol))
					newrow = 0;
				break;

			case 'q':
				return(-1);
				/* NO REACHED */
			case 's':
			case '\n':
				return( ((prevrow * 5) + (prevcol/15)) );
				/* NOT REACHED */
		}

	/* unhighlite previously highlighted entry (prevrow,prevcol) */
		lite(selwin,prevrow,prevcol,0);
		wrefresh(selwin);

	/* highlight newly indicated entry */
		lite(selwin,newrow, newcol,1);
		wrefresh(selwin);

	/* now set the old corrdinates to the current coordinates */
		prevrow = newrow;
		prevcol = newcol;

	}
	while(!retflag);
}
