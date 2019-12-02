/*
**	A R G V E C T O R
**
**	Copyright 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Argument Vector
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Sep 26 15:44:57 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	The functions in this module provide for translation between
**	PARCELS and (char **) vectors such as argument and environment
**	variable lists.
**
**	See also argvector2.c and argvector3.c.
*/

static char copyright[]="Copyright 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)argvector.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "buffers.h"
#include "nmalloc.h"
#include "argvector.h"

/*
**	freevector: free an argv.
*/
int
freevector(char **argv)
	{
	char **argvp;

	if (argv!=NULL)
		{
		for (argvp=argv; *argvp!=NULL; argvp++)
			(void)buffree(*argvp);
		(void)nfree((void *)argv);
		}

	return(0);
	}
