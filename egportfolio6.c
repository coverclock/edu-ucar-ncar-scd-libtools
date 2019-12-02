/*
** @(#)egportfolio6.c	4.2 94/05/18 jsloan@ncar.ucar.edu
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

main(argc,argv)
int argc;
char **argv;
	{
	PORTFOLIO box;

	initportfolio(&box);
	fprintf(stderr,"import=%d\n",stuffparmfile(0,&box));
	fprintf(stderr,"export=%d\n",unstuffparmfile(1,&box));
	(void)freeportfolio(&box);

	bufpool(stdout);
	skvpool(stdout);
	sklpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
