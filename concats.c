/*
**	C O N C A T S
**
**	Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Concatenate Strings
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Jul 30 16:12:28 MDT 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)concats.c	4.10 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <stdarg.h>
#include "concats.h"

#define NEXT	va_arg(args,char *)

/*
**	concats: concatenate a series of strings into a single
**	destination string.
**
**	appends: same as concats, but appends to the end of
**	the destination string.
**
**	Arguments:
**		d	destination string, assummed large enough
**		s1	first source string
**		 :
**		sN	last source string
**		sN+1	must be equal to NULL
**
**	Returns:	address of destination string or NULL if error
*/
char *
concats(char *buffer,...)
	{
	va_list args;
	char *p, *s, *d;

	va_start(args,buffer);
	if ((d=buffer)!=NULL)
		for (*d='\0', p=d, s=NEXT; s!=NULL; s=NEXT, *p='\0')
			for (; *s!='\0'; )
				*(p++)=(*(s++));
	va_end(args);
	return(d);
	}

char *
appends(char *buffer,...)
	{
	va_list args;
	char *p, *s, *d;

	va_start(args,buffer);
	if ((d=buffer)!=NULL)
		for (p=d+strlen(d), s=NEXT; s!=NULL; s=NEXT, *p='\0')
			for (; *s!='\0'; )
				*(p++)=(*(s++));
	va_end(args);
	return(d);
	}
