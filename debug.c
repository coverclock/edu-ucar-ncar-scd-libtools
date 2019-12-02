/*
**	D E B U G
**
**	U N D E R   D E V E L O P M E  N T
**
**	Copyright 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Debugging Stuff
**	Program		libtools
**	Project		tools library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Dec  8 08:59:59 MST 1992
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
*/

static char copyright[]="Copyright 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)debug.c	4.8 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "debug.h"

int
debugging(char *keyword, int request)
	{
	char *value;
	int debug, level;
	extern char *getenv();

	debug=0;

	/*
	** If the user has requested a debug level, set it.
	*/

	if (request>=0)
		debug=request;

	/*
	** Check the DEBUG keyword in the environment.
	*/

	if (keyword==NULL)
		level=0;
	else if ((value=getenv(keyword))==NULL)
		level=0;
	else if (strlen(value)==0)
		level=1;
	else
		switch (*value)
			{
		case 'N':
		case 'n':
			level=0;
			break;
		case 'Y':
		case 'y':
			level=1;
			break;
		case 'F':
		case 'f':
			level=2;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			level=atoi(value);
			break;
		default:
			level=0;
			break;
			}

	/*
	** If the DEBUG keyword sets a higher debug level, use it.
	*/

	if (level>debug)
		debug=level;
	
	return(debug);
	}
