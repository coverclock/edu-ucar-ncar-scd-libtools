/*
**	@(#)egs2i.c	4.6 96/06/12 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "s2i.h"

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
	char *a0, *string, cresult;
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
	test=atol(string);
	printf("%s %d ``%s''=%ld=0x%lx\n",a0,lim,string,test,test);

	if ((lresult=atol(string))!=(long)test)
		fprintf(stderr,"atol %s==%ld!=%ld\n",
			string,lresult,(long)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)atol(string);
		a=cpuseconds();
		printf("atol=%ld %d\n",lresult,a-b);
		}

	if ((lresult=strtol(string,(char **)NULL,10))!=(long)test)
		fprintf(stderr,"strtol(10) %s==%ld!=%ld\n",
			string,lresult,(long)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)strtol(string,(char **)NULL,10);
		a=cpuseconds();
		printf("strtol(10)=%ld %d\n",lresult,a-b);
		}

	if ((lresult=strtol(string,(char **)NULL,0))!=(long)test)
		fprintf(stderr,"strtol(0) %s==%ld!=%ld\n",
			string,lresult,(long)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			strtol(string,(char **)NULL,0);
		a=cpuseconds();
		printf("strtol(0)=%ld %d\n",lresult,a-b);
		}

	if ((rc=sscanf(string,"%ld",&lresult))!=1)
		fprintf(stderr,"sscanf %s failed %d\n",string,rc);
	else if (lresult!=(long)test)
		fprintf(stderr,"scanf %s==%ld!=%ld\n",
			string,lresult,(long)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)sscanf(string,"%ld",&lresult);
		a=cpuseconds();
		printf("sscanf=%ld %d\n",lresult,a-b);
		}

	if ((rc=s2c(string,&cresult))!=1)
		fprintf(stderr,"s2c %s failed %d\n",string,rc);
	else if (cresult!=(char)test)
		fprintf(stderr,"s2c %s==%d!=%d\n",string,cresult,(char)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)s2c(string,&cresult);
		a=cpuseconds();
		printf("s2c=%d %d\n",cresult,a-b);
		}

	if ((rc=s2s(string,&sresult))!=1)
		fprintf(stderr,"s2s %s failed %d\n",string,rc);
	else if (sresult!=(short)test)
		fprintf(stderr,"s2s %s==%d!=%d\n",string,sresult,(short)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)s2s(string,&sresult);
		a=cpuseconds();
		printf("s2s=%d %d\n",sresult,a-b);
		}

	if ((rc=s2i(string,&iresult))!=1)
		fprintf(stderr,"s2i %s failed %d\n",string,rc);
	else if (iresult!=(int)test)
		fprintf(stderr,"s2i %s==%d!=%d\n",string,iresult,(int)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)s2i(string,&iresult);
		a=cpuseconds();
		printf("s2i=%d %d\n",iresult,a-b);
		}

	if ((rc=s2l(string,&lresult))!=1)
		fprintf(stderr,"s2l %s failed %d\n",string,rc);
	else if (lresult!=(long)test)
		fprintf(stderr,"s2l %s==%ld!=%ld\n",
			string,lresult,(long)test);
	else
		{
		b=cpuseconds();
		for (ndx=lim; ndx>0; ndx--)
			(void)s2l(string,&lresult);
		a=cpuseconds();
		printf("s2l=%d %d\n",lresult,a-b);
		}

	if (s2x(string,&cresult,sizeof(cresult))!=1)
		fprintf(stderr,"s2x %s sizeof %d failed %d\n",
			string,sizeof(cresult),rc);
	else if (cresult!=(char)test)
		fprintf(stderr,"s2x %s==%d!=%d\n",string,cresult,(char)test);
	else
		printf("s2x=%d sizeof=%d\n",cresult,sizeof(cresult));

	if (s2x(string,&sresult,sizeof(sresult))!=1)
		fprintf(stderr,"s2x %s sizeof %d failed %d\n",
			string,sizeof(sresult),rc);
	else if (sresult!=(short)test)
		fprintf(stderr,"s2x %s==%d!=%d\n",string,sresult,(short)test);
	else
		printf("s2x=%d sizeof=%d\n",sresult,sizeof(sresult));

	if (s2x(string,&iresult,sizeof(iresult))!=1)
		fprintf(stderr,"s2x %s sizeof %d failed %d\n",
			string,sizeof(iresult),rc);
	else if (iresult!=(int)test)
		fprintf(stderr,"s2x %s==%d!=%d\n",string,iresult,(int)test);
	else
		printf("s2x=%d sizeof=%d\n",iresult,sizeof(iresult));

	if (s2x(string,&lresult,sizeof(lresult))!=1)
		fprintf(stderr,"s2x %s sizeof %d failed %d\n",
			string,sizeof(lresult),rc);
	else if (lresult!=(long)test)
		fprintf(stderr,"s2x %s==%d!=%d\n",string,lresult,(long)test);
	else
		printf("s2x=%d sizeof=%d\n",lresult,sizeof(lresult));

	exit(0);
	}
