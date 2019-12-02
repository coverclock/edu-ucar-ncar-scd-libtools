/*
**	P A R C E L
**
**	Copyright 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)parcel.h	4.28 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_PARCEL
#define H_PARCEL

#include <stdio.h>
#include "libtstrings.h"
#include "keyval.h"
#include "libtproto.h"

typedef KEYVAL * PARCEL;

#define PARCELQUOTE(string)       (strpbrk(string," =\t")!=NULL)
/* #define PARCELQUOTE(string)       ((strchr(string,' ')!=NULL)||(strchr(string,'\t')!=NULL)||(strchr(string,'=')!=NULL)) */

/*
** Largest word possible in a parcel (for performance
** reasons in nextparser(3l), this is not dynamically
** resized as the string is filled, as is common elsewhere)
*/
#define PARCELWORD	(4096)

extern PARCEL *initparcel LIBTPROTO((PARCEL *parcelp));

extern PARCEL *bundle LIBTPROTO((char *parm, char *value, PARCEL *parcelp));

extern PARCEL *bundlep LIBTPROTO((BUFFER parm, BUFFER value, PARCEL *parcelp));

extern char *unbundle LIBTPROTO((char *parm, PARCEL parcel));

extern PARCEL *rebundle LIBTPROTO((PARCEL parcel, PARCEL *parcelp));

extern PARCEL *enbundle LIBTPROTO((PARCEL parcel, PARCEL *parcelp));

extern int countparcel LIBTPROTO((PARCEL parcel));

extern int freeparcel LIBTPROTO((PARCEL parcel));

extern int travparcel LIBTPROTO((PARCEL parcel, int (*g)(), void *vp));

/*
** The stuff below can be found in the ancillary module parcel2.
*/

extern int fmtparcel LIBTPROTO((PARCEL parcel, char *fmt, char *buffer));

extern void dumparcel LIBTPROTO((PARCEL *parcel, FILE *fp));

extern PARCEL *bundlef LIBTPROTO((char *parm, PARCEL *parcelp, char *buffer, char *format,...));

extern char *unbundlef LIBTPROTO((char *parm, PARCEL parcel, char *format, void *argp));

#endif /* !H_PARCEL */
