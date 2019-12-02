/*
**	T R E E S S T A T S
**
**	U N D E R   D E V E L O P M E N T
**
**	Copyright 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Trees Statistics
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
*/

static char copyright[]="Copyright 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)treesstats.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <math.h>
#include "errors.h"
#include "lists.h"
#include "log2.h"
#include "trees.h"

/*
**	tstats: return the number of nodes, and compile other
**	interesting statistics about the tree.
*/
static int
treestats(TREE *n, struct treestats *t, int height)
	{
	int lnodes, rnodes;

	if (n==NULL)
		return(0);

	if (t!=NULL)
		{
		if (height>t->t_maxheight)
			t->t_maxheight=height;
		if ((n->t_left==NULL)&&(n->t_right==NULL))
			{
			t->t_avgheight=((t->t_avgheight*t->t_maxwidth)
				+height)/(t->t_maxwidth+1);
			t->t_maxwidth++;
			}
		}

	lnodes=treestats(n->t_left,t,height+1);
	rnodes=treestats(n->t_right,t,height+1);

	return(lnodes+rnodes+1);
	}

int
tstats(TREE *tree, struct treestats *t)
	{
	int nodes;
	double optwidth, optheight, avlheight;

	if (t!=NULL)
		{
		t->t_maxheight=0;
		t->t_optheight=0;
		t->t_avgheight=0;
		t->t_avlheight=0;
		t->t_maxwidth=0;
		t->t_optwidth=0;
		}

	nodes=treestats(tree,t,1);

	if (t!=NULL)
		{
		t->t_numnodes=nodes;
		optwidth=((double)nodes+1.0)/2.0;
		optheight=log2(optwidth)+1.0;
		avlheight=log2((double)nodes)+0.25;
		t->t_optwidth=optwidth;
		t->t_optheight=optheight;
		t->t_avlheight=avlheight;
		}

	return(nodes);
	}
