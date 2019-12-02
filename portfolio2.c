/*
**	P O R T F O L I O 2
**
**	Copyright 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Additional Portfolio Functions
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Oct 12 13:58:29 MDT 1993
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This module contains additional portfolio-handling functions that
**	are extensions of those in the basic portfolio(3l) module.
*/

static char copyright[]="Copyright 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)portfolio2.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <stdarg.h>
#include "errors.h"
#include "nextparser.h"
#include "buffers.h"
#include "keyword.h"
#include "skiplists.h"
#include "portfolio.h"

/*
**	fmtportfolio: format portfolio stuff using the provided control
**	string which is in the format "KEYWORD=printfmt ...".
*/
int
fmtportfolio(PORTFOLIO *p, char *f, char *b)
	{
	BUFFER buffer;
	char *key, *fmt, *val;
	int length;

	if ((buffer=bufalloc(strlen(f)+1))==NULL)
		return(-1);
	length=buflength(buffer);

	/*
	** March through the format string, getting each keyword
	** and printf-string pair, fetch the keyword's value
	** from the parcel, and format it into the output string.
	*/

	*b='\0';

	while ((f=nextfrombuffer(f,buffer,length))!=NULL)
		{

		key=(char *)buffer;
		fmt=keyword(buffer);

		if ((val=unstuff(key,p))==NULL)
			val="";

		(void)sprintf(b,fmt,val);

		b+=strlen(b);

		}

	(void)buffree(buffer);

	return(0);
	}

/*
**	dumportfolio: dump a portfolio in a form that gives a programmer
**	some prayer of determining the structure of the underlying
**	tree.
*/
int
dumportfolio(PORTFOLIO *p, FILE *fp)
	{
	if (fp==NULL)
		{
		seterrno(EINVAL);
		return(-1);
		}
	return(skldump(p,(int (*)())NULL,(void *)NULL, fp));
	}

/*
**	stufff: create a value using a printf-style format and
**	a list of arguments and stuff(3l) it into a portfolio.
**
**	Arguments:
**		parm		keyword string
**		portfolio	pointer to portfolio
**		buffer		buffer large enough to contain value
**		format		format string
**		va_list		argument list for format string
**
**	Returns:		value of bundle()
*/
PORTFOLIO *
stufff(char *parm, PORTFOLIO *portfolio, char *buffer, char *format,...)
	{
	va_list args;
	
	va_start(args,format);

	(void)vsprintf(buffer,format,args);

	va_end(args);

	return(stuff(parm,buffer,portfolio));
	}

/*
**	unstufff: extract a value from a portfolio in an
**	unstuff(3l)-like fashion, and convert it into another
**	type via sscanf(3) using a printf-style format.
**
**	Arguments:
**		parm		keyword string
**		portfolio	portfolio
**		format		format string
**		argp		pointer to output variable
**
**	Returns:		same as unbundle(3l)
*/
char *
unstufff(char *parm, PORTFOLIO *portfolio, char *format, void *argp)
	{
	char *value;

	if ((value=unstuff(parm,portfolio))==NULL)
		return(value);

	if (sscanf(value,format,argp)!=1)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	return(value);
	}
