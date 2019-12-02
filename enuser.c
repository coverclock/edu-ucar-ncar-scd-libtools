/*
**	E N U S E R
**
**	Copyright 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Enuser
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed May  5 12:16:31 MDT 1993
**	Organization	NCAR/SCD, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Become another user. Then become yourself again.
**
**	The application using this must be setuid root. This will
**	probably not work on pure POSIX-compliant systems.
**
**	BTW, I can't really test this code since I don't have root
**	anywhere except on an RS6K AIX system. Hence, the BSD stuff
**	(although I've written such code in prior reincarnations)
**	has never been tested.
*/

static char copyright[]="Copyright 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)enuser.c	4.3 94/05/31 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <unistd.h>
#include "libttypes.h"

#ifdef ENUSER_SETEUID

int
enuser(uid_t uid, gid_t gid)
	{
	if (setegid(gid)<0)
		return(-1);
	if (seteuid(uid)<0)
		return(-2);
	return(0);
	}

int
deuser(void)
	{
	uid_t uid;
	gid_t gid;

	gid=getgid();
	uid=getuid();
	if (setegid(gid)<0)
		return(-5);
	if (seteuid(uid)<0)
		return(-6);
	return(0);
	}

#else /* ENUSER_SETEUID */
#ifdef ENUSER_SETUIDX

/*
** This works under AIX, but has some undesirable side effects
** regarding unwanted capabilities after enuser() executes. Thanks
** to John Merrill (jhm@ncar.ucar.edu) for this trick.
*/

#include <sys/id.h>

int
enuser(uid_t uid, gid_t gid)
	{
	if (setreuid(0,0)<0)
		return(-7);
	if (setregid(0,0)<0)
		return(-8);
	if (setgidx(ID_REAL|ID_EFFECTIVE,gid)<0)
		return(-9);
	if (setuidx(ID_REAL|ID_EFFECTIVE,uid)<0)
		return(-10);
	return(0);
	}

int
deuser(void)
	{
	uid_t uid;
	gid_t gid;

	gid=getgidx(ID_SAVED);
	uid=getuidx(ID_SAVED);
	if (setuid(uid)<0)
		return(-13);
	if (setgid(gid)<0)
		return(-14);
	}

#else /* ENUSER_SETUIDX */

int
enuser(uid_t uid, gid_t gid)
	{
	return(-15);
	}

int
deuser(void)
	{
	return(-16);
	}

#endif /* ENUSER_SETUIDX */
#endif /* ENUSER_SETEUID */
