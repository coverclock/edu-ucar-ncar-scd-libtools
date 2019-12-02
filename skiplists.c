/*
**	S K I P   L I S T S
**
**	Copyright 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		skiplists
**	Program		Skip Lists
**	Project		libtools
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Jun 29 11:31:06 MDT 1993
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This module implements the skiplist data type. This software
**	is based on C code originally written by William Pugh at the
**	University of Maryland Institute for Advanced Computer Science.
**	The code has been modified to fit into the usual libtools way of
**	doing things.
**
**	From the original source code by Dr. Pugh: "Skip Lists are a
**	probabilistic alternative to balanced trees, as described in
**	the June 1990 issue of CACM and were invented by William Pugh
**	in 1987."
**
**	Unlike trees, skiplists have a maximum possible size. However,
**	in this implementation, it's really really really big. If I
**	interpret [Pugh 1990] correctly, it's around 4^16 nodes.
**	This is based on
**
**		MaxLevels = log   (MaxSize)
**	                       1/p
**
**	where p=0.25 and MaxLevels=16. The function randomlevel() has
**	a brief discussion on how the value of p is determined.
**
**	References
**
**	Pugh, William, "Skip Lists: A Probabilistic Alternative to
**	Balanced Trees", CACM, June 1990
**
**	Pugh, William, "Concurrent Maintenance of Skip Lists",
**	UMIACS-TR-90-80, U. of Md. Inst. for Adv. Comp. Sci., April 1989
**
**	Pugh, William, "A Skip List Cookbook", UMIACS-TR-89-72.1,
**	U. of Md. Inst. for Adv. Comp. Sci., July 1989
**
**	Pugh, William, original C and Pascal source code and documents,
**	mimsy.umd.edu:pub/skipLists
*/

static char copyright[]="Copyright 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)skiplists.c	4.5 96/06/10 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <stdlib.h>
#include "libtstrings.h"
#include "libtvalues.h"
#include "nmalloc.h"
#include "lists.h"
#include "skiplists.h"

#define SKIPLISTLEV		(BITS(long)/2)
#define SKIPLISTMAX		(SKIPLISTLEV-1)

/*
**	This portion of the module manages the pools of skiplist
**	nodes. We only allocate a new skiplist node when we have
**	too. Otherwise allocation and deallocation is a trivial
**	link or unlink. Note that the skiplist node contains a link
**	to the user data. The user data itself is opaque, and is only
**	dealt with using functions passed to this module by the caller.
*/

#ifdef SKIPLIST_STATS
#define ALLOCATING(l)	do { \
			P(sem_skiplist_libt); \
			allocated[l]++; \
			V(sem_skiplist_libt); \
			} while (0)
#define USING(l)	do { \
			P(sem_skiplist_libt); \
			active[l]++; \
			V(sem_skiplist_libt); \
			} while (0)
#define FREEING(l)	(active[l]--)

#else /* SKIPLIST_STATS */
#define ALLOCATING(l)	(0)
#define USING(l)	(0)
#define FREEING(l)	(0)
#endif /* SKIPLIST_STATS */

static LIST pools[SKIPLISTLEV]={NULL};	/* thread safe: sem_skiplist_libt */
static int allocated[SKIPLISTLEV]={0};	/* thread safe: sem_skiplist_libt */
static int active[SKIPLISTLEV]={0};	/* thread safe: sem_skiplist_libt */

static int randombits=0;		/* thread safe: sem_skiplist_libt */
static int randomleft=0;		/* thread safe: sem_skiplist_libt */
static int randomfirst=1;		/* thread safe: sem_skiplist_libt */
static int randomusable=0;		/* thread safe: sem_skiplist_libt */

/*
**	freenode: free memory associated with a single node.
**
**	Arguments:
**		e	pointer to node
**		h	pointer to function to free data (maybe NULL)
**		vp	pointer to state info
**
**	Returns:	return code from h (0 if okay)
*/
static int
freenode(SKIPLISTNODE *e, int (*h)(), void *vp)
	{
	int rc;

	rc=0;
	if (e!=NULL)
		{
		if ((h!=NULL)&&(e->skl_link!=NULL))
			rc=(*h)(e->skl_link,vp);
		P(sem_skiplist_libt);
		(void)lput(&(pools[e->skl_level]),(LISTNODE *)&e->skl_link);
		FREEING(e->skl_level);
		V(sem_skiplist_libt);
		}

	return(rc);
	}

/*
**	allocnode: allocate memory for, and initialize, a new node.
**
**	Arguments:
**		level	level of node to allocate
**		node	pointer to data node (may be NULL)
**
**	Returns:	pointer to node, or NULL if in error
*/
static SKIPLISTNODE *
allocnode(int level, void *node)
	{
	SKIPLISTNODE *e;

	if (!((0<=level)&&(level<=SKIPLISTMAX)))
		return(NULL);

	P(sem_skiplist_libt);
	e=(SKIPLISTNODE *)lget(&pools[level]);
	V(sem_skiplist_libt);

	if (e==NULL)
		if ((e=(SKIPLISTNODE *)nmalloc(sizeof(SKIPLISTNODE)+(level*sizeof(SKIPLISTNODE *))))==NULL)
			return(NULL);
		else
			ALLOCATING(level);

	e->skl_link=node;
	e->skl_level=level;
	(void)bzero(&e->skl_next[0],(sizeof(SKIPLISTNODE *)*(level+1)));
		
	USING(level);

	return(e);
	}

/*
**	randomlevel: generate a random level for use by insert().
**
**	This algorithm hardcodes p=0.25. Notice the mask which picks
**	off two bits (&0x3) which are then shifted out (>>2). Since
**	the level is only incremented when the bits are 00, and the
**	choices are (00, 01, 10, 11), this yields p=0.25. A rand()
**	return of one int yields log2(RAND_MAX) usable bits and
**	(31/2) or 15 usable 2-bit random numbers. This value of 15 has
**	no correlation (as I understand the algorithm) with the maximum
**	number of levels.
**
**	This function (derived from Pugh's original C code) implements the
**	"dirty hack" described in his paper by short circuiting the loop
**	if the generated level is equal to the maximum. Given the frequency
**	with which this should occur, this check is probably less
**	efficient than Pugh's original code. However, statistically
**	speaking, an infinite loop is possible with his original code
**	(a one in a bazillion chance).
*/
static int
randomlevel(void)
	{
	int level, i, b, n;

	P(sem_skiplist_libt);

	if (randomfirst)
		{
		randombits=rand();
		/*
		** [1] randomusable=(BITS(int)-1)/2;
		**
		** [2] randomusable=log2((double)RAND_MAX)/2;
		*/
		randomusable=0;
		for (i=BITS(n)-1, b=1, n=RAND_MAX; i>0; i--, b++, n>>=1)
			if (n&1)
				randomusable=b;
		randomusable/=2;
		randomleft=randomusable;
		randomfirst=0;
		}

	level=0;
   	do
		{
		if (level==SKIPLISTMAX)
			break;
		b=randombits&0x3;
		if (!b)
			level++;
		randombits>>=2;
		randomleft--;
		if (randomleft==0)
			{
			randombits=rand();
			randomleft=randomusable;
			}
		}
	while (!b);

	V(sem_skiplist_libt);

	return(level);
	}

/*
**	search: common generic skiplist search algorithm.
*/
static SKIPLISTNODE *
search(SKIPLIST *l, void *e, int (*f)(), void *vp, SKIPLISTNODE **upd)
	{
	int k;
	SKIPLISTNODE *p, *q;

	for (q=NULL, p=l->skl_head, k=l->skl_levels; k>=0; k--)
		{
		while (((q=p->skl_next[k])!=NULL)&&((*f)(q->skl_link,e,vp)<0))
			p=q;
		upd[k]=p;
		}

	return(q);
	}

/*
**	insert: common generic skiplist insert algorithm.
*/
static void *
insert(SKIPLIST *l, void *e, SKIPLISTNODE **upd)
	{
	int k;
	SKIPLISTNODE *p, *q;

	k=randomlevel();

    	if (k>l->skl_levels)
		{
		l->skl_levels++;
		k=l->skl_levels;
		upd[k]=l->skl_head;
		}

	if ((q=allocnode(k,e))==NULL)
		return(NULL);

	for (; k>=0; k--)
		{	
		p=upd[k];
		q->skl_next[k]=p->skl_next[k];
		p->skl_next[k]=q;
		}

	return(e);
	}

/*
**	sklinorder: traverses a skiplist, applying a caller
**	supplied function to data segment on each node.
**
**	Arguments:
**		l	pointer to skiplist
**		h	function to apply to each node; if h returns
**			non-zero, traversal is shorted and sklinorder
**			returns h value
**		vp	pointer to be passed along (may be NULL)
**
**	Prototype:	int (*h)(void *userptr, void *vp)
*/
int
sklinorder(SKIPLIST *l, int (*h)(), void *vp)
	{
	SKIPLISTNODE *e;
	int rc;

	if (h==NULL)
		return(0);

	e=l->skl_head;
	if (e!=NULL)
		for (e=e->skl_next[0]; e!=NULL; e=e->skl_next[0])
			if ((rc=(*h)(e->skl_link,vp))!=0)
				return(rc);

	return(0);
	}

/*
**	sklfree: free all memory associated with a skiplist.
**
**	Arguments:
**		l	pointer to list
**		h	pointer to function to free data segment
**		vp	pointer to state data
**
**	Returns:	rc from h (zero if okay)
**
**	Prototype:	int (*h)(void *userptr, void *vp)
*/
int
sklfree(SKIPLIST *l, int (*h)(), void *vp)
	{
	SKIPLISTNODE *p, *q;
	int rc, rrc;

	if (l!=NULL)
		for (rrc=0, p=l->skl_head; p!=NULL; p=q)
			{
    			q=p->skl_next[0];
    			if ((rc=freenode(p,h,vp))!=0)
				rrc=rc;
			}
	return(rrc);
	}

/*
**	sklcount: count the number of nodes in a skiplist.
**
**	Arguments:
**		l	skiplist
**
**	Returns:	count
*/
static int
countnode(SKIPLISTNODE *n, void *countp)
	{
	if (n!=NULL)
		(*((int *)countp))++;

	return(0);
	}

int
sklcount(SKIPLIST *l)
	{
	int count;

	count=0;

	if (l!=NULL)
		(void)sklinorder(l,countnode,(void *)&count);

	return(count);
	}

/*
**	sklpool: print out the current pool statistics. Mostly
**	used for debugging.
*/
int
sklpool(FILE *fp)
	{
	int i, b, tb, tact, tall;

	P(sem_skiplist_libt);

	for (tb=0, tact=0, tall=0, i=0; i<SKIPLISTLEV; i++)
		{
		b=(sizeof(SKIPLISTNODE)+(i*sizeof(SKIPLISTNODE *)))
			*allocated[i];
		tb+=b;
		tact+=active[i];
		tall+=allocated[i];
		fprintf(fp,
			"sklpool: %8dlevel %8dbytes %8dactive %8dallocated\n",
			i,b,active[i],allocated[i]);
		}
		
	V(sem_skiplist_libt);

	fprintf(fp,"sklpool:         total %8dbytes %8dactive %8dallocated\n",
		tb,tact,tall);

	return(tb);
	}

/*
**	sklshutdown: release all memory associated with skiplists.
*/
void
sklshutdown(void)
	{
	SKIPLISTNODE *e;
	int i;

	P(sem_skiplist_libt);

	for (i=0; i<SKIPLISTLEV; i++)
		{
		for (e=(SKIPLISTNODE *)lget(&pools[i]); e!=NULL; 
				e=(SKIPLISTNODE *)lget(&pools[i]))
			(void)nfree((void *)e);
		/* N.B. Cannot release what the application owns */
		allocated[i]=active[i];
		}

	V(sem_skiplist_libt);
	}

/*
**	sklinit: initialize a skiplist. It is assumed to already
**	be empty, since the pointers are tossed away. The caller
**	is responsible for calling sklfree() beforehand as necessary.
*/
SKIPLIST *
sklinit(SKIPLIST *l)
	{
	SKIPLISTNODE **p;
	int i;

	if (l!=NULL)
		{
		l->skl_levels=0;
		if ((l->skl_head=allocnode(SKIPLISTMAX,NULL))==NULL)
			return(NULL);
		/* allocnode() zeros out skl_next array. */
		}

	return(l);
	}

/*
**	sklput: put an item in the skiplist. If an error occurs, NULL
**	is returned. If an item already exists with the same key, the
**	item is replaced, and the user pointer to the old item is
**	returned (e.g. useful for free'ing dynamically acquired storage).
**	Otherwise, the item is added to the skiplist and the user
**	pointer to the new item is returned (same as the value of e in
**	the calling sequence). Hence the caller must compare the return
**	value to NULL and to e to determine if the return value points
**	to an old item.
**
**	Prototype:	int (*f)(void *userptr1, void *userptr2, void *vp)
*/
void *
sklput(SKIPLIST *l, void *e, int (*f)(), void *vp)
	{
	int k;
	void *o;
	SKIPLISTNODE *upd[SKIPLISTLEV], *p, *q;

	if ((q=search(l,e,f,vp,upd))!=NULL)
		if ((*f)(q->skl_link,e,vp)==0)
			{
			/*
			** Node exists. Change it.
			*/
			o=q->skl_link;
			q->skl_link=e;
			return(o);
			}

	/*
	** Node does not exist. Insert it.
	*/

	return(insert(l,e,upd));
	}

/*
**	skladd: put an item in the skiplist. If an error occurs,
**	NULL is returned. If an item with the same key already
**	exists in the skiplist, the new item is added anyway (so
**	many items with the same key may exist in the skiplist),
**	and the user pointer to the new item is returned.
**
**	Prototype:	int (*f)(void *userptr1, void *userptr2, void *vp)
*/
void *
skladd(SKIPLIST *l, void *e, int (*f)(), void *vp)
	{
	int k;
	void *o;
	SKIPLISTNODE *upd[SKIPLISTLEV], *p, *q;

	(void)search(l,e,f,vp,upd);
	return(insert(l,e,upd));
	}

/*
**	skldel: delete an item from a skiplist. If the item is
**	found in the skiplist, the user pointer to the item is
**	returned (e.g. useful for free'ing dynamically acquired
**	storage) and the item is deleted. If the item is not
**	found, NULL is returned.
**
**	Prototype:	int (*f)(void *userptr1, void *userptr2, void *vp)
*/
void *
skldel(SKIPLIST *l, void *e, int (*f)(), void *vp)
	{
	int k, m;
	void *o;
	SKIPLISTNODE *upd[SKIPLISTLEV], *p, *q;

	if ((q=search(l,e,f,vp,upd))!=NULL)
		if ((*f)(q->skl_link,e,vp)==0)
			{
			/*
			** Node exists. Delete it.
			*/
			o=q->skl_link;
			m=l->skl_levels;
			for (k=0; ((k<=m)&&((p=upd[k])->skl_next[k]==q)); k++) 
	  			p->skl_next[k]=q->skl_next[k];
			(void)freenode(q,(int (*)())NULL,(void *)NULL);
        		while((l->skl_head->skl_next[m]==NULL)&&(m>0))
	     			m--;
			l->skl_levels=m;
			return(o);
			}

	/*
	** Node does not exist.
	*/

	return(NULL);
	}

/*
**	sklget: get an item from a skiplist. If the item is found,
**	the user pointer to the item is returned. If it is not found,
**	NULL is returned.
**
**	Prototype:	int (*f)(void *userptr1, void *userptr2, void *vp)
*/
void *
sklget(SKIPLIST *l, void *e, int (*f)(), void *vp)
	{
	int k;
	SKIPLISTNODE *upd[SKIPLISTLEV], *p, *q;

	if ((q=search(l,e,f,vp,upd))!=NULL)
		if ((*f)(q->skl_link,e,vp)==0)
			return(q->skl_link);

	return(NULL);
	}

/*
**	skldump: dump a skiplist to the indicated file descriptor.
**
**	Arguments:
**		l	pointer to skiplist
**		h	function to dump user portion
**		vp	void pointer passed to h()
**		fp	file pointer
**
**	Returns:	number of items in skiplist
**
**	Prototype:	int (*h)(void *userptr, void *vp, FILE *fp)
*/
int
skldump(SKIPLIST *l, int (*h)(), void *vp, FILE *fp)
	{
	SKIPLISTNODE *e;
	int ll, c;

	if (l==NULL)
		{
		fprintf(fp,"skldump: skiplist=0\n",l);
		return(0);
		}

	e=l->skl_head;

	fprintf(fp,"skldump: skiplist 0x%lx(%d,0x%lx)\n",
		l,l->skl_levels,e);

	c=0;
	while (e!=NULL)
		{
		fprintf(fp,"skldump: skipnode 0x%lx(0x%lx,%d,(",
			e,e->skl_link,e->skl_level);
		for (ll=0; ll<=e->skl_level; ll++)
			fprintf(fp," %lx",e->skl_next[ll]);
		fprintf(fp," ))\n");

		if (h!=NULL)
			if (e!=l->skl_head)
				(void)(*h)(e->skl_link,vp,fp);

		e=e->skl_next[0];
		c++;
		}

	return(c);
	}
