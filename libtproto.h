/*
**	L I B T P R O T O
**
**	Copyright 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)libtproto.h	4.1 94/05/16 jloan@ncar.ucar.edu
**
**	This is a little macro that simplifies prototyping function
**	declarations for porting between ANSI C and non-ANSI C
**	compilers.
*/

#ifndef H_LIBTPROTO
#define H_LIBTPROTO

#ifdef PROTOTYPE_LIBTOOLS
#define LIBTPROTO(list)	list
#else /* PROTOTYPE_LIBTOOLS */
#define LIBTPROTO(list)	()
#endif /* PROTOTYPE_LIBTOOLS */

#endif /* !H_LIBTPROTO */
