#ifndef	SYMBOL_H
#define	SYMBOL_H

/*
 *-IMPORTS:
 *	<sys/compat.h>
 *		CONST
 *		EXTERN_C_BEGIN
 *		EXTERN_C_END
 *		PROTO ()
 *	<stddef.h>
 *		size_t
 */

#include <sys/compat.h>
#include <stddef.h>


/*
 * Don't pull in the other application #includes.
 */

#ifndef	BUILD_T
#define	BUILD_T
typedef	struct builder	build_t;
#endif

#ifndef	INPUT_T
#define	INPUT_T
typedef	struct input	input_t;
#endif

#ifndef	SYMBOL_T
#define	SYMBOL_T
typedef struct symbol	symbol_t;
#endif

#ifndef	LEX_T
#define	LEX_T
typedef	struct lexinfo	lex_t;
#endif

#ifndef	TOKEN_T
#define	TOKEN_T
typedef	struct token	token_t;
#endif


/*
 * This is the structure tokens are built into.
 */

struct symbol {
	symbol_t      *	s_next;		/* next symbol */
	CONST unsigned char
		      *	s_data;		/* octets occupied by symbol data */
	size_t		s_size;		/* length of symbol data */
};


/*
 * Alternate EOF character so our clients don't have to pull in <stdio.h>,
 * should be the same as READ_EOF and IN_EOF.
 */

#define	SYM_EOF		-1


typedef	int	(* dev_func_p)	PROTO ((input_t * _input, lex_t * _lexp));


EXTERN_C_BEGIN

int		symbol_init	PROTO ((void));

void		read_dev_file	PROTO ((CONST char * _name,
					dev_func_p _devfuncp));
int		read_symbol	PROTO ((input_t * _input, lex_t * _lexp,
					symbol_t ** _sym));
symbol_t      *	sym_intern	PROTO ((token_t * tok));
symbol_t      *	sym_find	PROTO ((CONST unsigned char * data,
					size_t len));
symbol_t      *	string_to_symbol PROTO ((CONST unsigned char * str,
					 size_t len));

EXTERN_C_END

#endif	/* ! defined (SYMBOL_H) */
