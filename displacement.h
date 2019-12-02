/*
**	D I S P L A C E M E N T
**
**	Copyright 1990, 1991, 1992 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)displacement.h	4.5 94/05/16 jsloan@ncar.ucar.edu
**
**	THIS HEADER FILE IS PROVIDED FOR BACKWARD COMPATIBILITY. It has
**	be replaced by the OFFSET macro in offset.h, which has slightly
**	different arguments, and which uses the ANSI C standard macro
**	offsetof available in stddef.h on ANSI-compliant systems. If
**	I were you, I'd advert my eyes and not use it.
**
**	This macro calculates the displacement of a field in a
**	structure. This code is illegal in C, and is certainly
**	questionable practice. I used it when necessary (and when I'm
**	convinced it works on the target architecture), but try not to
**	talk about it too much. None of the functions in libtools or
**	libipc use it, so its failure to port to a new platform is not
**	a major cause for concern.
*/

#ifndef H_DISPLACEMENT
#define H_DISPLACEMENT

#define DISPLACEMENT(s,f1,f2) ((int)(((char *)&(((s)0)->f1))-((char *)&(((s)0)->f2))))

#endif /* !H_DISPLACEMENT */
