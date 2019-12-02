/*
** @(#)egcache.c	4.4 95/09/14 jsloan@ncar.ucar.edu
**
** This program tests the generic cache manager by managing a cache
** containing integer keys and values. The only failures should be
** in the section FAILURES (just before CYCLES).
**
** Flags:
**
** -d		Debug: turn on cache tracing (very verbose)
** -v		Verbose: dump cache after each test section
** -m		Map: produce map of cache operations (verbose)
** -c c		Cache: cache size is c items
** -n n		Number: backing store contains n items
** -s s		Stress: run s stress test cycles
** -r r		Read: run r read cycles
*/

#include <stdio.h>
#include "libtvalues.h"
#include "libtstrings.h"
#include "nmalloc.h"
#include "origin.h"
#include "cache.h"
#ifdef _IBMR2
#ifdef _AIX
#include <signal.h>
#endif /* _AIX */
#endif /* _IBMR2 */


#define CM_C	(0)
#define CM_P	(1)
#define CM_B	(2)
#define CM_D	(3)
#define CM_RO	(4)
#define CM_R	(5)
#define CM_W	(6)
#define CM_F	(7)
#define CM_ER	(8)
#define CM_ERB	(9)
#define CM_EW	(10)
#define CM_EWB	(11)
#define CM_EWT	(12)
#define CM_EI	(13)
#define CM_ED	(14)
#define CM_EB	(15)
#define CM_EF	(16)
#define CM_ERO	(17)
#define CM_ERW	(18)
#define CM_LEN	(19)

static char *cachemaplabels[CM_LEN]=
	{
	"ISCACHED",
	"ISPROTCD",
	"ISBUSY",
	"ISDIRTY",
	"ISRDONLY",
	"FREAD",
	"FWRITE",
	"FFREE",
	"READ",
	"READBY",
	"WRITE",
	"WRITEBY",
	"WRITTHRU",
	"INVALIDT",
	"DIRTY",
	"BUSY",
	"FIND",
	"READONLY",
	"READWRIT"
	};

static char cachemapnames[CM_LEN]="cpbdosmfRrWwTIDBFON";

struct cachemapline
	{
	char	cm_line[CM_LEN];
	int	cm_cached;
	int	cm_store;
	int	cm_expect;
	};

struct cachemap
	{
	CACHE *			cm_cache;
	struct cachemapline *	cm_map;
	};

struct item
	{
	int	i_key;
	int	i_val;
	};

static char *Argvzero="unknown";
static int Debug=0;
static int Verbose=0;
static int Map=0;
static int Things=0;
static int Tries=0;
static int Attempts=0;
static int Probation=0;
static int Protected=0;

static int Event=(-1);
static struct item *store;

static CACHE cache;
static CACHECONFIG config;

static struct cachemap map;

static struct item *
ialloc()
	{
	struct item *ii;

	if ((ii=(struct item *)nmalloc(sizeof(struct item)))==NULL)
		{
		fprintf(stderr,"nmalloc(%d) failed\n",sizeof(struct item));
		exit(99);
		}
	return(ii);
	}

static int
funcmap(l,vm,s)
CACHELINE *l;
void *vm;
CACHESTATE *s;
	{
	struct cachemap *m;
	struct item *ii;
	char c;

	m=(struct cachemap *)vm;
	if ((ii=(struct item *)l->cl_payload)==NULL)
		return(0);
	if (!((0<=ii->i_key)&&(ii->i_key<Things)))
		return(0);
	if (CacheLineIsCached(m->cm_cache,l))
		{
		m->cm_map[ii->i_key].cm_line[CM_C]=cachemapnames[CM_C];
		m->cm_map[ii->i_key].cm_cached=ii->i_val;
		}
	if (CacheLineIsProtected(m->cm_cache,l))
		m->cm_map[ii->i_key].cm_line[CM_P]=cachemapnames[CM_P];
	if (CacheLineIsBusy(m->cm_cache,l))
		m->cm_map[ii->i_key].cm_line[CM_B]=cachemapnames[CM_B];
	if (CacheLineIsDirty(m->cm_cache,l))
		m->cm_map[ii->i_key].cm_line[CM_D]=cachemapnames[CM_D];
	if (CacheLineIsReadonly(m->cm_cache,l))
		m->cm_map[ii->i_key].cm_line[CM_RO]=cachemapnames[CM_RO];
	return(0);
	}

static void
allocmap()
	{
	int len, i;
	struct cachemapline *cml;

	len=sizeof(struct cachemapline)*Things;
	if ((cml=(struct cachemapline *)nmalloc(len))==NULL)
		exit(98);
	map.cm_cache=&cache;
	map.cm_map=cml;

	for (i=0; i<Things; i++)
		{
		Event++;
		map.cm_map[i].cm_expect=Event;
		map.cm_map[i].cm_store=Event;
		map.cm_map[i].cm_cached=(-1);
		}
	}

static void
initmap()
	{
	int i, j;

	for (i=0; i<Things; i++)
		for (j=0; j<CM_LEN; j++)
			map.cm_map[i].cm_line[j]='.';
	}

static void
mapper(fp)
FILE *fp;
	{
	int i, j;

	(void)cacheapply(&cache,funcmap,(void *)&map);

	if (!Map)
		return;

	fprintf(fp,"%8s ","LINE");
	for (i=0; i<Things; i++)
		fprintf(fp,"%1.1d",i%10);
	fputc('\n',fp);

	for (j=0; j<CM_LEN; j++)
		{
		fprintf(fp,"%8s ",cachemaplabels[j]);
		for (i=0; i<Things; i++)
			fputc(map.cm_map[i].cm_line[j],fp);
		fputc('\n',fp);
		}

	fprintf(fp,"%8s","COUNT");
	for (i=0; i<CA_SEGS; i++)
		fprintf(fp," %4.4d",cache.ca_count[i]);
	fputc('\n',fp);
	if (cache.ca_cf!=NULL)
		{
		fprintf(fp,"%8s","LIMIT");
		for (i=0; i<CA_SEGS; i++)
			fprintf(fp," %4.4d",cache.ca_cf->cf_limit[i]);
		fputc('\n',fp);
		}
	fprintf(fp,"%8s","MAXIMUM");
	for (i=0; i<CA_SEGS; i++)
		fprintf(fp," %4.4d",cache.ca_stats.cs_maximum[i]);
	fputc('\n',fp);

	fprintf(fp,"%8s","LINE");
	for (i=0; i<Things; i++)
		fprintf(fp," %4.4d",i);
	fputc('\n',fp);

	fprintf(fp,"%8s","CACHED");
	for (i=0; i<Things; i++)
		if (map.cm_map[i].cm_cached<0)
			fprintf(fp," %3.3d",map.cm_map[i].cm_cached);
		else
			fprintf(fp," %4.4d",map.cm_map[i].cm_cached);
	fputc('\n',fp);

	fprintf(fp,"%8s","STORED");
	for (i=0; i<Things; i++)
		if (map.cm_map[i].cm_store<0)
			fprintf(fp," %3.3d",map.cm_map[i].cm_store);
		else
			fprintf(fp," %4.4d",map.cm_map[i].cm_store);
	fputc('\n',fp);

	fprintf(fp,"%8s","EXPECTED");
	for (i=0; i<Things; i++)
		if (map.cm_map[i].cm_expect<0)
			fprintf(fp," %3.3d",map.cm_map[i].cm_expect);
		else
			fprintf(fp," %4.4d",map.cm_map[i].cm_expect);
	fputc('\n',fp);

	initmap();
	}

static int
funcfree(p,s)
CACHEPAYLOAD *p;
CACHESTATE *s;
	{
	struct item *ii;

	ii=(struct item *)p;
	fprintf(stderr,"FREEING %d[%d]\n",ii->i_key,ii->i_val);
	map.cm_map[ii->i_key].cm_line[CM_F]=cachemapnames[CM_F];
	(void)nfree((void *)ii);
	return(0);
	}

static int
funccompare(a,b,s)
CACHEPAYLOAD *a;
CACHEPAYLOAD *b;
CACHESTATE *s;
	{
	struct item *ia, *ib;

	ia=(struct item *)a;
	ib=(struct item *)b;
	return((ia->i_key)-(ib->i_key));
	}

static int
funccount(p,s)
CACHEPAYLOAD *p;
CACHESTATE *s;
	{
	return(1);
	}

static int
funcdump(p,fp,s)
CACHEPAYLOAD *p;
FILE *fp;
CACHESTATE *s;
	{
	struct item *ii;

	ii=(struct item *)p;
	fprintf(fp,"PAYLOAD %d[%d]\n",ii->i_key,ii->i_val);
	return(0);
	}

static int
funcwrite(p,s)
CACHEPAYLOAD *p;
CACHESTATE *s;
	{
	struct item *ii;

	ii=(struct item *)p;
	fprintf(stderr,"WRITING %d[%d]\n",ii->i_key,ii->i_val);
	if (!((0<=ii->i_key)&&(ii->i_key<Things)))
		{
		fprintf(stderr,"MISSING %d\n",ii->i_key);
		return(-1);
		}
	map.cm_map[ii->i_key].cm_line[CM_W]=cachemapnames[CM_W];
	map.cm_map[ii->i_key].cm_store=ii->i_val;
	map.cm_map[ii->i_key].cm_expect=map.cm_map[ii->i_key].cm_store;

	return(0);
	}

static CACHEPAYLOAD *
funcread(p,s)
CACHEPAYLOAD *p;
CACHESTATE *s;
	{
	struct item *ii, *ni;

	ii=(struct item *)p;
	fprintf(stderr,"READING %d\n",ii->i_key);
	if (!((0<=ii->i_key)&&(ii->i_key<Things)))
		{
		fprintf(stderr,"MISSING %d\n",ii->i_key);
		return((CACHEPAYLOAD *)0);
		}
	ni=ialloc();
	ni->i_key=ii->i_key;
	ni->i_val=map.cm_map[ni->i_key].cm_store;
	map.cm_map[ni->i_key].cm_line[CM_R]=cachemapnames[CM_R];
	fprintf(stderr,"READING %d[%d]\n",ni->i_key,ni->i_val);
	return((CACHEPAYLOAD *)ni);
	}

static CACHELINE *
tryreading(i)
int i;
	{
	struct item item, *ii;
	CACHELINE *l;

	fprintf(stderr,"CACHEREAD %d\n",i);
	item.i_key=i;

	if ((l=cacheread(&cache,(void *)&item))==NULL)
		{
		fprintf(stderr,"failed cacheread NULL\n");
		(void)cachelinedump(&cache,l,stderr);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed cacheread payload=NULL\n",ii);
		(void)cachelinedump(&cache,l,stderr);
		l=NULL;
		}
	else if (ii->i_key!=i)
		{
		fprintf(stderr,"failed cacheread %d!=%d\n",ii->i_key,i);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else
		{
		map.cm_map[ii->i_key].cm_line[CM_ER]=cachemapnames[CM_ER];
		fprintf(stderr,"CACHEREAD %d[%d]\n",ii->i_key,ii->i_val);
		(void)cachecomplete(&cache,l);
		}

	mapper(stderr);

	return(l);
	}

static CACHELINE *
tryfinding(i)
int i;
	{
	struct item item, *ii;
	CACHELINE *l;

	fprintf(stderr,"CACHEFIND %d\n",i);
	item.i_key=i;

	if ((l=cachefind(&cache,(void *)&item))==NULL)
		fprintf(stderr,"CACHEFIND not cached\n");
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed cachefind payload=NULL\n",ii);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else if (ii->i_key!=i)
		{
		fprintf(stderr,"failed cachefind %d!=%d\n",ii->i_key,i);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else
		{
		map.cm_map[ii->i_key].cm_line[CM_EF]=cachemapnames[CM_EF];
		fprintf(stderr,"CACHEFIND %d[%d]\n",ii->i_key,ii->i_val);
		(void)cachecomplete(&cache,l);
		}

	mapper(stderr);

	return(l);
	}

static CACHELINE *
tryreadingby(i)
int i;
	{
	struct item item, *ii;
	CACHELINE *l;

	fprintf(stderr,"CACHEREADBY %d\n",i);
	item.i_key=i;
	if ((l=cachereadby(&cache,(void *)&item))==NULL)
		{
		fprintf(stderr,"failed cachereadby NULL\n");
		(void)cachelinedump(&cache,l,stderr);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed cachereadby payload=NULL\n",ii);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else if (ii->i_key!=i)
		{
		fprintf(stderr,"failed cachereadby %d!=%d\n",ii->i_key,i);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else
		{
		map.cm_map[ii->i_key].cm_line[CM_ERB]=cachemapnames[CM_ERB];
		fprintf(stderr,"CACHEREADBY %d[%d]\n",ii->i_key,ii->i_val);
		(void)cachecomplete(&cache,l);
		}

	mapper(stderr);

	return(l);
	}

static CACHELINE *
trywriting(i)
int i;
	{
	struct item *ii, *ni;
	CACHELINE *l;

	Event++;
	fprintf(stderr,"CACHEWRITE %d[%d]\n",i,Event);
	ni=ialloc();
	ni->i_key=i;
	ni->i_val=Event;
	if ((l=cachewrite(&cache,(void *)ni))==NULL)
		{
		fprintf(stderr,"failed cachewrite NULL\n");
		(void)cachelinedump(&cache,l,stderr);
		(void)nfree((void *)ni);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed cachewrite payload=NULL\n",ii);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else if (ii->i_key!=i)
		{
		fprintf(stderr,"failed cachewrite %d!=%d\n",ii->i_key,i);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else
		{
		map.cm_map[i].cm_line[CM_EW]=cachemapnames[CM_EW];
		(void)cachecomplete(&cache,l);
		}

	mapper(stderr);

	return(l);
	}

static CACHELINE *
trywritingthru(i)
int i;
	{
	struct item *ii, *ni;
	CACHELINE *l;

	Event++;
	fprintf(stderr,"CACHEWRITETHRU %d[%d]\n",i,Event);
	ni=ialloc();
	ni->i_key=i;
	ni->i_val=Event;
	if ((l=cachewritethru(&cache,(void *)ni))==NULL)
		{
		fprintf(stderr,"failed cachewritethru NULL\n");
		(void)cachelinedump(&cache,l,stderr);
		(void)nfree((void *)ni);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed cachewritethru payload=NULL\n",ii);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else if (ii->i_key!=i)
		{
		fprintf(stderr,"failed cachewritethru %d!=%d\n",ii->i_key,i);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else
		{
		map.cm_map[i].cm_line[CM_EW]=cachemapnames[CM_EW];
		map.cm_map[i].cm_expect=Event;
		(void)cachecomplete(&cache,l);
		}

	mapper(stderr);

	return(l);
	}

static CACHELINE *
trywritingby(i)
int i;
	{
	struct item *ii, *ni;
	CACHELINE *l;

	Event++;
	fprintf(stderr,"CACHEWRITEBY %d[%d]\n",i,Event);
	ni=ialloc();
	ni->i_key=i;
	ni->i_val=Event;
	if ((l=cachewriteby(&cache,(void *)ni))==NULL)
		{
		fprintf(stderr,"failed cachewriteby NULL\n");
		(void)cachelinedump(&cache,l,stderr);
		(void)nfree((void *)ni);
		mapper(stderr);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed cachewriteby payload=NULL\n",ii);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else if (ii->i_key!=i)
		{
		fprintf(stderr,"failed cachewriteby %d!=%d\n",ii->i_key,i);
		(void)cachelinedump(&cache,l,stderr);
		(void)cachecomplete(&cache,l);
		l=NULL;
		}
	else
		{
		map.cm_map[i].cm_line[CM_EWB]=cachemapnames[CM_EWB];
		map.cm_map[i].cm_expect=Event;
		(void)cachecomplete(&cache,l);
		}

	mapper(stderr);

	return(l);
	}

static int
trydirtying(l)
CACHELINE *l;
	{
	struct item *ii;
	int rc;

	if (l==NULL)
		{
		fprintf(stderr,"failed trydirtying NULL");
		(void)cachelinedump(&cache,l,stderr);
		return(-99);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed trydirtying payload=NULL");
		(void)cachelinedump(&cache,l,stderr);
		return(-98);
		}

	Event++;
	fprintf(stderr,"CACHEDIRTY %d[%d]\n",ii->i_key,Event);
	ii->i_val=Event;
	if ((rc=cachedirty(&cache,l))<0)
		{
        	fprintf(stderr,"failed cachedirty %d\n",rc);
		(void)cachelinedump(&cache,l,stderr);
		}
	else
		map.cm_map[ii->i_key].cm_line[CM_ED]=cachemapnames[CM_ED];

	mapper(stderr);

	return(rc);
	}

static int
tryinvalidating(l)
CACHELINE *l;
	{
	struct item *ii;
	int rc, key;

	if (l==NULL)
		{
		fprintf(stderr,"failed tryinvalidating NULL");
		return(-99);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed tryinvalidating payload=NULL");
		(void)cachelinedump(&cache,l,stderr);
		return(-98);
		}

	key=ii->i_key;

	fprintf(stderr,"CACHEINVALIDATE %d[%d]\n",ii->i_key,ii->i_val);
	if ((rc=cacheinvalidate(&cache,l))<0)
		{
        	fprintf(stderr,"failed cacheinvalidate %d\n",rc);
		(void)cachelinedump(&cache,l,stderr);
		}
	else
		map.cm_map[key].cm_line[CM_EI]=cachemapnames[CM_EI];

	mapper(stderr);

	return(rc);
	}

static int
trybusying(l)
CACHELINE *l;
	{
	struct item *ii;
	int rc;

	if (l==NULL)
		{
		fprintf(stderr,"failed trybusying NULL");
		return(-99);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed trybusying payload=NULL");
		(void)cachelinedump(&cache,l,stderr);
		return(-98);
		}

	fprintf(stderr,"CACHEBUSY %d[%d]\n",ii->i_key,ii->i_val);
	if ((rc=cachebusy(&cache,l))<0)
		{
        	fprintf(stderr,"failed cachebusy %d\n",rc);
		(void)cachelinedump(&cache,l,stderr);
		}
	else
		map.cm_map[ii->i_key].cm_line[CM_EB]=cachemapnames[CM_EB];

	mapper(stderr);

	return(rc);
	}

static int
tryreadonly(l)
CACHELINE *l;
	{
	struct item *ii;
	int rc;

	if (l==NULL)
		{
		fprintf(stderr,"failed tryreadonly NULL");
		return(-99);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed tryreadonly payload=NULL");
		(void)cachelinedump(&cache,l,stderr);
		return(-98);
		}

	fprintf(stderr,"CACHEREADONLY %d[%d]\n",ii->i_key,ii->i_val);
	if ((rc=cachereadonly(&cache,l))<0)
		{
        	fprintf(stderr,"failed cachereadonly %d\n",rc);
		(void)cachelinedump(&cache,l,stderr);
		}
	else
		map.cm_map[ii->i_key].cm_line[CM_ERO]=cachemapnames[CM_ERO];

	mapper(stderr);

	return(rc);
	}

static int
tryreadwrite(l)
CACHELINE *l;
	{
	struct item *ii;
	int rc;

	if (l==NULL)
		{
		fprintf(stderr,"failed tryreadwrite NULL");
		return(-99);
		}
	else if ((ii=(struct item *)l->cl_payload)==NULL)
		{
		fprintf(stderr,"failed tryreadwrite payload=NULL");
		(void)cachelinedump(&cache,l,stderr);
		return(-98);
		}

	fprintf(stderr,"CACHEREADWRITE %d[%d]\n",ii->i_key,ii->i_val);
	if ((rc=cachereadwrite(&cache,l))<0)
		{
        	fprintf(stderr,"failed cachereadwrite %d\n",rc);
		(void)cachelinedump(&cache,l,stderr);
		}
	else
		map.cm_map[ii->i_key].cm_line[CM_ERW]=cachemapnames[CM_ERW];

	mapper(stderr);

	return(rc);
	}

static int
funcflush(l,vm,s)
CACHELINE *l;
void *vm;
CACHESTATE *s;
	{
	struct cachemap *m;
	struct item *ii;
	char c;

	m=(struct cachemap *)vm;
	if ((ii=(struct item *)l->cl_payload)==NULL)
		return(0);
	if (!((0<=ii->i_key)&&(ii->i_key<Things)))
		return(0);
	if (CacheLineIsCached(m->cm_cache,l))
		if (CacheLineIsDirty(m->cm_cache,l))
			map.cm_map[ii->i_key].cm_expect=
				map.cm_map[ii->i_key].cm_cached;
	return(0);
	}

static int
tryflushing()
	{
	int rc, i;

	fprintf(stderr,"CACHEFLUSH\n");
	(void)cacheapply(&cache,funcflush,(void *)&map);
	if ((rc=cacheflush(&cache))<0)
               	fprintf(stderr,"failed cacheflush %d\n",rc);

	mapper(stderr);

	return(rc);
	}

static int
randy(lim)
int lim;
	{
	long r;
	int i;

	r=rand();
	i=(int)(((double)r/(double)RAND_MAX)*(double)(lim+1));
	if (i>lim)
		i=lim;
	return(i);
	}

main(argc,argv)
int argc;
char **argv;
	{
	int opt;
	extern char *optarg;
	extern int optind;
	int inerror, usage;
	int i, j, k;
	CACHELINE *l;
	CACHEPAYLOAD *p;
	int rc, hits, reads;
#ifdef _IBMR2
#ifdef _AIX
	struct sigaction action;
#endif /* _AIX */
#endif /* _IBMR2 */

#ifdef _IBMR2
#ifdef _AIX
	(void)bzero(&action,sizeof(action));
	action.sa_handler=SIG_DFL;
	action.sa_flags=SA_FULLDUMP;
	(void)sigaction(SIGSEGV,&action,NULL);
#endif /* _AIX */
#endif /* _IBMR2 */

	Argvzero=((Argvzero=strrchr(argv[0],'/'))==NULL)?argv[0]:Argvzero+1;

	Debug=0;
	Verbose=0;
	Map=0;
	Things=0;
	Tries=0;
	Attempts=0;
	Protected=0;
	Probation=0;

	usage=0;
	while ((opt=getopt(argc,argv,"dvc:C:mn:r:s:"))>=0)
		{
		inerror=0;
		switch (opt)
			{
		case 'd':
			Debug=1;
			break;
		case 'v':
			Verbose=1;
			break;
		case 'm':
			Map=1;
			break;
		case 'c':
			Probation=atoi(optarg);
			break;
		case 'C':
			Protected=atoi(optarg);
			break;
		case 'n':
			Things=atoi(optarg);
			break;
		case 'r':
			Attempts=atoi(optarg);
			break;
		case 's':
			Tries=atoi(optarg);
			break;
		default:
			usage++;
			}
		if (inerror>0)
			{
			fprintf(stderr,"%s: bad value -- -%c %s\n",
				Argvzero,opt,optarg);
			usage++;
			}
		}
	if (usage)
		{
		fprintf(stderr,"usage: %s %s\n",Argvzero,
"-d -v -c probation -C protected -n items -s stresses -r reads");
		exit(1);
		}

	fprintf(stderr,
"%s: %d items, %d probation, %d protected, %d stresses, %d reads\n",
		Argvzero,Things,Probation,Protected,Tries,Attempts);

	config.cf_free=funcfree;
	config.cf_compare=funccompare;
	config.cf_strategy=NULL;
	config.cf_count=funccount;
	config.cf_dump=funcdump;
	config.cf_write=funcwrite;
	config.cf_read=funcread;
	config.cf_trace=Debug?stderr:NULL;
	config.cf_limit[CA_PROB]=Probation;
	config.cf_limit[CA_PROT]=Protected;

	allocmap();
	initmap();
	mapper(stderr);
	
	fprintf(stderr,"* INITIALIZE\n");
	if (cacheinit(&cache,&config)<0)
		{
		fprintf(stderr,"failed cacheinit\n");
		exit(2);
		}
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* READ FORWARD\n");
	for (i=0; i<Things; i++)
		(void)tryreading(i);
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* READ BACKWARD\n");
	for (i=Things-1; i>=0; i--)
		(void)tryreading(i);
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* WRITE FORWARD\n");
	for (i=0; i<Things; i++)
		(void)trywriting(i);
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* WRITE BACKWARD\n");
	for (i=Things-1; i>=0; i--)
		(void)trywriting(i);
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* WRITETHROUGH FORWARD\n");
	for (i=0; i<Things; i++)
		(void)trywritingthru(i);
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* WRITETHROUGH BACKWARD\n");
	for (i=Things-1; i>=0; i--)
		(void)trywritingthru(i);
	if (Verbose)
		(void)cachedump(&cache,stderr);

/**********
	fprintf(stderr,"* READ STORE FAILURES\n");
	(void)tryreading(-1);
	(void)tryreading(Things);
	(void)tryreadingby(-1);
	(void)tryreadingby(Things);

	fprintf(stderr,"* WRITE STORE FAILURES\n");
	(void)trywritingthru(-1);
	(void)trywritingthru(Things);
	(void)trywritingby(-1);
	(void)trywritingby(Things);
	(void)tryflushing();
**********/

	fprintf(stderr,"* BUSY/DIRTY READ COMPLETION FAILURES\n");
	l=tryreading(0);
	(void)trydirtying(l);
	(void)trybusying(l);
	(void)trywriting(0);
	(void)tryinvalidating(l);
	(void)tryflushing();
	(void)tryinvalidating(l);

	fprintf(stderr,"* BUSY/DIRTY WRITE COMPLETION FAILURES\n");
	l=trywriting(0);
	(void)trydirtying(l);
	(void)trybusying(l);
	if (l!=NULL)
		(void)cachecomplete(&cache,l);

	fprintf(stderr,"* READONLY FAILURES\n");
	l=tryreading(0);
	(void)tryreadonly(l);
	(void)trydirtying(l);
	(void)trywriting(0);
	(void)tryreadwrite(l);
	(void)trydirtying(l);
	(void)trywriting(0);
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* CYCLE\n");
	for (i=0; i<Things; i++)
		{
		(void)tryreading(i);
		(void)trywritingthru(i);
		(void)trywriting(i);
		(void)tryflushing();
		l=tryreading(i);
		(void)trydirtying(l);
		(void)tryflushing();
		(void)tryinvalidating(l);
		}
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* PROMOTION\n");
	for (i=0; i<Things; i++)
		for (j=0; j<2; j++)
			(void)tryreading(i);
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* FINDING\n");
	for (i=0; i<Things; i++)
		(void)tryfinding(i);

	fprintf(stderr,"* STRESS\n");
	for (k=0; k<Tries; k++)
		{
		i=randy(Things-1);
		j=randy(9);
		switch (j)
			{
		case 0:
			l=tryreading(i);
			break;
		case 1:
			l=trywriting(i);
			break;
		case 2:
			if ((l!=NULL)&&(CacheLineIsCached(&cache,l))&&(!CacheLineIsDirty(&cache,l)))
				(void)tryinvalidating(l);
			break;
		case 3:
			if ((l!=NULL)&&(CacheLineIsCached(&cache,l)))
				(void)trydirtying(l);
			break;
		case 4:
			l=tryreadingby(i);
			break;
		case 5:
			l=trywritingby(i);
			break;
		case 6:
			l=trywritingthru(i);
			break;
		case 7:
			(void)tryflushing();
			break;
		case 8:
			if ((l!=NULL)&&(CacheLineIsCached(&cache,l)))
				(void)tryreadonly(l);
			break;
		case 9:
			if ((l!=NULL)&&(CacheLineIsCached(&cache,l)))
				(void)tryreadwrite(l);
			break;
			}
		}
	if (Verbose)
		(void)cachedump(&cache,stderr);

	fprintf(stderr,"* HITS\n");
	hits=cache.ca_stats.cs_hit;
	reads=cache.ca_stats.cs_read;
	for (k=0; k<Attempts; k++)
		{
		i=randy(Things-1);
		(void)tryreading(i);
		}
	hits=cache.ca_stats.cs_hit-hits;
	reads=cache.ca_stats.cs_read-reads;
	if (Verbose)
		(void)cachedump(&cache,stderr);
	fprintf(stderr,"hits(%d@[%d/%d,%d/%d])=(%d/%d)*100%=%f\n",
		Things,
		cache.ca_stats.cs_maximum[CA_PROB],
		cache.ca_cf->cf_limit[CA_PROB],
		cache.ca_stats.cs_maximum[CA_PROT],
		cache.ca_cf->cf_limit[CA_PROT],
		hits,reads,
		((double)hits/(double)reads)*100.0);

	fprintf(stderr,"* COHERENCY\n");
	(void)tryflushing();
	for (i=0; i<Things; i++)
		if (map.cm_map[i].cm_store!=map.cm_map[i].cm_expect)
			fprintf(stderr,"check failed %d[%d!=%d]\n",
				i,map.cm_map[i].cm_store,
				map.cm_map[i].cm_expect);
	fprintf(stderr,"check complete\n");

	exit(0);
	}
