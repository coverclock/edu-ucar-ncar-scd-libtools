/*
**	S E R I A L I Z E
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Serialize
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue May 17 09:12:23 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	This module defines an implementation for the libtools
**	serialization primitives. The current implemention supports
**	only PTHREADS under DCE, but other implementations are
**	possible.
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)serialize.c	1.4 94/07/29 jsloan@ncar.ucar.edu";

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "serialize.h"

#ifdef PTHREADS_LIBTOOLS

/*
**	semInit: initialize a single counting semaphore. The counting
**	semaphore is implemented using DCE mutex operations. Two
**	mutexen are actually used, one to protect the semaphore
**	structure itself, and another to queue the thread on the
**	semaphore.
*/
void
semInit(SEMAPHORE *s, int count)
	{
	int rc;

	rc=pthread_mutex_init(&s->sem_mutex,pthread_mutexattr_default);
	assert(rc>=0);

	rc=pthread_mutex_init(&s->sem_queue,pthread_mutexattr_default);
	assert(rc>=0);

	rc=pthread_mutex_lock(&s->sem_queue);
	assert(rc>=0);

	s->sem_count=count;
	s->sem_min=count;
	s->sem_max=count;
	s->sem_p=0;
	s->sem_v=0;
	}

/*
**	semP: Do a Dijkstra P operation on a counting semaphore.
*/
void
semP(SEMAPHORE *s)
	{
	int rc, count;

	rc=pthread_mutex_lock(&s->sem_mutex);
	assert(rc>=0);

	s->sem_p++;
	s->sem_count--;
	if (s->sem_count<s->sem_min)
		s->sem_min=s->sem_count;
	count=s->sem_count;

	rc=pthread_mutex_unlock(&s->sem_mutex);
	assert(rc>=0);

	if (count<0)
		{
		rc=pthread_mutex_lock(&s->sem_queue);
		assert(rc>=0);
		}
	}

/*
**	semV: Do a Dijkstra V operation on a counting semaphore.
*/
void
semV(SEMAPHORE *s)
	{
	int rc, count;

	rc=pthread_mutex_lock(&s->sem_mutex);
	assert(rc>=0);

	s->sem_v++;
	s->sem_count++;
	if (s->sem_count>s->sem_max)
		s->sem_max=s->sem_count;
	count=s->sem_count;

	rc=pthread_mutex_unlock(&s->sem_mutex);
	assert(rc>=0);

	if (count<=0)
		{
		pthread_mutex_unlock(&s->sem_queue);
		assert(rc>=0);
		}
	}

/*
**	semSW: do a Short Wait (not really a semaphore operation)
**	to relinquish control to another thread.
*/
void
semSw(void)
	{
	pthread_yield();
	}

/*
**	semDump: print a semaphore to the indicated file pointer. Mostly
**	used for debugging.
*/
void
semDump(SEMAPHORE *s, FILE *fp)
	{
	int count, p, v, n, x;

	(void)pthread_mutex_lock(&s->sem_mutex);

	count=s->sem_count;
	p=s->sem_p;
	v=s->sem_v;
	n=s->sem_min;
	x=s->sem_max;

	(void)pthread_mutex_unlock(&s->sem_mutex);

	fprintf(fp,"semDump: 0x%x %d<=%d<=%d p=%d v=%d\n",
		s,n,count,x,p,v);
	}

#endif /* PTHREADS_LIBTOOLS */
