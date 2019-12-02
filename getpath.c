/*
**	G E T P A T H
**
**	Copyright 1990, 1992, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Get Path 
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Oct 17 08:40:28 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)getpath.c	4.9 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <stdarg.h>
#include "nmalloc.h"
#include "getpath.h"

#define PATHPRESENT(p)		((*(p)=='/')||(*(p)=='.'))

/*
**	getpath: create a path name in a printf()-kind of way. If
**	a root path is specified, and the path created from the
**	format and its arguments is not absolute, the root path is
**	prepended to create an absolute path.
**
**	Arguments:
**		path	target buffer in which the path is built
**		root	root path (may be NULL)
**		format	printf-format to create file name
**		va_args	components of file name
**
**	Returns:	path
*/
char *
getpath(char *path, char *root, char *format,...)
	{
	va_list args;

	if ((path==NULL)||(format==NULL))
		return(NULL);

	va_start(args,format);

	vsprintf(path,format,args);

	if (!PATHPRESENT(path))
		if (root!=NULL)
			{
			char *f, *t;
			int n, lp, lr;

			lp=strlen(path)+1;
			lr=strlen(root)+1;
			for (n=lp, f=path+lp, t=path+lr+lp; n>0; n--)
				*(--t)=*(--f);
			for (n=lr, f=root, t=path; n>0; n--)
				*(t++)=*(f++);
			*(path+lr-1)='/';
			}

	va_end(args);

	return (path);
	}
