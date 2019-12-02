/*
**	D U M P A R G S
**
**	Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Dump Arguments
**	Program		TAGS
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Apr  9 09:57:44 MDT 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
*/

static char copyright[]="Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)dumpargs.c	4.4 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>

int
dumpargs(FILE *fp, int argc, char **argv)
	{
	int count, quote;

	if (argc<0)
		{
		argc=(-argc);
		quote=1;
		}
	else
		quote=0;
	for (count=0; argc>0; count++, argc--, argv++)
		{
		if (quote)
			fputc('`',fp);
		fputs(*argv,fp);
		if (quote)
			fputc('\'',fp);
		if (argc>1)
			fputc(' ',fp);
		}
	fputc('\n',fp);
	return(count);
	}
