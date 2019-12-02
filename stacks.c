/*
**	S T A C K S
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Stacks
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Sep 25 11:07:43 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	General routines for LIFO linked stack management.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)stacks.c	4.9 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "stacks.h"

/*
**	sget: get the top item on the stack without popping it off.
**
**	Arguments:
**		s	pointer to stack structure
**
**	Returns:	pointer to stacknode structure, or NULL if
**			empty or error
*/
STACK
sget(STACK *s)
	{
	return(s==NULL?NULL:*s);
	}

/*
**	spop: pop the top item on the stack.
**
**	Arguments:
**		s	pointer to stack structure
**
**	Returns:	pointer to stacknode structure, or NULL if
**			empty or error
*/
STACK
spop(STACK *s)
	{
	STACK n;

	if (s==NULL)
		return(NULL);
	if ((n=(*s))!=NULL)
		*s=n->s_next;
	return(n);
	}

/*
**	spush: push onto the top of the stack.
**
**	Arguments:
**		s	pointer to stack structure
**		n	pointer to stacknode structure
**
**	Returns:	pointer to stacknode structure or NULL if error
*/
STACK
spush(STACK *s, STACK n)
	{
	if ((s==NULL)||(n==NULL))
		return(NULL);
	n->s_next=(*s);
	*s=n;
	return(n);
	}

/*
**	sbody: return pointer to data portion of stacknode
**	structure.
**
**	Arguments:
**		n	pointer to stacknode structure
**
**	Returns:	pointer to data portion of stacknode structure,
**			or NULL if error
*/
char *
sbody(STACK n)
	{
	return(n==NULL?NULL:((char *)n)+(sizeof(struct stacknode)));
	}
