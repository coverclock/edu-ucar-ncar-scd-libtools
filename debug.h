/*
**	D E B U G
**
**	U N D E R   D E V E L O P M E N T
**
**	Copyright 1992, 1993 University Corporation for Atmospheric Research
**	All Rights Reserved
**
**	@(#)debug.h	4.8 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_DEBUG
#define H_DEBUG

#include "libtproto.h"

#define DEBUGGING(l)	(debug>=(l))
#define DEBUGF(l,s)	(debug>=(l) ? fprintf s : 0)
#define DEBUGON(l)	(debug=(l))
#define DEBUGOFF()	(debug=0)

extern int debugging LIBTPROTO((char *keyword, int level));

#endif /* !H_DEBUG */
