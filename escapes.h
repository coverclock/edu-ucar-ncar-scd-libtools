/*
**	E S C A P E S
**
**	Copyright 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)escapes.h	4.9 94/09/29 jsloan@ncar.ucar.edu
**
*/

#ifndef H_ESCAPES
#define H_ESCAPES

#include "buffers.h"
#include "libtproto.h"

extern char *esclapseto LIBTPROTO((char *from, char *to));

extern char *escpandto LIBTPROTO((char *from, char *to));

extern BUFFER esclapsenew LIBTPROTO((char *from));

extern BUFFER escpandnew LIBTPROTO((char *from));

extern BUFFER esclapse LIBTPROTO((BUFFER old));

extern BUFFER escpand LIBTPROTO((BUFFER old));

extern int esclapsein LIBTPROTO((char *string, int length));

#endif /* !H_ESCAPES */
