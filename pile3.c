/*
**	P I L E
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Pile
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Dec  8 14:42:19 MST 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	The functions in this module implement the PILE data structure.
**	A PILE is an array of the form "char (*)[][2]" where the first
**	column of each row contains a pointer to a keyword string and
**	the second column contains a pointer to a value string. It
**	is an argv-like structure suitable for sequential access.
**
**	See also pile.c and pile2.c.
*/

static char copyright[] = "Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)pile3.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "libtstrings.h"
#include "nmalloc.h"
#include "buffers.h"
#include "skeyval.h"
#include "portfolio.h"
#include "pile.h"

/*
**	unstuffpile: convert a portfolio into a pile.
**
**	Arguments:
**		portfolio	pointer to portfolio
**
**	Returns:	pointer to pile
*/
static int
enlist(SKEYVAL *node, void *listpp)
	{
	BUFFER lhs, rhs;

	if (node!=NULL)
		{
		if ((lhs=bufdup(node->skv_keyword))==NULL)
			return(-1);
		if ((rhs=bufdup(node->skv_value))==NULL)
			{
			(void)buffree(lhs);
			return(-2);
			}
		*((*((BUFFER **)listpp))++)=lhs;
		*((*((BUFFER **)listpp))++)=rhs;
		}

	return(0);
	}

char **
unstuffpile(PORTFOLIO *portfolio)
	{
	char **list, **listp;
	int count, rc;

	if (portfolio==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	count=countportfolio(portfolio);

	if ((list=(char **)nmalloc(2*(sizeof(char *))*(count+1)))==NULL)
		return(NULL);

	listp=list;

	rc=travportfolio(portfolio,enlist,(void *)&listp);

	*(listp++)=NULL;
	*(listp++)=NULL;

	if (rc!=0)
		{
		(void)freepile(list);
		return(NULL);
		}

	return(list);
	}

/*
**	stuffpile: merge a pile into a portfolio.
**
**	Arguments:
**		list		pointer to pile
**		portfolio	pointer to portfolio
**
**	Returns:	pointer to parcel
*/
PORTFOLIO *
stuffpile(char **list, PORTFOLIO *portfolio)
	{
	char *lhs, *rhs;

	if ((list==NULL)||(portfolio==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	while (*list)
		{
		lhs=(*(list++));
		rhs=(*(list++));
		if (stuff(lhs,rhs,portfolio)==NULL)
			return(NULL);
		}

	return(portfolio);
	}
