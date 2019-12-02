/* @(#)egnmalloc.c	4.2 94/05/18 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "errors.h"
#include "nmalloc.h"

main(argc,argv)
int argc;
char **argv;
        {
        int cnt;
	void *p, **t;
	int n, l, b;

	if (argc!=2)
		{
		fprintf(stderr,"usage: %s count\n",argv[0]);
		exit(1);
		}

	cnt=atoi(argv[1]);
	printf("%s %d\n",argv[0],cnt);

	if (cnt<=0)
		{
		fprintf(stderr,"usage: %s count\n",argv[0]);
		exit(2);
		}

	(void)nmalloclog(stdout);

	l=sizeof(void *)*cnt;
	if ((t=(void **)nmalloc(l))==NULL)
		{
		perror("nmalloc table FAILED");
		exit(3);
		}

	printf("%s: nmalloc[table]=0x%x[%d]\n",argv[0],t,l);

	nmallocinfo(stdout);
	nmallocpool(stdout);

	for (n=0; n<cnt; n++)
		{
		b=n+1;
		if ((t[n]=(void *)nmalloc(b))==NULL)
			perror("nmalloc FAILED");
		}

	nmallocinfo(stdout);
	nmallocpool(stdout);

	for (n=0; n<cnt; n++)
		{
		b=(n+1)*2;
		if ((t[n]=(void *)nrealloc(t[n],b))==NULL)
			perror("nrealloc FAILED");
		}

	nmallocinfo(stdout);
	nmallocpool(stdout);

	for (n=0; n<cnt; n++)
		nfree((void *)t[n]);

	nmallocinfo(stdout);
	nmallocpool(stdout);

	for (n=0; n<cnt; n++)
		{
		b=n+1;
		if ((t[n]=namalloc(b))==NULL)
			perror("namalloc FAILED");
		}

	nmallocinfo(stdout);
	nmallocpool(stdout);

	for (n=0; n<cnt; n++)
		{
		b=(n+1)*2;
		if ((t[n]=narealloc(t[n],b))==NULL)
			perror("narealloc FAILED");
		}

	nmallocinfo(stdout);
	nmallocpool(stdout);

	for (n=0; n<cnt; n++)
		nfree((void *)t[n]);

	nfree((void *)t);

	nmallocinfo(stdout);
	nmallocpool(stdout);

	exit(0);
        }
