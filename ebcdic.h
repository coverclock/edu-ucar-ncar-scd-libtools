/*
**	E B C D I C
**
**	Copyright 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)ebcdic.h	4.6 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_EBCDIC
#define H_EBCDIC

#include "libtproto.h"

extern char ncare2a[], ncara2e[];

#define ebcdic2ascii	ncare2a
#define ascii2ebcdic	ncara2e

extern int tablexlate LIBTPROTO((char *buf, int len, char *table));

#define Ebcdic2Ascii(buf,len)	tablexlate(buf,len,ebcdic2ascii)
#define Ascii2Ebcdic(buf,len)	tablexlate(buf,len,ascii2ebcdic)

#endif /* !H_EBCDIC */
