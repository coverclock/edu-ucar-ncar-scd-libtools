/*
**	N E W   M A L L O C
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		New Memory Allocation
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Oct  6 11:19:34 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	I expect to have to implement a new memory allocation/deallocation
**	scheme in the future, perhaps with either on-demand or automatic
**	garbage collection. All of the code I've written uses _these_
**	routines, instead of the UNIX malloc/free, to handle dynamic
**	memory requests. The overhead is slightly higher, but it'll make
**	a future conversion trivial, as far as existing code is concerned
**	anyway.
**
**	This also makes is easy to add telemetry code to instrument
**	memory allocation and deallocation. Note that for applications
**	which include "nmalloc.h", if NMALLOC_LIBTOOLS is not defined,
**	the header file simply defines the "new" functions to be the
**	"old" functions, and this module is not included. This makes it
**	easy to switch between instrumented allocation and
**	uninstrumented (but more space and time efficient) allocation.
**
**	It is important to note that there are TWO classes of
**	calls: e.g. "nmalloc" is used when allocating memory which may
**	contain pointers to other dynamically acquired memory, and
**	"namalloc" for things like I/O buffers which never contain
**	legitimate addresses. This will speed up mark and sweep garbage
**	collection in the future since the I/O buffers and their ilk
**	need not be scanned for pointers (which I'm not convinced is
**	a workable strategy, but I've read some work done by others
**	using just this technique; with C I think the problem is
**	mostly intractable due to the language itself).
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)nmalloc.c	4.39 94/07/18 jsloan@ncar.ucar.edu";

#ifndef NMALLOC_LIBTOOLS
#define NMALLOC_LIBTOOLS
#endif /* !NMALLOC_LIBTOOLS */

#include "libtools.h"
#include <stdio.h>
#include <stdlib.h>
#include "libtstrings.h"
#include "slack.h"
#include "queues.h"
#include "nmalloc.h"

/*
**	Define some mallocly macros to make porting easier,
**	and also to possible use other memory allocation systems in the
**	future.
*/

#ifdef OLDMALLOC_LIBTOOLS

extern char * malloc();
extern char * realloc();
extern int free();

#define MALLOC(n)		((void *)malloc((size_t)(n)))
#define AMALLOC(n)		((void *)malloc((size_t)(n)))
#define REALLOC(p,n)		((void *)realloc(((char *)(p)),(size_t)(n)))
#define AREALLOC(p,n)		((void *)realloc(((char *)(p)),(size_t)(n)))
#define FREE(p)			((void)free((char *)(p)))

#else /* OLDMALLOC_LIBTOOLS */

#define MALLOC(n)		(malloc(n))
#define AMALLOC(n)		(malloc(n))
#define REALLOC(p,n)		(realloc((p),(n)))
#define AREALLOC(p,n)		(realloc((p),(n)))
#define FREE(p)			(free(p))

#endif /* OLDMALLOC_LIBTOOLS */

/*
**	This is the header that's prefixed onto every section of allocated
**	memory. Because of doubleword alignment, there is probably a
**	certain amount of wasted space here. I was tempted to pad with
**	maybe-useful fields, but on architectures like the Y-MP where
**	everything is a double, it's just wasted space.
*/

struct Memory
	{
	QUEUENODE	mem_link;	/* This has to be first. */
	int		mem_size;
	int		mem_flags;
#define			MEM_ATOMIC	0x1
	};

typedef struct Memory MEMORY;

/*
**	Define some macros to make it easier to convert between requested
**	size and actual size, and also to handle double word alignment.
*/

#define ALIGN		double
#define OVERHEAD	(sizeof(MEMORY)+SLACK(sizeof(MEMORY),sizeof(ALIGN)))
#define SIZE(size)	((size)+OVERHEAD+SLACK((size)+OVERHEAD,sizeof(ALIGN)))
#define BODY(ptr)	((void *)(((char *)(ptr))+OVERHEAD))
#define BLOCK(ptr)	((MEMORY *)(((char *)(ptr))-OVERHEAD))

/*
**	Define some macros to simplify our bookkeeping.
*/

#define ENTERING(module)	do { \
			P(sem_nmalloc_libt); \
			(nms.module++); \
			V(sem_nmalloc_libt); \
			} while (0)

#define ALLOCATING(ptr,size,flags)	do { \
			P(sem_nmalloc_libt); \
			(ptr)->mem_size=(size); \
			(ptr)->mem_flags=(flags); \
			nms.nms_actbytes+=(size); \
			nms.nms_actactual+=SIZE(size); \
			nms.nms_actsegments++; \
			nms.nms_totbytes+=(size); \
			nms.nms_totactual+=SIZE(size); \
			nms.nms_totsegments++; \
			(void)qinsert(&queue,qtail(&queue),&((ptr)->mem_link)); \
			V(sem_nmalloc_libt); \
			} while (0)

#define FREEING(ptr)	do { \
			P(sem_nmalloc_libt); \
			(void)qdelete(&queue,&((ptr)->mem_link)); \
			nms.nms_actbytes-=((ptr)->mem_size); \
			nms.nms_actactual-=SIZE((ptr)->mem_size); \
			nms.nms_actsegments--; \
			V(sem_nmalloc_libt); \
			} while (0)

/*
** Here are some shared variables.
*/

static NMALLOCSTATS nms={0};		/* thread safe: sem_nmalloc_libt */
static FILE *logfp=NULL;		/* thread safe: sem_nmalloc_libt */
static QUEUE queue={0};			/* thread safe: sem_nmalloc_libt */

/*
** Functions
*/

void *
nmalloc(size_t size)
	{
	int actual;
	MEMORY *cb;
	void *ptr;

	ENTERING(nms_nmalloc);

	actual=SIZE(size);

	if ((cb=(MEMORY *)MALLOC(actual))!=NULL)
		{
		ALLOCATING(cb,size,0);
		ptr=BODY(cb);
		}
	else
		ptr=NULL;

	if (logfp!=NULL)
		fprintf(logfp,
		"nmalloc[%d]: nmalloc(%d)=0x%lx[%d:%d|0x%x] s=%d b=%d a=%d\n",
			getpid(),size,ptr,size,actual,0,
			nms.nms_actsegments,nms.nms_actbytes,nms.nms_actactual);

	return(ptr);
	}

void *
namalloc(size_t size)
	{
	int actual;
	MEMORY *cb;
	void *ptr;

	ENTERING(nms_namalloc);

	actual=SIZE(size);

	if ((cb=(MEMORY *)AMALLOC(actual))!=NULL)
		{
		ALLOCATING(cb,size,MEM_ATOMIC);
		ptr=BODY(cb);
		}
	else
		ptr=NULL;

	if (logfp!=NULL)
		fprintf(logfp,
		"namalloc[%d]: nmalloc(%d)=0x%lx[%d:%d|0x%x] s=%d b=%d a=%d\n",
			getpid(),size,ptr,size,actual,MEM_ATOMIC,
			nms.nms_actsegments,nms.nms_actbytes,nms.nms_actactual);

	return(ptr);
	}

void *
nrealloc(void *ptr, size_t size)
	{
	int actual, was, wasf;
	MEMORY *cb;
	void *new;

	ENTERING(nms_nrealloc);

	actual=SIZE(size);

	if (ptr==NULL)
		{
		cb=(MEMORY *)MALLOC(actual);
		was=0;
		wasf=0;
		}
	else
		{
		cb=BLOCK(ptr);
		was=cb->mem_size;
		wasf=cb->mem_flags;
		FREEING(cb);
		cb=(MEMORY *)REALLOC(cb,actual);
		}

	if (cb!=NULL)
		{
		ALLOCATING(cb,size,0);
		new=BODY(cb);
		}
	else
		new=NULL;

	if (logfp!=NULL)
		fprintf(logfp,
"nmalloc[%d]: nrealloc(0x%lx[%d:%d|0x%x],%d)=0x%lx[%d:%d|0x%x] s=%d b=%d a=%d\n",
			getpid(),ptr,was,SIZE(was),wasf,size,
			new,size,actual,0,
			nms.nms_actsegments,nms.nms_actbytes,nms.nms_actactual);

	return(new);
	}

void *
narealloc(void *ptr, size_t size)
	{
	int actual, was, wasf;
	MEMORY *cb;
	void *new;

	ENTERING(nms_narealloc);

	actual=SIZE(size);

	if (ptr==NULL)
		{
		cb=(MEMORY *)AMALLOC(actual);
		was=0;
		wasf=0;
		}
	else
		{
		cb=BLOCK(ptr);
		was=cb->mem_size;
		wasf=cb->mem_flags;
		FREEING(cb);
		cb=(MEMORY *)AREALLOC(cb,actual);
		}

	if (cb!=NULL)
		{
		ALLOCATING(cb,size,MEM_ATOMIC);
		new=BODY(cb);
		}
	else
		new=NULL;

	if (logfp!=NULL)
		fprintf(logfp,
"nmalloc[%d]: narealloc(0x%lx[%d:%d|0x%x],%d)=0x%lx[%d:%d|0x%x] s=%d b=%d a=%d\n",
			getpid(),ptr,was,SIZE(was),wasf,size,
			new,size,actual,MEM_ATOMIC,
			nms.nms_actsegments,nms.nms_actbytes,nms.nms_actactual);

	return(new);
	}

void
nfree(void *ptr)
	{
	MEMORY *cb;

	ENTERING(nms_nfree);

	cb=BLOCK(ptr);
	FREEING(cb);
	FREE(cb);

	if (logfp!=NULL)
		fprintf(logfp,
		"nmalloc[%d]: free(0x%lx[%d:%d|0x%x]) s=%d b=%d a=%d\n",
			getpid(),ptr,cb->mem_size,SIZE(cb->mem_size),
			cb->mem_flags,
			nms.nms_actsegments,nms.nms_actbytes,nms.nms_actactual);
	}

NMALLOCSTATS *
nmalloced(NMALLOCSTATS *np)
	{
	P(sem_nmalloc_libt);
	if (np!=NULL)
		(void)bcopy((char *)&nms,(char *)np,sizeof(NMALLOCSTATS));
	V(sem_nmalloc_libt);
	return(np);
	}

int
nmallocinfo(FILE *fp)
        {
	NMALLOCSTATS n;
	int oh;

	(void)nmalloced(&n);

        fprintf(fp,"nmallocinfo: actbytes=%d\n",n.nms_actbytes);
        fprintf(fp,"nmallocinfo: actactual=%d\n",n.nms_actactual);
        fprintf(fp,"nmallocinfo: actsegments=%d\n",n.nms_actsegments);
        fprintf(fp,"nmallocinfo: totbytes=%d\n",n.nms_totbytes);
        fprintf(fp,"nmallocinfo: totactual=%d\n",n.nms_totactual);
        fprintf(fp,"nmallocinfo: totsegments=%d\n",n.nms_totsegments);
        fprintf(fp,"nmallocinfo: nmalloc=%d\n",n.nms_nmalloc);
        fprintf(fp,"nmallocinfo: namalloc=%d\n",n.nms_namalloc);
        fprintf(fp,"nmallocinfo: nrealloc=%d\n",n.nms_nrealloc);
        fprintf(fp,"nmallocinfo: narealloc=%d\n",n.nms_narealloc);
        fprintf(fp,"nmallocinfo: nfree=%d\n",n.nms_nfree);

	return(nms.nms_actactual);
        }

FILE *
nmalloclog(FILE *fp)
	{
	FILE *oldfp;

	P(sem_nmalloc_libt);
	oldfp=logfp;
	logfp=fp;
	V(sem_nmalloc_libt);
	return(oldfp);
	}

int
nmallocapply(int (*f)(), void *vp)
	{
	MEMORY *cb;
	int rc;

	if (f==NULL)
		return(0);
	P(sem_nmalloc_libt);
	for (rc=0, cb=(MEMORY *)qhead(&queue); cb!=NULL; cb=(MEMORY *)qnext((QUEUENODE *)cb))
		if ((rc=(*f)(BODY(cb),cb->mem_size,cb->mem_flags,vp))!=0)
			break;
	V(sem_nmalloc_libt);
	return(rc);
	}

int
fpool(void *p, int s, int f, void *v)
	{
        fprintf((FILE *)v,"nmallocpool: 0x%lx[%d:%d|0x%x]\n",p,s,SIZE(s),f);
	return(0);
	}

int
nmallocpool(FILE *fp)
	{
	(void)nmallocapply(fpool,(void *)fp);
	return(nms.nms_actactual);
	}
