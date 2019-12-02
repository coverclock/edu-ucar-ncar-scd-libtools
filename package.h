/*
**	P A C K A G E
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)package.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_PACKAGE
#define H_PACKAGE

#include "parcel.h"
#include "portfolio.h"
#include "libtproto.h"

/*
** Starting size for a package
*/
#define PACKAGESIZE	(32)

typedef char PACKAGE;

extern PACKAGE *unbundlepackage LIBTPROTO((PARCEL parcel));

extern PARCEL *bundlepackage LIBTPROTO((PACKAGE *package, PARCEL *parcelp));

extern PACKAGE *unstuffpackage LIBTPROTO((PORTFOLIO *portfolio));

extern PORTFOLIO *stuffpackage LIBTPROTO((PACKAGE *package, PORTFOLIO *portfolio));

extern int freepackage LIBTPROTO((PACKAGE *package));

#endif /* !H_PACKAGE */
