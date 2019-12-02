/*
**	S K E Y V A L
**
**	Copyright 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Keywords and Values
**	Program		libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Oct 11 13:19:00 MDT 1993
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	These functions manage a SKIPLIST structure (skiplists(3l)) into
**	which are placed nodes containing an ASCII key and a
**	corresponding ASCII value. Nodes are inserted
**	into the list, and then retrived or deleted by key. This
**	provides a table lookup mechanism that is both memory and
**	CPU efficient. Although this is hardly even a simple database
**	system (rather it is intended to provide a simple in-core
**	cache mechanism that is both space and time efficient), it
**	was inspired by BSD's dbm and Sun's Yellow Pages. Unlike dbm,
**	there is no hashing; it's a skiplist search.
**
**	This code is a little peculiar because it was designed to
**	mimic as closely as possible the calling sequences of functions
**	in the keyval module. Because of some basic differences between
**	trees (on which keyvals are based) and skiplists (on which
**	skeyvals are based), I had to jury rig some stuff to keep from
**	having to change mucho code in other modules.
*/

static char copyright[]="Copyright 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)skeyval.c	4.3 94/07/18 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libtstrings.h"
#include "skiplists.h"
#include "lists.h"
#include "buffers.h"
#include "nmalloc.h"
#include "skeyval.h"

/*
**	This is a pool of unused skeyval structures; skeyval always
**	tries to allocate from here and only if the pool is empty
**	does it malloc a new structure. Freed nodes are returned here.
*/
static LIST pool={NULL,NULL};		/* thread safe: sem_skeyval_libt */
static int allocated=0;			/* thread safe: sem_skeyval_libt */
static int active=0;			/* thread safe: sem_skeyval_libt */

#ifdef SKEYVAL_STATS
#define ALLOCATING()	do { \
			P(sem_skeyval_libt); \
			allocated++; \
			V(sem_skeyval_libt); \
			} while (0)
#define USING()		do { \
			P(sem_skeyval_libt); \
			active++; \
			V(sem_skeyval_libt); \
			} while (0)
#define FREEING()	(active--)
#else /* SKEYVAL_STATS */
#define ALLOCATING()	(0)
#define USING()		(0)
#define FREEING()	(0)
#endif /* SKEYVAL_STATS */

/*
**	skvinit: initialize a skeyvals structure.
*/
SKEYVALS *
skvinit(SKEYVALS *list)
	{
	return(sklinit(list));
	}

/*
**	freenode: free memory associated with a single node.
**
**	Arguments:
**		e	pointer to node
**
**	Returns:	void
*/
static int
freenode(void *ee, void *unused)
	{
	SKEYVAL *e;

	if ((e=(SKEYVAL *)ee)!=(SKEYVAL *)NULL)
		{
		if (e->skv_keyword!=NULL)
			(void)buffree(e->skv_keyword);

		if (e->skv_value!=NULL)
			(void)buffree(e->skv_value);

		P(sem_skeyval_libt);
		(void)lput(&pool,&e->skv_next);
		FREEING();
		V(sem_skeyval_libt);
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
static SKEYVAL *
allocnode(char *key, char *val, int dup)
	{
	SKEYVAL *e;

	if (key==NULL)
		return(NULL);

	P(sem_skeyval_libt);
	e=(SKEYVAL *)lget(&pool);
	V(sem_skeyval_libt);

	if (e==NULL)
		if ((e=(SKEYVAL *)nmalloc(sizeof(SKEYVAL)))==NULL)
			return(NULL);
		else
			ALLOCATING();

	USING();

	if (!dup)
		{
		e->skv_keyword=(BUFFER)key;
		e->skv_value=(BUFFER)val;
		}
	else if ((e->skv_keyword=bufdup(key))==NULL)
		{
		(void)freenode((void *)e,(void *)NULL);
		return(NULL);
		}
	else if (val==NULL)
		e->skv_value=(BUFFER)val;
	else if ((e->skv_value=bufdup(val))==NULL)
		{
		(void)freenode((void *)e,(void *)NULL);
		return(NULL);
		}

	return(e);
	}

/*
**	skvcompare: compare two skeyval nodes and return a value
**	in the manner of strcmp(3). This used to be internal
**	only, but proved useful in other skeyval-related contexts.
*/
static int
skvcompare(void *ee, void *pp)
	{
	SKEYVAL *e, *p;

	e=(SKEYVAL *)ee;
	p=(SKEYVAL *)pp;
	return((p==NULL)?e->skv_value==NULL:strcmp(e->skv_keyword,p->skv_keyword));
	}

/*
**	skvget: given a key, return the corresponding value from
**	the list.
**
**	Arguments:
**		key	pointer to key string
**		list	pointer to list
**
**	Returns:	pointer to value, or NULL if not found
*/
char *
skvget(char *key, SKEYVALS *list)
	{
	SKEYVAL parm, *val;

	parm.skv_keyword=(BUFFER)key;

	if ((val=(SKEYVAL *)sklget(list,(void *)&parm,skvcompare,(void *)0))==NULL)
		return(NULL);

	return((char *)val->skv_value);
	}

/*
**	skvput: add a new key/value pair to the list; or if a pair
**	with the same key exists, replace it; or if val is NULL,
**	delete it.
**
**	skvputp: same as kvput except doesn't make a copy of the
**	strings. This is a useful optimization with parcels(3l).
**
**	Arguments:
**		key	pointer to key string
**		val	pointer to value string or NULL if deleting
**		list	pointer to list
**
**	Returns:	list or NULL if error
*/
SKEYVALS *
skvput(char *key, char *val, SKEYVALS *list)
	{
	SKEYVAL *old, *new, del;

	if (val==NULL)
		{
		del.skv_keyword=(BUFFER)key;
		if ((old=skldel(list,(void *)&del,skvcompare,(void *)0))!=(SKEYVAL *)NULL)
			(void)freenode((void *)old,(void *)NULL);
		}
	else
		{
		if ((new=allocnode(key,val,1))==NULL)
			return(NULL);
		if ((old=(SKEYVAL *)sklput(list,(void *)new,skvcompare,(void *)0))==(SKEYVAL *)NULL)
			{
			(void)freenode((void *)new,(void *)NULL);
			return(NULL);
			}
		if (old!=new)
			(void)freenode((void *)old,(void *)NULL);
		}

	return(list);
	}

SKEYVALS *
skvputp(BUFFER key, BUFFER val, SKEYVALS *list)
	{
	SKEYVAL *old, *new, del;

	if (val==NULL)
		{
		del.skv_keyword=key;
		if ((old=skldel(list,(void *)&del,skvcompare,(void *)0))!=(SKEYVAL *)NULL)
			(void)freenode((void *)old,(void *)NULL);
		}
	else
		{
		if ((new=allocnode(key,val,0))==NULL)
			return(NULL);
		if ((old=(SKEYVAL *)sklput(list,(void *)new,skvcompare,(void *)0))==(SKEYVAL *)NULL)
			{
			(void)freenode((void *)new,(void *)NULL);
			return(NULL);
			}
		if (old!=new)
			(void)freenode((void *)old,(void *)NULL);
		}

	return(list);
	}

/*
**	skvfree: free all memory associated with a list.
**
**	Arguments:
**		list	pointer to list
**
**	Returns:	zero
*/
int
skvfree(SKEYVALS *list)
	{
	return(sklfree(list,freenode,(void *)NULL));
	}

/*
**	skvdump: print the contents of a list in order on
**	standard error (used mostly for debugging).
**
**	Arguments:
**		list	pointer to list
**
**	Returns:	zero
*/
static int
dumpnode(void *ee, void *fp)
	{
	SKEYVAL *e;

	if ((e=(SKEYVAL *)ee)!=(SKEYVAL *)NULL)
		fprintf((FILE *)fp,"%s=%s\n",e->skv_keyword,e->skv_value);
	return(0);
	}

int
skvdump(SKEYVALS *list, FILE *fp)
	{
	return(sklinorder(list,dumpnode,(void *)fp));
	}

/*
**	skvmerge: merge two skeyval lists. If the destination list
**	is empty, this amounts to a copy.
**
**	Arguments:
**		from	pointer to source skeyval structure
**		to	pointer to destination skeyval structure
*/
static int
mergenode(void *ee, void *to)
	{
	SKEYVAL *e;

	if ((e=(SKEYVAL *)ee)!=(SKEYVAL *)NULL)
		if (skvput(e->skv_keyword,e->skv_value,(SKEYVALS *)to)==(SKEYVALS *)NULL)
			return(-1);

	return(0);
	}

int
skvmerge(SKEYVALS *from, SKEYVALS *to)
	{
	return(sklinorder(from,mergenode,(void *)to));
	}

/*
**	skvaugment: merge two skeyval lists, copying only those nodes
**	which do not have corresponding keywords in the destination.
**	Compare this with skvmerge() above.
**
**	Arguments:
**		from	pointer to source skeyval structure
**		to	pointer to destination skeyval structure
*/
static int
augmentnode(void *ee, void *to)
	{
	SKEYVAL *e;

	if ((e=(SKEYVAL *)ee)!=(SKEYVAL *)NULL)
		if (skvget(e->skv_keyword,(SKEYVALS *)to)==NULL)
			if (skvput(e->skv_keyword,e->skv_value,(SKEYVALS *)to)==(SKEYVALS *)NULL)
				return(-1);
	return(0);
	}

int
skvaugment(SKEYVALS *from, SKEYVALS *to)
	{
	return(sklinorder(from,augmentnode,(void *)to));
	}

/*
**	skvcount: count the number of nodes in a list.
**
**	Arguments:
**		list	list
**
**	Returns:	count
*/
static int
countnode(void *ee, void *countp)
	{
	if (ee!=(void *)NULL)
		(*((int *)countp))++;
	return(0);
	}

int
skvcount(SKEYVALS *list)
	{
	int count;

	count=0;
	(void)sklinorder(list,countnode,(void *)&count);
	return(count);
	}

/*
**	skvpool: print out the current pool statistics. Mostly
**	used for debugging.
*/
int
skvpool(FILE *fp)
	{
	int total;

	P(sem_skeyval_libt);
	total=sizeof(SKEYVAL)*allocated;
	fprintf(fp,"skvpool: %8dbytes %8dactive %8dallocated\n",
		total,active,allocated);
	V(sem_skeyval_libt);

	return(total);
	}

/*
**	skvshutdown: release all memory associated with skeyvals.
*/
void
skvshutdown(void)
	{
	SKEYVAL *e;

	P(sem_skeyval_libt);

	for (e=(SKEYVAL *)lget(&pool); e!=NULL; e=(SKEYVAL *)lget(&pool))
		(void)nfree((void *)e);

	/* N.B. Cannot release what the application owns */
	allocated=active;

	V(sem_skeyval_libt);
	}
