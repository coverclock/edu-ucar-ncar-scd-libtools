/*
**	L O C K S M I T H
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Locksmith
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Dec 19 09:47:47 MST 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This module contains functions that offer a more friendly
**	interface to the network lock daemon functions.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)locksmith.c	4.8 96/06/10 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <fcntl.h>
#include "errors.h"
#include "locksmith.h"

/*
**	locksmith: use fcntl(2) to perform network-wide locking on
**	an entire file using lockd(8).
**
**	Arguments:
**		fd	file descriptor to be locked.
**		flag	-2=exclusive non-blocking write lock
**			-1=shared non-blocking read lock
**			 0=unlock
**			 1=shared blocking read lock
**			 2=exclusive blocking write lock
**
**	Returns:	0 if okay, -1 if fcntl error, -2 if bad
**			arguments, -3 if non-blocking and interrupted.
*/
int
locksmith(int fd, int flag)
	{
	struct flock cb;

	switch (flag)
		{
	case LOCKSMITH_UNLOCK:
		cb.l_type=F_UNLCK;
		break;
	case LOCKSMITH_NSHARED:
	case LOCKSMITH_SHARED:
		cb.l_type=F_RDLCK;
		break;
	case LOCKSMITH_NEXCLUSIVE:
	case LOCKSMITH_EXCLUSIVE:
		cb.l_type=F_WRLCK;
		break;
	default:
		seterrno(EINVAL);
		return(LOCKSMITH_INVALID);
		}
	cb.l_whence=0;
	cb.l_start=0;
	cb.l_len=0;
	/*
	** Retry file locking if the caller specified waiting
	** and the call is interrupted.
	*/
	while (fcntl(fd,(flag<0)?F_SETLK:F_SETLKW,&cb)==(-1))
		if (geterrno()!=EINTR)
			return(LOCKSMITH_FAILED);
		else if (flag<0)
			return(LOCKSMITH_INTERRUPT);
	return(0);
	}
