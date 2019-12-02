/*
** @(#)egparcel6.c	4.14 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "parcels.h"
#include "buffers.h"
#include "nmalloc.h"

main(argc,argv)
int argc;
char **argv;
	{
	PARCEL box;

	initparcel(&box);
	fprintf(stderr,"import=%d\n",bundleparmfile(0,&box));
	fprintf(stderr,"export=%d\n",unbundleparmfile(1,box));
	(void)freeparcel(box);

	bufpool(stdout);
	kvpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
