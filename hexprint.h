/*
**	H E X P R I N T
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)hexprint.h	1.1 94/06/02 jsloan@ncar.ucar.edu
*/

#ifndef H_HEXPRINT
#define H_HEXPRINT

#include <stdio.h>
#include "libtproto.h"

extern char *hexprint LIBTPROTO((FILE *file, char *string, int length));

#endif /* !H_HEXPRINT */
