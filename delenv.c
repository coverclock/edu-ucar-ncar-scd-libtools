/*
**	D E L E N V
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Delete Environmental Variable
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Sep 11 13:42:54 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function supplies what getenv and putenv lack, a way to
**	delete an environmental variable.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)delenv.c	4.10 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "nmalloc.h"
#include "libtstrings.h"

int
delenv(char *parm)
	{
	extern char **environ;
	char **old, **new, *ptr;

	for (old=environ; *old!=NULL; old++)
		{
		if ((ptr=strchr(*old,'='))!=NULL)
			*ptr='\0';
		if (strcmp(parm,*old)==0)
			break;
		if (ptr!=NULL)
			*ptr='=';
		}
	if (old!=NULL)
		{
		nfree((char *)old);
		for (new=old+1; *new!=NULL; )
			*(old++)=(*(new++));
		*old=NULL;
		}
	return(0);
	}
