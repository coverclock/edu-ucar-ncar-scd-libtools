/*
**	M A Y B E N U L L
**
**	Copyright 1996 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Maybe Null
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri May 24 13:26:48 MDT 1996
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Simple little routine done for the SOLARIS port regression
**	testing that handles printing null addresses in %s format.
*/

static char copyright[]="Copyright 1996 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)maybenull.c	1.3 96/06/04 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include "maybenull.h"
#include <stdio.h>

/*
**	maybenull:	pass address through unchanged if not NULL,
**			else pass along address of "NULL" constant.
*/
char *
maybenull(void *c)
	{
	return((c==(void *)NULL)?"(null)":(char *)c);
	}
