/*
**	B A S E N A M E
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Base Name
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Oct 18 09:23:34 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)basename.c	4.7 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libtstrings.h"

/*
**	basename: returns a pointer to the basename portion of
**	the supplied pathname (i.e. the file name without the
**	leading path.
*/
char *
basename(char *path)
	{
	char *name;

	if ((name=strrchr(path,'/'))==NULL)
		return(path);
	else
		return(++name);
	}
