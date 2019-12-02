/* @(#)egtimeclock.c	4.6 96/06/19 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include <errno.h>
#include "libtvalues.h"
#include "nmalloc.h"
#include "timeclock.h"

#define SIZE	(1024*64)

main()
	{
	char *buffer;
	TIMESTAMP now, then, cpu, low, usecs[4];
	HIGHTIMESTAMP high, *h1, *h2;
	long before, after, n;
	double usec;

	then=cpuseconds();
	before=memorybreak();

	low=timeclock();
	h1=hightimeclock((HIGHTIMESTAMP *)0);
	h2=hightimeclock(&high);
	printf("low=%d sec\n",low);
	printf("high1=%d sec, %d nsec\n",h1->t_sec,h1->t_nsec);
	printf("high2=%d sec, %d nsec\n",h2->t_sec,h2->t_nsec);

	if ((buffer=namalloc(SIZE))==NULL)
		{
		perror("namalloc");
		exit(1);
		}

	printf("tsymdhms=%s\n",tsymdhms(buffer));
	printf("tshmsymd=%s\n",tshmsymd(buffer));
	printf("tsmdyhms=%s\n",tsmdyhms(buffer));
	printf("tshms=%s\n",tshms(buffer));
	printf("tsyj=%s\n",tsyj(buffer));
	printf("tsymd=%s\n",tsymd(buffer));
	printf("tsdate=%s\n",tsdate(buffer));
	printf("timeelapsed=%s\n",timeelapsed(buffer,timeclock()));

	after=memorybreak();
	printf("memory(%d)=%d\n",SIZE,after-before);

	printf("bytes %s swapped\n",mustswap()?"are":"are not");

	n=0;
	now=1;
	do
		{
		now=now*2;
		usec=cpuuseconds(usecs);
		n++;
		}
	while (usecs[2]<1||usec<60000000.0);
	now=cpuseconds();
	cpu=now-then;
	usec=cpuuseconds(usecs);
	printf("cpu(%ld)=%ld-%ld=%ld=%s=%lfusec=[%ld+%ldusec+%ld+%ldusec]\n",
		n,now,then,cpu,timeelapsed(buffer,cpu),usec,
		usecs[0],usecs[1],usecs[2],usecs[3]);

	exit(0);
	}
