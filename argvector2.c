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
**	See also argvector.c and argvector3.c.
*/

static char copyright[]="Copyright 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)argvector2.c	4.2 94/05/18 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "libtstrings.h"
#include "keyword.h"
#include "escapes.h"
#include "trees.h"
#include "keyval.h"
#include "buffers.h"
#include "nmalloc.h"
#include "parcel.h"
#include "argvector.h"

/*
**	bundlevector: step through an argument or environment vector
**	and bundle the parameters into a parcel.
**
**	Arguments:
**		argc	usual argc
**		argv	usual argv
**		parcelp	pointer to the parcel
**
**	Returns:	parcelp if successful, NULL otherwise.
*/
PARCEL *
bundlevector(int argc, char **argv, PARCEL *parcelp)
	{
	char *lhs, *rhs;
	BUFFER arg, lhsp, rhsp;

	if ((argv==NULL)||(parcelp==NULL))
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

		if (bundlep(lhsp,rhsp,parcelp)==NULL)
			{
			(void)buffree(lhsp);
			(void)buffree(rhsp);
			return(NULL);
			}

		}

	return(parcelp);
	}

/*
**	unbundlevector: convert a parcel into an argument vector
**	suitable for framing in routines like execv.
**
**	Arguments:
**		parcel	input parcel
**
**	Returns:	an argument vector, or NULL if failure
*/
static int
vectorizer(PARCEL n, void *vp)
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
	if ((string=bufalloc(strlen(n->kv_keyword)+strlen(n->kv_value)+2))==NULL)
		return(-2);

	(void)concats(string,n->kv_keyword,"=",n->kv_value,NULL);

	*(argv++)=(char *)string;

	*argvp=argv;

	return(0);
	}

char **
unbundlevector(PARCEL parcel)
	{
	int length;
	char **argv, **argvp;

	length=sizeof(char *)*(countparcel(parcel)+1);
	if ((argv=(char **)nmalloc(length))==NULL)
		return(NULL);
	(void)bzero(argv,length);

	argvp=argv;
	if (travparcel(parcel,vectorizer,(void *)&argvp)!=0)
		{
		(void)freevector(argv);
		return(NULL);
		}

	/* N.B. bzero() supplies the final NULL */

	return(argv);
	}
