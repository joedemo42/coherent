/*
 *	The typedef rvalue defines the basic entity upon which bc
 *	operates.
 */

typedef struct {
	mint	mantissa;
	int	scale;
}	rvalue;


/*
 *	The typedef array defines the concept of an array of rvalues.
 */

typedef struct {
	rvalue	*avalue;
	int	size;
}	array;


/*
 *	The typedef bcstate is used to hold the state of the bc-
 *	pseudo machine which should be restored when returning from
 *	a function call.
 */

typedef struct {
	union code	*spc;		/* pc to return to */
	union stkent	*sfp,		/* frame pointer to return to */
			*stos;		/* stack pointer to return to */
} bcstate;


/*
 *	The typedef stkent defines the union used for run-time stack
 *	entries.
 */

typedef union  stkent {
	rvalue		rvalue,
			*lvalue;
	array		*alvalue;
	bcstate		bcstate;
}	stkent;


/*
 *	The typedef opcode is the list of opcodes for the hypothetical
 *	bc-machine.
 */

#define LOAD	0	/*
			 * Convert TOS from an l_value to an r_value.
			 */
#define LIBASE	1
#define LOBASE	2
#define LSCALE	3	/*
			 * Push the r_value of the special variable ibase
			 * (resp. obase, scale).
			 */
#define STORE	4	/*
			 * Store the r_value TOS at the l_value
			 * TOS[-1].  Leave the r_value on the stack
			 * but remove the l_value.
			 */
#define SIBASE	5
#define SOBASE	6
#define SSCALE	7	/*
			 * Copy the r_value in TOS into ibase (rep. obase,
			 * scale).  Also, check to see that the value is
			 * acceptable.
			 */
#define POP	8	/*
			 * Throw away the r_value TOS.
			 */
#define PRVAL	9	/*
			 * Push the r_value corresponding to the l_value TOS
			 * onto the stack.  Note that the old TOS is left
			 * on the stack.
			 */
#define PGLSC	10
#define PLOSC	11	/*
			 * Push the l_value of a global (local) scalar onto
			 * the stack.  The address (resp. stack frame offset)
			 * of the scalar follows the opcode.
			 */
#define PLISC	12	/* Push the l_value of a literal scalar onto the
			 * stack.  The address of the scalar follows the
			 * the opcode.  Distinguished from PGLSC so literal
			 * storage space can be recovered, otherwise the same.
			 */
#define PGLAE	13
#define	PLOAE	14	/*
			 * Push the l_value of a global (local) array
			 * element onto the stack.  The address (resp. stack
			 * frame offset) of the array follows the opcode
			 * and TOS is the subscript.  Note that the subscript
			 * is first removed from the stack.
			 */
#define PGLAR	15
#define PLOAR	16	/*
			 * Push the l_value of a global (local) array onto
			 * the stack.  The address (resp. stack fram offset)
			 * of the array follows the opcode.  (Used to pass
			 * an entire array as a function argument.)
			 */
#define STOP	17	/*
			 * Stop interpreting bc-machine pseudo-instructions
			 * and return.  This is only used at the end of
			 * the code compiled for immediate execution.
			 */
#define	CALL	18	/*
			 * Call the function whoose dictionary entry
			 * is pointed to by the word following this
			 * opcode.  The number of arguments is contained
			 * in the word following that.
			 * Just before the CALL, the stack looks like:
			 *	TOS->	last parameter
			 *		...
			 *		first parameter
			 *		rest of stack
			 * and just after the CALL, it looks like:
			 *	TOS->	state to return to
			 *		last automatic variable
			 *		...
			 *		first automatic variable
			 *		last parameter
			 *		...
			 *	FRAME->	first parameter
			 *		rest of stack
			 */
#define RETURN	19	/*
			 * Return from the function whoose dictionary entry
			 * is pointed to by the word following this opcode.
			 * Just before the RETURN, the stack looks like:
			 *	TOS->	r_value to return
			 *		state to return to
			 *		last automatic variable
			 *		...
			 *		first automatic variable
			 *		last parameter
			 *		...
			 *	FRAME->	first parameter
			 *		rest of stack
			 * Just after the RETURN, the stack looks like:
			 *	TOS->	r_value returned
			 *		rest of stack
			 * Note that all prameters and automatic variables
			 * must be freed and removed from the stack.
			 */
#define INC	20
#define DEC	21	/*
			 * Add 1 (resp. -1) to the r_value TOS.
			 */
#define PRNUM	22	/*
			 * Print the r_value TOS and then remove it from
			 * the stack.
			 */
#define PRSTR	23	/*
			 * Print the string whose address follows this
			 * opcode.  Note that no newline is added.
			 */
#define PRNL	24	/*
			 * Print a new line character.
			 */

#define	LENGTH	25
#define SCALE	26
#define SQRT	27	/*
			 * Replace the r_value TOS with its length in bytes,
			 * scale factor, or square root respectively.
			 */
#define ADD	28
#define	SUB	29
#define MUL	30
#define	DIV	31
#define REM	32
#define EXP	33	/*
			 * Replace the r_values TOS and TOS[-1] with
			 * TOS + TOS[-1] (resp. TOS - TOS[-1], TOS * TOS[-1],
			 * TOS / TOS[-1], TOS % TOS[-1], TOS ^ TOS[-1]).
			 */
#define NEG	34	/*
			 * Replace the r_value TOS with -TOS.
			 */
#define BRALW	35
#define BRNEV	36	/*
			 * Branch always (respectively never).  The new
			 * pc will be the address of the following code
			 * item plus its contents.
			 */
#define BRLT	37
#define BRLE	38
#define BREQ	39
#define BRGE	40
#define BRGT	41
#define BRNE	42	/*
			 * Branch if the r_value TOS[-1] is less than
			 * (respectively less or equal, equal,
			 * greater or equal, greater and unequal)
			 * then the r_value TOS.  Note that both TOS and
			 * TOS[-1] are removed from the stack.
			 */
#define EXIT	43	/* Exit from bc, used to be immediately done
			 * whether quit appeared in a conditional,
			 * function definition or anywhere whatsoever.
			 */
typedef int opcode;

/*
 *	The typedef code is the type of entry in the array of
 *	pseudo-code which the pseudo-machine executes.
 */

typedef union code {
	int		opcode;
	int		address;	/* relative code address */
	rvalue		*lvalue;	/* global scalar or constant */
	array		*alvalue;	/* global array */
	char		*svalue;	/* string */
	int		ivalue;		/* stack offset and misc. counts */
	struct dicent	*dvalue;	/* function name */
} code;
