/* dnams.c
   Get all known dialer names.

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
const char _uuconf_dnams_rcsid[] = "$Id: dnams.c,v 1.1 93/07/30 08:07:08 bin Exp Locker: bin $";
#endif

/* Get all known dialer names.  */

int
uuconf_dialer_names (pglobal, ppzdialers)
     pointer pglobal;
     char ***ppzdialers;
{
  struct sglobal *qglobal = (struct sglobal *) pglobal;
  char **pztaylor;
  char **pzhdb;
  int iret;

  *ppzdialers = NULL;
  pztaylor = NULL;
  pzhdb = NULL;

#if HAVE_TAYLOR_CONFIG
  iret = uuconf_taylor_dialer_names (pglobal, &pztaylor);
  if (iret != UUCONF_SUCCESS)
    return iret;
#endif

#if HAVE_HDB_CONFIG
  if (qglobal->qprocess->fhdb)
    {
      iret = uuconf_hdb_dialer_names (pglobal, &pzhdb);
      if (iret != UUCONF_SUCCESS)
	return iret;
    }
#endif

  if (pzhdb == NULL)
    *ppzdialers = pztaylor;
  else if (pztaylor == NULL)
    *ppzdialers = pzhdb;
  else
    {
      char **pz;

      iret = UUCONF_SUCCESS;

      for (pz = pztaylor; *pz != NULL; pz++)
	{
	  iret = _uuconf_iadd_string (qglobal, *pz, FALSE, TRUE,
				      ppzdialers, (pointer) NULL);
	  if (iret != UUCONF_SUCCESS)
	    break;
	}

      if (iret == UUCONF_SUCCESS)
	{
	  for (pz = pzhdb; *pz != NULL; pz++)
	    {
	      iret = _uuconf_iadd_string (qglobal, *pz, FALSE, TRUE,
					  ppzdialers, (pointer) NULL);
	      if (iret != UUCONF_SUCCESS)
		break;
	    }
	}

      if (pztaylor != NULL)
	free ((pointer) pztaylor);
      if (pzhdb != NULL)
	free ((pointer) pzhdb);
    }

  if (iret == UUCONF_SUCCESS && *ppzdialers == NULL)
    iret = _uuconf_iadd_string (qglobal, (char *) NULL, FALSE, FALSE,
				ppzdialers, (pointer) NULL);

  return iret;
}
