/*
**	C O N C A T S
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)concats.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_CONCATS
#define H_CONCATS

#include "libtproto.h"

extern char *concats LIBTPROTO((char *buffer,...));
extern char *appends LIBTPROTO((char *buffer,...));

#endif /* !H_CONCATS */
