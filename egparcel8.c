/*
** @(#)egparcel8.c	4.2 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "parcels.h"
#include "buffers.h"
#include "nmalloc.h"
#include "keyword.h"

main(argc,argv)
int argc;
char **argv;
	{
	char *key, *val;
	PARCEL box, crate, poke;

	initparcel(&box);
	initparcel(&crate);
	initparcel(&poke);

	(void)bundleparmfile(0,&box);
	fprintf(stderr,"original:\n");
	fflush(stderr);
	(void)unbundleparmfile(2,box);
	(void)rebundle(box,&poke);

	for (argv++; argc>1; argc--)
		{
		val=keyword(key=*(argv++));
		(void)bundle(key,val,&crate);
		}
	fprintf(stderr,"updates:\n");
	fflush(stderr);
	(void)unbundleparmfile(2,crate);

	fprintf(stderr,"rebundle:\n");
	fflush(stderr);
	(void)rebundle(crate,&box);
	(void)unbundleparmfile(2,box);

	fprintf(stderr,"enbundle:\n");
	fflush(stderr);
	(void)enbundle(crate,&poke);
	(void)unbundleparmfile(2,poke);

	(void)freeparcel(box);
	(void)freeparcel(crate);
	(void)freeparcel(poke);

	bufpool(stdout);
	kvpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
