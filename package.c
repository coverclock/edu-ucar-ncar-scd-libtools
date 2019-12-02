/*
**	P A C K A G E
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Package
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Sep 26 15:44:57 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	The functions in this module implement the PACKAGE data structure.
**	A PACKAGE is a ASCII string containing a parameter list
**	in human readable form. This structure can be easily
**	printed or read.
**
**	See also package2.c and package3.c.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)package.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "buffers.h"
#include "package.h"

/*
**	freepackage: free memory associated with package structure.
*/
int
freepackage(PACKAGE *package)
	{
	return(package==NULL?0:buffree(package));
	}
