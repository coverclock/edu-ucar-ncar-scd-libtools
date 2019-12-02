/*
**	N E X T P A R S E R
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)nextparser.h	4.8 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_NEXTPARSER
#define H_NEXTPARSER

#include <stdio.h>
#include "libtproto.h"

extern char *nextfrombuffer LIBTPROTO((char *in, char *out, int len));

extern int nextfromfile LIBTPROTO((FILE *fp, char *out, int len));

#endif /* !H_NEXTPARSER */
