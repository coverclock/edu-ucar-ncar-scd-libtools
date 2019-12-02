/*
** @(#)egparcel2.c	4.14 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "parcels.h"
#include "buffers.h"
#include "nmalloc.h"

main()
	{
	PARCEL box, container;
	PACKAGE *package;

	initparcel(&box);
	initparcel(&container);

	bundle("PARM1","parm 1",&box);
	bundle("PARM2","parm 2",&box);
	bundle("PARM3","\"parm 3\"",&box);

	bundle("PARMS",package=unbundlepackage(box),&box);
	(void)freepackage(package);
	printf("parcel=``%s''\n",package=unbundlepackage(box));
	(void)freepackage(package);
	printf("parms=``%s''\n",unbundle("PARMS",box));

	(void)bundlepackage(unbundle("PARMS",box),&container);

	printf("parm1=``%s''\n",unbundle("PARM1",container));
	printf("parm2=``%s''\n",unbundle("PARM2",container));
	printf("parm3=``%s''\n",unbundle("PARM3",container));

	(void)freeparcel(box);
	(void)freeparcel(container);

	bufpool(stdout);
	kvpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
