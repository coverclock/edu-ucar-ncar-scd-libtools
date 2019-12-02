/*
**	O F F S E T
**
**	Copyright 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)offset.h	4.2 95/09/14 jsloan@ncar.ucar.edu
**
**	This macro calculates the byte displacement of a field within
**	a structure. This code uses an ANSI C macro (when available),
**	but even so is questionable practice. None of the functions in
**	libtools or libipc use it, so its failure to port to a new platform
**	is not a major cause for concern.
**
**	N.B.	although offsetof() is defined in the ANSI C compiler
**		available on CRAY YMP platforms, the alternate "roll
**		your own" definition optionally used here does in fact
**		yield the correct result in the egoriginoffset
**		regression test.
*/

#ifndef H_OFFSET
#define H_OFFSET

#include <stddef.h>

#ifndef offsetof
/* Assume sizeof_t isn't defined either */
#define offsetof(t,m)	((int)(((char *)&(((t *)0)->m))-((char *)((t *)0))))
#endif /* offsetof */

#define OFFSET(t,f)	offsetof(t,f)

#endif /* !H_OFFSET */
