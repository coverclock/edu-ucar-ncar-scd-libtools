/*
**	B L O C K L E T T E R
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)blockletter.h	4.6 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_BLOCKLETTER
#define H_BLOCKLETTER

#include <stdio.h>
#include "libtproto.h"

#define BlockLetter(t,c)	(((t)[(c)]==NULL)?(t)[' ']:(t)[(c)])

extern char **BlockTable LIBTPROTO((char *path));

extern int BlockFree LIBTPROTO((char **table));

extern int BlockDump LIBTPROTO((char **table,  FILE *fp));

#endif /* !H_BLOCKLETTER */
