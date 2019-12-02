/*
**	E R R O R S
**
**	Copyright 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)errors.h	4.1 94/05/16 jsloan@ncar.ucar.edu
*/

#ifndef H_ERRORS
#define H_ERRORS

#include <errno.h>
#include "libtproto.h"

/*
**	It's not clear that defining these to be the same values
**	as on other UNIXen is doing anyone any favors.
*/

#ifndef EINTR
#define EINTR		256		/* interrupted system call */
#endif /* !EINTR */
#ifndef E2BIG
#define E2BIG		257		/* arg list too long */
#endif /* !E2BIG */
#ifndef EINVAL
#define EINVAL		258		/* Invalid argument */
#endif /* !EINVAL */
#ifndef ENOSYS
#define ENOSYS		259		/* function not implemented */
#endif /* !ENOSYS */

#ifdef ERRNO_LIBTOOLS

#ifdef seterrno
#undef seterrno
#endif /* seterrno */

extern int seterrno LIBTPROTO((int error));

#ifdef geterrno
#undef geterrno
#endif /* geterrno */

extern int geterrno LIBTPROTO((void));

#else /* ERRNO_LIBTOOLS */

#ifndef seterrno
#define	seterrno(e)	(errno=(e))
#endif /* !seterrno */

#ifndef geterrno
#define geterrno()	(errno)
#endif /* !geterrno */

#endif /* ERRNO_LIBTOOLS */

extern char *errnotxt LIBTPROTO((int err, char *msg));
extern char *errtxt LIBTPROTO((char *msg));

#endif /* !H_ERRORS */
