/*
** @(#)egportfolio3.c	4.2 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "libttypes.h"
#include "libtvalues.h"
#include "portfolio.h"
#include "nmalloc.h"
#include "argvector.h"
#include "package.h"
#include "packet.h"
#include "parmfile.h"
#include "pile.h"

static void
makeportfolio(box,size)
PORTFOLIO *box;
int size;
	{
	int ndx, range, number;
	double drange, pcent;
	char name[32];

	initportfolio(box);
	for (ndx=size; ndx>0; ndx--)
		for (;;)
			{
			range=rand();
			drange=range;
			pcent=drange/((double)RAND_MAX);
			number=(size*100)*pcent;
#ifdef DEBUG
			printf("r=%d d=%lg p=%lg s=%d n=%d\n",
				range,drange,pcent,size,number);
#endif
			sprintf(name,"%d",number);
			if (unstuff(name,box)==NULL)
				{
				(void)stuff(name,name,box);
				break;
				}
			}
	}

main(argc,argv)
int argc;
char **argv;
	{
	PORTFOLIO one, two, three;
	int silent;

	if (argc!=4)
		fprintf(stderr,"usage: %s silent size1 size2\n",argv[0]), exit(1);

	silent=atoi(argv[1]);

	makeportfolio(&one,atoi(argv[2]));
	printf("Portfolio One: %d\n",countportfolio(&one));
	fflush(stdout);
	if (!silent)
		unstuffparmfile(1,&one);

	makeportfolio(&two,atoi(argv[3]));
	printf("Portfolio Two: %d\n",countportfolio(&two));
	fflush(stdout);
	if (!silent)
		unstuffparmfile(1,&two);

	initportfolio(&three);
	restuff(&one,&three);
	restuff(&two,&three);
	printf("Portfolio Three: %d\n",countportfolio(&three));
	fflush(stdout);
	if (!silent)
		unstuffparmfile(1,&three);

	(void)freeportfolio(&one);
	(void)freeportfolio(&two);
	(void)freeportfolio(&three);

	if (!silent)
		{
		bufpool(stdout);
		skvpool(stdout);
		sklpool(stdout);
		nmallocinfo(stdout);
		}

	bufshutdown();
	skvshutdown();

	if (!silent)
		{
		bufpool(stdout);
		skvpool(stdout);
		sklpool(stdout);
		nmallocinfo(stdout);
		}

	exit(0);
	}
