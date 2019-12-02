/*
**	D u p
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Dup
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu May 19 08:42:46 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	For whatever reason, dup(2) and dup2(2) are not implemented
**	under AIX's OSF DCE pthreads, but fcntl(2). While it is true that
**	fcntl is POSIX compliant, dup and dup2 are defined in the POSIX
**	standard for historical reasons, and both can be implemented
**	in terms of fcntl.
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)dup.c	1.1 94/05/19 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef NODUP_LIBTOOLS
int
dup(int old)
	{
	return(fcntl(old,F_DUPFD,0));
	}

int
dup2(int old, int new)
	{
	if (old==new)
		return(new);
	(void)close(new);
	return(fcntl(old,F_DUPFD,new));
	}
#endif /* NODUP_LIBTOOLS */
