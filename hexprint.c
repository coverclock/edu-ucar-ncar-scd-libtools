/*
**	H E X P R I N T
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Hexadecimal Printing
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Aug  9 10:16:37 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	hexprint is a simple little routine useful in debugging.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)hexprint.c	4.8 94/06/02 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "hexprint.h"

/*
**	hexprint: prints in hex a data field of a given width. No
**	terminating newline is printed.
**
**	Arguments:
**		file	file pointer
**		string	pointer to data area
**		length	length in bytes of data area
**
**	Returns:	address just past end of string
*/
char *
hexprint(FILE *fp, char *string, int length)
	{
	int len;
	unsigned char *s;

	s=(unsigned char *)string;
	for (len=(length<0)?(-length):length; len>0; len--)
		{
		fprintf(fp,"%02.02x",*(s++));
		if ((length<0)&&(len>1))
			fputc('.',fp);
		}
	return(string);
	}
