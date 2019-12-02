/*
**	L I B T P A R A M
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)libtparam.h	1.5 96/05/28 jsloan@ncar.ucar.edu
*/

#ifndef H_LIBTPARAM
#define H_LIBTPARAM

#include <sys/param.h>
/*
**	N.B.	netdb.h is only included here as a result of the
**		SOLARIS port. SOLARIS moved the MAXHOSTNAMELEN definition
**		from sys/param.h to netdb.h.
*/
#include <netdb.h>

#ifndef MAXPATHLEN
#ifdef PATH_MAX
#define MAXPATHLEN	(PATH_MAX+1)
#else /* PATH_MAX */
#define MAXPATHLEN	(256)
#endif /* PATH_MAX */
#endif /* MAXPATHLEN */

#endif /* !H_LIBTPARAM */
