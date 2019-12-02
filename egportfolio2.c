/*
** @(#)egportfolio2.c	4.2 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
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
	PORTFOLIO box, container;
	PACKAGE *package;

	initportfolio(&box);
	initportfolio(&container);

	stuff("PARM1","parm 1",&box);
	stuff("PARM2","parm 2",&box);
	stuff("PARM3","\"parm 3\"",&box);

	stuff("PARMS",package=unstuffpackage(&box),&box);
	(void)freepackage(package);
	printf("portfolio=``%s''\n",package=unstuffpackage(&box));
	(void)freepackage(package);
	printf("parms=``%s''\n",unstuff("PARMS",&box));

	(void)stuffpackage(unstuff("PARMS",&box),&container);

	printf("parm1=``%s''\n",unstuff("PARM1",&container));
	printf("parm2=``%s''\n",unstuff("PARM2",&container));
	printf("parm3=``%s''\n",unstuff("PARM3",&container));

	(void)freeportfolio(&box);
	(void)freeportfolio(&container);

	bufpool(stdout);
	skvpool(stdout);
	sklpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
