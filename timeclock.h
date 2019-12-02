/*
**	T I M E C L O C K
**
**	Copyright 1990-1996 University Corporation for Atmospheric Research
**	All Rights Reserved
**
**	@(#)timeclock.h	4.3 96/06/18 jsloan@ncar.ucar.edu
**
*/

#ifndef H_TIMECLOCK
#define H_TIMECLOCK

#include "libtproto.h"

struct hightimestamp { unsigned long t_sec; unsigned long t_nsec; };

typedef unsigned long TIMESTAMP;
typedef struct hightimestamp HIGHTIMESTAMP;
typedef unsigned long MEMORYBREAK;

extern TIMESTAMP timeclock LIBTPROTO((void));
extern HIGHTIMESTAMP *hightimeclock LIBTPROTO((HIGHTIMESTAMP *t));

extern char * timeformat LIBTPROTO((char *buffer, int bufsize, char *format, TIMESTAMP seconds, int utc));

extern char * timeelapsed LIBTPROTO((char *buffer, TIMESTAMP seconds));

extern TIMESTAMP cpuseconds LIBTPROTO((void));
extern double cpuuseconds LIBTPROTO((TIMESTAMP times[4]));

extern int mustswap LIBTPROTO((void));

extern MEMORYBREAK memorybreak LIBTPROTO((void));

/*
**	Here are some commonly used time formats...
**
**	CAREFUL!	SCCS steps on some of these guys if the percent
**			signs are not escaped. Some C compilers complain
**			about incorrect escape sequence and then go
**			ahead and remove the backslash (which is what
**			you want).
*/

#define	TF_YMDHMS	"%y/%m/%d %H:%M:%S"
#define TF_HMSYMD	"%H:%M:%S %y/%m/%d"
#define TF_MDYHMS	"%m/%d/%y %H:%M:%S"
#define TF_HMS		"%H\%M\%S"			/* SCCS likes H */
#define TF_YJ		"%y\%j"
#define TF_YMD		"%Y\%m\%d"			/* SCCS likes Y */
#define TF_DATE		"%a %b %e %T %Z %Y"

/*
**	and some time formatting macros (note the buffer sizes)...
*/

#define tfymdhms(b,t)	timeformat(b,18,TF_YMDHMS,t,0)
#define tfhmsymd(b,t)	timeformat(b,18,TF_HMSYMD,t,0)
#define tfmdyhms(b,t)	timeformat(b,18,TF_MDYHMS,t,0)
#define tfhms(b,t)	timeformat(b,7,TF_HMS,t,0)
#define tfyj(b,t)	timeformat(b,6,TF_YJ,t,0)
#define tfymd(b,t)	timeformat(b,9,TF_YMD,t,0)
#define tfdate(b,t)	timeformat(b,29,TF_DATE,t,0)

/*
**	and finally some time stamping macros.
*/

#define tsymdhms(b)	tfymdhms(b,timeclock())
#define tshmsymd(b)	tfhmsymd(b,timeclock())
#define tsmdyhms(b)	tfmdyhms(b,timeclock())
#define tshms(b)	tfhms(b,timeclock())
#define tsyj(b)		tfyj(b,timeclock())
#define tsymd(b)	tfymd(b,timeclock())
#define tsdate(b)	tfdate(b,timeclock())

#endif /* !H_TIMECLOCK */
