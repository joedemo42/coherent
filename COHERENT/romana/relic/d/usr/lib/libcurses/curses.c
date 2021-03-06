/*
 * Copyright (c) 1981 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef COHERENT
#ifndef lint
static uchar sccsid[] = "@(#)curses.c	5.5 (Berkeley) 6/30/88";
#endif /* not lint */
#endif /* not COHERENT */

/*
 * Define global variables
 *
 */
#include	"curses.h"

bool	_echoit		= TRUE,	/* set if stty indicates ECHO		*/
	_rawmode	= FALSE,/* set if stty indicates RAW mode	*/
	My_term		= FALSE,/* set if user specifies terminal type	*/
	_endwin		= FALSE;/* set if endwin has been called	*/

uchar	ttytype[50],		/* long name of tty			*/
	*Def_term	= "unknown";	/* default terminal type	*/

int	_tty_ch		= 1,	/* file channel which is a tty		*/
	LINES,			/* number of lines allowed on screen	*/
	COLS,			/* number of columns allowed on screen	*/
	_res_flg;		/* sgtty flags for reseting later	*/

WINDOW	*stdscr		= NULL,
	*curscr		= NULL;

tflgent_t * tflgmap = tflgtab;	/* Boolean field into variable xlat tab	*/
tstrent_t * tstrmap = tstrtab;	/* String  field into variable xlat tab	*/
tkeyent_t * tkeymap = tkeytab;	/* Key token into variable xlat table	*/

# ifdef DEBUG
FILE	*outf;			/* debug output file			*/
# endif

SGTTY	_tty;			/* tty modes				*/

bool		AM, BS, CA, DA, DB, EO, HC, HZ, IN, MI, MS, NC, NS, OS, UL,
		XB, XN, XT, XS, XX;
uchar		*AL,
		*BC, *BT,
		*CD, *CE, *CL, *CM, *CR, *CS,
		*DC, *DL, *DM, *DO,
		*ED, *EI,
		*HO,
		*IC, *IM, *IP,
		*KE, *KS,
		*LL,
		*MA,
		*ND, *NL,
		*RC,
		*SC, *SE, *SF, *SO, *SR,
		*TA, *TE, *TI,
		*UC, *UE, *UP, *US,
		*AL_PARM, *DL_PARM, *UP_PARM, *DOWN_PARM,
		*LEFT_PARM, *RIGHT_PARM;
uchar		PC;

/*
 * Capabilities added by INETCO to termcap.
 * NOTE: Variable names chosen for compatability with terminfo,
 *	 rather than choose arbitrary names.
 */
int		magic_cookie_glitch;

uchar		*bell,
		*cursor_invisible, *cursor_normal, *cursor_visible,
		*enter_am_mode, *enter_bold_mode, *enter_blink_mode,
		*enter_delete_mode, *enter_dim_mode, *enter_protected_mode,
		*enter_reverse_mode, *enter_secure_mode,
		*exit_am_mode, *exit_attribute_mode,
		*flash_screen,
		*init_1string, *init_2string, *init_3string,
		*key_a1, *key_a3,
		*key_b2, *key_backspace, *key_beg, *key_btab,
		*key_c1, *key_c3, *key_cancel, *key_catab, *key_clear,
		*key_close, *key_command, *key_copy, *key_create, *key_ctab,
		*key_dc, *key_dl, *key_down,
		*key_eic, *key_end, *key_enter, *key_eol, *key_eos, *key_exit,
		*key_f0,  *key_f1,  *key_f2,  *key_f3,  *key_f4,
		*key_f5,  *key_f6,  *key_f7,  *key_f8,  *key_f9,
		*key_f10, *key_f11, *key_f12, *key_f13, *key_f14,
		*key_f15, *key_f16, *key_f17, *key_f18, *key_f19,
		*key_f20, *key_f21, *key_f22, *key_f23, *key_f24,
		*key_f25, *key_f26, *key_f27, *key_f28, *key_f29,
		*key_f30, *key_f31, *key_f32, *key_f33, *key_f34,
		*key_f35, *key_f36, *key_f37, *key_f38, *key_f39,
		*key_f40, *key_f41, *key_f42, *key_f43, *key_f44,
		*key_f45, *key_f46, *key_f47, *key_f48, *key_f49,
		*key_f50, *key_f51, *key_f52, *key_f53, *key_f54,
		*key_f55, *key_f56, *key_f57, *key_f58, *key_f59,
		*key_f60, *key_f61, *key_f62, *key_f63,
		*key_find,
		*key_help, *key_home,
		*key_ic, *key_il,
		*key_left, *key_ll,
		*key_mark, *key_message, *key_move,
		*key_next, *key_npage,
		*key_open, *key_options,
		*key_ppage, *key_previous, *key_print,
		*key_redo, *key_reference, *key_refresh, *key_replace,
		*key_restart, *key_resume, *key_right,
		*key_save, *key_sbeg, *key_scancel, *key_scommand, *key_scopy,
		*key_screate, *key_sdc, *key_sdl, *key_select, *key_send,
		*key_seol, *key_sexit, *key_sf, *key_sfind, *key_shelp,
		*key_shome, *key_sic, *key_sleft, *key_smessage, *key_smove,
		*key_snext, *key_soptions, *key_sprevious, *key_sprint,
		*key_sr, *key_sredo, *key_sreplace, *key_sright, *key_srsume,
		*key_ssave, *key_ssuspend, *key_stab, *key_sundo, *key_suspend,
		*key_undo, *key_up,
		*reset_1string, *reset_2string, *reset_3string;

/*
 * From the tty modes...
 */

bool	GT, NONL, UPPERCASE, normtty, _pfast;
