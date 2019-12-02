/*
**	N M A L L O C
**
**	Copyright 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)nmalloc.h	4.22 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_NMALLOC
#define H_NMALLOC

#include <stdio.h>
#include <stdlib.h>
#include "queues.h"
#include "libtproto.h"

struct nmallocstats
	{
	int	nms_actbytes;
	int	nms_actactual;
	int	nms_actsegments;
	int	nms_totbytes;
	int	nms_totactual;
	int	nms_totsegments;
	int	nms_nmalloc;
	int	nms_namalloc;
	int	nms_nrealloc;
	int	nms_narealloc;
	int	nms_nfree;
	};

typedef struct nmallocstats NMALLOCSTATS;

#ifdef NMALLOC_LIBTOOLS

extern void * nmalloc LIBTPROTO((size_t size));
extern void * namalloc LIBTPROTO((size_t size));
extern void * nrealloc LIBTPROTO((void *ptr, size_t size));
extern void * narealloc LIBTPROTO((void *ptr, size_t size));
extern void nfree LIBTPROTO((void *pointer));
extern NMALLOCSTATS * nmalloced LIBTPROTO((NMALLOCSTATS *p));
extern int nmallocinfo LIBTPROTO((FILE *fp));
extern int nmallocpool LIBTPROTO((FILE *fp));
extern FILE * nmalloclog LIBTPROTO((FILE *fp));
extern int nmallocapply LIBTPROTO((int (*f)(), void *vp));
/* f(ptr,size,flags,vp) */

#else /* NMALLOC_LIBTOOLS */

#ifdef OLDMALLOC_LIBTOOLS

extern char * malloc();
extern char * realloc();
extern int free();

#define nmalloc(s)	((void *)malloc((size_t)s))
#define	namalloc(s)	((void *)malloc((size_t)s))
#define nrealloc(p,s)	((void *)realloc(((void *)(p)),(size_t)(s)))
#define narealloc(p,s)	((void *)realloc(((void *)(p)),(size_t)(s)))
#define nfree(p)	((void)free((void *)p))

#else /* OLDMALLOC_LIBTOOLS */

#define nmalloc(s)	(malloc(s))
#define	namalloc(s)	(malloc(s))
#define nrealloc(p,s)	(realloc((p),(s)))
#define narealloc(p,s)	(realloc((p),(s)))
#define nfree(p)	(free(p))

#endif /* OLDMALLOC_LIBTOOLS */

#define nmalloced(c,t)		((NMALLOCSTATS *)0)
#define nmallocinfo(fp)		(0)
#define nmallocpool(fp)		(0)
#define nmalloclog(fp)		((FILE *)NULL)
#define nmallocapply(f,vp)	(0)

#endif /* NMALLOC_LIBTOOLS */

#endif /* !H_NMALLOC */
