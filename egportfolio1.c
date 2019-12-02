/*
** @(#)egportfolio1.c	4.3 96/05/24 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "skiplists.h"
#include "skeyval.h"
#include "portfolio.h"
#include "argvector.h"
#include "package.h"
#include "packet.h"
#include "parmfile.h"
#include "pile.h"
#include "buffers.h"
#include "maybenull.h"
#include "nmalloc.h"

main(argc,argv)
int argc;
char **argv;
	{
	PORTFOLIO box, crate, container;
	PACKAGE *package, *string;
	PILE thing;
	char **p;

	initportfolio(&box);
	printf("unstuff D=``%s''\n",maybenull(unstuff("D",&box)));
	string="A=1 B=2 C=3";
	stuffpackage(string,&box);
	printf("unstuff A=``%s''\n",maybenull(unstuff("A",&box)));
	printf("unstuff B=``%s''\n",maybenull(unstuff("B",&box)));
	printf("unstuff C=``%s''\n",maybenull(unstuff("C",&box)));
	printf("unstuff D=``%s''\n",maybenull(unstuff("D",&box)));
	stuff("D","4",&box);
	printf("unstuff D=``%s''\n",maybenull(unstuff("D",&box)));
	stuff("E","#\\\"",&box);
	stuff("F","Parm Eff",&box);
	stuff("B","bee",&box);
	printf("unstuffpackage=``%s'' (%d)\n",maybenull(package=unstuffpackage(&box)),countportfolio(&box));
	printf("unstuff B=``%s''\n",maybenull(unstuff("B",&box)));
	printf("unstuff B=``%s''\n",maybenull(unstuff("B",&box)));
	printf("unstuff F=``%s''\n",maybenull(unstuff("E",&box)));
	printf("unstuff F=``%s''\n",maybenull(unstuff("F",&box)));
	(void)freepackage(package);

	initportfolio(&crate);
	stuffpackage("ALPHA=a BETA=b GAMMA=c DELTA=d",&crate);
	restuff(&crate,&box);
	printf("merged ``%s''\n",package=unstuffpackage(&box));
	(void)freepackage(package);

	initportfolio(&container);
	printf("box=%d container=%d\n",
		countportfolio(&box),countportfolio(&container));
	thing=unstuffpile(&box);
	for (p=thing; *p; p+=2)
		printf("0x%lx: %s=%s\n",p,*p,*(p+1));
	(void)stuffpile(thing,&container);
	printf("stuffpiled ``%s''\n",package=unstuffpackage(&container));
	(void)freepackage(package);
	(void)freepile(thing);

	(void)freeportfolio(&box);
	(void)freeportfolio(&crate);
	(void)freeportfolio(&container);

	bufpool(stdout);
	skvpool(stdout);
	sklpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
