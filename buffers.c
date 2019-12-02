/*
**	B U F F E R S
**
**	Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Buffers
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Jun 18 13:03:40 MDT 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Problem: The keyval(3l) functions malloc and free a LOT of
**	strings, usually in support of parcel-based operations (message
**	passing and parameter handling). Not only is malloc inefficient,
**	but short strings cause the heap to be fragmented. This package
**	still uses malloc, but only malloc's buffers of certain fixed
**	lengths, and keeps free'd buffers around in pools to be reused.
**	This leads to some overhead and wasteage, but is about 30%
**	faster (on my Sun-4), and also instrumentable.
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)buffers.c	4.34 96/06/10 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "nmalloc.h"
#include "libtstrings.h"
#include "lists.h"
#include "slack.h"
#include "buffers.h"

/*
**	Buffer is prefix prepended to each actual buffer.
*/

struct Buffer
	{
	LISTNODE	B_next;		/* pointer to next Buffer */
	int		B_size;		/* size of this buffer */
	};

#define EXTRA		(SLACK(sizeof(struct Buffer),sizeof(double)))
#define	OVERHEAD	(sizeof(struct Buffer)+EXTRA)
#define SIZE(l)		((l)+SLACK((l),sizeof(double))+OVERHEAD)
#define FRONT(c)	((struct Buffer *)(((char *)(c))-OVERHEAD))
#define BACK(c)		((BUFFER)(((char *)(c))+OVERHEAD))

/*
**	Pool is the structure that manages a collection of buffers
**	of common length. Some of the fields are for maintaining
**	statistics (if BUFFER_STATS is defined). The macros below
**	define the statistics gathering functions.
*/

struct Pool
	{
	int	P_size;		/* size of strings in this pool */
	int	P_inuse;	/* number of strings in use from this pool */
	int	P_count;	/* total number of strings from this pool */
	int	P_slack;	/* average slack bytes per string */
	LIST	P_list;		/* FIFO of Buffers */
	};

#ifdef BUFFERS_STATS
#define ALLOCATING(p)	((p)->P_count++)
#define USING(p,l)	do { \
			(p)->P_inuse++; \
			(p)->P_slack=(((p)->P_slack*((p)->P_inuse-1))+ \
				((p)->P_size-(l)))/(p)->P_inuse; \
			} while (0)
#define FREEING(p)	((p)->P_inuse--)
#else /* BUFFER_STATS */
#define ALLOCATING(p)	(0)
#define USING(p,l)	(0)
#define FREEING(p)	(0)
#endif /* BUFFER_STATS */

/*
**	malloc() allocates on double word boundries so that the
**	result pointer is compatible with virtually pointer
**	type. Hence, it doesn't make sense to allocate anything
**	smaller than doublewords, or else you'll have
**	some unusable slack at the end of the string. Also, each
**	string has eight bytes of overhead. Yeah, I know, that
**	seems excessive, but I decided that the string itself
**	(not just the prefix) should start on a double word too,
**	so that we don't have to make too many assumptions about
**	what the caller is _really_ using the string buffer for.
**
**	Below is a hash function which generates a pool array
**	index from a string length. It is defined as a macro
**	to produce in-line code for performance, and there is
**	also a function call for regression testing.
**
**	MINPOWER should be set such that (2^MINPOWER)>=sizeof(double).
*/

#define MINPOWER	3

#define POOL(i)	{(1<<((i)+MINPOWER)),0,0,0,NULL,NULL}

static struct Pool pools[]=		/* thread safe: sem_buffers_libt */
        {
        POOL(0),        /* [0]:2^3=8 */
        POOL(1),        /* [1]:2^4=16 */
        POOL(2),        /* [2]:2^5=32 */
        POOL(3),        /* [3]:2^6=64 */
        POOL(4),        /* [4]:2^7=128 */
        POOL(5),        /* [5]:2^8=256 */
        POOL(6),        /* [6]:2^9=512 */
        POOL(7),        /* [7]:2^10=1024 */
        POOL(8),        /* [8]:2^11=2048 */
        POOL(9),        /* [9]:2^12=4096 */
        POOL(10)        /* [10]:2^13=8192 */
        };

#define MAXINDEX	((sizeof(pools)/sizeof(struct Pool))-1)

#define HASH(l,i)	for (i=0, l=((l)>0)?(((l)-1)>>2):0; \
					(i<=MAXINDEX)&&(l>1); \
					i++, l>>=1) \
                		continue;

/*
**	bufhash: generate an index into the string pools given a
**	length. This function is global to aid in regression testing.
**
**	N.B.	Could be faster to do a log2 function here ... but I
**		bet not, since it would have to convert to double and
**		back again, as well as doing the actual log2 calculation.
*/
int
bufhash(unsigned int l)
        {
	int i;

	HASH(l,i)
	return(i);
        }

/*
**	buflength: return the actual allocated size of a string.
*/
int
buflength(BUFFER c)
	{
	struct Buffer *s;

	if (c==NULL)
		{
		seterrno(EINVAL);
		return(-1);
		}
	s=FRONT(c);
	return(s->B_size);
	}

/*
**	bufalloc: allocate a string.
*/
BUFFER
bufalloc(int l)
	{
	int i;
	unsigned int ll;
	struct Pool *p;
	struct Buffer *s;
	BUFFER c;

	if (l<0)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	ll=l;
	HASH(ll,i);

	c=NULL;

	if (i>MAXINDEX)
		{

		/*
		** No pool large enough, malloc.
		*/

		if ((s=(struct Buffer *)namalloc(SIZE(l)))!=NULL)
			{
			s->B_size=l;
			c=BACK(s);
			}

		}
	else
		{

		/*
		** Assign a string from a pool; if the pool is empty,
		** malloc a new string of a standard size.
		*/

		P(sem_buffers_libt);

		p=&pools[i];

		if ((s=(struct Buffer *)lget(&p->P_list))==NULL)
			if ((s=(struct Buffer *)namalloc(SIZE(p->P_size)))!=NULL)
				ALLOCATING(p);

		if (s!=NULL)
			{
			s->B_size=p->P_size;
			c=BACK(s);
			USING(p,l);
			}

		V(sem_buffers_libt);

		}

	return(c);
	}


/*
**	buffree: free the string c.
*/
int
buffree(BUFFER c)
	{
	int i;
	unsigned int ll;
	struct Pool *p;
	struct Buffer *s;

	if (c==NULL)
		{
		seterrno(EINVAL);
		return(-10);
		}

	s=FRONT(c);

	ll=s->B_size;
	HASH(ll,i);

	if (i>MAXINDEX)
		nfree((void *)s);
	else
		{
		P(sem_buffers_libt);

		p=&pools[i];
		(void)lput(&p->P_list,&s->B_next);
		FREEING(p);

		V(sem_buffers_libt);
		}

	return(0);
	}

/*
**	bufdup: allocate a string at least as long as the contents of
**	string s (including the terminating nul), copy the contents
**	of s into it, and return it.
*/
BUFFER
bufdup(char *c)
	{
	BUFFER n;

	if (c==NULL)
		return(NULL);

	if ((n=bufalloc(strlen(c)+1))==NULL)
		return(NULL);
	(void)strcpy(n,c);

	return(n);
	}

/*
**	bufinsure: if string s is not at least l long, allocate a
**	new string of length at least l, free s, and return a pointer
**	to the new string.
**
**	bufrealloc: same as bufinsure, except the contents of the original
**	string is copied into the new string.
**
**	bufexpand: same as bufrealloc, except strcpy is used instead of
**	bcopy.
*/
static BUFFER
insure(BUFFER c, int l)
	{
	int r;

	if (l<0)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	if ((r=buflength(c))<0)
		return(NULL);

	return((r>=l)?c:bufalloc(l));
	}

BUFFER
bufinsure(BUFFER c, int l)
	{
	BUFFER n;

	if ((n=insure(c,l))==NULL)
		return(NULL);

	if (c!=n)
		(void)buffree(c);

	return(n);
	}

BUFFER
bufrealloc(BUFFER c, int l)
	{
	BUFFER n;

	if ((n=insure(c,l))==NULL)
		return(NULL);

	if (c!=n)
		{
		(void)bcopy(c,n,buflength(c));
		(void)buffree(c);
		}

	return(n);
	}

BUFFER
bufexpand(BUFFER c, int l)
	{
	BUFFER n;

	if ((n=insure(c,l))==NULL)
		return(NULL);

	if (c!=n)
		{
		(void)strcpy(n,c);
		(void)buffree(c);
		}

	return(n);
	}

/*

/*
**	bufpool: dump the pool statistics. Mostly for debugging.
*/
int
bufpool(FILE *fp)
	{
	int i, tsize, tinuse, tcount;

	P(sem_buffers_libt);

	for (i=0, tsize=0, tinuse=0, tcount=0; i<=MAXINDEX; i++)
		{
		fprintf(fp,
	"bufpool: %8dbytes %8dactive %8dallocated %8davgslack\n",
			pools[i].P_size,pools[i].P_inuse,
			pools[i].P_count,pools[i].P_slack);
		tsize+=(pools[i].P_size+sizeof(struct Buffer))*pools[i].P_count;
		tinuse+=pools[i].P_inuse;
		tcount+=pools[i].P_count;
		}

	fprintf(fp,
		"bufpool: %8dbytes %8dactive %8dallocated         total\n",
		tsize,tinuse,tcount);

	V(sem_buffers_libt);

	return(tsize);
	}

/*
**	bufshutdown: release all internally held dynamically
**	acquired storage.
*/
void
bufshutdown(void)
	{
	struct Buffer *b;
	struct Pool *p;
	int i;

	P(sem_buffers_libt);

	for (i=0; i<=MAXINDEX; i++)
		{
		p=&pools[i];

		for (b=(struct Buffer *)lget(&p->P_list); b!=NULL;
				b=(struct Buffer *)lget(&p->P_list))
			(void)nfree((void *)b);

		/* N.B Cannot release what the application owns */
		p->P_count=p->P_inuse;
		}

	V(sem_buffers_libt);
	}
