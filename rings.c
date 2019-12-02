/*
**	R I N G S
**
**	Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Rings
**	Program		TAGS
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu Jan 10 10:19:40 MST 1991
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Implements the character ring buffer data structure.
**
**	N.B.	Although this code was tested when it was written,
**		it was never used in production.
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)rings.c	4.11 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "nmalloc.h"
#include "rings.h"

/*
**	rinit: initialize a ring for the first time.
**
**	Arguments:
**		r	pointer to ring structure.
**		n	size of desired ring in bytes.
**
**	Returns:	pointer to ring structure, or NULL if error
*/
RING *
rinit(RING *r, int n)
	{
	if ((r==NULL)||(n<=0))
		return(NULL);
	if ((r->r_base=nmalloc(n))==NULL)
		return(NULL);
	r->r_past=r->r_base+n;
	r->r_from=r->r_base;
	r->r_to=r->r_base;
	r->r_used=0;
	r->r_free=n;
	return(r);
	}

/*
**	rreinit: reinitialize a ring. If the size differs from what
**	was previously requested the ring is not reallocated. If
**	size is zero, the ring is freed.
**
**	Arguments:
**		r	pointer to ring structure.
**		n	size of desired ring in bytes.
**
**	Returns:	pointer to ring structure, or NULL if error
*/
RING *
rreinit(RING *r, int n)
	{
	char *p;

	if ((r==NULL)||(n<0))
		return(NULL);
	if (r->r_base==NULL)
		return(NULL);
	if (n==0)
		{
		(void)nfree(r->r_base);
		r->r_base=NULL;
		r->r_past=NULL;
		}
	else if (n!=(r->r_past-r->r_base))
		{
		if ((p=nrealloc(r->r_base,n))==NULL)
			return(NULL);
		r->r_base=p;
		r->r_past=r->r_base+n;
		}
	r->r_from=r->r_base;
	r->r_to=r->r_base;
	r->r_used=0;
	r->r_free=n;
	return(r);
	}

/*
**	rextract: remove the next char from the ring buffer
**	and return it.
**
**	Arguments:
**		r	pointer to ring structure.
**		cp	pointer to char variable; if NULL char is
**			merely removed.
**
**	Returns:	>0 if a char was returned, 0 if ring empty,
**			<0 if error.
*/
int
rextract(RING *r, char *cp)
	{
	if (r==NULL)
		return(-1);
	if (r->r_base==NULL)
		return(-2);
	if (r->r_used<=0)
		return(0);
	if (cp!=NULL)
		*cp=(*(r->r_from));
	r->r_from++;
	if (r->r_from>=r->r_past)
		r->r_from=r->r_base;
	r->r_free++;
	r->r_used--;
	return(1);
	}

/*
**	rpeek: get the next char from the ring buffer
**	and return it without removing it from the ring. This
**	is a simple way to see if a char is available.
**
**	Arguments:
**		r	pointer to ring structure.
**		cp	pointer to char variable; if NULL char is
**			merely removed.
**
**	Returns:	>0 if a char was returned, 0 if ring empty,
**			<0 if error.
*/
int
rpeek(RING *r, char *cp)
	{
	if (r==NULL)
		return(-1);
	if (r->r_base==NULL)
		return(-2);
	if (r->r_used<=0)
		return(0);
	if (cp!=NULL)
		*cp=(*(r->r_from));
	return(1);
	}

/*
**	rinsert: insert a char into the ring buffer.
**
**	Arguments:
**		r	pointer to ring structure.
**		c	char to insert.
**
**	Returns:	>0 if a char was inserted, 0 if ring full,
**			<0 if error.
*/
int
rinsert(RING *r, char c)
	{
	if (r==NULL)
		return(-1);
	if (r->r_base==NULL)
		return(-2);
	if (r->r_free<=0)
		return(0);
	*(r->r_to)=c;
	r->r_to++;
	if (r->r_to>=r->r_past)
		r->r_to=r->r_base;
	r->r_used++;
	r->r_free--;
	return(1);
	}
