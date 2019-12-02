/*
**	T R E E S A V L
**
**	U N D E R   D E V E L O P M E N T
**
**	Copyright 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Trees AVL
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu May 28 12:32:31 MDT 1992
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	N.B.	I am complete confident that the AVL trees
**		implementation here does _not_ work correctly.
**		However, the application for which this was
**		intended uses skiplists instead, which are better
**		in virtually all respects. Maybe one day I will
**		revisit this.
**
**	This module contains additional functions used to manipulate
**	trees (3l), but which are not part of the base trees package.
**
**	This includes a function to balance a tree using the AVL [1]
**	algorithm, a function to dump a tree for debugging, and a
**	function which calculates statistics about the shape of a tree.
**	Wirth says that empirical evidence suggests rebalancing
**	a tree after every two insertions or every five deletions,
**	but that the decreased search time must be weighed against
**	the cost of rebalancing [2]. Many thanks to [3] for pointers
**	on AVL implementation.
**
**	[1]	Adelson-Velskii, G. M. and Landis, E. M., Doklady
**		Akademia Nauk SSSR, 146, (1962), 263-266 (English
**		trans. in Soviet Math, 3, 1259-1263)
**
**	[2]	Wirth, N., Algorithms + Data Structures = Programs,
**		Prentice-Hall, 1976, 215-226
**
**	[3]	Vixie, P., avl-subs, comp.sources.unix, volume 11
*/

static char copyright[]="Copyright 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)treesavl.c	4.15 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <math.h>
#include "errors.h"
#include "lists.h"
#include "trees.h"

#define FULL(n)		(((n)->t_left!=NULL)&&((n)->t_right!=NULL))

/*
**	This structure is used to pass state information between
**	successive revursive calls.
*/
struct State
	{
	int	(*S_funcp)();
	TREE	*S_to;
	};

/*
**	avlput: Insert the node n into the to tree to using the
**	AVL algorithm, so that the resulting tree is balanced.
**	Because the original tree is traversed depth first, we don't
**	concern ourselves with where the left and right links of n
**	may have been pointing. Note that we have to keep track of
**	several of the predecessors of the insertion point.
**
**	N.B.	This function is under development, and is
**		believed NOT to work correctly. Although it
**		does produce a correctly ordered tree, the tree
**		isn't balanced the way I believe the AVL algorithm
**		specifies. I'm working on it.
*/
static int
avlput(TREE *node, TREE **child, TREE **parent, TREE **gparent, int (*comparator)())
	{
	TREE *cp;
	TREE *pp;
	TREE *gpp;
	int rc, c;

#ifdef DEBUG
	fprintf(stderr,"avlput(0x%lx, ",node);
	(void)tdumpn(child,stderr);
	fputs(", ",stderr);
	(void)tdumpn(parent,stderr);
	fputs(", ",stderr);
	(void)tdumpn(gparent,stderr);
	fputs(")\n",stderr);
#endif

	/*
	** If this node is used, then we're not done searching.
	** Keep moving down the tree, keeping track of parents
	** and grandparents.
	*/

	if (*child!=NULL)
		{
		c=(*comparator)(node,*child);
		if (c<0)
			{
			if ((rc=avlput(node,&((*child)->t_left),child,parent,comparator))!=0)
				return(rc);
			}
		else if (c>0)
			{
			if ((rc=avlput(node,&((*child)->t_right),child,parent,comparator))!=0)
				return(rc);
			}
		else
			/*
`			** If we ever reach this spot, the caller
			** has a heinous comparator function, or a
			** bogus tree.
			*/
			return(-1);

		return(0);
		}

	/*
	** Insert the new node. Note that all new nodes have
	** already had their right and left links nulled out
	** (and since we're travering depth first, we don't have
	** to worry about unprocessed children).
	*/

#ifdef DEBUG
	fprintf(stderr,"avlput *(0x%lx)=0x%lx\n",child,node);
#endif
	
	*child=node;

	/*
	** Check the resulting tree for balance. If the tree violates
	** the AVL property, then it has a node where one branch is
	** more than 1 node longer then the other. Note that gparent
	** or parent can be null if the tree isn't deep enough, but
	** otherwise *gparent and *parent cannot be null.
	*/

	if (gparent==NULL)
		return(0);
	if (parent==NULL)
		return(0);
	if (FULL(*gparent))
		return(0);
	if (FULL(*parent))
		return(0);

#ifdef DEBUG
	fprintf(stderr,"avlput balancing\n");
	fprintf(stderr,"avlput grandparent\n");
	(void)tdump(gparent,stderr);
	fprintf(stderr,"avlput parent\n");
	(void)tdump(parent,stderr);
	fprintf(stderr,"avlput child\n");
	(void)tdump(child,stderr);
#endif

	/*
	** Make copies of the pointers to the nodes so that
	** we can write over the originals with impunity.
	*/

	gpp=*gparent;
	pp=*parent;
	cp=*child;

	/*
	** The resulting tree is out of balance. Balance it using
	** the appropriate combinations of single or double rotates
	** to the left or right.
	*/

	if (gpp->t_left==NULL)
		{
		if (pp->t_left==NULL)
			{
			/* Single Rotate Left */
#ifdef DEBUG
			fprintf(stderr,"avlput single rotate left\n");
#endif
			pp->t_left=gpp;
			gpp->t_right=NULL;
			*gparent=pp;
			}
		else
			{
			/* Double Rotate Left */
#ifdef DEBUG
			fprintf(stderr,"avlput double rotate left\n");
#endif
			cp->t_left=gpp;
			cp->t_right=pp;
			gpp->t_right=NULL;
			pp->t_left=NULL;
			*gparent=cp;
			}
		}
	else
		{
		if (pp->t_right==NULL)
			{
			/* Single Rotate Right */
#ifdef DEBUG
			fprintf(stderr,"avlput single rotate right\n");
#endif
			pp->t_right=gpp;
			gpp->t_left=NULL;
			*gparent=pp;
			}
		else
			{
			/* Double Rotate Right */
#ifdef DEBUG
			fprintf(stderr,"avlput double rotate right\n");
#endif
			cp->t_right=gpp;
			cp->t_left=pp;
			gpp->t_left=NULL;
			pp->t_right=NULL;
			*gparent=cp;
			}
		}

#ifdef DEBUG
	fprintf(stderr,"avlput grandparent\n");
	(void)tdump(gparent,stderr);
	fprintf(stderr,"avlput parent\n");
	(void)tdump(parent,stderr);
	fprintf(stderr,"avlput child\n");
	(void)tdump(child,stderr);
#endif

	return(0);
	}

/*
**	avl: Invoke the recursive AVL algorithm.
*/
static int
avl(TREE *node, void *vp)
	{
	struct State *s;

	s=(struct State *)vp;
	node->t_left=NULL;
	node->t_right=NULL;
	return(avlput(node,&s->S_to,NULL,NULL,s->S_funcp));
	}

/*
**	tbalance: Balance the tree whose root is pointed to by
**	the address at location fromp. The old tree is replaced
**	by the new tree.
**
**	The comparison function (int (*comparator)(e,p)) that is an
**	argument to tbalance() must return a -1, 0, or +1 if the
**	value of tree node e is less than, equal to, or greater than
**	that of treenode p. In practice, in this particular
**	application, the two nodes should never be equal, or
**	else the original tree is bogus.
**
**	Returns:
**			-2	Arguments bogus
**			-1	Comparator function bogus
**			0	Original tree empty
**			1	Tree has been balanced
*/
int
tbalance(TREE **fromp, int (*comparator)())
	{
	int rc;
	struct State state;

	if ((fromp==NULL)||(comparator==NULL))
		{
		seterrno(EINVAL);
		return(-2);
		}

	if (*fromp==NULL)
		return(0);

	state.S_funcp=comparator;
	state.S_to=NULL;

	/*
	** We must traverse the original tree postorder (depth
	** first) to insure the children are processed before the
	** parent, since we're dismantling the original tree as
	** we go.
	*/

	rc=tpostorder(*fromp,avl,(void *)&state);

	*fromp=state.S_to;

	if (rc!=0)
		return(rc);

	return(1);
	}
