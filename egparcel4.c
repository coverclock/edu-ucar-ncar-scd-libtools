/*
** @(#)egparcel4.c	4.15 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "parcels.h"
#include "buffers.h"
#include "nmalloc.h"

main()
	{
	PARCEL box;
	int n1, n2;
	char *s1, *s2, *s3;
	char buffer[512];

	initparcel(&box);
	s1="John";
	s2="Sloan";
	n1=8292;
	n2=0;
	if (bundlef("KEYWORD",&box,buffer,"%s %s %d",s1,s2,n1)==NULL)
		perror("bundlef"), exit(1);
	if ((s3=unbundlef("KEYWORD",box,"%*s%*s%d",(void *)&n2))==NULL)
		perror("unbundlef1"), exit(2);
	printf("``%s''=%d\n",s3,n2);
	if ((s3=unbundlef("FOOBAR",box,"%*s%*s%d",(void *)&n2))==NULL)
		perror("unbundlef2");
	if ((s3=unbundlef("KEYWORD",box,"%d",(void *)&n2))==NULL)
		perror("unbundlef3");
	(void)freeparcel(box);

	bufpool(stdout);
	kvpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
