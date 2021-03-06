/*
 * The Bourne shell.
 * This shell is dedicated to Ciaran Gerald Aidan O'Donnell.
 * May he live a thousand minutes (long enough to fix up YACC).
 * It is also dedicated to Steve Bourne.
 * May he live a thousand seconds.
 */
%{
#include "sh.h"

#define YYERROR	{ yyerrflag=1; goto YYerract; }

extern	NODE	*node();
%}

%union {
	NODE	*yu_node;
	char	*yu_strp;
	int	yu_nval;
}

%token _ANDF
%token _ASGN
%token _CASE
%token _CBRAC
%token _DO
%token _DONE
%token _DSEMI
%token _ELIF
%token _ELSE
%token _ESAC
%token _FI
%token _FOR
%token _IF
%token _IN
%token _IORS
%token _NAME
%token _NULL
%token _OBRAC
%token _ORF
%token _PARENS
%token _RET
%token _THEN
%token _UNTIL
%token _WHILE

%type <yu_node>	arg		arg_list	case_line	case_list
%type <yu_node>	cmd		cmd_line	cmd_list	cmd_seq
%type <yu_node>	control		do_list		else_part	in_name_list
%type <yu_node>	logical_cmd	name_list	opt_cmd_seq	pattern_list
%type <yu_node>	pipe_cmd	sub_shell

%type <yu_node>	command		simple_command	redirect_list	cmd_prefix
%type <yu_node>	cmd_word	cmd_suffix	cmd_name	redirect_list
%type <yu_node>	asgn_node	redirect_node	name_node
%type <yu_node>	function_definition	compound_command
%type <yu_node>	non_keyword_name

%type <yu_strp>	asgn		name		redirect

%type <yu_nval> whuntile

%%

session:
	session cmd_line
|
;

cmd_line:
	'\n' {
		sesp->s_node = NULL;
		reset(RCMD);
		NOTREACHED;
	}
|
	cmd_list '\n' {
		sesp->s_node = $1;
		reset(errflag ? RERR : RCMD);
		NOTREACHED;
	}
|	error '\n' {
		keyflush();
		keyflag = 1;
		reset(RERR);
		NOTREACHED;
	}
;

if:	_IF optnls ;

then:	_THEN optnls ;

elif:	_ELIF optnls ;

else:	_ELSE optnls ;

whuntile:	_WHILE optnls {	$$ = NWHILE;	}
|	_UNTIL optnls {	$$ = NUNTIL;	}
;

do:	_DO optnls | _DO ';' optnls ;

in:	_IN | _IN sep ;

oror:	_ORF optnls;

andand:	_ANDF optnls;

or:	'|' optnls;

oparen:	'(' optnls ;

obrack:	_OBRAC optnls ;

cparen:	')' optnls ;

dsemi:	_DSEMI optnls ;

cmd_list:
	logical_cmd {
		$$ = $1;
	}
|	logical_cmd '&' {
		$$ = node(NBACK, $1, NULL);
	}
|	logical_cmd ';' {
		$$ = $1;
	}
|	logical_cmd '&' cmd_list {
		$$ = node(NBACK, $1, $3);
	}
|	logical_cmd ';' cmd_list {
		$$ = node(NLIST, $1, $3);
	}
;

logical_cmd:
	pipe_cmd {
		$$ = $1;
	}
|	pipe_cmd oror logical_cmd {
		$$ = node(NORF, $1, $3);
	}
|	pipe_cmd andand logical_cmd {
		$$ = node(NANDF, $1, $3);
	}
;

pipe_cmd:
	cmd or pipe_cmd {
		$$ = node(NPIPE, $1, $3);
	}
|	cmd {
		$$ = $1;
	}
;

/*
 * In the original grammar, no distinction between simple command and compound
 * commands was made. This, along with the right-recursive formulation of the
 * command grammar, created a need for lookahead that defeated the complex
 * machinery for context-sensitive lexing that is required.
 */

cmd:	turn_on_keywords command {
		$$ = $2;
		keypop ();
	}
;

turn_on_keywords: {
		keypush ();
		keyflag = 1;
	}
;

command:
	simple_command {
		$$ = node (NCOMS, $1, NULL);
	}
|	compound_command {
		$$ = node (NCOMS, $1, NULL);
	}
|	compound_command redirect_list {
		$$ = node (NCOMS, $1->n_next = $2, NULL);
	}
|	function_definition {
		$$ = node (NCOMS, $1, NULL);
	}
|	_RET name {
		$$ = node (NRET, $2, NULL);
	}
|	_RET {
		$$ = node (NRET, "", NULL);
	}
;

compound_command:
	control {
		$$ = node (NCTRL, $1, NULL);
	}
;
	
function_definition:
	name _PARENS optnls obrack cmd_seq _CBRAC  {
		$$ = node (NCTRL, node (NFUNC, $1, $5), NULL);
	}
;

redirect_list:
	redirect_node {
		$$ = $1;
	}
|	redirect_list redirect_node {
		($$ = $1)->n_next = $2;
	}
;

simple_command:
	cmd_prefix cmd_word cmd_suffix {
		(($$ = $1)->n_next = $2)->n_next = $3;
	}
|	cmd_prefix cmd_word {
		($$ = $1)->n_next = $2;
	}
|	cmd_prefix {
		$$ = $1;
	}
|	cmd_name cmd_suffix {
		($$ = $1)->n_next = $2;
	}
|	cmd_name {
		$$ = $1;
	}
;

cmd_prefix:
	redirect_node {
		$$ = $1;
	}
|	redirect_node cmd_prefix {
		($$ = $1)->n_next = $2;
	}
|	asgn_node {
		$$ = $1;
	}
|	asgn_node cmd_prefix {
		($$ = $1)->n_next = $2;
	}
;

cmd_name:
	name_node {
		$$ = $1;
		keyflag = 0;
	}
;

cmd_word:
	name_node {
		$$ = $1;
		keyflag = 0;
	}
;

/*
 * The main part of this shell has some silliness with assignments and some
 * flag called '-k'. To support this, we allow assignments after the command
 * name and code elsewhere turns them back into parameters... it seems
 * preferable to do it here, but because of the '-k' thing we'll just
 * accept them.
 */

cmd_suffix:
	redirect_node {
		$$ = $1;
	}
|	redirect_node cmd_suffix {
		($$ = $1)->n_next = $2;
	}
|	non_keyword_name {
		$$ = $1;
	}
|	non_keyword_name cmd_suffix {
		($$ = $1)->n_next = $2;
	}
|	asgn_node {
		$$ = $1;
	}
|	asgn_node cmd_suffix {
		($$ = $1)->n_next = $2;
	}
;

non_keyword_name:
	non_keyword_string {
		$$ = node (NARGS, duplstr (strt, 0), NULL);
	}
;

/*
 * Many of the following cause S/R conflicts. This reflects the fact that the
 * decision about whether to recognise a token in a given place needs some
 * extra disambiguation or not. In all cases, the correct result is to shift
 * (treating the reserved word as a normal word), which is the default.
 */
non_keyword_string:
	_NAME
|	_CASE
|	_DO
|	_DONE
|	_ELIF
|	_ELSE
|	_ESAC
|	_FI
|	_FOR
|	_IF
|	_IN
|	_RET
|	_THEN
|	_UNTIL
|	_WHILE
;
	
/*
 * Replaced by detailed cases above.
cmd:
	arg_list_init arg_list {
		$$ = node(NCOMS, $2, NULL);
		keypop();
	}
|	_RET name {
		$$ = node(NRET, $2, NULL);
	}
|	_RET {
		$$ = node(NRET, "", NULL);
	}
;

arg_list_init:
	{
		keypush();
		keyflag = 1;
	}
;

arg_list:
	arg arg_list {
		if (($1->n_type == NCTRL && $2->n_type == NARGS)
		 || ($1->n_type == NARGS && $2->n_type == NCTRL)) {
			YYERROR;
		}
		($$ = $1)->n_next = $2;
	}
|	arg {
		$$ = $1;
	}
;

arg:
	redirect_node {
		$$ = $1;
	}
|	name_node {
		$$ = $1;
		keyflag = 0;
	}
|	asgn_node {
		$$ = $1;
	}
|	control {
		if (!keyflag) {
			YYERROR;
		}
		$$ = node(NCTRL, $1, NULL);
		keyflag = 0;
	}
;
*/

/*
 * The form of the following productions arranges for the contents of the
 * global "strt" to be duplicated ASAP, hopefully before lookahead gets
 * involved.
 */

redirect_node:	redirect {
		$$ = node (NIORS, $1, NULL);
	}
;

redirect:	_IORS {
		$$ = duplstr (strt, 0);
	}
;

name_node: name {
		$$ = node (NARGS, $1, NULL);
	}
;

name:	_NAME {
		$$ = duplstr (strt, 0);
	}
;


asgn_node: asgn {
		$$ = node (NASSG, $1, NULL);
	}
;

asgn:	_ASGN {
		$$ = duplstr (strt, 0);
	}
;

control:
	_FOR name in_name_list sep do_list _DONE {
		$$ = node(NFOR, $2, node(NFOR2, $3, node(NLIST, $5, NULL)));
		$$->n_next->n_next->n_next = $$->n_next;
	}
|	_FOR name in_name_list do_list _DONE {
		$$ = node(NFOR, $2, node(NFOR2, $3, node(NLIST, $4, NULL)));
		$$->n_next->n_next->n_next = $$->n_next;
	}
|	_CASE name sep in case_list _ESAC {
		$$ = node(NCASE, $2, $5);
	}
|	_CASE name in case_list _ESAC {
		$$ = node(NCASE, $2, $4);
	}
|	whuntile cmd_seq do_list _DONE {
		$$ = node($1, $2, node(NLIST, $3, NULL));
		$$->n_next->n_next = $$;
	}
|	if cmd_seq then opt_cmd_seq else_part _FI {
		$$ = node(NIF, node(NNULL, $2, $4), $5);
	}
|	oparen opt_cmd_seq ')' {
		$$ = node(NPARN, $2, NULL);
	}
|	obrack opt_cmd_seq _CBRAC {
		$$ = node(NBRAC, $2, NULL);
	}
;

in_name_list:
	_IN name_list {
		$$ = $2;
	}
|	{
		$$ = node(NARGS, "\"$@\"", NULL);
	}
;

name_list:
	name name_list {
		$$ = node(NARGS, $1, $2);
	}
|	{
		$$ = NULL;
	}
;

case_list:
	case_line dsemi case_list {
		register NODE *np;

		for (np=$1; np->n_next; np=np->n_next)
			;
		np->n_next = $3;
		$$ = $1;
	}
|	case_line {
		$$ = $1;
	}
|	{
		$$ = NULL;
	}
;

case_line:
	pattern_list cparen opt_cmd_seq {
		$$ = node(NCASE2, $3, $1);
	}
;

pattern_list:
	name '|' pattern_list {
		$$ = node(NCASE3, $1, $3);
	}
|	name {
		$$ = node(NCASE3, $1, NULL);
	}
;

do_list:
	do opt_cmd_seq {
		$$ = $2;
	}
|	{
		$$ = NULL;
	}
;

else_part:
	elif cmd_seq then opt_cmd_seq else_part {
		$$ = node(NIF, node(NNULL, $2, $4), $5);
	}
|	else opt_cmd_seq {
		$$ = node(NELSE, $2, NULL);
	}
|	{
		$$ = NULL;
	}
;

opt_cmd_seq:
	cmd_seq {
		$$ = $1;
	}
|
	{
		$$ = NULL;
	}
;

cmd_seq:
	cmd_list nls cmd_seq {
		$$ = node(NLIST, $1, $3);
	}
|	cmd_list optnls {
		$$ = $1;
	}
;

sep:	nls
|	';'
|	';' nls
;

optnls:	nls
|
;

nls:	'\n'
|	nls '\n'
;

%%
/*
 * Create a node.
 */
NODE *
node(type, auxp, next)
NODE *auxp, *next;
{
	register NODE *np;

	np = (NODE *) balloc(sizeof (NODE));
	np->n_type = type;
	np->n_auxp = auxp;
	np->n_next = next;
	return np;
}

#define NBPC 8
#define NKEY 8
static char keys[NKEY] = { 0 };
static int  keyi = NKEY * NBPC;

keyflush()
{
	register char *kp;

	for (kp = keys+NKEY; kp > keys; *--kp = 0)
		;
	keyi = NKEY * NBPC;
}

keypop()
{
	register char	*kp;
	register int	km;

	if ((km = keyi++) >= NKEY * NBPC) {
		panic(11);
		NOTREACHED;
	}
	kp = keys + (km / NBPC);
	km = 1 << (km %= NBPC);
	keyflag = (*kp & km) ? 1 : 0;
	*kp &= ~km;
}

keypush()
{
	register char	*kp;
	register int	km;

	if ((km = --keyi) < 0) {
		panic(12);
		NOTREACHED;
	}
	if (keyflag) {
		kp = keys + (km / NBPC);
		km = 1 << (km %= NBPC);
		*kp |= km;
	}
}
/*
 * The following fragments might implement named pipes.
 * The token declaration goes in the header.
 * The nopen production should go with the others of its ilk.
 * The production fragment goes into arg:
%token _NOPEN _NCLOSE
nopen:	_NOPEN optnls ;

|	nopen pipe_cmd ')' {
		$$ = node(NRPIPE, $2, NULL);
	}
|	oparen pipe_cmd _NCLOSE {
		$$ = node(NWPIPE, $2, NULL);
	}
 *
 */
