/* fresys.c
   Free system information.

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
const char _uuconf_fresys_rcsid[] = "$Id: fresys.c,v 1.1 93/07/30 08:07:12 bin Exp Locker: bin $";
#endif

/* Free the memory allocated for a system.  */

#undef uuconf_system_free

/*ARGSUSED*/
int
uuconf_system_free (pglobal, qsys)
     pointer pglobal;
     struct uuconf_system *qsys;
{
  uuconf_free_block (qsys->uuconf_palloc);
  return UUCONF_SUCCESS;
}
