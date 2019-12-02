/*
**	N E W   M A L L O C
**
**	Copyright 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)newmalloc.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_NEWMALLOC
#define H_NEWMALLOC

#ifndef NMALLOC_LIBTOOLS
#define NMALLOC_LIBTOOLS
#endif /* NMALLOC_LIBTOOLS */
#include "nmalloc.h"

#define malloc(s)	(nmalloc(s))
#define realloc(p,s)	(nrealloc((p),(s)))
#define free(p)		(nfree(p))

#endif /* !H_NEWMALLOC */
