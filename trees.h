/*
**	T R E E S
**
**	Copyright 1989, 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)trees.h	4.9 94/05/16 jsloan@ncar.ucar.edu
*/

#ifndef H_TREES
#define H_TREES

#include <stdio.h>
#include "libtproto.h"

struct treenode
	{
	struct treenode * t_next;	/* for breadth-first */
	struct treenode * t_left;	/* left branch */
	struct treenode * t_right;	/* right branch */
	};

typedef struct treenode TREE;

struct treestats
	{
	int t_maxheight;	/* height of tallest branch */
	int t_optheight;	/* optimal height if balanced */
	int t_avlheight;	/* typical height if AVL tree */
	int t_avgheight;	/* average height of branches */
	int t_maxwidth;		/* width at widest point (leaves) */
	int t_optwidth;		/* optimal width if balanced */
	int t_numnodes;		/* number of nodes in tree */
	};

typedef struct treestats TREESTATS;

extern TREE *tput LIBTPROTO((TREE **rp, TREE *e, int (*f)()));

extern TREE *tget LIBTPROTO((TREE *r, TREE *e, int (*f)()));

extern int tpostorder LIBTPROTO((TREE *r, int (*g)(), void *vp));

extern int tpreorder LIBTPROTO((TREE *r, int (*g)(), void *vp));

extern int tinorder LIBTPROTO((TREE *r, int (*g)(), void *vp));

extern int trevorder LIBTPROTO((TREE *r, int (*g)(), void *vp));

extern char *tbody LIBTPROTO((TREE *n));

/*
** The functions below are in the ancilliary trees2 module.
*/

extern int tbalance LIBTPROTO((TREE **rp, int (*f)()));

extern int tstats LIBTPROTO((TREE *tree, TREESTATS *t));

extern int tleftright LIBTPROTO((TREE *r, int (*g)()));

extern int trightleft LIBTPROTO((TREE *r, int (*g)()));

extern int tdumpn LIBTPROTO((TREE **nodep, FILE *fp));

extern int tdump LIBTPROTO((TREE **treep, FILE *fp));

#define kvbalance(treep)        tbalance(treep,kvcompare)
#define balanceparcel(parcelp)  kvbalance(parcelp)

#endif /* !H_TREES */
