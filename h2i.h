/*
**	H 2 I
**
**	Copyright 1994	University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)h2i.h	1.1 94/07/14 jsloan@ncar.ucar.edu
*/

#ifndef H_H2I
#define H_H2I

#include "libtproto.h"

#ifdef SSCANF_LIBTOOLS

#define h2i(sp,ip)	sscanf(sp,"%x",ip)
#define h2l(sp,lp)	sscanf(sp,"%lx",lp)
#define h2s(sp,hp)	sscanf(sp,"%hx",hp)

#else /* SSCANF_LIBTOOLS */

extern int h2i LIBTPROTO((char *string, int *ip));
extern int h2l LIBTPROTO((char *string, long *lp));
extern int h2s LIBTPROTO((char *string, short *hp));

#endif /* SSCANF_LIBTOOLS */

#endif /* !H_H2I */
