/*
**	C A C H E
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Generic Cache Implementation
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu Jan 20 09:59:36 MST 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	This module implements a generic cache datatype. It doesn't care
**	what it caches, only the cache management is implemented.
**	Everything is taken care of by user-supplied functions. This
**	means application-specific caches can be built on top of this.
**
**	The default replacement algorithm is Segmented Least Recently
**	Used (SLRU) [1]. This implements a two-level cache, where items
**	that are referenced for the first time compete for cache space
**	with items that have referenced only N times; once the N+1-th
**	reference occurs, the item competes with like-items in a second
**	level of the cache. This lessens cache turbulence across the
**	entire set of cached items. By configuring a cache with a
**	SLRU threshold of 0, a single level cache can be implemented.
**
**	1.	R. Karedla et al., "Caching Strategies to Improve Disk
**		System Performance", IEEE COMPUTER, Vol. 27, No. 3,
**		March 1994
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)cache.c	4.8 96/03/08 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libtstrings.h"
#include "errors.h"
#include "nmalloc.h"
#include "lists.h"
#include "queues.h"
#include "origin.h"
#include "cache.h"

/*
**	M A C R O S
*/

/*
**	DEBUGGING
*/

#ifdef CACHE_TRACE
#define	TRACE(c,f,l)	((c)->ca_cf->cf_trace==NULL?0:\
				fprintf((c)->ca_cf->cf_trace,\
					"cache 0x%x: %s (0x%x)\n",\
					(c),(f),(l)))
#else /* CACHE_TRACE */
#define TRACE(c,f,x)	(0)
#endif /* CACHE_TRACE */

#define	ERROR(c,f,l)	((c)->ca_cf->cf_trace==NULL?0:\
				fprintf((c)->ca_cf->cf_trace,\
					"cache 0x%x: %s (0x%x)\n",\
					(c),(f),(l)))

/*
**	WRAPPERS FOR USER SUPPLIED FUNCTIONS
*/

#define CAS(c)			((c)->ca_cf->cf_state)
#define CAC(c)			((c)->ca_cache)
#define CLP(l)			((l)->cl_payload)
#define CFF(c,f)		((c)->ca_cf->f)

#define CAX(f,a,n)		(((f)!=NULL)?((*(f))a):(n))

#define CACOMPARE(c,a,b)	CAX(CFF(c,cf_compare),(CLP(a),CLP(b),CAS(c)),0)
#define CASTRATEGY(c,a,b)	CAX(CFF(c,cf_strategy),((a),(b),(c)),1)
#define CAFREE(c,l)		(CLP(l)!=(void *)0?(CAX(CFF((c),cf_free),(CLP(l),CAS(c)),0)):0)
#define CACOUNT(c,l)		CAX(CFF(c,cf_count),(CLP(l),CAS(c)),1)
#define CAWRITE(c,l)		CAX(CFF(c,cf_write),(CLP(l),CAS(c)),0)
#define CAREAD(c,l)		CAX(CFF(c,cf_read),(CLP(l),CAS(c)),(void *)0)
#define CADUMP(c,l,fp)		(CLP(l)!=(void *)0?(CAX(CFF((c),cf_dump),(CLP(l),(fp),CAS(c)),0)):0)

/*
**	COMMON MACROS
*/

#define	SETFLAG(c,l,b)		((l)->cl_flags|=(b))
#define UNSETFLAG(c,l,b)	((l)->cl_flags&=~(b))

#define BUSY(c,l)		SETFLAG((c),(l),CL_BUSY)
#define UNBUSY(c,l)		UNSETFLAG((c),(l),CL_BUSY)

#define READONLY(c,l)		SETFLAG((c),(l),CL_READONLY)
#define UNREADONLY(c,l)		UNSETFLAG((c),(l),CL_READONLY)

#define DIRTY(c,l)		SETFLAG((c),(l),CL_DIRTY)
#define UNDIRTY(c,l)		UNSETFLAG((c),(l),CL_DIRTY)

#define CACHED(c,l)		SETFLAG((c),(l),CL_CACHED)
#define UNCACHED(c,l)		UNSETFLAG((c),(l),CL_CACHED)

#define PROTECTED(c,l)		SETFLAG((c),(l),CL_PROTECTED)
#define UNPROTECTED(c,l)	UNSETFLAG((c),(l),CL_PROTECTED)

#define FLUSH(c,l)		(CacheLineIsDirty((c),(l))?\
					(UNDIRTY((c),(l)),CAWRITE((c),(l))):0)

#define SLRU(c,l)		(CacheLineIsProtected((c),(l))?CA_PROT:CA_PROB)

#define INCREMENT(c,f)		(++(c)->ca_stats.f)
#define DECREMENT(c,f)		(--(c)->ca_stats.f)

/*
**	C A C H E   I M P L E M E N T A T I O N
*/

#define INITIALIZE(c)	(sklinit(&CAC(c))!=NULL)
#define LOCATE(c,l)	((CACHELINE *)sklget(&CAC(c),(l),comparef,(void *)(c)))
#define INSERT(c,l)	((CACHELINE *)sklput(&CAC(c),(l),comparef,(void *)(c)))
#define DELETE(c,l)	((CACHELINE *)skldel(&CAC(c),(l),comparef,(void *)(c)))
#define APPLY(c,f,v)	(sklinorder(&CAC(c),(f),(void *)(v)))

/*
**	Enqueue, Dequeue: handle the housekeeping of the LRU
**	queues.
*/
static void
dequeue(CACHE *c, CACHELINE *l)
	{
	int i;

	if (CacheLineIsQueued(c,l))
		{
		i=SLRU(c,l);
		(void)qdelete(&c->ca_queue[i],&l->cl_next);
		UNSETFLAG(c,l,CL_QUEUED);
		c->ca_count[i]-=CACOUNT(c,l);
		}
	}

static void
enqueue(CACHE *c, CACHELINE *l)
	{
	int i;

	dequeue(c,l);
	i=SLRU(c,l);
	(void)qinsert(&c->ca_queue[i],qtail(&c->ca_queue[i]),&l->cl_next);
	SETFLAG(c,l,CL_QUEUED);
	c->ca_count[i]+=CACOUNT(c,l);
	if (c->ca_count[i]>c->ca_stats.cs_maximum[i])
		c->ca_stats.cs_maximum[i]=c->ca_count[i];
	}

/*
**	promote: promote a cacheline from the probationary segment
**	to the protected segment. N.B.: for applications where CACOUNT
**	may return something other than the value one (1), it is
**	possible for the count of one of the segments to exceed its
**	limit, although the total count across both segments will
**	not exceed the total limit. This is because of an optimization
**	that vastly simplifies this code.
*/
static void
promote(CACHE *c, CACHELINE *l)
	{
	CACHELINE *o;
	QUEUENODE *qn;
	int nl, no;

	nl=CACOUNT(c,l);
	(void)qdelete(&c->ca_queue[CA_PROB],&l->cl_next);
	c->ca_count[CA_PROB]-=nl;

	if (c->ca_count[CA_PROT]>=c->ca_cf->cf_limit[CA_PROT])
		if ((qn=qhead(&c->ca_queue[CA_PROT]))!=NULL)
			{
			o=ORIGIN(qn,CACHELINE,cl_next);
			no=CACOUNT(c,o);

			(void)qdelete(&c->ca_queue[CA_PROT],qn);
			c->ca_count[CA_PROT]-=no;

			(void)qinsert(&c->ca_queue[CA_PROB],
				qtail(&c->ca_queue[CA_PROB]),qn);
			c->ca_count[CA_PROB]+=no;
			if (c->ca_count[CA_PROB]>c->ca_stats.cs_maximum[CA_PROB])
				c->ca_stats.cs_maximum[CA_PROB]=c->ca_count[CA_PROB];

			UNPROTECTED(c,o);
			}

	(void)qinsert(&c->ca_queue[CA_PROT],qtail(&c->ca_queue[CA_PROT]),&l->cl_next);
	c->ca_count[CA_PROT]+=nl;
	if (c->ca_count[CA_PROT]>c->ca_stats.cs_maximum[CA_PROT])
		c->ca_stats.cs_maximum[CA_PROT]=c->ca_count[CA_PROT];

	PROTECTED(c,l);
	}

/*
**	comparef: Compare two cachelines. Applied to each item in the
**	cache.
*/
static int
comparef(void *va, void *vb, void *vc)
	{
	CACHE *c;
	CACHELINE *a;
	CACHELINE *b;

	a=(CACHELINE *)va;
	b=(CACHELINE *)vb;
	c=(CACHE *)vc;
	return(CACOMPARE(c,a,b));
	}

/*
**	I N T E R N A L   S T A T I C   V A R I A B L E S
*/

static LIST pool={NULL};	/* thread safe: sem_cache_libt */
static int allocated=0;		/* thread safe: sem_cache_libt */
static int active=0;		/* thread safe: sem_cache_libt */

/*
**	M E M O R Y   M A N A G E M E N T   F U N C T I O N S
*/

/*
**	freenode: Free the storage associated with a cacheline and
**	its associated user data.
*/
static int
freenode(CACHE *c, CACHELINE *l)
	{
	CACHECONFIG *cf;
	int rc;

	if (l==NULL)
		return(1);

	rc=CAFREE(c,l);
	UNCACHED(c,l);
	P(sem_cache_libt);
	(void)lput(&pool,(LISTNODE *)&l->cl_next);
	active--;
	DECREMENT(c,cs_active);
	V(sem_cache_libt);

	return(rc);
	}

/*
**	allocnode: Allocate storage for a cacheline and associate it
**	with caller supplied user data.
*/
static CACHELINE *
allocnode(CACHE *c, void *p)
	{
	CACHELINE *l;

	P(sem_cache_libt);
	l=(CACHELINE *)lget(&pool);
	V(sem_cache_libt);

	if (l==NULL)
		if ((l=(CACHELINE *)nmalloc(sizeof(CACHELINE)))==NULL)
			return(NULL);
		else
			{
			allocated++;
			INCREMENT(c,cs_allocated);
			}

	bzero((char *)l,sizeof(CACHELINE));
	l->cl_payload=p;

	active++;
	INCREMENT(c,cs_active);

	return(l);
	}

/*
**	U S E R   I N T E R F A C E   I M P L E M E N T A T I O N
*/

/*
**	invalidate: mark an entry as invalid, i.e. it may be in
**	the cache, but a more current copy exists elsewhere, and
**	hence the cache copy can be ditched.
**
**	N.B.	Note that this immediately frees the cached
**		structure (via freenode). Another approach
**		might be to simply mark the entry as invalid,
**		and wait to free it until the space is needed.
**		However, lazy freeing is a problem for an application
**		who is invalidating a cache entry because the
**		application wants the storage immediately freed.
**		Doing it this way means we don't have to add
**		a second mechanism for immediate (vs. lazy)
**		freeing.
*/
static int
invalidate(CACHE *c, CACHELINE *l)
	{
	CACHELINE *n;

	TRACE(c,"invalidating",l);

	if (!CacheLineIsCached(c,l))
		{
		TRACE(c,"is cached failed",l);
		return(-1);
		}

	if (CacheLineIsBusy(c,l))
		{
		TRACE(c,"is not busy failed",l);
		return(-2);
		}

	if (CacheLineIsDirty(c,l))
		{
		TRACE(c,"is not dirty failed",l);
		return(-3);
		}

	if ((n=DELETE(c,l))!=l)
		{
		ERROR(c,"delete failed",n);
		return(-4);
		}

	UNCACHED(c,l);
	dequeue(c,l);

	if (freenode(c,l)<0)
		{
		ERROR(c,"freenode failed",l);
		return(-5);
		}

	TRACE(c,"invalidated",l);

	return(0);
	}

/*
**	clearf: a wrapper around invalidate() that can be APPLYed
**	to an entire cache, invalidating all entries in the cache.
*/
static int
clearf(void *vl, void *vc)
	{
	CACHE *c;
	CACHELINE *l;
	int rc;

	l=(CACHELINE *)vl;
	c=(CACHE *)vc;

	TRACE(c,"clearing",l);

	if (CacheLineIsBusy(c,l))
		{
		TRACE(c,"is not busy failed",l);
		return(-1);
		}
	if (FLUSH(c,l)<0)
		{
		ERROR(c,"flush failed",l);
		return(-2);
		}
	if (invalidate(c,l)<0)
		{
		TRACE(c,"invalidate failed",l);
		return(-3);
		}

	TRACE(c,"cleared",l);

	return(0);
	}

/*
**	accommodated: return TRUE if there is enough space in
**	cache "c" to accomodate a cacheline "l" of size "need".
**	Note that cache lines are dropped only from the
**	probationary segment.
*/
static int
accommodated(CACHE *c, CACHELINE *l)
	{
	CACHELINE *o, *oo;
	int n;

	TRACE(c,"accommodate",l);

	n=CACOUNT(c,l);

	o=(CACHELINE *)qhead(&c->ca_queue[CA_PROB]);
	while ((o!=NULL)&&(((c)->ca_count[CA_PROB]+n)>(c)->ca_cf->cf_limit[CA_PROB]))

		{
		oo=(CACHELINE *)qnext((QUEUENODE *)o);
		if ((!CacheLineIsBusy(c,o))&&(o!=l)&&CASTRATEGY(c,o,l))
			(void)clearf((void *)o,(void *)c);
		o=oo;
		}

	if (((c)->ca_count[CA_PROB]+n)>(c)->ca_cf->cf_limit[CA_PROB])
		{
		TRACE(c,"no accommodation",l);
		return(0);
		}

	TRACE(c,"accommodated",l);

	return(1);
	}

/*
**	cacheclear: clear (invalidate) all entries in a cache.
*/
int
cacheclear(CACHE *c)
	{
	int rc;

	INCREMENT(c,cs_clear);

	TRACE(c,"cacheclear",c);

	if ((rc=APPLY(c,clearf,c))<0)
		ERROR(c,"apply clearf failed",c);

	TRACE(c,"cacheclear complete",c);

	return(rc);
	}

/*
**	cacheinit: Initialize a cache structure.
*/
int
cacheinit(CACHE *c, CACHECONFIG *cf)
	{
	int i;

	bzero((char *)c,sizeof(CACHE));
	c->ca_cf=cf;

	INCREMENT(c,cs_init);

	TRACE(c,"cacheinit",cf);

	if (!INITIALIZE(c))
		{
		ERROR(c,"initialize failed",cf);
		return(-1);
		}

	for (i=0; i<CA_SEGS; i++)
		(void)qinit(&c->ca_queue[i]);

	TRACE(c,"cacheinit complete",cf);

	return(0);
	}

/*
**	cacheshutdown: Release all internally held memory.
*/
void
cacheshutdown(void)
	{
	LISTNODE *n;

	P(sem_cache_libt);
	while ((n=lget(&pool))!=NULL)
		{
		(void)nfree((char *)n);
		allocated--;
		}
	V(sem_cache_libt);
	}

/*
**	cacheflush: Flush the dirty items from the cache.
*/
static int
flushf(void *vl, void *vc)
	{
	CACHE *c;
	CACHELINE *l;

	l=(CACHELINE *)vl;
	c=(CACHE *)vc;
	TRACE(c,"flushing",l);

	if (FLUSH(c,l)<0)
		{
		ERROR(c,"flush failed",l);
		return(-1);
		}

	TRACE(c,"flushed",l);

	return(0);
	}

int
cacheflush(CACHE *c)
	{
	int rc;

	INCREMENT(c,cs_flush);

	TRACE(c,"cacheflush",c);

	if ((rc=APPLY(c,flushf,c))<0)
		ERROR(c,"apply flushf failed",c);

	TRACE(c,"cacheflush complete",c);

	return(rc);
	}

/*
**	cachelinedump: Dump debugging info about a cacheline.
*/
int
cachelinedump(CACHE *c, CACHELINE *l, FILE *fp)
	{
	INCREMENT(c,cs_linedump);

	if (c==NULL)
		{
		fprintf(fp,"cacheline NULL\n");
		return(-1);
		}

	TRACE(c,"cachelinedump",l);

	if (l==NULL)
		{
		fprintf(fp,"cacheline 0x%x\n",l);
		return(-2);
		}

	fprintf(fp,"cacheline 0x%x: next    0x%x\n",l,l->cl_next.q_next);
	fprintf(fp,"cacheline 0x%x: prev    0x%x\n",l,l->cl_next.q_prev);
	fprintf(fp,"cacheline 0x%x: flags   0x%x ( %s%s%s%s%s%s)\n",l,
		l->cl_flags,
		CacheLineIsCached(c,l)?"cached ":"",
		CacheLineIsDirty(c,l)?"dirty ":"",
		CacheLineIsBusy(c,l)?"busy ":"",
		CacheLineIsReadonly(c,l)?"readonly ":"",
		CacheLineIsQueued(c,l)?"queued ":"",
		CacheLineIsProtected(c,l)?"protected ":"");
	fprintf(fp,"cacheline 0x%x: payload 0x%x\n",l,l->cl_payload);

	return(0);
	}

/*
**	cachedump: Dump debugging info about a cache.
*/
struct dumpstuff
	{
	CACHE *	ds_cache;
	FILE *	ds_fp;
	};

static int
dumpf(void *vl, void *vd)
	{
	CACHE *c;
	CACHECONFIG *cf;
	CACHELINE *l;
	struct dumpstuff *d;
	int rc;

	l=(CACHELINE *)vl;
	d=(struct dumpstuff *)vd;
	c=d->ds_cache;
	cf=c->ca_cf;

	(void)cachelinedump(c,l,d->ds_fp);

	if ((rc=CADUMP(c,l,d->ds_fp))<0)
		return(rc);

	return(0);
	}

int
cachedump(CACHE *c, FILE *fp)
	{
	CACHECONFIG *cf;
	CACHESTATS *s;
	struct dumpstuff ds;
	int rc, i;

	INCREMENT(c,cs_dump);

	cf=c->ca_cf;

	TRACE(c,"cachedump",cf);

	s=&c->ca_stats;

	ds.ds_cache=c;
	ds.ds_fp=(fp!=NULL)?fp:(cf->cf_trace!=NULL)?cf->cf_trace:stderr;

	fprintf(fp,"cache 0x%x: dump\n");
	fprintf(fp,"cache 0x%x: next    0x%x\n",c,c->ca_next);
	fprintf(fp,"cache 0x%x: config  0x%x\n",c,cf);
	fprintf(fp,"cache 0x%x: stat    0x%x\n",c,s);

	for (i=0; i<CA_SEGS; i++)
		{
		fprintf(fp,"cachesegment %d: count   %d\n",
			i,c->ca_count[i]);
		fprintf(fp,"cachesegment %d: qsize   %d\n",
			i,c->ca_queue[i].q_size);
		fprintf(fp,"cachesegment %d: qhead   0x%x\n",
			i,c->ca_queue[i].q_head);
		fprintf(fp,"cachesegment %d: qtail   0x%x\n",
			i,c->ca_queue[i].q_tail);
		}

	fprintf(fp,"cacheconfig 0x%x: state     0x%x\n",cf,cf->cf_state);
	fprintf(fp,"cacheconfig 0x%x: freef     0x%x\n",cf,cf->cf_free);
	fprintf(fp,"cacheconfig 0x%x: comparef  0x%x\n",cf,cf->cf_compare);
	fprintf(fp,"cacheconfig 0x%x: strategyf 0x%x\n",cf,cf->cf_strategy);
	fprintf(fp,"cacheconfig 0x%x: countf    0x%x\n",cf,cf->cf_count);
	fprintf(fp,"cacheconfig 0x%x: dumpf     0x%x\n",cf,cf->cf_dump);
	fprintf(fp,"cacheconfig 0x%x: writef    0x%x\n",cf,cf->cf_write);
	fprintf(fp,"cacheconfig 0x%x: readf     0x%x\n",cf,cf->cf_read);
	fprintf(fp,"cacheconfig 0x%x: trace     %d\n",cf,
		cf->cf_trace==NULL?(-1):fileno(cf->cf_trace));
	for (i=0; i<CA_SEGS; i++)
		fprintf(fp,"cacheconfig 0x%x: limit %d   %d\n",
			cf,i,cf->cf_limit[i]);

	fprintf(fp,"cachestats 0x%x: init       %d\n",s,s->cs_init);
	fprintf(fp,"cachestats 0x%x: dump       %d\n",s,s->cs_dump);
	fprintf(fp,"cachestats 0x%x: read       %d\n",s,s->cs_read);
	fprintf(fp,"cachestats 0x%x: find       %d\n",s,s->cs_find);
	fprintf(fp,"cachestats 0x%x: write      %d\n",s,s->cs_write);
	fprintf(fp,"cachestats 0x%x: readby     %d\n",s,s->cs_readby);
	fprintf(fp,"cachestats 0x%x: writeby    %d\n",s,s->cs_writeby);
	fprintf(fp,"cachestats 0x%x: writethru  %d\n",s,s->cs_writethru);
	fprintf(fp,"cachestats 0x%x: invalidate %d\n",s,s->cs_invalidate);
	fprintf(fp,"cachestats 0x%x: dirty      %d\n",s,s->cs_dirty);
	fprintf(fp,"cachestats 0x%x: busy       %d\n",s,s->cs_busy);
	fprintf(fp,"cachestats 0x%x: complete   %d\n",s,s->cs_complete);
	fprintf(fp,"cachestats 0x%x: flush      %d\n",s,s->cs_flush);
	fprintf(fp,"cachestats 0x%x: clear      %d\n",s,s->cs_clear);
	fprintf(fp,"cachestats 0x%x: allocated  %d\n",s,s->cs_allocated);
	fprintf(fp,"cachestats 0x%x: active     %d\n",s,s->cs_active);
	fprintf(fp,"cachestats 0x%x: full       %d\n",s,s->cs_full);
	fprintf(fp,"cachestats 0x%x: locked     %d\n",s,s->cs_locked);
	fprintf(fp,"cachestats 0x%x: hit        %d\n",s,s->cs_hit);
	for (i=0; i<CA_SEGS; i++)
		fprintf(fp,"cachestats 0x%x: maximum %d  %d\n",
			s,i,s->cs_maximum[i]);

	if ((rc=APPLY(c,dumpf,&ds))<0)
		ERROR(c,"apply dumpf failed",cf);

	TRACE(c,"cachedump complete",cf);

	return(rc);
	}

/*
**	cacheread: Read and return a cacheline from the cache, where
**	that cacheline has a payload that compares true with the caller
**	supplied payload.
*/
CACHELINE *
cacheread(CACHE *c, CACHEPAYLOAD *p)
	{
	CACHELINE l, *n, *nn;
	CACHEPAYLOAD *pp;

	INCREMENT(c,cs_read);

	TRACE(c,"cacheread",p);

	n=NULL;

	CLP(&l)=p;
	if ((n=LOCATE(c,&l))!=NULL)
		{
		INCREMENT(c,cs_hit);
		TRACE(c,"have",p);
		if (!CacheLineIsBusy(c,n))
			{
			BUSY(c,n);
			INCREMENT(c,cs_locked);
			}
		if (CacheLineIsProtected(c,n))
			enqueue(c,n);
		else
			promote(c,n);
		}
	else if ((pp=CAREAD(c,&l))==NULL)
		ERROR(c,"caread failed",p);
	else if ((n=allocnode(c,pp))==NULL)
		{
		ERROR(c,"allocnode failed",p);
		CLP(&l)=pp;
		(void)CAFREE(c,&l);
		}
	else if (!accommodated(c,n))
		{
		INCREMENT(c,cs_full);
		TRACE(c,"accommodation failed",n);
		CLP(n)=NULL;
		if (freenode(c,n)<0)
			ERROR(c,"freenode failed",n);
		n=NULL;
		}
	else if ((nn=INSERT(c,n))==NULL)
		ERROR(c,"insert failed",n);
	else
		{
		TRACE(c,"cached",n);
		if (nn!=n)
			if (freenode(c,nn)<0)
				ERROR(c,"freenode failed",nn);
			
		CACHED(c,n);
		BUSY(c,n);
		INCREMENT(c,cs_locked);
		enqueue(c,n);
		}

	TRACE(c,"cacheread complete",n);

	return(n);
	}

/*
**	cachefind: Find and return a cacheline from the cache, where
**	that cacheline has a payload that compares true with the caller
**	supplied payload. If the payload is not in the cache, NULL is
**	returned. This allows the caller to find out if a requested
**	payload is already in the cache. This may be necessary for
**	cache-coherency protocols.
*/
CACHELINE *
cachefind(CACHE *c, CACHEPAYLOAD *p)
	{
	CACHELINE l, *n;

	INCREMENT(c,cs_find);

	TRACE(c,"cachefind",p);

	n=NULL;

	CLP(&l)=p;
	if ((n=LOCATE(c,&l))!=NULL)
		{
		TRACE(c,"have",p);
		if (!CacheLineIsBusy(c,n))
			{
			BUSY(c,n);
			INCREMENT(c,cs_locked);
			}
		if (CacheLineIsProtected(c,n))
			enqueue(c,n);
		else
			promote(c,n);
		}

	TRACE(c,"cachefind complete",n);

	return(n);
	}

/*
**	cachewrite: Write a payload into the cache.
**
**	cachewritethru: Write a payload into the cache and immediately
**	flush it to the backing store.
**
**	genwrite: generic routine that implements both.
*/
static CACHELINE *
genwrite(CACHE *c, CACHEPAYLOAD *p, int thru)
	{
	CACHELINE *n, *nn;

	if ((n=allocnode(c,p))==NULL)
		{
		ERROR(c,"allocnode failed",p);
		return(NULL);
		}

	if ((nn=LOCATE(c,n))!=NULL)
		{
		if (CacheLineIsBusy(c,nn))
			{
			TRACE(c,"is not busy failed",nn);
			CLP(n)=NULL;
			if (freenode(c,n)<0)
				ERROR(c,"freenode failed",n);
			return(NULL);
			}

		if (CacheLineIsReadonly(c,nn))
			{
			TRACE(c,"is not readonly failed",nn);
			CLP(n)=NULL;
			if (freenode(c,n)<0)
				ERROR(c,"freenode failed",n);
			return(NULL);
			}

		if (CacheLineIsProtected(c,nn))
			PROTECTED(c,n);

		if (FLUSH(c,nn)<0)
			{
			ERROR(c,"flush failed",nn);
			CLP(n)=NULL;
			if (freenode(c,n)<0)
				ERROR(c,"freenode failed",n);
			return(NULL);
			}

		if (invalidate(c,nn)<0)
			{
			TRACE(c,"invalidate failed",nn);
			CLP(n)=NULL;
			if (freenode(c,n)<0)
				ERROR(c,"freenode failed",n);
			return(NULL);
			}
		}

	if (!accommodated(c,n))
		{
		INCREMENT(c,cs_full);
		TRACE(c,"accommodation failed",n);
		CLP(n)=NULL;
		if (freenode(c,n)<0)
			ERROR(c,"freenode failed",n);
		return(NULL);
		}

	if ((nn=INSERT(c,n))==NULL)
		{
		ERROR(c,"insert failed",n);
		CLP(n)=NULL;
		if (freenode(c,n)<0)
			ERROR(c,"freenode failed",n);
		return(NULL);
		}

	TRACE(c,"cached",n);

	if (n!=nn)
		if (freenode(c,nn)<0)
			ERROR(c,"freenode failed",nn);

	CACHED(c,n);
	DIRTY(c,n);
	BUSY(c,n);
	INCREMENT(c,cs_locked);
	enqueue(c,n);

	if (thru)
		if (FLUSH(c,n)<0)
			{
			ERROR(c,"flush failed",n);
			return(NULL);
			}

	return(n);
	}

CACHELINE *
cachewrite(CACHE *c, CACHEPAYLOAD *p)
	{
	CACHELINE *l;

	INCREMENT(c,cs_write);
	TRACE(c,"cachewrite",p);
	l=genwrite(c,p,0);
	TRACE(c,"cachewrite complete",l);
	return(l);
	}

CACHELINE *
cachewritethru(CACHE *c, CACHEPAYLOAD *p)
	{
	CACHELINE *l;

	INCREMENT(c,cs_writethru);
	TRACE(c,"cachewritethru",p);
	l=genwrite(c,p,1);
	TRACE(c,"cachewritethru complete",l);
	return(l);
	}

/*
**	cachereadby: Return a cacheline directly from the backing
**	store, without going through the cache, where the payload
**	matches the caller supplied payload.
*/
CACHELINE *
cachereadby(CACHE *c, CACHEPAYLOAD *p)
	{
	CACHELINE l, *n;
	CACHEPAYLOAD *pp;

	INCREMENT(c,cs_readby);

	TRACE(c,"cachereadby",p);

	n=NULL;
	CLP(&l)=p;
	if ((pp=CAREAD(c,&l))==NULL)
		ERROR(c,"caread failed",p);
	else if ((n=allocnode(c,pp))==NULL)
		{
		ERROR(c,"allocnode failed",p);
		CLP(&l)=pp;
		if (CAFREE(c,&l)<0)
			ERROR(c,"cafree failed",l);
		}
	else
		BUSY(c,n);

	TRACE(c,"cachereadby complete",pp);

	return(n);
	}

/*
**	cachewriteby: Write a payload directly to the backing
**	store, without going through the cache, where the payload
**	matches the caller supplied payload.
*/
CACHELINE *
cachewriteby(CACHE *c, CACHEPAYLOAD *p)
	{
	CACHELINE l, *n;
	int rc;

	INCREMENT(c,cs_writeby);

	TRACE(c,"cachewriteby",p);

	n=NULL;
	CLP(&l)=p;
	if ((rc=CAWRITE(c,&l))<0)
		ERROR(c,"cawrite failed",p);
	else if ((n=allocnode(c,p))==NULL)
		{
		ERROR(c,"allocnode failed",p);
		CLP(&l)=p;
		if (CAFREE(c,&l)<0)
			ERROR(c,"cafree failed",l);
		}
	else
		BUSY(c,n);

	TRACE(c,"cachewriteby complete",p);

	return(n);
	}

/*
**	cacheinvalidate: Invalidates an item in the cache as identified
**	by its cacheline.
*/
int
cacheinvalidate(CACHE *c, CACHELINE *l)
	{
	INCREMENT(c,cs_invalidate);

	TRACE(c,"cacheinvalidate",l);

	if (!CacheLineIsCached(c,l))
		{
		if (freenode(c,l)<0)
			{
			ERROR(c,"freenode failed",l);
			return(-1);
			}
		}
	else
		{
		UNBUSY(c,l);
		if (invalidate(c,l)<0)
			{
			TRACE(c,"invalidate failed",l);
			return(-2);
			}
		}

	TRACE(c,"cacheinvalidate complete",l);

	return(0);
	}

/*
**	cachedirty: Mark a cacheline as being dirty.
**
**	cacheclean: Mark a cacheline as being clean.
*/
int
cachedirty(CACHE *c, CACHELINE *l)
	{
	INCREMENT(c,cs_dirty);

	TRACE(c,"cachedirty",l);

	if (!CacheLineIsCached(c,l))
		{
		TRACE(c,"is cached failed",l);
		return(-1);
		}

	if (CacheLineIsReadonly(c,l))
		{
		TRACE(c,"is not readonly failed",l);
		return(-2);
		}

	DIRTY(c,l);
	enqueue(c,l);

	TRACE(c,"cachedirty complete",l);

	return(0);
	}

int
cacheclean(CACHE *c, CACHELINE *l)
	{
	INCREMENT(c,cs_clean);

	TRACE(c,"cacheclean",l);

	if (!CacheLineIsCached(c,l))
		{
		TRACE(c,"is cached failed",l);
		return(-1);
		}

	UNDIRTY(c,l);
	enqueue(c,l);

	TRACE(c,"cacheclean complete",l);

	return(0);
	}

/*
**	cachebusy: Mark a cacheline as being busy, preventing it from
**	being removed from the cache. (Reading a cacheline from the cache
**	does the same thing.)
*/
int
cachebusy(CACHE *c, CACHELINE *l)
	{
	INCREMENT(c,cs_busy);

	TRACE(c,"cachebusy",l);

	if (!CacheLineIsCached(c,l))
		{
		TRACE(c,"is cached failed",l);
		return(-1);
		}

	BUSY(c,l);
	INCREMENT(c,cs_locked);
	if (CacheLineIsProtected(c,l))
		enqueue(c,l);
	else
		promote(c,l);

	TRACE(c,"cachebusy complete",l);

	return(0);
	}

/*
**	cachereadonly: Mark a cacheline as being readonly, preventing
**	it from being dirtied.
*/
int
cachereadonly(CACHE *c, CACHELINE *l)
	{
	INCREMENT(c,cs_readonly);

	TRACE(c,"cachereadonly",l);

	if (!CacheLineIsCached(c,l))
		{
		TRACE(c,"is cached failed",l);
		return(-1);
		}

	READONLY(c,l);
	enqueue(c,l);

	TRACE(c,"cachereadonly complete",l);

	return(0);
	}

/*
**	cachereadwrite: Mark a cacheline as being writable, allowing
**	it to be dirtied.
*/
int
cachereadwrite(CACHE *c, CACHELINE *l)
	{
	INCREMENT(c,cs_readwrite);

	TRACE(c,"cachereadwrite",l);

	if (!CacheLineIsCached(c,l))
		{
		TRACE(c,"is cached failed",l);
		return(-1);
		}

	UNREADONLY(c,l);
	enqueue(c,l);

	TRACE(c,"cachereadwrite complete",l);

	return(0);
	}

/*
**	cachecomplete: Mark a cacheline as being unbusy, so that
**	it can be removed from the cache if necessary.
*/
int
cachecomplete(CACHE *c, CACHELINE *l)
	{
	INCREMENT(c,cs_complete);

	TRACE(c,"cachecomplete",l);

	if (CacheLineIsBusy(c,l))
		{
		UNBUSY(c,l);
		if (CacheLineIsCached(c,l))
			DECREMENT(c,cs_locked);
		else if (freenode(c,l)<0)
			{
			ERROR(c,"freenode failed",l);
			return(-1);
			}
		}

	TRACE(c,"cachecomplete complete",l);

	return(0);
	}

/*
**	cacheapply: Apply a caller supplied function to each cacheline
**	in the cache. Rife with potential for abuse.
*/
struct applystuff
	{
	CACHE *	as_cache;
	int	(*as_function)();
	void	*as_vp;
	};

static int
applyf(void *vl, void *va)
	{
	CACHE *c;
	CACHELINE *l;
	struct applystuff *a;
	int rc;

	l=(CACHELINE *)vl;
	a=(struct applystuff *)va;
	c=(CACHE *)a->as_cache;

	TRACE(c,"applying",l);

	rc=(*a->as_function)(l,a->as_vp,CAS(a->as_cache));

	TRACE(c,"applied",l);

	return(rc);
	}

int
cacheapply(CACHE *c, int (*f)(), void *vp)
	{
	struct applystuff a;

	a.as_cache=c;
	a.as_function=f;
	a.as_vp=vp;
	return(APPLY(c,applyf,(void *)&a));
	}
