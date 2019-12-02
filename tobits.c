/*
**	T O B I T S
**
**	Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		To Bits
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Jan 12 12:09:28 MST 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
*/

static char copyright[]="Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)tobits.c	4.8 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libtvalues.h"

/*
**	tobits: returns a string in a user buffer that contains
**	ASCII 1s and 0s equivalent to the bit pattern in the supplied
**	data.
**
**	e.g.	tobits((unsigned char *)&integer,sizeof(integer),buffer);
*/
char *
tobits(unsigned char *field, int length, char *buffer)
	{
	int byte, bit;
	char *p;
	unsigned char c;

	for (p=buffer, byte=length; byte>0; byte--)
		for (c=(*(field++)), bit=BITSPERBYTE; bit>0; bit--, c=c<<1)
			*(p++)=((c&((char)(1<<(BITS(char)-1))))!=0)?'1':'0';
	*p='\0';
	return(buffer);
	}
