/*
**	A T O A R G V
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		ASCII to Argv
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Oct 16 13:52:26 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**	Abstract
**
**	Man is this ugly or what?
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)atoargv.c	4.12 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libtstrings.h"
#include "buffers.h"
#include "nmalloc.h"
#include "nextparser.h"
#include "atoargv.h"

/*
**	freeargv: free an argument list.
*/
int
freeargv(char **argv)
	{
	char **argvp;

	for (argvp=argv; *argvp!=NULL; argvp++)
		(void)buffree(*argvp);
	(void)nfree(argv);
	return(0);
	}

/*
**	atoargv: converts a character string of arguments in
**	nextparser(3l) form into an argv-like array suitable for
**	execv etc.
**
**	Arguments:
**		name	optional argv[0] (e.g. program name)
**		string	argument string
**
**	Returns:	address of dynamically acquired argv array
**			if successful, NULL otherwise.
*/
char **
atoargv(char *name, char *string)
	{
	BUFFER buffer;
	char *current;
	int count, size, length;
	char **argv, **argvp;

	/*
	** If string==NULL, the argv will at least have a
	** terminating NULL. If name is non-NULL, it will
	** also contain name. Hence, if both arguments are
	** NULL, there is still a valid return value that is
	** non-NULL (i.e. an empty argv).
	*/

	count=(name==NULL)?1:2;

	buffer=NULL;

	if (string!=NULL)
		{

		if ((buffer=bufalloc(strlen(string)+1))==NULL)
			return(NULL);
		length=buflength(buffer);

		current=string;
		while ((current=nextfrombuffer(current,buffer,length))!=NULL)
				count++;

		}

	size=count*sizeof(char *);
	if ((argv=(char **)nmalloc(size))==NULL)
		{
		if (buffer!=NULL)
			(void)buffree(buffer);
		return(NULL);
		}

	argvp=argv;

	if (name!=NULL)
		{
		if ((*(argvp++)=(char *)bufdup(name))==NULL)
			{
			if (buffer!=NULL)
				(void)buffree(buffer);
			(void)freeargv(argv);
			return(NULL);
			}
		count--;
		}

	current=string;

	for (count--; count>0; count--)
		{
		current=nextfrombuffer(current,buffer,length);
		if ((*(argvp++)=(char *)bufdup(buffer))==NULL)
			{
			if (buffer!=NULL)
				(void)buffree(buffer);
			(void)freeargv(argv);
			return(NULL);
			}
		}

	*(argvp++)=NULL;

	if (buffer!=NULL)
		(void)buffree(buffer);

	return(argv);
	}
