/*
**	S I G F U L L D U M P
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Signal Full Dump
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu Jun 30 15:36:47 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Some systems, who will remain nameless, but whose initials are
**	AIX, do not generate a full core dump when a process aborts.
**	This is a feature, and makes debugging especially challenging.
**	This little function deals with this by causing those systems
**	(and you know who you are) to generate a full core dump when a
**	specified signal occurs. This should be done after any signal
**	handlers are installed, since the installation of a signal handler
**	may reset the full dump flag for that signal.
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)sigfulldump.c	1.1 94/06/30 jsloan@ncar.ucar.edu";

#include <stdio.h>
#include <signal.h>
#include "sigfulldump.h"

int
sigfulldump(int sig)
	{
#ifdef SA_FULLDUMP
	struct sigaction action;

	if (sigaction(sig,NULL,&action)<0)
		return(-1);

	action.sa_flags|=SA_FULLDUMP;

	if (sigaction(sig,&action,NULL)<0)
		return(-2);

	return(0);
#else /* SA_FULLDUMP */
	return(1);
#endif /* SA_FULLDUMP */
	}
