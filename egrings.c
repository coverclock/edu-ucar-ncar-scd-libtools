/* @(#)egrings.c	4.2 94/05/18 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "errors.h"
#include "nmalloc.h"
#include "rings.h"

main(argc,argv)
int argc;
char **argv;
	{
	int rc, n, i;
	char ci, co;
	RING buffer;

	if (argc<2)
		fprintf(stderr,"usage: %s bufsize\n",argv[0]), exit(1);

	n=atoi(argv[1]);
	printf("%s: bufsize=%d\n",argv[0],n);

	if (rinit(&buffer,n/2)==NULL)
		perror("rinit"), exit(2);
	if (rreinit(&buffer,n)==NULL)
		perror("rreinit"), exit(3);

	while ((i=getchar())!=EOF)
		{
		ci=i;
		if ((rc=rinsert(&buffer,ci))<0)
			perror("rinsert"), exit(4);
		else if (rc==0)
			{
			while ((rc=rpeek(&buffer,NULL))>0)
				{
				if ((rc=rextract(&buffer,&co))<0)
					perror("rextract"), exit(5);
				else if (rc==0)
					fprintf(stderr,"rextract: hosed\n"), exit(6);
				(void)putchar(co);
				}
			if (rc<0)
				perror("rpeek"), exit(7);
			if ((rc=rinsert(&buffer,ci))<0)
				perror("rinsert"), exit(8);
			else if (rc==0)
				fprintf(stderr,"rinsert: hosed\n"), exit(9);
			}
		}

	while ((rc=rpeek(&buffer,NULL))>0)
		{
		if ((rc=rextract(&buffer,&co))<0)
			perror("rextract"), exit(10);
		else if (rc==0)
			fprintf(stderr,"rextract: hosed\n"), exit(11);
		(void)putchar(co);
		}

	nmallocinfo(stdout);

	exit(0);
	}
