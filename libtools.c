/*
**	L I B T O O L S
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Libtools Initialization
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue May 17 09:35:23 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	This module contains all the semaphore structures used by
**	libtools, along with the code to initialize them.
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)libtools.c	1.7 94/07/29 jsloan@ncar.ucar.edu";

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "libtools.h"

#ifdef PTHREADS_LIBTOOLS

#define SEMAPHORES	(12)

/*
**	This is an array of semaphores used by libtools. Putting them
**	in an array makes is easier to dump and initialize.
*/
static SEMAPHORE semaphores[SEMAPHORES];

/*
**	Here are pointers into the semaphore array. These names
**	are the ones actually used by libtools.
*/
const SEMAPHORE *sem_buffers_libt=&semaphores[0];
const SEMAPHORE *sem_cache_libt=&semaphores[1];
const SEMAPHORE *sem_keyval_libt=&semaphores[2];
const SEMAPHORE *sem_keyword_libt=&semaphores[3];
const SEMAPHORE *sem_nextfrompath_libt=&semaphores[4];
const SEMAPHORE *sem_nextparser_libt=&semaphores[5];
const SEMAPHORE *sem_nmalloc_libt=&semaphores[6];
const SEMAPHORE *sem_skeyval_libt=&semaphores[7];
const SEMAPHORE *sem_skiplist_libt=&semaphores[8];
const SEMAPHORE *sem_timeformat_libt=&semaphores[9];
const SEMAPHORE *sem_timeclock_libt=&semaphores[10];
const SEMAPHORE *sem_memorybreak_libt=&semaphores[11];

/*
**	Here's a list of semaphore names that is used solely
**	for debugging output.
*/
static char *names[]=
	{
	"sem_buffers_libt",
	"sem_cache_libt",
	"sem_keyval_libt",
	"sem_keyword_libt",
	"sem_nextfrompath_libt",
	"sem_nextparser_libt",
	"sem_nmalloc_libt",
	"sem_skeyval_libt",
	"sem_skiplist_libt",
	"sem_timeformat_libt",
	"sem_timeclock_libt",
	"sem_memorybreak_libt"
	};

/*
**	libtoolsdump: dump out all the secret data structures
**	used by libtools. In this case, they're all semaphores.
*/
void
libtoolsdump(FILE *fp)
	{
	int i;

	for (i=0; i<SEMAPHORES; i++)
		{
		fputs(names[i],fp);
		fputs(": ",fp);
		semDump(&semaphores[i],fp);
		}
	}

/*
**	inittools: a local function that initializes all secret
**	data structures (all semaphores) used by libtools.
*/
static void
inittools(void)
	{
	int i;

	for (i=0; i<SEMAPHORES; i++)
		semInit(&semaphores[i],1);
	}

/*
**	libtools: this is a function that is called prior to every
**	P/V operation in libtools. Yes, this is expensive, but it
**	was the simplest way to retrofit libtools to be threadsafe.
**	It should eventually be replaced by something more efficient.
**	In any case, DCE pthreads insures that the initialization
**	will be performed only once ever.
*/
void
libtools(void)
	{
	int rc;
	static pthread_once_t once=pthread_once_init;

	rc=pthread_once(&once,inittools);
	assert(rc>=0);
	}

#endif /* PTHREADS_LIBTOOLS */
