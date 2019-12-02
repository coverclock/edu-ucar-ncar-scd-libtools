/*
**	P O R T F O L I O
**
**	Copyright 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Portfolio
**	Program		libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Oct 12 10:34:34 MDT 1993
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
*/

static char copyright[]="Copyright 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)portfolio.c	4.2 94/05/18 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "libtstrings.h"
#include "nextparser.h"
#include "keyword.h"
#include "lists.h"
#include "escapes.h"
#include "skiplists.h"
#include "skeyval.h"
#include "extract.h"
#include "buffers.h"
#include "nmalloc.h"
#include "portfolio.h"

/*
**	unstuff: extract a value from a PORTFOLIO structure given
**	an identifying parameter name.
**
**	Arguments:
**		parm		pointer to parameter name string
**		portfolio		pointer to portfolio structure
**
**	Returns:		value string or NULL if not found
*/
char *
unstuff(char *parm, PORTFOLIO *portfolio)
	{
	return(parm==NULL?NULL:skvget(parm,portfolio));
	}

/*
**	stuff: add a new (replace an existing) parameter in a
**	portfolio structure.
**
**	stuffp: same as stuff, except the parm and its value
**	are not copied. This is a useful optimization.
**
**	Arguments:
**		parm		pointer to parameter name string
**		value		pointer to parameter value string
**				(NULL means delete)
**		portfolio		pointer to portfolio structure
**
**	Results:		portfolio or NULL if error (errno set)
*/
PORTFOLIO *
stuff(char *parm, char *value, PORTFOLIO *portfolio)
	{
	if ((parm==NULL)||(portfolio==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	return(skvput(parm,value,portfolio));
	}

PORTFOLIO *
stuffp(BUFFER parm, BUFFER value, PORTFOLIO *portfolio)
	{
	if ((parm==NULL)||(portfolio==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	return(skvputp(parm,value,portfolio));
	}

/*
**	restuff: merges the contents of one PORTFOLIO into another.
**
**	Arguments:
**		from	pointer to source PORTFOLIO
**		to	pointer to destination PORTFOLIO
**
**	Returns:	to or NULL if error (errno set)
*/
PORTFOLIO *
restuff(PORTFOLIO *from, PORTFOLIO *to)
	{
	if ((from==NULL)||(to==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	if (skvmerge(from,to)<0)
		return(NULL);

	return(to);
	}

/*
**	enstuff: merges the contents of one PORTFOLIO into another,
**	inserting only those pairs in which the keyword does not
**	appear in the destination. Compare to restuff() above.
**
**	Arguments:
**		from	pointer to source PORTFOLIO
**		to	pointer to destination PORTFOLIO
**
**	Returns:	to or NULL if error (errno set)
*/
PORTFOLIO *
enstuff(PORTFOLIO *from, PORTFOLIO *to)
	{
	if ((from==NULL)||(to==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	if (skvaugment(from,to)<0)
		return(NULL);

	return(to);
	}

/*
**	countportfolio: return a count of the number of items
**	in a portfolio.
*/
int
countportfolio(PORTFOLIO *portfolio)
	{
	return(skvcount(portfolio));
	}

/*
**	freeportfolio: free memory associated with portfolio structure.
*/
int
freeportfolio(PORTFOLIO *portfolio)
	{
	(void)skvfree(portfolio);
	return(0);
	}

/*
**	initportfolio: initialize an empty portfolio structure.
**
**	Arguments:
**		portfolio	pointer to portfolio structure
**
**	Returns:		parcelp or NULL if error (errno set)
*/
PORTFOLIO *
initportfolio(PORTFOLIO *portfolio)
	{
	if (portfolio==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	if (skvinit(portfolio)==NULL)
		return(NULL);

	return(portfolio);
	}

/*
**	travparcel: traverse a parcel, applying a user supplied
**	function to each node, with a way to carry along state.
**	This is ugly, but it hides the void pointer stuff intrinsic
**	to skiplists from the user.
*/
struct travs
	{
	int	(*t_f)();
	void	*t_vp;
	};

typedef struct travs TRAVS;

static int
travf(void *up, void *vp)
	{
	return((*(((TRAVS *)vp)->t_f))((SKEYVAL *)up,((TRAVS *)vp)->t_vp));
	}

int
travportfolio(PORTFOLIO *portfolio, int (*g)(), void *vp)
	{
	struct travs s;

	s.t_f=g;
	s.t_vp=vp;

	return(sklinorder(portfolio,travf,(void *)&s));
	}
