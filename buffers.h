/*
**	B U F F E R S
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)buffers.h	4.18 94/05/31 jsloan@ncar.ucar.edu
**
*/

#ifndef H_BUFFERS
#define H_BUFFERS

#include <stdio.h>
#include "libtproto.h"

typedef char * BUFFER;

extern int bufhash LIBTPROTO((unsigned int size));

extern int buflength LIBTPROTO((BUFFER string));

extern BUFFER bufalloc LIBTPROTO((int size));

extern int buffree LIBTPROTO((BUFFER string));

extern BUFFER bufdup LIBTPROTO((char *string));

extern BUFFER bufrealloc LIBTPROTO((BUFFER string, int size));

extern BUFFER bufexpand LIBTPROTO((BUFFER string, int size));

extern BUFFER bufinsure LIBTPROTO((BUFFER string, int size));

extern int bufpool LIBTPROTO((FILE *fp));

extern void bufshutdown LIBTPROTO((void));

#endif /* !H_BUFFERS */
