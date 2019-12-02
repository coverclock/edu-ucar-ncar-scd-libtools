/*
**	S E R I A L I Z E
**
**	Copyright 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)serialize.h	4.5 94/07/29 jsloan@ncar.ucar.edu
**
**	This header file defines the P/V operations on semaphores. This
**	provides hooks to add a serialization mechanism to protect
**	critical sections (mostly shared data) in the libtools code
**	if the library is every used in an application under threads
**	(lightweight processes). You're on your own to define P, V,
**	and the various semaphores used in libtools. All the semaphores
**	used in libtools are binary semaphores, and have a naming
**	convention of "sem_<name>_libt" where <name> is the module or
**	data type name. These will need to be global, since they have
**	to be declared and initialized external to the modules in which
**	they are used. The P/V operations provide fairly fine locking
**	granularity, although I expect some hotspots and/or bottlenecks
**	will be inevitable.
**
**	Note: a similar semaphore naming convention was followed in
**	libipc and libmessages, using "sem_<name>_libi" and
**	"sem_<name>_libm" respectively.
**
**	The more I dink around with semaphores the more I appreciate
**	Hoare's concept of monitors. Semaphores seemed so simple back
**	in my real-time microkernel days, and now they seem so inadequate.
*/

#ifndef H_SERIALIZE
#define H_SERIALIZE

#ifdef PTHREADS_LIBTOOLS

#include <pthread.h>
#include <stdio.h>

struct semaphore
	{
	pthread_mutex_t		sem_mutex;
	pthread_mutex_t		sem_queue;
	int			sem_count;
	int			sem_min;
	int			sem_max;
	int			sem_p;
	int			sem_v;
	};

typedef struct semaphore SEMAPHORE;

extern void semP(SEMAPHORE *s);
extern void semV(SEMAPHORE *s);
extern void semSw(void);
extern void semInit(SEMAPHORE *s, int count);
extern void semDump(SEMAPHORE *s, FILE *fp);

#endif /* PTHREADS_LIBTOOLS */

#endif /* !H_SERIALIZE */
