/* globals.c: these are our global variable, structs, file pointers... */

#include <stdio.h>
#include <curses.h>
#include <fcntl.h>
#include "uuinstall.h"

char names[MAXENTRIES][15];	/* array of port names read */
int  startpos[MAXENTRIES];	/* corresponding start line in file */
int  total_entries_found;	/* total number opf ports read from file */
short	dialflag;		/* flag to indicate that we're working w/dial */
short	sysflag;		/* flag to indicate that we're working w/sys */
short	portflag;		/* flag to indicate that we're working w/port */
char	action;			/* action to be performed on a file entry. */
char	litestring[15];		/* used to highlight and select a name */

/* selection window coordinates used to highlight and select entries */
int newrow, prevrow, newcol, prevcol;

WINDOW *selwin;			/* window of existing entries */
WINDOW *promptwin;		/* window to print prompts in */
WINDOW *menwin;			/* window of menu options */
WINDOW *portwin;		/* view/create/edit port file entries */


/* functions */
char * get_data();

/* character arrays to hold port info when entering port data */

char portname[22];
char porttype[14];
char portdev[22];
char portspeed[8];
char portdial[22];


/* character arrays to hold dial info when entring dialer data */

char dialname[24];
char dialchat[85];
char dialtout[17];
char dialfail1[30];
char dialfail2[30];
char dialfail3[30];
char dialplete[85];
char dialabort[85];

/* character arrays to hold sys info when entering data */

char sysname[17];
char systime[71];
char sysspeed[12];
char sysport[21];
char sysphone[71];
char syschat[87];
char sysmyname[16];
char sysprot[15];
char syscmds[87];
char sysread[87];
char syswrite[87];
