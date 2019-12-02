/*
**	K E Y V A L
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Keywords and Values
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Sep 20 15:58:49 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	These functions manage a TREE structure (trees(3l)) into
**	which are placed nodes containing an ASCII key and a
**	corresponding ASCII value. Nodes are inserted
**	into the tree, and then retrived or deleted by key. This
**	provides a table lookup mechanism that is both memory and
**	CPU efficient. Although this is hardly even a simple database
**	system (rather it is intended to provide a simple in-core
**	cache mechanism that is both space and time efficient), it
**	was inspired by BSD's dbm and Sun's Yellow Pages. Unlike dbm,
**	there is no hashing; it's a simple binary search.
**
**	N.B. There is no explicit initialization call. The pointer which
**	is the root of the tree must be initialized to NULL.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)keyval.c	4.31 94/07/18 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libtstrings.h"
#include "trees.h"
#include "lists.h"
#include "buffers.h"
#include "nmalloc.h"
#include "keyval.h"

/*
**	This is a pool of unused keyval structures; keyval always
**	tries to allocate from here and only if the pool is empty
**	does it malloc a new structure. Freed nodes are returned here.
**
**	Note: This list makes use of the t_next field in the
**	tree structure which is used for breadth-first
**	traversal of a tree (and keyvals are based on trees).
**	Since the t_next field is never used for both linking
**	unused keyvals and breadth-first traversal at the same
**	time, all is well.
*/
static LIST pool={NULL,NULL};		/* thread safe: sem_keyval_libt */
static int allocated=0;			/* thread safe: sem_keyval_libt */
static int active=0;			/* thread safe: sem_keyval_libt */

#ifdef KEYVAL_STATS
#define ALLOCATING()	do { \
			P(sem_keyval_libt); \
			allocated++; \
			V(sem_keyval_libt); \
			} while (0)
#define USING()		do { \
			P(sem_keyval_libt); \
			active++; \
			V(sem_keyval_libt); \
			} while (0)
#define FREEING()	(active--)
#else /* KEYVAL_STATS */
#define ALLOCATING()	(0)
#define USING()		(0)
#define FREEING()	(0)
#endif /* KEYVAL_STATS */

/*
**	freenode: free memory associated with a single node.
**
**	Arguments:
**		e	pointer to node
**
**	Returns:	void
*/
static int
freenode(KEYVAL *e, void *unused)
	{
	if (e!=NULL)
		{

		if (e->kv_keyword!=NULL)
			(void)buffree(e->kv_keyword);

		if (e->kv_value!=NULL)
			(void)buffree(e->kv_value);

		P(sem_keyval_libt);
		(void)lput(&pool,(LISTNODE *)&e->kv_tree.t_next);
		FREEING();
		V(sem_keyval_libt);

		}

	return(0);
	}

/*
**	allocnode: allocate memory for, and initialize, a new node.
**
**	Arguments:
**		key	address of key string
**		val	address of val string
**		dup	if true, make copies of strings
**
**	Returns:	pointer to node, or NULL if in error
*/
static KEYVAL *
allocnode(char *key, char *val, int dup)
	{
	KEYVAL *e;

	if (key==NULL)
		return(NULL);

	P(sem_keyval_libt);
	e=(KEYVAL *)lget(&pool);
	V(sem_keyval_libt);

	if (e==NULL)
		if ((e=(KEYVAL *)nmalloc(sizeof(KEYVAL)))==NULL)
			return(NULL);
		else
			ALLOCATING();

	USING();

	if (!dup)
		{
		e->kv_keyword=(BUFFER)key;
		e->kv_value=(BUFFER)val;
		}
	else if ((e->kv_keyword=bufdup(key))==NULL)
		{
		(void)freenode(e,NULL);
		return(NULL);
		}
	else if (val==NULL)
		e->kv_value=(BUFFER)val;
	else if ((e->kv_value=bufdup(val))==NULL)
		{
		(void)freenode(e,NULL);
		return(NULL);
		}

	return(e);
	}

/*
**	kvcompare: compare two keyval nodes and return a value
**	in the manner of strcmp(3). This used to be internal
**	only, but proved useful in other keyval-related contexts.
*/
int
kvcompare(KEYVAL *e, KEYVAL *p)
	{
	return((p==NULL)?e->kv_value==NULL:strcmp(e->kv_keyword,p->kv_keyword));
	}

/*
**	kvget: given a key, return the corresponding value from
**	the tree.
**
**	Arguments:
**		key	pointer to key string
**		tree	pointer to root of tree
**
**	Returns:	pointer to value, or NULL if not found
*/
char *
kvget(char *key, KEYVAL *tree)
	{
	KEYVAL parm, *val;

	parm.kv_keyword=(BUFFER)key;

	if ((val=(KEYVAL *)tget(&(tree->kv_tree),&(parm.kv_tree),kvcompare))==NULL)
		return(NULL);

	return((char *)val->kv_value);
	}

/*
**	kvput: add a new key/value pair to the tree; or if a pair
**	with the same key exists, replace it; or if val is NULL,
**	delete it.
**
**	kvputp: same as kvput except doesn't make a copy of the
**	strings. This is a useful optimization with parcels(3l).
**
**	Arguments:
**		key	pointer to key string
**		val	pointer to value string or NULL if deleting
**		treep	value/result pointer to pointer to root of tree
**
**	Returns:	tree or NULL if error
*/
KEYVAL **
kvput(char *key, char *val, KEYVAL **treep)
	{
	KEYVAL *old, *new;

	if ((new=allocnode(key,val,1))==NULL)
		return(NULL);

	if ((old=(KEYVAL *)tput((TREE **)treep,&(new->kv_tree),kvcompare))!=NULL)
		(void)freenode(old,NULL);

	if (val==NULL)
		(void)freenode(new,NULL);

	return(treep);
	}

KEYVAL **
kvputp(BUFFER key, BUFFER val, KEYVAL **treep)
	{
	KEYVAL *old, *new;

	if ((new=allocnode(key,val,0))==NULL)
		return(NULL);

	if ((old=(KEYVAL *)tput((TREE **)treep,&(new->kv_tree),kvcompare))!=NULL)
		(void)freenode(old,NULL);

	if (val==NULL)
		{
		new->kv_keyword=NULL;
		(void)freenode(new,NULL);
		}

	return(treep);
	}

/*
**	kvfree: free all memory associated with a tree.
**
**	Arguments:
**		tree	pointer to root of tree
**
**	Returns:	zero
*/
int
kvfree(KEYVAL *tree)
	{
	if (tree!=NULL)
		return(tpostorder(&(tree->kv_tree),freenode,(void *)NULL));

	return(0);
	}

/*
**	kvdump: print the contents of a tree in order on
**	standard error (used mostly for debugging).
**
**	Arguments:
**		tree	pointer to root of tree
**
**	Returns:	zero
*/
static int
dumpnode(KEYVAL *node, void *fp)
	{
	if (node!=NULL)
		fprintf((FILE *)fp,"%s=%s\n",node->kv_keyword,node->kv_value);

	return(0);
	}

int
kvdump(KEYVAL *tree, FILE *fp)
	{
	if (tree!=NULL)
		return(tinorder(&(tree->kv_tree),dumpnode,(void *)fp));

	return(0);
	}

/*
**	kvmerge: merge two keyval trees. If the destination tree
**	is empty, this amounts to a copy.
**
**	Arguments:
**		from	pointer to source keyval structure
**		top	address of pointer to destination keyval structure
*/
static int
mergenode(KEYVAL *node, void *top)
	{
	if (node!=NULL)
		if (kvput(node->kv_keyword,node->kv_value,(KEYVAL **)top)==NULL)
			return(-1);

	return(0);
	}

int
kvmerge(KEYVAL *from, KEYVAL **top)
	{
	if (top==NULL)
		return(-1);

	return(tpreorder(&(from->kv_tree),mergenode,(void *)top));
	}

/*
**	kvaugment: merge two keyval trees, copying only those nodes
**	which do not have corresponding keywords in the destination.
**	Compare this with kvmerge() above.
**
**	Arguments:
**		from	pointer to source keyval structure
**		top	address of pointer to destination keyval structure
*/
static int
augmentnode(KEYVAL *node, void *top)
	{
	if (node!=NULL)
		{
		if (kvget(node->kv_keyword,*((KEYVAL **)top))==NULL)
			if (kvput(node->kv_keyword,node->kv_value,(KEYVAL **)top)==NULL)
				return(-1);
		}
	return(0);
	}

int
kvaugment(KEYVAL *from, KEYVAL **top)
	{
	if (top==NULL)
		return(-1);

	return(tpreorder(&(from->kv_tree),augmentnode,(void *)top));
	}

/*
**	kvcount: count the number of nodes in a tree.
**
**	Arguments:
**		tree	tree structure
**
**	Returns:	count
*/
static int
countnode(KEYVAL *node, void *countp)
	{
	if (node!=NULL)
		(*((int *)countp))++;

	return(0);
	}

int
kvcount(KEYVAL *tree)
	{
	int count;

	count=0;

	if (tree!=NULL)
		(void)tinorder(&(tree->kv_tree),countnode,(void *)&count);

	return(count);
	}

/*
**	kvpool: print out the current pool statistics. Mostly
**	used for debugging.
*/
int
kvpool(FILE *fp)
	{
	int total;

	P(sem_keyval_libt);
	total=sizeof(KEYVAL)*allocated;
	fprintf(fp,"kvpool: %8dbytes %8dactive %8dallocated\n",
		total,active,allocated);
	V(sem_keyval_libt);

	return(total);
	}

/*
**	kvshutdown: release all memory associated with keyvals.
*/
void kvshutdown(void)
	{
	KEYVAL *e;

	P(sem_keyval_libt);

	for (e=(KEYVAL *)lget(&pool); e!=NULL; e=(KEYVAL *)lget(&pool))
		(void)nfree((void *)e);

	/* N.B. Cannot release what the application owns */
	allocated=active;

	V(sem_keyval_libt);
	}
