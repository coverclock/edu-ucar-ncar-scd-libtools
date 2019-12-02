/*
**	@(#)egh2i.c	1.1 94/07/14 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "h2i.h"

#ifdef MVS_LIBTOOLS
#define TIMESTAMP	long
#define cpuseconds()	((TIMESTAMP)0)
#else /* MVS_LIBTOOLS */
#include "timeclock.h"
#endif /* MVS_LIBTOOLS */

main(argc,argv)
int argc;
char **argv;
	{
	int lim, ndx, rc;
	char *a0, *string;
	long lresult, test;
	short sresult;
	int iresult;
	TIMESTAMP b, a;

	if (argc!=3)
		{
		fprintf(stderr,"usage: %s count string\n",argv[0]);
		exit(1);
		}

	a0=argv[0];
	lim=atoi(argv[1]);
	string=argv[2];
	test=strtol(string,(char **)NULL,16);
	printf("%s %d ``%s''=%lx\n",a0,lim,string,test);

	if ((lresult=strtol(string,(char **)NULL,16))!=(long)test)
		fprintf(stderr,"strtol(16) %s==%lx!=%lx\n",
			string,lresult,(long)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)strtol(string,(char **)NULL,16);
		a=cpuseconds();
		printf("strtol(16)=%lx %d\n",lresult,a-b);
		}

	if ((lresult=strtol(string,(char **)NULL,0))!=(long)test)
		fprintf(stderr,"strtol(0) %s==%lx!=%lx\n",
			string,lresult,(long)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)strtol(string,(char **)NULL,0);
		a=cpuseconds();
		printf("strtol(0)=%lx %d\n",lresult,a-b);
		}

	if ((rc=sscanf(string,"%lx",&lresult))!=1)
		fprintf(stderr,"sscanf %s failed %d\n",string,rc);
	else if (lresult!=(long)test)
		fprintf(stderr,"scanf %s==%lx!=%lx\n",
			string,lresult,(long)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)sscanf(string,"%lx",&lresult);
		a=cpuseconds();
		printf("sscanf=%lx %d\n",lresult,a-b);
		}

	if ((rc=h2s(string,&sresult))!=1)
		fprintf(stderr,"h2s %s failed %d\n",string,rc);
	else if (sresult!=(short)test)
		fprintf(stderr,"h2s %s==%x!=%x\n",string,sresult,(short)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)h2s(string,&sresult);
		a=cpuseconds();
		printf("h2s=%x %d\n",sresult,a-b);
		}

	if ((rc=h2i(string,&iresult))!=1)
		fprintf(stderr,"h2i %s failed %d\n",string,rc);
	else if (iresult!=(int)test)
		fprintf(stderr,"h2i %s==%x!=%x\n",string,iresult,(int)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)h2i(string,&iresult);
		a=cpuseconds();
		printf("h2i=%x %d\n",iresult,a-b);
		}

	if ((rc=h2l(string,&lresult))!=1)
		fprintf(stderr,"h2l %s failed %d\n",string,rc);
	else if (lresult!=(long)test)
		fprintf(stderr,"h2l %s==%lx!=%lx\n",
			string,lresult,(long)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			h2l(string,&lresult);
		a=cpuseconds();
		printf("h2l=%x %d\n",lresult,a-b);
		}

	exit(0);
	}
