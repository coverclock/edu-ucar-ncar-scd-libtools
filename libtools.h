/*
**	L I B T O O L S
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)libtools.h	4.7 94/06/06 jsloan@ncar.ucar.edu
*/

#ifndef H_LIBTOOLS
#define H_LIBTOOLS

#ifndef PROTOTYPE_LIBTOOLS
#define PROTOTYPE_LIBTOOLS
#endif /* !PROTOTYPE_LIBTOOLS */

#include "serialize.h"
#include <stdio.h>

/*
**	Believe it or not, MVS has a pretty good POSIX compatibility
**	library _except_ for the fact that other than in the context
**	of sockets, file descriptors are not implemented. So you can
**	do an fopen(3) and get a FILE pointer, but you cannot do an
**	open(2) and get a file descriptor.
*/
#ifdef MVS_LIBTOOLS
#define fdopen(fd,mode)		((FILE *)0)
#define close(fd)		(-1)
#define dup(fd)			(-1)
#define read(fd,buf,len)	(-1)
#define write(fd,buf,len)	(-1)
#define fileno(fp)		(-1)
#define geteuid()		(-1)
#define getuid()		(-1)
#define getpid()		(-1)
#define getgid()		(-1)
#define delenv(name)		(-1)
#endif /* MVS_LIBTOOLS */

/*
**	It would have been better planning maybe to keep the
**	semaphores local to each module and have conditionally
**	compiled code in each module to initialize the semaphore,
**	but I wanted to have one single point of initialization
**	per library that an application would have to call.
*/
#ifdef PTHREADS_LIBTOOLS

extern const SEMAPHORE *sem_buffers_libt;
extern const SEMAPHORE *sem_cache_libt;
extern const SEMAPHORE *sem_keyval_libt;
extern const SEMAPHORE *sem_keyword_libt;
extern const SEMAPHORE *sem_nextfrompath_libt;
extern const SEMAPHORE *sem_nextparser_libt;
extern const SEMAPHORE *sem_nmalloc_libt;
extern const SEMAPHORE *sem_skeyval_libt;
extern const SEMAPHORE *sem_skiplist_libt;
extern const SEMAPHORE *sem_timeformat_libt;
extern const SEMAPHORE *sem_timeclock_libt;
extern const SEMAPHORE *sem_memorybreak_libt;

extern void libtools(void);
extern void libtoolsdump(FILE *fp);

#define P(sem)          (libtools(),semP((SEMAPHORE *)sem))
#define V(sem)          (libtools(),semV((SEMAPHORE *)sem))
#define SW()		(semSw())

#else /* PTHREADS_LIBTOOLS */

#define	P(sem)		(0)
#define V(sem)		(0)
#define SW()		(0)

#endif /* PTHREADS_LIBTOOLS */

#endif /* !H_LIBTOOLS */
