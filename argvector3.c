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
**	PORTFOLIOS and (char **) vectors such as argument and environment
**	variable lists.
**
**	See also argvector.c and argvector2.c.
*/

static char copyright[]="Copyright 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)argvector3.c	4.2 94/05/18 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "libtstrings.h"
#include "keyword.h"
#include "escapes.h"
#include "skiplists.h"
#include "skeyval.h"
#include "buffers.h"
#include "nmalloc.h"
#include "portfolio.h"
#include "argvector.h"

/*
**	stuffvector: step through an argument or environment vector
**	and stuff the parameters into a portfolio.
**
**	Arguments:
**		argc		usual argc
**		argv		usual argv
**		portfolio	pointer to the portfolio
**
**	Returns:	parcelp if successful, NULL otherwise.
*/
PORTFOLIO *
stuffvector(int argc, char **argv, PORTFOLIO *portfolio)
	{
	char *lhs, *rhs;
	BUFFER arg, lhsp, rhsp;

	if ((argv==NULL)||(portfolio==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	for (; (argc>0)&&(*argv!=NULL); argc--, argv++)
		{

		/*
		** We have to work from a copy of the argument because
		** keyword() modifies its argument as a side effect; we
		** shouldn't make any assumptions about what the caller
		** is going to do with the argument list after he calls
		** us.
		*/

		if ((arg=bufdup(*argv))==NULL)
			return(NULL);

		rhs=keyword(arg);
		lhs=(char *)arg;

		if ((lhsp=esclapsenew(lhs))==NULL)
			{
			(void)buffree(arg);
			return(NULL);
			}
		if ((rhsp=esclapsenew(rhs))==NULL)
			{
			(void)buffree(lhsp);
			(void)buffree(arg);
			return(NULL);
			}

		(void)buffree(arg);

		if (stuffp(lhsp,rhsp,portfolio)==NULL)
			{
			(void)buffree(lhsp);
			(void)buffree(rhsp);
			return(NULL);
			}

		}

	return(portfolio);
	}

/*
**	unstuffvector: convert a portfolio into an argument vector
**	suitable for framing in routines like execv.
**
**	Arguments:
**		portfolio	input portfolio
**
**	Returns:	an argument vector, or NULL if failure
*/
static int
vectorizer(SKEYVAL *n, void *vp)
	{
	char **argv, ***argvp;
	BUFFER string;

	if ((n==NULL)||((argvp=(char ***)vp)==NULL))
		{
		seterrno(EINVAL);
		return(-1);
		}

	argv=*argvp;

	/* 2=strlen("=")+1(for nul) */
	if ((string=bufalloc(strlen(n->skv_keyword)+strlen(n->skv_value)+2))==NULL)
		return(-2);

	(void)concats(string,n->skv_keyword,"=",n->skv_value,NULL);

	*(argv++)=(char *)string;

	*argvp=argv;

	return(0);
	}

char **
unstuffvector(PORTFOLIO *portfolio)
	{
	int length;
	char **argv, **argvp;

	length=sizeof(char *)*(countportfolio(portfolio)+1);
	if ((argv=(char **)nmalloc(length))==NULL)
		return(NULL);
	(void)bzero(argv,length);

	argvp=argv;
	if (travportfolio(portfolio,vectorizer,(void *)&argvp)!=0)
		{
		(void)freevector(argv);
		return(NULL);
		}

	/* N.B. bzero() supplies the final NULL */

	return(argv);
	}
