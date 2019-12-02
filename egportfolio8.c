/*
** @(#)egportfolio8.c	4.2 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "portfolio.h"
#include "buffers.h"
#include "nmalloc.h"
#include "keyword.h"
#include "argvector.h"
#include "package.h"
#include "packet.h"
#include "parmfile.h"
#include "pile.h"

main(argc,argv)
int argc;
char **argv;
	{
	char *key, *val;
	PORTFOLIO box, crate, poke;

	initportfolio(&box);
	initportfolio(&crate);
	initportfolio(&poke);

	(void)stuffparmfile(0,&box);
	fprintf(stderr,"original:\n");
	fflush(stderr);
	(void)unstuffparmfile(2,&box);
	(void)restuff(&box,&poke);

	for (argv++; argc>1; argc--)
		{
		val=keyword(key=*(argv++));
		(void)stuff(key,val,&crate);
		}
	fprintf(stderr,"updates:\n");
	fflush(stderr);
	(void)unstuffparmfile(2,&crate);

	fprintf(stderr,"restuff:\n");
	fflush(stderr);
	(void)restuff(&crate,&box);
	(void)unstuffparmfile(2,&box);

	fprintf(stderr,"enstuff:\n");
	fflush(stderr);
	(void)enstuff(&crate,&poke);
	(void)unstuffparmfile(2,&poke);

	(void)freeportfolio(&box);
	(void)freeportfolio(&crate);
	(void)freeportfolio(&poke);

	bufpool(stdout);
	skvpool(stdout);
	sklpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
