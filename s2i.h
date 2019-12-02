/*
**	S 2 I
**
**	Copyright 1993	University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)s2i.h	4.2 96/06/12 jsloan@ncar.ucar.edu
*/

#ifndef H_S2I
#define H_S2I

#include "libtproto.h"

extern int s2c LIBTPROTO((char *string, char *cp));
#ifdef SSCANF_LIBTOOLS
#define s2s(sp,hp)	sscanf(sp,"%hd",hp)
#define s2i(sp,ip)	sscanf(sp,"%d",ip)
#define s2l(sp,lp)	sscanf(sp,"%ld",lp)
#else /* SSCANF_LIBTOOLS */
extern int s2s LIBTPROTO((char *string, short *hp));
extern int s2i LIBTPROTO((char *string, int *ip));
extern int s2l LIBTPROTO((char *string, long *lp));
#endif /* SSCANF_LIBTOOLS */
extern int s2x LIBTPROTO((char *string, void *xp, int szof));

#endif /* !H_S2I */
