/*
**	R I N G S
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)rings.h	4.7 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_RINGS
#define H_RINGS

#include "libtproto.h"

struct ring
	{
	char	*r_base;
	char	*r_past;
	char	*r_from;
	char	*r_to;
	int	r_used;
	int	r_free;
	};

typedef struct ring RING;

extern int rextract LIBTPROTO((RING *r, char *cp));

extern int rpeek LIBTPROTO((RING *r, char *cp));

extern int rinsert LIBTPROTO((RING *r, char c));

extern RING *rinit LIBTPROTO((RING *r, int n));

extern RING *rreinit LIBTPROTO((RING *r, int n));

#endif /* !H_RINGS */
