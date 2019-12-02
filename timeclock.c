/*
**	T I M E   C L O C K
**
**	Copyright 1989-1996 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Time Clock
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Dec  8 17:04:05 MST 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	This module started out being about time keeping, but it
**	evolved into a place where system specific stuff is stowed
**	away in order to ease porting to new platforms. Hence, this
**	is a likely place to have problems when installing libtools
**	on new platforms.
*/

static char copyright[]="Copyright 1989-1996 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)timeclock.c	4.7 96/06/18 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "libttypes.h"
#include "libtstrings.h"
#ifdef MVS_LIBTOOLS
#include <bsdtime.h>
#else /* MVS_LIBTOOLS */
#ifdef CPUUSECONDS_GETRUSAGE
#include <sys/time.h>
#include <sys/resource.h>
#else /* CPUUSECONDS_GETRUSAGE */
#ifdef CPUUSECONDS_TIMES
#include <sys/types.h>
#include <sys/param.h>
#include <sys/times.h>
#endif /* CPUUSECONDS_TIMES */
#endif /* CPUUSECONDS_GETRUSAGE */
#endif /* MVS_LIBTOOLS */
#ifdef TIMECLOCK_GETTIMEOFDAY
#include <sys/time.h>
#endif /* TIMECLOCK_GETTIMEOFDAY */
#include "timeclock.h"

/*
**	timeclock: return the current time in seconds. This
**	works the same as the old BSD time(3) function, which
**	in later versions of BSD uses the current gettimeofday(2)
**	system call. It is provided for systems which lack the
**	older, obsolesced call. POSIX.1 defines time(3).
**
**	hightimeclock: return a higher resolution time in a
**	structure. The first field is in seconds, the second
**	in nanoseconds (although less significant digits may
**	be zero if the native timer does not support nanosecond
**	resolution).
**
**	Note that POSIX.1 lacks any timer function with higher
**	resolution than time(3), which is in seconds.
*/
TIMESTAMP
timeclock(void)
	{
#ifdef TIMECLOCK_GETTIMEOFDAY
	struct timeval tic;
	int rc;

	P(sem_timeclock_libt);
#ifdef TIMECLOCK_NOTIMEZONE
	rc=gettimeofday(&tic);
#else  /* TIMECLOCK_NOTIMEZONE */
	rc=gettimeofday(&tic,(struct timezone *)0);
#endif /* TIMECLOCK_NOTIMEZONE */
	V(sem_timeclock_libt);
	if (rc<0)
		return((TIMESTAMP)(-1));
	return((TIMESTAMP)tic.tv_sec);
#else /* TIMECLOCK_GETTIMEOFDAY */
	return((TIMESTAMP)time((time_t *)0));
#endif /* TIMECLOCK_GETTIMEOFDAY */
	}

HIGHTIMESTAMP *
hightimeclock(HIGHTIMESTAMP *t)
	{
	static HIGHTIMESTAMP ts;	/* deliberately unprotected */
	HIGHTIMESTAMP *tp;
#ifdef TIMECLOCK_GETTIMEOFDAY
	struct timeval tic;
	int rc;

	P(sem_timeclock_libt);
#ifdef TIMECLOCK_NOTIMEZONE
	rc=gettimeofday(&tic);
#else /* TIMECLOCK_NOTIMEZONE */
	rc=gettimeofday(&tic,(struct timezone *)0);
#endif /* TIMECLOCK_NOTIMEZONE */
	V(sem_timeclock_libt);
	if (rc<0)
		return(NULL);
	tp=(t==(HIGHTIMESTAMP *)0)?&ts:t;
	tp->t_sec=tic.tv_sec;
	tp->t_nsec=tic.tv_usec*1000;
#else /* TIMECLOCK_GETTIMEOFDAY */
	time_t tics;

	P(sem_timeclock_libt);
	tics=time((time_t *)0);
	V(sem_timeclock_libt);
	if (tics==(time_t)(-1))
		return(NULL);
	tp=(t==(HIGHTIMESTAMP *)0)?&ts:t;
	tp->t_sec=tics;
	tp->t_nsec=0;
#endif /* TIMECLOCK_GETTIMEOFDAY */
	return(tp);
	}

/*
**	timeformat: format the provided time in an buffer according
**	to the printf-style format string. This is merely a wrapper
**	around the POSIX strftime function. If utc is true, UTC
**	is used, otherwise local time is used.
**
**	N.B.	The libc functions gmtime() and localtime() are
**		apparently not reentrant.
*/
char *
timeformat(char *buffer, int bufsize, char *format, TIMESTAMP seconds, int utc)
	{
	time_t tod;
	struct tm toc, *tp;	/* thread safe: sem_timeformat_libt */

	tod=(time_t)seconds;

	P(sem_timeformat_libt);
	if ((tp=utc?gmtime(&tod):localtime(&tod))!=NULL)
		(void)bcopy(tp,&toc,sizeof(toc));
	V(sem_timeformat_libt);

	if (tp==NULL)
		return(NULL);

	(void)strftime(buffer,bufsize,format,&toc);

	return(buffer);
	}

/*
**	timeelapsed: simple little routine to return an elapsed time
**	string in D/HH:MM:SS format, given elapsed seconds.
*/
char *
timeelapsed(char *buffer, TIMESTAMP seconds)
	{
	TIMESTAMP d, h, m, s;

	d=seconds/(24*60*60);
	seconds=seconds%(24*60*60);
	h=seconds/(60*60);
	seconds=seconds%(60*60);
	m=seconds/60;
	s=seconds%60;
	if (d>0)
		sprintf(buffer,"%ld/%.02ld:%.02ld:%.02ld",d,h,m,s);
	else
		sprintf(buffer,"%.02ld:%.02ld:%.02ld",h,m,s);
	return(buffer);
	}

/*
**	mustswap: returns true of bytes are swapped. This can help
**	with programs that depend on byte order and big/little
**	endianess. This code was swiped from the nearly identical
**	ipcmustswap() in libipc, which in turn swiped the idea from
**	X11 (which probably got it from somewhere lost in the mists
**	of time). In general you want to use the canned routines
**	[hn]to[nh][sl](3), but if you gotta swap a buncho stuff, this
**	could be handy.
*/
int
mustswap(void)
	{
	static unsigned long mustswap=1; /* thread safe: read-only */
	return(*(char *)&mustswap);
	}

/*
**	memorybreak: this is a wrapper around the sbrk(2) system call.
**	It is almost guaranteed not to work for the following reasons:
**
**	o	sbrk(2) isn't portable, and isn't part of the POSIX
**		IEEE 1003.1 standard (it is however widely
**		implemented).
**
**	o	The only possible way this might ever work is to
**		call it twice and subtract the results. This amounts
**		to pointer arithmetic which is nearly always a Bad
**		Thing(tm).
**
**	o	It also does some machinations to try to infer
**		something about the host machine architecture and
**		how addressing is done. This inference is almost
**		sure to be wrong.
**
**	o	It depends on sbrk(2) always expanding memory by
**		an integral number of words.
**
**	Why is it here? Because I haven't yet figured out any other
**	way to determine memory usage.
*/
MEMORYBREAK
memorybreak(void)
	{
#ifdef MEMORYBREAK_SBRKCHAR
	extern char *sbrk();
#endif /* MEMORYBREAK_SBRKCHAR */
#ifdef MEMORYBREAK_SBRKVOID
	extern void *sbrk();
#endif /* MEMORYBREAK_SBRKVOID */
	static char word[sizeof(int)+1]; /* thread safe: read-only */
	MEMORYBREAK mult;
	char *addr;

	mult=sizeof(int)/(((long)&word[sizeof(int)])-((long)&word[0]));
	P(sem_memorybreak_libt);
	addr=(char *)sbrk(0);
	V(sem_memorybreak_libt);
	return(mult*(MEMORYBREAK)addr);
	}

/*
**	cpuuseconds: return a double containing the elapsed CPU
**	time in microseconds, and if an array is provided, return
**	four long words containing user seconds, user microseconds,
**	system seconds, system microseconds.
*/
#define USEC	(1000000)
double
cpuuseconds(TIMESTAMP vec[4])
	{
	double total;
	TIMESTAMP usrs, usru, syss, sysu;
#ifdef CPUUSECONDS_GETRUSAGE
	struct rusage cputime;
#else /* CPUUSECONDS_GETRUSAGE */
#ifdef CPUUSECONDS_TIMES
	struct tms cputime;
#endif /* CPUUSECONDS_TIMES */
#endif /* CPUUSECONDS_GETRUSAGE */

#ifdef CPUUSECONDS_GETRUSAGE
	if (getrusage(RUSAGE_SELF,&cputime)<0)
		return(-1.0);
	usrs=cputime.ru_utime.tv_sec;
	usru=cputime.ru_utime.tv_usec;
	syss=cputime.ru_stime.tv_sec;
	sysu=cputime.ru_stime.tv_usec;
#else  /* CPUSECONDS_GETRUSAGE */
#ifdef CPUUSECONDS_TIMES
	if (times(&cputime)<0)
		return(-1.0);
	usrs=cputime.tms_utime/HZ;
	usru=((cputime.tms_utime-(usrs*HZ))*USEC)/HZ;
	syss=cputime.tms_stime/HZ;
	sysu=((cputime.tms_stime-(syss*HZ))*USEC)/HZ;
#else /* CPUUSECONDS_TIMES */
#ifdef CPUUSECONDS_CLOCK
	usru=clock();
	usrs=0;
	sysu=0;
	syss=0;
#else /* CPUUSECONDS_CLOCK */
	usru=0;
	usrs=0;
	sysu=0;
	syss=0;
#endif /* CPUUSECONDS_CLOCK */
#endif /* CPUUSECONDS_TIMES */
#endif /* CPUUSECONDS_GETRUSAGE */

	total=(double)usru+(double)sysu+
		((double)usrs*(double)USEC)+((double)syss*(double)USEC);
	if (vec!=(TIMESTAMP *)0)
		{
		vec[0]=usrs;
		vec[1]=usru;
		vec[2]=syss;
		vec[3]=sysu;
		}

	return(total);
	}

/*
**	cpuseconds: returns the number of CPU seconds used.
*/
TIMESTAMP
cpuseconds(void)
	{
	return((TIMESTAMP)((cpuuseconds(NULL)/(double)USEC)+0.5));
	}
