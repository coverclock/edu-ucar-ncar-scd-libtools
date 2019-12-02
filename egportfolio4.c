/*
** @(#)egportfolio4.c	4.2 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "portfolio.h"
#include "buffers.h"
#include "nmalloc.h"
#include "argvector.h"
#include "package.h"
#include "packet.h"
#include "parmfile.h"
#include "pile.h"

main()
	{
	PORTFOLIO box;
	int n1, n2;
	char *s1, *s2, *s3;
	char buffer[512];

	initportfolio(&box);
	s1="John";
	s2="Sloan";
	n1=8292;
	n2=0;
	if (stufff("KEYWORD",&box,buffer,"%s %s %d",s1,s2,n1)==NULL)
		perror("stufff"), exit(1);
	if ((s3=unstufff("KEYWORD",&box,"%*s%*s%d",(void *)&n2))==NULL)
		perror("unstufff1"), exit(2);
	printf("``%s''=%d\n",s3,n2);
	if ((s3=unstufff("FOOBAR",&box,"%*s%*s%d",(void *)&n2))==NULL)
		perror("unstufff2");
	if ((s3=unstufff("KEYWORD",&box,"%d",(void *)&n2))==NULL)
		perror("unstufff3");
	(void)freeportfolio(&box);

	bufpool(stdout);
	skvpool(stdout);
	sklpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
