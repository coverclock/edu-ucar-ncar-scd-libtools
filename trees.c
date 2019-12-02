/*
**	T R E E S
**
**	Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Binary Tree Routines
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Aug 28 09:08:13 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Trees contains routines to handle modifying, searching, and
**	traversing binary trees.
**
**	N.B.	The comparison function (*f)(e,p) that is an argument for
**		tput() must return a -1, 0, or +1 if the value of
**		tree node e is less than, equal to, or greater than that
**		of treenode p. In addition, it must return a 1 (true) if
**		p is NULL and the value of e indicates that the node
**		should be deleted. Thus (*f)(e,p) does double duty: it
**		detects a delete request OR compares the keys of
**		the nodes, depending upon the value of p.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)trees.c	4.10 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "trees.h"

/*
**	treeput: this does the actual work of tput(), but carries
**	along the control variable (p) upon which the recursion is based.
**	This makes the user interface a little simpler, even though
**	its not necessary.
*/
static TREE *
treeput(TREE **h, TREE **p, TREE *e, int (*f)())
	{
	TREE *t;
	int c;

	if ((h==NULL)||(p==NULL)||(e==NULL)||(f==NULL))
		return(NULL);

	if (*p==NULL)
		{

		/*
		**	Node does not exist.
		*/

		t=(*p);

		if (!(*f)(e,NULL))
			/*
			**	Add node.
			*/
			*p=e;

		}
	else
		{

		c=(*f)(e,*p);

		if (c<0)
			t=treeput(h,&((*p)->t_left),e,f);
		else if (c>0)
			t=treeput(h,&((*p)->t_right),e,f);
		else
			{

			/*
			**	Node exists.
			*/

			if (!(*f)(e,NULL))
				{
				/*
				**	Change node.
				*/
				t=(*p);
				e->t_left=(*p)->t_left;
				e->t_right=(*p)->t_right;
				*p=e;
				}

			else

				{
				/*
				**	Delete node.
				*/
				t=(*p);
				*p=NULL;
				(void)treeput(h,h,t->t_left,f);
				(void)treeput(h,h,t->t_right,f);
				}

			}
		}

	return(t);
	}

/*
**	tput: change, delete or add a node.
**
**	Arguments:
**		h	pointer to the root of the tree (i.e. the
**			address of the address of the first node in
**			the tree; upon the first tput this would be
**			the address of a variable initialized to NULL)
**		e	pointer to the node to change, add or delete.
**		f	function which compares node keys or detects
**			a request for delete
**
**	Returns:	pointer to the deleted or replaced node (if,
**			for example, it is to be free()'d), or NULL
**			if the node did not exist
*/
TREE *
tput(TREE **h, TREE *e, int (*f)())
	{
	if (e==NULL)
		return(NULL);

	e->t_left=NULL;
	e->t_right=NULL;

	return(treeput(h,h,e,f));
	}

/*
**	tget: retrieve a node give a node with the same key.
**
**	Arguments:
**		p	pointer to the next node (from the caller's
**			perspective this is the root)
**		e	node containing the key to search for
**		f	function which compares node keys
**
**	Returns:	pointer to retrieved node with same key, or
**			NULL if node not found.
*/
TREE *
tget(TREE *p, TREE *e, int (*f)())
	{
	TREE *t;
	int c;

	if ((e==NULL)||(f==NULL))
		return(NULL);

	if (p==NULL)
		t=NULL;
	else
		{

		c=(*f)(e,p);

		if (c<0)
			t=tget(p->t_left,e,f);
		else if (c>0)
			t=tget(p->t_right,e,f);
		else
			t=p;

		}

	return(t);
	}

/*
**	tpostorder: traverse the tree postorder. This is useful when
**	children nodes must be processed before their parents, e.g.
**	when using free() to release dynamically allocated nodes.
**
**	Arguments:
**		p	pointer to next node (from the caller's
**			perspective this is the root of the tree)
**		f	function to call against each node as the
**			tree is traversed.
**
**		vp	a variable pointer passed to each invocation
**			of f(); may be used to pass state info.
**
**	tpreorder: same as tpostorder but in preorder. Useful for
**	copying trees without linearizing them.
**
**	tinorder: same as before, but in order. Useful for processing
**	nodes in collating sequence.
**
**	trevorder: same as inorder, but reversed.
**
**	In all cases, the function f returns
**
**		rc<0:	exit traversal, return rc
**		rc==0:	continue traversal, return 0 when complete
**		rc>0:	exit traversal, return rc
*/
int
tpostorder(TREE *p, int (*f)(), void *vp)
	{
	int rc;

	if (f==NULL)
		return(-999);

	if (p!=NULL)
		{
		if ((rc=tpostorder(p->t_left,f,vp))!=0)
			return(rc);
		if ((rc=tpostorder(p->t_right,f,vp))!=0)
			return(rc);
		if ((rc=(*f)(p,vp))!=0)
			return(rc);
		}

	return(0);
	}

int
tpreorder(TREE *p, int (*f)(), void *vp)
	{
	int rc;

	if (f==NULL)
		return(-999);

	if (p!=NULL)
		{
		if ((rc=(*f)(p,vp))!=0)
			return(rc);
		if ((rc=tpreorder(p->t_left,f,vp))!=0)
			return(rc);
		if ((rc=tpreorder(p->t_right,f,vp))!=0)
			return(rc);
		}

	return(0);
	}

int
tinorder(TREE *p, int (*f)(), void *vp)
	{
	int rc;

	if (f==NULL)
		return(-999);

	if (p!=NULL)
		{
		if ((rc=tinorder(p->t_left,f,vp))!=0)
			return(rc);
		if ((rc=(*f)(p,vp))!=0)
			return(rc);
		if ((rc=tinorder(p->t_right,f,vp))!=0)
			return(rc);
		}

	return(0);
	}

int
trevorder(TREE *p, int (*f)(), void *vp)
	{
	int rc;

	if (f==NULL)
		return(-999);

	if (p!=NULL)
		{
		if ((rc=trevorder(p->t_right,f,vp))!=0)
			return(rc);
		if ((rc=(*f)(p,vp))!=0)
			return(rc);
		if ((rc=trevorder(p->t_left,f,vp))!=0)
			return(rc);
		}

	return(0);
	}

/*
**	tbody: returns the address of the body of a node (whatever
**	the body is, it is assumed to begin directly following the
**	treenode structure header).
**
**	Arguments:
**		n	address of a node from a tree
**
**	Returns:	pointer past the treenode struct header
*/
char *tbody(TREE *n)
	{
	return(n==NULL?NULL:((char *)n)+(sizeof(TREE)));
	}
