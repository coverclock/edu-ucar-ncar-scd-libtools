/*
**	O R I G I N
**
**	Copyright 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)origin.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
**	This macro calculates the origin of a structure given a pointer
**	to a field in the structure. It tries to do so in an ANSI C
**	compliant manner, but never the less is still questionable
**	practice. None of the functions in libtools or libipc use it, so
**	its failure to port to a new platform is not a major cause for
**	concern. Caveat Emptor.
*/

#ifndef H_ORIGIN
#define H_ORIGIN

#include "offset.h"

/* #define ORIGIN(p,t,f) ((t *)(((char *)(p))-((int)(&(((t *)0)->f))))) */

#define ORIGIN(p,t,f)	((t *)(((char *)(p))-OFFSET(t,f)))

#endif /* !H_ORIGIN */
