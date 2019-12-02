/*
**	B A C K C O P Y
**
**	Copyright 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Back Copy
**	Program		libtools
**	Project		libtools
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu Aug  3 11:06:46 MDT 1995
**
*/

static char copyright[]="Copyright 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)backcopy.c	1.1 95/08/03 jsloan@ncar.ucar.edu";

#include "libtools.h"

int
backcopy(char *sa, int sl, char *da, int dl)
	{
	char *sb, *db;
	int n;

	sb=sa+sl;
	db=da+dl;
	n=0;
	while ((dl>0)&&(sl>0))
		{
		*(--db)=(*(--sb));
		dl--;
		sl--;
		n++;
		}

	return(n);
	}
