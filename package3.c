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
**	See also package.c and package2.c
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)package3.c	4.2 94/05/31 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "libtstrings.h"
#include "keyword.h"
#include "escapes.h"
#include "skiplists.h"
#include "skeyval.h"
#include "extract.h"
#include "nextparser.h"
#include "buffers.h"
#include "portfolio.h"
#include "package.h"

/*
**	unstuffpackage: traverse a PORTFOLIO structure, placing all the
**	parameters in a PACKAGE string. wrapper() is the local recursive
**	function used by unstuffpackage().
**
**	Arguments:
**		portfolio	pointer to portfolio structure
**
**	Returns:		pointer to PACKAGE string or NULL
**				if error (errno set)
*/
static int
wrapper(SKEYVAL *n, void *vp)
	{
	char *kq, *vq;
	int length, kl, vl;
	BUFFER key, val, *old, new;

	if ((n==NULL)||((old=(BUFFER *)vp)==NULL))
		{
		seterrno(EINVAL);
		return(-1);
		}
	if (*old==NULL)
		{
		seterrno(EINVAL);
		return(-2);
		}

	key=escpandnew(n->skv_keyword);
	if (PORTFOLIOQUOTE(key))
		{
		kq="\"";
		kl=2;
		}
	else
		{
		kq="";
		kl=0;
		}

	val=escpandnew(n->skv_value);
	if (PORTFOLIOQUOTE(val))
		{
		vq="\"";
		vl=2;
		}
	else
		{
		vq="";
		vl=0;
		}

	/* 3=strlen("=")+strlen(" ")+1(for nul) */
	length=strlen(*old)+strlen(key)+kl+strlen(val)+vl+3;
	if ((new=bufexpand(*old,length))==NULL)
		{
		(void)buffree(key);
		(void)buffree(val);
		return(-3);
		}

	(void)appends(new,kq,key,kq,"=",vq,val,vq," ",NULL);

	*old=new;

	(void)buffree(key);
	(void)buffree(val);

	return(0);
	}

PACKAGE *
unstuffpackage(PORTFOLIO *portfolio)
	{
	char *last;
	BUFFER string;
	int length;

	string=bufalloc(PACKAGESIZE);
	*string='\0';

	/*
	** Traverse the PORTFOLIO, build the PACKAGE recursively,
	** and when complete terminate the PACKAGE correctly.
	*/

	if (travportfolio(portfolio,wrapper,(void *)&string)!=0)
		{
		(void)buffree(string);
		return(NULL);
		}

	length=strlen(string);
	if (length>0)
		if (*(last=string+length-1)==' ')
			*last='\0';

	return((PACKAGE *)string);
	}

/*
**	stuffpackage: convert a PACKAGE into a PORTFOLIO so that the
**	parameters can be accessed efficiently.
**
**	Arguments:
**		package		pointer to input PACKAGE string
**		portfolio	pointer to output PORTFOLIO structure
**
**	Returns:		parcelp or NULL if error (errno set)
*/
PORTFOLIO *
stuffpackage(PACKAGE *package, PORTFOLIO *portfolio)
	{
	char *current, *lhs, *rhs;
	BUFFER lhsp, rhsp, word;
	int length;

	if ((package==NULL)||(portfolio==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	/*
	** Allocate working buffer.
	*/

	if ((word=bufalloc(strlen(package)+1))==NULL)
		return(NULL);
	length=buflength(word);

	/*
	** Use nextfrombuffer(3l) to get the next word from package,
	** break it up into keyword and value, and stow the
	** results in the parcel.
	*/

	current=package;

	while((current=nextfrombuffer(current,word,length))!=NULL)
		{

		rhs=keyword(word);
		lhs=(char *)word;

		if ((lhsp=esclapsenew(lhs))==NULL)
			{
			(void)buffree(word);
			return(NULL);
			}
		if ((rhsp=esclapsenew(rhs))==NULL)
			{
			(void)buffree(lhsp);
			(void)buffree(word);
			return(NULL);
			}
		
		if (stuffp(lhsp,rhsp,portfolio)==NULL)
			{
			(void)buffree(lhsp);
			(void)buffree(rhsp);
			(void)buffree(word);
			return(NULL);
			}
		}

	(void)buffree(word);

	return(portfolio);
	}
