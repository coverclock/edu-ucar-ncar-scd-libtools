/*
**	R E A D S W R I T E S
**
**	Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Reads and Writes
**	Program		TAGS
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Mar 13 10:32:03 MST 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	The casual user may note that using the standard I/O library
**	is a better idea. This module is mostly here to be used with
**	sockets and what-not.
*/

static char copyright[]="Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)readswrites.c	4.6 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>

int
reads(int fd, char *b, int l)
	{
	int n, t;

	for (n=0; n<l; n+=t)
		if ((t=read(fd,b+n,l-n))<=0)
			return(t);
	return(n);
	}

int
writes(int fd, char *b, int l)
	{
	int n, t;

	for (n=0; n<l; n+=t)
		if ((t=write(fd,b+n,l-n))<=0)
			return(t);
	return(n);
	}
