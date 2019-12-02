/*
**	P A R C E L
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Parcel
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Sep 26 15:44:57 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	These functions provide a mechanism for packaging parameter
**	information into a human-readable character string, perhaps
**	prior to transmitting it via message passing. A parameter
**	consists of a keyword ASCII string that uniquely identifies
**	the parameter, and a value ASCII string that is the
**	corresponding value of the parameter. There are three other
**	principle data structures: a PACKAGE, a PARCEL, and a PILE.
**
**	A PARCEL is a data structure containing parameters in a form
**	which can be accessed directly and quickly by the application.
**	This implementation uses binary trees. See PORTFOLIO for a
**	competing implementation that does similar things but
**	implemented with skiplists.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)parcel.c	4.22 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "libtstrings.h"
#include "nextparser.h"
#include "keyword.h"
#include "lists.h"
#include "escapes.h"
#include "trees.h"
#include "keyval.h"
#include "extract.h"
#include "buffers.h"
#include "nmalloc.h"
#include "parcel.h"

/*
**	unbundle: extract a value from a PARCEL structure given
**	an identifying parameter name.
**
**	Arguments:
**		parm		pointer to parameter name string
**		parcel		pointer to parcel structure
**
**	Returns:		value string or NULL if not found
*/
char *
unbundle(char *parm, PARCEL parcel)
	{
	return(parm==NULL?NULL:kvget(parm,parcel));
	}

/*
**	bundle: add a new (replace an existing) parameter in a
**	parcel structure.
**
**	bundlep: same as bundle, except the parm and its value
**	are not copied. This is a useful optimization.
**
**	Arguments:
**		parm		pointer to parameter name string
**		value		pointer to parameter value string
**				(NULL means delete)
**		parcelp		pointer to address of parcel structure
**
**	Results:		parcelp or NULL if error (errno set)
*/
PARCEL *
bundle(char *parm, char *value, PARCEL *parcelp)
	{
	if ((parm==NULL)||(parcelp==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	return(kvput(parm,value,parcelp));
	}

PARCEL *
bundlep(BUFFER parm, BUFFER value, PARCEL *parcelp)
	{
	if ((parm==NULL)||(parcelp==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	return(kvputp(parm,value,parcelp));
	}

/*
**	rebundle: merges the contents of one PARCEL into another.
**
**	Arguments:
**		parcel	pointer to source PARCEL
**		parcelp	address of pointer to destination PARCEL
**
**	Returns:	parcelp or NULL if error (errno set)
*/
PARCEL *
rebundle(PARCEL parcel, PARCEL *parcelp)
	{
	if (parcelp==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	if (kvmerge(parcel,parcelp)<0)
		return(NULL);

	return(parcelp);
	}

/*
**	enbundle: merges the contents of one PARCEL into another,
**	inserting only those pairs in which the keyword does not
**	appear in the destination. Compare to rebundle() above.
**
**	Arguments:
**		parcel	pointer to source PARCEL
**		parcelp	address of pointer to destination PARCEL
**
**	Returns:	parcelp or NULL if error (errno set)
*/
PARCEL *
enbundle(PARCEL parcel, PARCEL *parcelp)
	{
	if (parcelp==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	if (kvaugment(parcel,parcelp)<0)
		return(NULL);

	return(parcelp);
	}

/*
**	countparcel: return a count of the number of items
**	in a parcel.
*/
int
countparcel(PARCEL parcel)
	{
	return(kvcount(parcel));
	}

/*
**	initparcel: initialize an empty parcel structure.
**
**	Arguments:
**		parcelp		pointer to the address of parcel structure
**
**	Returns:		parcelp or NULL if error (errno set)
*/
PARCEL *
initparcel(PARCEL *parcelp)
	{
	if (parcelp==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	*parcelp=NULL;

	return(parcelp);
	}

/*
**	freeparcel: free memory associated with parcel structure.
*/
int
freeparcel(PARCEL parcel)
	{
	return(parcel==NULL?0:kvfree(parcel));
	}

/*
**	travparcel: traverse a parcel, applying a user supplied
**	function to each node, with a way to carry along state.
*/
int
travparcel(PARCEL parcel, int (*g)(), void *vp)
	{
	return(tpreorder(&(parcel->kv_tree),g,vp));
	}
