/*
**	A T O A R G V
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)atoargv.h	4.6 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_ATOARGV
#define H_ATOARGV

#include "libtproto.h"

#define ATOARGVLEN	1024

extern char **atoargv LIBTPROTO((char *name, char *string));

extern int freeargv LIBTPROTO((char **argv));

#endif /* !H_ATOARGV */
