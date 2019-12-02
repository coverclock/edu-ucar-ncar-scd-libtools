/*
**	A R G V E C T O R
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)argvector.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_ARGVECTOR
#define H_ARGVECTOR

#include "parcel.h"
#include "portfolio.h"
#include "libtproto.h"

extern PARCEL *bundlevector LIBTPROTO((int argc, char **argv, PARCEL *parcelp));

extern char **unbundlevector LIBTPROTO((PARCEL parcel));

extern PORTFOLIO *stuffvector LIBTPROTO((int argc, char **argv, PORTFOLIO *portfolio));

extern char **unstuffvector LIBTPROTO((PORTFOLIO *portfolio));

extern int freevector LIBTPROTO((char **argv));

#endif /* !H_ARGVECTOR */
