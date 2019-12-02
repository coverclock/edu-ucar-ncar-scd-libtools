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
**	See also pile.c and pile3.c.
*/

static char copyright[] = "Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)pile2.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "libtstrings.h"
#include "nmalloc.h"
#include "buffers.h"
#include "keyval.h"
#include "parcel.h"
#include "pile.h"

/*
**	unbundlepile: convert a parcel into a pile.
**
**	Arguments:
**		parcel	pointer to parcel
**
**	Returns:	pointer to pile
*/
static int
enlist(PARCEL node, void *listpp)
	{
	BUFFER lhs, rhs;

	if (node!=NULL)
		{
		if ((lhs=bufdup(node->kv_keyword))==NULL)
			return(-1);
		if ((rhs=bufdup(node->kv_value))==NULL)
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
unbundlepile(PARCEL parcel)
	{
	char **list, **listp;
	int count, rc;

	if (parcel==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	count=countparcel(parcel);

	if ((list=(char **)nmalloc(2*(sizeof(char *))*(count+1)))==NULL)
		return(NULL);

	listp=list;

	rc=travparcel(parcel,enlist,(void *)&listp);

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
**	bundlepile: merge a pile into a parcel.
**
**	Arguments:
**		list	pointer to pile
**		parcelp	address of pointer to parcel
**
**	Returns:	pointer to parcel
*/
PARCEL *
bundlepile(char **list, PARCEL *parcelp)
	{
	char *lhs, *rhs;

	if ((list==NULL)||(parcelp==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	while (*list)
		{
		lhs=(*(list++));
		rhs=(*(list++));
		if (bundle(lhs,rhs,parcelp)==NULL)
			return(NULL);
		}

	return(parcelp);
	}
