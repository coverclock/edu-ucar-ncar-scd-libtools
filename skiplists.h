/*
**	S K I P   L I S T S
**
**	Copyright 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)skiplists.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_SKIPLISTS
#define H_SKIPLISTS

#include <stdio.h>
#include "libtproto.h"

struct skiplistnode
	{
	void *			skl_link;	/* must be at top */
	int			skl_level;
	struct skiplistnode *	skl_next[1];
	};

typedef struct skiplistnode SKIPLISTNODE;

struct skiplist
	{
	SKIPLISTNODE *	skl_head;		/* must be at top */
	int		skl_levels;
	};

typedef struct skiplist SKIPLIST;

extern int sklinorder LIBTPROTO((SKIPLIST *l, int (*h)(), void *vp));

extern int sklfree LIBTPROTO((SKIPLIST *l, int (*g)(), void *vp));

extern int sklcount LIBTPROTO((SKIPLIST *l));

extern int sklpool LIBTPROTO((FILE *fp));

extern void sklshutdown LIBTPROTO((void));

extern int skldump LIBTPROTO((SKIPLIST *l, int (*h)(), void *vp, FILE *fp));

extern SKIPLIST * sklinit LIBTPROTO((SKIPLIST *l));

extern void * sklput LIBTPROTO((SKIPLIST *l, void *e, int (*f)(), void *vp));

extern void * skladd LIBTPROTO((SKIPLIST *l, void *e, int (*f)(), void *vp));

extern void * skldel LIBTPROTO((SKIPLIST *l, void *e, int (*f)(), void *vp));

extern void * sklget LIBTPROTO((SKIPLIST *l, void *e, int (*f)(), void *vp));

#endif /* !H_SKIPLISTS */
