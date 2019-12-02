/*
**	L I S T S
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Lists
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Sep 22 11:00:47 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	General routines for FIFO linked list management.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)lists.c	4.8 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "lists.h"

/*
**	lget: get from the front of the list.
**
**	Arguments:
**		l	pointer to list structure
**
**	Returns:	pointer to listnode structure, or NULL if empty
*/
LISTNODE *
lget(LIST *l)
	{
	LISTNODE *n;

	n=NULL;
	if (l!=NULL)
		if ((n=l->l_head)!=NULL)
			if ((l->l_head=n->l_next)==NULL)
				l->l_tail=NULL;
	return(n);
	}

/*
**	lnext: given a node, return the next node on the list.
*/
LISTNODE *
lnext(LISTNODE *n)
	{
	return(n==NULL?NULL:n->l_next);
	}

/*
**	lpeek: peek at the first item on the list without
**	removing it.
**
**	Arguments:
**		l	pointer to list structure
**
**	Returns:	pointer to listnode structure, or NULL if empty
*/
LISTNODE *
lpeek(LIST *l)
	{
	return(l==NULL?NULL:l->l_head);
	}

/*
**	lput: put to the end of the list.
**
**	Arguments:
**		l	pointer to list structure
**		n	pointer to listnode structure
**
**	Returns:	pointer to listnode structure or NULL
**			if error
*/
LISTNODE *
lput(LIST *l, LISTNODE *n)
	{
	if ((l==NULL)||(n==NULL))
		return(NULL);
	n->l_next=NULL;
	if (l->l_head==NULL)
		l->l_head=n;
	if (l->l_tail!=NULL)
		(l->l_tail)->l_next=n;
	l->l_tail=n;
	return(n);
	}

/*
**	linit: initialize list structure
**
**	Arguments:
**		l	poitner to list structure
**
**	Returns:	pointer to list structure, or NULL if
**			error
*/
LIST *
linit(LIST *l)
	{
	if (l!=NULL)
		{
		l->l_head=NULL;
		l->l_tail=NULL;
		}
	return(l);
	}

/*
**	lbody: return pointer to data portion of listnode
**	structure.
**
**	Arguments:
**		n	pointer to listnode structure
**
**	Returns:	pointer to data portion of listnode structure,
**			or NULL if error
*/
char *
lbody(LISTNODE *n)
	{
	return(n==NULL?NULL:((char *)n)+(sizeof(struct listnode)));
	}
