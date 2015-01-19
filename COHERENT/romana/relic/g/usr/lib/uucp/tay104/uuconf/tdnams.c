/* tdnams.c
   Get all known dialer names from the Taylor UUCP configuration files.

   Copyright (C) 1992 Ian Lance Taylor

   This file is part of the Taylor UUCP uuconf library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License
   as published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   The author of the program may be contacted at ian@airs.com or
   c/o Infinity Development Systems, P.O. Box 520, Waltham, MA 02254.
   */

#include "uucnfi.h"

#if USE_RCS_ID
const char _uuconf_tdnams_rcsid[] = "$Id: tdnams.c,v 1.1 93/07/30 08:07:42 bin Exp Locker: bin $";
#endif

#include <errno.h>

static int indialer P((pointer pglobal, int argc, char **argv, pointer pvar,
		       pointer pinfo));

/* Get the names of all the dialers from the Taylor UUCP configuration
   files.  */

int
uuconf_taylor_dialer_names (pglobal, ppzdialers)
     pointer pglobal;
     char ***ppzdialers;
{
  struct sglobal *qglobal = (struct sglobal *) pglobal;
  struct uuconf_cmdtab as[2];
  char **pz;
  int iret;
  
  *ppzdialers = NULL;

  as[0].uuconf_zcmd = "dialer";
  as[0].uuconf_itype = UUCONF_CMDTABTYPE_FN | 2;
  as[0].uuconf_pvar = (pointer) ppzdialers;
  as[0].uuconf_pifn = indialer;

  as[1].uuconf_zcmd = NULL;

  iret = UUCONF_SUCCESS;

  for (pz = qglobal->qprocess->pzdialfiles; *pz != NULL; pz++)
    {
      FILE *e;

      e = fopen (*pz, "r");
      if (e == NULL)
	{
	  if (FNO_SUCH_FILE ())
	    continue;
	  qglobal->ierrno = errno;
	  iret = UUCONF_FOPEN_FAILED | UUCONF_ERROR_ERRNO;
	  break;
	}

      iret = uuconf_cmd_file (pglobal, e, as, (pointer) NULL,
			      (uuconf_cmdtabfn) NULL,
			      UUCONF_CMDTABFLAG_BACKSLASH,
			      (pointer) NULL);

      (void) fclose (e);

      if (iret != UUCONF_SUCCESS)
	break;
    }

  if (iret != UUCONF_SUCCESS)
    {
      qglobal->zfilename = *pz;
      return iret | UUCONF_ERROR_FILENAME;
    }

  if (*ppzdialers == NULL)
    iret = _uuconf_iadd_string (qglobal, (char *) NULL, FALSE, FALSE,
				ppzdialers, (pointer) NULL);

  return UUCONF_SUCCESS;
}

/* Add a dialer name to the list.  */

/*ARGSUSED*/
static int
indialer (pglobal, argc, argv, pvar, pinfo)
     pointer pglobal;
     int argc;
     char **argv;
     pointer pvar;
     pointer pinfo;
{
  struct sglobal *qglobal = (struct sglobal *) pglobal;
  char ***ppzdialers = (char ***) pvar;
  int iret;

  iret = _uuconf_iadd_string (qglobal, argv[1], TRUE, TRUE, ppzdialers,
			      (pointer) NULL);
  if (iret != UUCONF_SUCCESS)
    iret |= UUCONF_CMDTABRET_EXIT;
  return iret;
}
