/*
**	E R R T X T
**
**	Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Error Text
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Oct 24 14:38:55 MDT 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)errtxt.c	4.11 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libtvalues.h"
#include "libtstrings.h"
#include "errors.h"

extern int sys_nerr;
extern char *sys_errlist[];
extern int usr_ferr, usr_lerr;
extern char *usr_errlist[];

char *
errnotxt(int err, char *msg)
	{
	char *txt, buffer[sizeof("Error ")+BITS(int)];

	if ((0<err)&&(err<sys_nerr))
		txt=sys_errlist[err];

	else if ((usr_ferr<=err)&&(err<=usr_lerr))
		txt=usr_errlist[err-usr_ferr];

	else
		{
		(void)sprintf(buffer,"Error %d",err);
		txt=buffer;
		}

	(void)strcpy(msg,txt);

	return(msg);
	}

char *
errtxt(char *msg)
	{
	return(errnotxt(geterrno(),msg));
	}
