/* $Header: /usr/src/lib/libcurses/RCS/tkeytab.c,v 1.1 89/04/07 13:22:19 src Exp $
 *
 *	The  information  contained herein  is a trade secret  of INETCO
 *	Systems, and is confidential information.   It is provided under
 *	a license agreement,  and may be copied or disclosed  only under
 *	the terms of that agreement.   Any reproduction or disclosure of
 *	this  material  without  the express  written  authorization  of
 *	INETCO Systems or persuant to the license agreement is unlawful.
 *
 *	Copyright (c) 1989
 *	An unpublished work by INETCO Systems, Ltd.
 *	All rights reserved.
 */

#include "curses.h"

/*
 * Default translation table: keyboard token into address of a string var.
 *
 *	Used by getkey() in scanning keyboard input.
 */
tkeyent_t tkeytab[] = {

	{ KEY_DOWN,	&key_down	},
	{ KEY_UP,	&key_up		},
	{ KEY_LEFT,	&key_left	},
	{ KEY_RIGHT,	&key_right	},
	{ KEY_HOME,	&key_home	},
	{ KEY_BACKSPACE,&key_backspace	},

	{ KEY_F(0),	&key_f0		},	/* function key 0	*/
	{ KEY_F(1),	&key_f1		},	/* function key 1	*/
	{ KEY_F(2),	&key_f2		},	/* function key 2	*/
	{ KEY_F(3),	&key_f3		},	/* function key 3	*/
	{ KEY_F(4),	&key_f4		},	/* function key 4	*/

	{ KEY_F(5),	&key_f5		},	/* function key 5	*/
	{ KEY_F(6),	&key_f6		},	/* function key 6	*/
	{ KEY_F(7),	&key_f7		},	/* function key 7	*/
	{ KEY_F(8),	&key_f8		},	/* function key 8	*/
	{ KEY_F(9),	&key_f9		},	/* function key 9	*/

	{ KEY_F(10),	&key_f10	},	/* function key 10	*/
	{ KEY_F(11),	&key_f11	},	/* function key	11	*/
	{ KEY_F(12),	&key_f12	},	/* function key	12	*/
	{ KEY_F(13),	&key_f13	},	/* function key	13	*/
	{ KEY_F(14),	&key_f14	},	/* function key	14	*/

	{ KEY_F(15),	&key_f15	},	/* function key	15	*/
	{ KEY_F(16),	&key_f16	},	/* function key	16	*/
	{ KEY_F(17),	&key_f17	},	/* function key	17	*/
	{ KEY_F(18),	&key_f18	},	/* function key	18	*/
	{ KEY_F(19),	&key_f19	},	/* function key	19	*/

	{ KEY_F(20),	&key_f20	},	/* function key	20	*/
	{ KEY_F(21),	&key_f21	},	/* function key	21	*/
	{ KEY_F(22),	&key_f22	},	/* function key	22	*/
	{ KEY_F(23),	&key_f23	},	/* function key	23	*/
	{ KEY_F(24),	&key_f24	},	/* function key	24	*/

	{ KEY_F(25),	&key_f25	},	/* function key 25	*/
	{ KEY_F(26),	&key_f26	},	/* function key	26	*/
	{ KEY_F(27),	&key_f27	},	/* function key 27	*/
	{ KEY_F(28),	&key_f28	},	/* function key	28	*/
	{ KEY_F(29),	&key_f29	},	/* function key	29	*/

	{ KEY_F(30),	&key_f30	},	/* function key	30	*/
	{ KEY_F(31),	&key_f31	},	/* function key	31	*/
	{ KEY_F(32),	&key_f32	},	/* function key	32	*/
	{ KEY_F(33),	&key_f33	},	/* function key	33	*/
	{ KEY_F(34),	&key_f34	},	/* function key	34	*/

	{ KEY_F(35),	&key_f35	},	/* function key	35	*/
	{ KEY_F(36),	&key_f36	},	/* function key	36	*/
	{ KEY_F(37),	&key_f37	},	/* function key	37	*/
	{ KEY_F(38),	&key_f38	},	/* function key	38	*/
	{ KEY_F(39),	&key_f39	},	/* function key	39	*/

	{ KEY_F(40),	&key_f40	},	/* function key	40	*/
	{ KEY_F(41),	&key_f41	},	/* function key	41	*/
	{ KEY_F(42),	&key_f42	},	/* function key	42	*/
	{ KEY_F(43),	&key_f43	},	/* function key	43	*/
	{ KEY_F(44),	&key_f44	},	/* function key	44	*/

	{ KEY_F(45),	&key_f45	},	/* function key	45	*/
	{ KEY_F(46),	&key_f46	},	/* function key	46	*/
	{ KEY_F(47),	&key_f47	},	/* function key	47	*/
	{ KEY_F(48),	&key_f48	},	/* function key	48	*/
	{ KEY_F(49),	&key_f49	},	/* function key	49	*/

	{ KEY_F(50),	&key_f50	},	/* function key	50	*/
	{ KEY_F(51),	&key_f51	},	/* function key	51	*/
	{ KEY_F(52),	&key_f52	},	/* function key	52	*/
	{ KEY_F(53),	&key_f53	},	/* function key	53	*/
	{ KEY_F(54),	&key_f54	},	/* function key	54	*/

	{ KEY_F(55),	&key_f55	},	/* function key	55	*/
	{ KEY_F(56),	&key_f56	},	/* function key	56	*/
	{ KEY_F(57),	&key_f57	},	/* function key	57	*/
	{ KEY_F(58),	&key_f58	},	/* function key	58	*/
	{ KEY_F(59),	&key_f59	},	/* function key	59	*/

	{ KEY_F(60),	&key_f60	},	/* function key	60	*/
	{ KEY_F(61),	&key_f61	},	/* function key	61	*/
	{ KEY_F(62),	&key_f62	},	/* function key	62	*/
	{ KEY_F(63),	&key_f63	},	/* function key	63	*/

	{ KEY_DL,	&key_dl		},	/* delete line		*/
	{ KEY_IL,	&key_il		},	/* insert line		*/
	{ KEY_DC,	&key_dc		},	/* delete char		*/
	{ KEY_IC,	&key_ic		},	/* insert char		*/
	{ KEY_EIC,	&key_eic	},	/* exit insert ch mode	*/

	{ KEY_CLEAR,	&key_clear	},	/* clear screen		*/
	{ KEY_EOS,	&key_eos	},	/* clear to screen end	*/
	{ KEY_EOL,	&key_eol	},	/* clear to line end	*/
	{ KEY_SF,	&key_sf		},	/* scroll 1 line forward*/
	{ KEY_SR,	&key_sr		},	/* scroll 1 line back	*/

	{ KEY_NPAGE,	&key_npage	},	/* next page		*/
	{ KEY_PPAGE,	&key_ppage	},	/* previous page	*/
	{ KEY_STAB,	&key_stab	},	/* set tab		*/
	{ KEY_CTAB,	&key_ctab	},	/* clear tab		*/
	{ KEY_CATAB,	&key_catab	},	/* clear all tabs	*/

	{ KEY_ENTER,	&key_enter	},	/* enter or send	*/
	{ KEY_SRESET,	NULL		},	/* soft (partial) reset	*/
	{ KEY_RESET,	NULL		},	/* reset or hard reset	*/
	{ KEY_PRINT,	&key_print	},	/* print or copy	*/
	{ KEY_LL,	&key_ll		},	/* home down or bottom	*/

	{ KEY_A1,	&key_a1		},	/* upper left of keypad	*/
	{ KEY_A3,	&key_a3		},	/* upper right of keypad*/
	{ KEY_B2,	&key_b2		},	/* center of keypad	*/
	{ KEY_C1,	&key_c1		},	/* lower left of keypad	*/
	{ KEY_C3,	&key_c3		},	/* lower right of keypad*/

	{ KEY_BTAB,	&key_btab	},	/* back tab key		*/
	{ KEY_BEG,	&key_beg	},	/* beg(inning) key	*/
	{ KEY_CANCEL,	&key_cancel	},	/* cancel key		*/
	{ KEY_CLOSE,	&key_close	},	/* close key		*/
	{ KEY_COMMAND,	&key_command	},	/* cmd (command) key	*/

	{ KEY_COPY,	&key_copy	},	/* copy key		*/
	{ KEY_CREATE,	&key_create	},	/* create key		*/
	{ KEY_END,	&key_end	},	/* end key		*/
	{ KEY_EXIT,	&key_exit	},	/* exit key		*/
	{ KEY_FIND,	&key_find	},	/* find key		*/

	{ KEY_HELP,	&key_help	},	/* help key		*/
	{ KEY_MARK,	&key_mark	},	/* mark key		*/
	{ KEY_MESSAGE,	&key_message	},	/* message key		*/
	{ KEY_MOVE,	&key_move	},	/* move key		*/
	{ KEY_NEXT,	&key_next	},	/* next object key	*/

	{ KEY_OPEN,	&key_open	},	/* open key		*/
	{ KEY_OPTIONS,	&key_options	},	/* options key		*/
	{ KEY_PREVIOUS,	&key_previous	},	/* previous object key	*/
	{ KEY_REDO,	&key_redo	},	/* redo key		*/
	{ KEY_REFERENCE,&key_reference	},	/* ref(erence) key	*/

	{ KEY_REFRESH,	&key_refresh	},	/* refresh key		*/
	{ KEY_REPLACE,	&key_replace	},	/* replace key		*/
	{ KEY_RESTART,	&key_restart	},	/* restart key		*/
	{ KEY_RESUME,	&key_resume	},	/* resume key		*/
	{ KEY_SAVE,	&key_save	},	/* save key		*/

	{ KEY_SBEG,	&key_sbeg	},	/* shifted beginning key*/
	{ KEY_SCANCEL,	&key_scancel	},	/* shifted cancel key	*/
	{ KEY_SCOMMAND,	&key_scommand	},	/* shifted command key	*/
	{ KEY_SCOPY,	&key_scopy	},	/* shifted copy key	*/
	{ KEY_SCREATE,	&key_screate	},	/* shifted create key	*/

	{ KEY_SDC,	&key_sdc	},	/* shifted del char key	*/
	{ KEY_SDL,	&key_sdl	},	/* shifted del line key	*/
	{ KEY_SELECT,	&key_select	},	/* select key		*/
	{ KEY_SEND,	&key_send	},	/* shifted end key	*/
	{ KEY_SEOL,	&key_seol	},	/* shifted EOL key	*/

	{ KEY_SEXIT,	&key_sexit	},	/* shifted exit key	*/
	{ KEY_SFIND,	&key_sfind	},	/* shifted find key	*/
	{ KEY_SHELP,	&key_shelp	},	/* shifted help key	*/
	{ KEY_SHOME,	&key_shome	},	/* shifted home key	*/
	{ KEY_SIC,	&key_sic	},	/* shifted input key	*/

	{ KEY_SLEFT,	&key_sleft	},	/* shifted left key	*/
	{ KEY_SMESSAGE,	&key_smessage	},	/* shifted message key	*/
	{ KEY_SMOVE,	&key_smove	},	/* shifted move key	*/
	{ KEY_SNEXT,	&key_snext	},	/* shifted next key	*/
	{ KEY_SOPTIONS,	&key_soptions	},	/* shifted options key	*/

	{ KEY_SPREVIOUS,&key_sprevious	},	/* shifted prev key	*/
	{ KEY_SPRINT,	&key_sprint	},	/* shifted print key	*/
	{ KEY_SREDO,	&key_sredo	},	/* shifted redo key	*/
	{ KEY_SREPLACE,	&key_sreplace	},	/* shifted replace key	*/
	{ KEY_SRIGHT,	&key_sright	},	/* shifted right key	*/

	{ KEY_SRSUME,	&key_srsume	},	/* shifted resume key	*/
	{ KEY_SSAVE,	&key_ssave	},	/* shifted save key	*/
	{ KEY_SSUSPEND,	&key_ssuspend	},	/* shifted suspend key	*/
	{ KEY_SUNDO,	&key_sundo	},	/* shifted undo key	*/
	{ KEY_SUSPEND,	&key_suspend	},	/* suspend key		*/

	{ KEY_UNDO,	&key_undo	},	/* undo key		*/

	{ 0,		NULL		}
};
