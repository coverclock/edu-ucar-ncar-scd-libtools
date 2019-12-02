/*
**	C A C H E
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)cache.h	4.3 94/07/13 jsloan@ncar.ucar.edu
*/

#ifndef H_CACHE
#define H_CACHE

#include "skiplists.h"
#include "queues.h"
#include "libtproto.h"

#define CA_PROB		(0)
#define CA_PROT		(1)
#define	CA_SEGS		(2)

#define CacheLineIsBusy(c,l)		(((l)->cl_flags&CL_BUSY)!=0)
#define CacheLineIsDirty(c,l)		(((l)->cl_flags&CL_DIRTY)!=0)
#define CacheLineIsCached(c,l)		(((l)->cl_flags&CL_CACHED)!=0)
#define CacheLineIsReadonly(c,l)	(((l)->cl_flags&CL_READONLY)!=0)
#define CacheLineIsQueued(c,l)		(((l)->cl_flags&CL_QUEUED)!=0)
#define CacheLineIsProtected(c,l)	(((l)->cl_flags&CL_PROTECTED)!=0)

typedef struct cache CACHE;
typedef struct cacheline CACHELINE;
typedef struct cacheconfig CACHECONFIG;
typedef struct cachestats CACHESTATS;
typedef void CACHESTATE;
typedef void CACHEPAYLOAD;

struct cacheline
	{
	QUEUENODE	cl_next;
	unsigned short	cl_unused1;
	unsigned short	cl_flags;
#define			CL_DIRTY	(0x0001)
#define			CL_BUSY		(0x0002)
#define			CL_QUEUED	(0x0004)
#define			CL_CACHED	(0x0008)
#define			CL_READONLY	(0x0010)
#define			CL_PROTECTED	(0x0020)
	void *		cl_payload;
	};

struct cacheconfig
	{
	CACHESTATE *	cf_state;
	int		(*cf_free) LIBTPROTO((CACHEPAYLOAD *p, CACHESTATE *s));
	int		(*cf_compare) LIBTPROTO((CACHEPAYLOAD *a, CACHEPAYLOAD *b, CACHESTATE *s));
	int		(*cf_strategy) LIBTPROTO((CACHELINE *a, CACHELINE *b, CACHE *c));
	int		(*cf_count) LIBTPROTO((CACHEPAYLOAD *p, CACHESTATE *s));
	int		(*cf_dump) LIBTPROTO((CACHEPAYLOAD *p, FILE *fp, CACHESTATE *s));
	int		(*cf_write) LIBTPROTO((CACHEPAYLOAD *p, CACHESTATE *s));
	CACHEPAYLOAD *	(*cf_read) LIBTPROTO((CACHEPAYLOAD *p, CACHESTATE *s));
	FILE *		cf_trace;
	int		cf_limit[CA_SEGS];
	};

struct cachestats
	{
	/*
	**	Function Call Counts
	*/
	int		cs_init;
	int		cs_dump;
	int		cs_linedump;
	int		cs_read;
	int		cs_find;
	int		cs_write;
	int		cs_readby;
	int		cs_writeby;
	int		cs_writethru;
	int		cs_invalidate;
	int		cs_dirty;
	int		cs_clean;
	int		cs_busy;
	int		cs_readonly;
	int		cs_readwrite;
	int		cs_complete;
	int		cs_flush;
	int		cs_clear;
	/*
	**	Event Counts
	*/
	int		cs_allocated;
	int		cs_active;
	int		cs_full;
	int		cs_locked;
	int		cs_hit;
	int		cs_maximum[CA_SEGS];
	};

struct cache
	{
	CACHE *		ca_next;
	CACHECONFIG *	ca_cf;
	CACHESTATS	ca_stats;
	int		ca_count[CA_SEGS];
	QUEUE		ca_queue[CA_SEGS];
	SKIPLIST	ca_cache;
	};

extern int		cacheinit	LIBTPROTO((CACHE *c, CACHECONFIG *cf));
extern void		cacheshutdown	LIBTPROTO((void));
extern int		cacheclear	LIBTPROTO((CACHE *c));
extern int		cacheflush	LIBTPROTO((CACHE *c));
extern int		cachedump	LIBTPROTO((CACHE *c, FILE *fp));
extern int		cachelinedump	LIBTPROTO((CACHE *c, CACHELINE *l, FILE *fp));
extern int		cachepool	LIBTPROTO((FILE *fp));
extern int		cacheinvalidate	LIBTPROTO((CACHE *c, CACHELINE *l));
extern int		cachedirty	LIBTPROTO((CACHE *c, CACHELINE *l));
extern int		cacheclean	LIBTPROTO((CACHE *c, CACHELINE *l));
extern int		cachebusy	LIBTPROTO((CACHE *c, CACHELINE *l));
extern int		cachereadonly	LIBTPROTO((CACHE *c, CACHELINE *l));
extern int		cachereadwrite	LIBTPROTO((CACHE *c, CACHELINE *l));
extern int		cachecomplete	LIBTPROTO((CACHE *c, CACHELINE *l));
extern int		cacheapply	LIBTPROTO((CACHE *c, int (*f) LIBTPROTO((CACHELINE *l, void *vp, CACHESTATE *s)), void *vp));
extern CACHELINE *	cachefind	LIBTPROTO((CACHE *c, CACHEPAYLOAD *p));
extern CACHELINE *	cacheread	LIBTPROTO((CACHE *c, CACHEPAYLOAD *p));
extern CACHELINE *	cachereadby	LIBTPROTO((CACHE *c, CACHEPAYLOAD *p));
extern CACHELINE *	cachewrite	LIBTPROTO((CACHE *c, CACHEPAYLOAD *p));
extern CACHELINE *	cachewriteby	LIBTPROTO((CACHE *c, CACHEPAYLOAD *p));
extern CACHELINE *	cachewritethru	LIBTPROTO((CACHE *c, CACHEPAYLOAD *p));

#endif /* !H_CACHE */
