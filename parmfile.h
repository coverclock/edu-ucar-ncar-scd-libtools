/*
**	P A R M F I L E
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)parmfile.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_PARMFILE
#define H_PARMFILE

#include "parcel.h"
#include "portfolio.h"
#include "libtproto.h"

/*
** Largest word possible in a parmfile (for performance
** reasons in nextparser(3l), this is not dynamically
** resized as the string is filled, as is common elsewhere)
*/
#define PARMFILEWORD	(4096)

extern int bundleparmfile LIBTPROTO((int fd, PARCEL *parcelp));

extern int unbundleparmfile LIBTPROTO((int fd, PARCEL parcel));

extern int stuffparmfile LIBTPROTO((int fd, PORTFOLIO *portfolio));

extern int unstuffparmfile LIBTPROTO((int fd, PORTFOLIO *portfolio));

#endif /* !H_PARMFILE */
