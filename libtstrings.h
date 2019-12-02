/*
**	L I B T S T R I N G S
**
**	Copyright 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)libtstrings.h	4.10 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_LIBTSTRINGS
#define H_LIBTSTRINGS

#ifdef MVS_LIBTOOLS
#include <bsdtocms.h>
#endif /* MVS_LIBTOOLS */

#ifdef STRINGS_LIBTOOLS
#include <strings.h>
#ifdef INDEX_LIBTOOLS
#define strchr	index
#define strrchr	rindex
#endif /* INDEX_LIBTOOLS */
#else /* STRINGS_LIBTOOLS */
#include <string.h>
#endif /* STRINGS_LIBTOOLS */

#ifndef STRINGPRINT
#define STRINGPRINT(s)	(((s)==NULL)?"(NULL)":((*(s)=='\0')?"\"\"":(s)))
#endif /* !STRINGPRINT */

#endif /* !H_LIBTSTRINGS */
