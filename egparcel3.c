/*
** @(#)egparcel3.c	4.20 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "libttypes.h"
#include "libtvalues.h"
#include "parcels.h"
#include "nmalloc.h"

static PARCEL
makeparcel(size)
int size;
	{
	PARCEL box;
	int ndx, range, number;
	double drange, pcent;
	char name[32];

	initparcel(&box);
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
			if (unbundle(name,box)==NULL)
				{
				(void)bundle(name,name,&box);
				break;
				}
			}
	return(box);
	}

main(argc,argv)
int argc;
char **argv;
	{
	PARCEL one, two, three;
	int silent;

	if (argc!=4)
		fprintf(stderr,"usage: %s silent size1 size2\n",argv[0]), exit(1);

	silent=atoi(argv[1]);

	one=makeparcel(atoi(argv[2]));
	printf("Parcel One: %d\n",countparcel(one));
	fflush(stdout);
	if (!silent)
		unbundleparmfile(1,one);

	two=makeparcel(atoi(argv[3]));
	printf("Parcel Two: %d\n",countparcel(two));
	fflush(stdout);
	if (!silent)
		unbundleparmfile(1,two);

	initparcel(&three);
	rebundle(one,&three);
	rebundle(two,&three);
	printf("Parcel Three: %d\n",countparcel(three));
	fflush(stdout);
	if (!silent)
		unbundleparmfile(1,three);

	(void)freeparcel(one);
	(void)freeparcel(two);
	(void)freeparcel(three);

	if (!silent)
		{
		bufpool(stdout);
		kvpool(stdout);
		nmallocinfo(stdout);
		}

	bufshutdown();
	kvshutdown();

	if (!silent)
		{
		bufpool(stdout);
		kvpool(stdout);
		nmallocinfo(stdout);
		}

	exit(0);
	}
