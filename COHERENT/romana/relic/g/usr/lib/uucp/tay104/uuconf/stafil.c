/* stafil.c
   Get the name of the UUCP statistics file.

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
const char _uuconf_stafil_rcsid[] = "$Id: stafil.c,v 1.1 93/07/30 08:07:33 bin Exp Locker: bin $";
#endif

/* Get the name of the UUCP statistics file.  */

int
uuconf_statsfile (pglobal, pzstats)
     pointer pglobal;
     const char **pzstats;
{
  struct sglobal *qglobal = (struct sglobal *) pglobal;

  *pzstats = qglobal->qprocess->zstatsfile;
  return UUCONF_SUCCESS;
}
