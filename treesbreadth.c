/*
**	T R E E S B R E A D T H
**
**	U N D E R   D E V E L O P M E N T
**
**	Copyright 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Trees Breadth First Traversal
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu May 28 12:32:31 MDT 1992
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This module contains additional functions used to manipulate
**	trees (3l), but which are not part of the base trees package.
**
**	Also included here are functions to traverse a tree
**	breadth-first, both left to right and right to left.
*/

static char copyright[]="Copyright 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)treesbreadth.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "lists.h"
#include "trees.h"

/*
**	tleftright: perform a breadth-first traversal of a tree
**	from left to right.
**
**	trightleft: same as tleftright but right to left.
**
**	In all cases, the function g returns
**
**		rc<0:   exit, return rc
**		rc==0:  continue traversal, return 0 when complete
**		rc>0:   exit traversal, return rc
*/
int
tleftright(TREE *r, int (*g)())
	{
	LIST list;
	TREE *p;
	int rc;

	if ((r==NULL)||(g==NULL))
		return(-999);

	(void)linit(&list);
	(void)lput(&list,(LISTNODE *)&r->t_next);

	while ((p=(TREE *)lget(&list))!=NULL)
		{

		if (p->t_left!=NULL)
			(void)lput(&list,(LISTNODE *)&p->t_left->t_next);
		if (p->t_right!=NULL)
			(void)lput(&list,(LISTNODE *)&p->t_right->t_next);

		if ((rc=(*g)(p))!=0)
			return(rc);

		}

	return(0);
	}

int
trightleft(TREE *r, int (*g)())
	{
	LIST list;
	TREE *p;
	int rc;

	if ((r==NULL)||(g==NULL))
		return(-999);

	(void)linit(&list);
	(void)lput(&list,(LISTNODE *)&r->t_next);

	while ((p=(TREE *)lget(&list))!=NULL)
		{

		if (p->t_right!=NULL)
			(void)lput(&list,(LISTNODE *)&p->t_right->t_next);
		if (p->t_left!=NULL)
			(void)lput(&list,(LISTNODE *)&p->t_left->t_next);

		if ((rc=(*g)(p))!=0)
			return(rc);

		}

	return(0);
	}
