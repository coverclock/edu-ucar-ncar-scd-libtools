/*
**	U S R _ E R R L I S T
**
**	Copyright 1992 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		User Error List
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Nov 13 13:05:31 MST 1992
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This is the default user error list array if the user
**	doesn't link in a different one.
*/

static char copyright[]="Copyright 1992 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)usr_errlist.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"

int usr_ferr=0;
int usr_lerr=0;

char *usr_errlist[]=
	{
	"Error 0"
	};

#ifdef MVS_LIBTOOLS
int sys_nerr=0;

char *sys_errlist[]=
	{
	"Error 0"
	};
#endif /* MVS_LIBTOOLS */
