/*
** @(#)egparcel1.c	4.21 96/05/24 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "trees.h"
#include "keyval.h"
#include "parcels.h"
#include "buffers.h"
#include "maybenull.h"
#include "nmalloc.h"

static void
cerrno(f)
void *f;
	{

	if (f==NULL)
		{
		printf("cerrno=NULL\n");
		if (errno!=0)
			perror("tparcel");
		}
	else
		printf("cerrno=okay\n");
	}

main(argc,argv)
int argc;
char **argv;
	{
	PARCEL box, crate, container;
	PACKAGE *package, *string;
	PILE stuff;
	char **p;

	initparcel(&box);
	printf("unbundle D=``%s''\n",maybenull(unbundle("D",box)));
	string="B=2 C=3 A=1";
	bundlepackage(string,&box);
	printf("unbundle A=``%s''\n",maybenull(unbundle("A",box)));
	printf("unbundle B=``%s''\n",maybenull(unbundle("B",box)));
	printf("unbundle C=``%s''\n",maybenull(unbundle("C",box)));
	printf("unbundle D=``%s''\n",maybenull(unbundle("D",box)));
	bundle("D","4",&box);
	printf("unbundle D=``%s''\n",maybenull(unbundle("D",box)));
	bundle("E","#\\\"",&box);
	bundle("F","Parm Eff",&box);
	bundle("B","bee",&box);
	printf("unbundlepackage=``%s'' (%d)\n",maybenull(package=unbundlepackage(box)),countparcel(box));
	printf("unbundle B=``%s''\n",maybenull(unbundle("B",box)));
	printf("unbundle B=``%s''\n",maybenull(unbundle("B",box)));
	printf("unbundle F=``%s''\n",maybenull(unbundle("E",box)));
	printf("unbundle F=``%s''\n",maybenull(unbundle("F",box)));
	(void)freepackage(package);

	initparcel(&crate);
	bundlepackage("BETA=b ALPHA=a DELTA=d GAMMA=c",&crate);
	rebundle(crate,&box);
	printf("merged ``%s''\n",package=unbundlepackage(box));
	(void)freepackage(package);

	initparcel(&container);
	printf("box=%d container=%d\n",
		countparcel(box),countparcel(container));
	stuff=unbundlepile(box);
	printf("unsorted\n");
	for (p=stuff; *p; p+=2)
		printf("0x%lx: %s=%s\n",p,*p,*(p+1));
	(void)sortpile(stuff);
	printf("sorted\n");
	for (p=stuff; *p; p+=2)
		printf("0x%lx: %s=%s\n",p,*p,*(p+1));
	(void)bundlepile(stuff,&container);
	printf("bundlepiled ``%s''\n",package=unbundlepackage(container));
	(void)freepackage(package);
	(void)freepile(stuff);

	cerrno((void *)bundlepackage(NULL,&box));
	cerrno((void *)unbundle(NULL,box));
	cerrno((void *)bundle(NULL,"2",&box));
	cerrno((void *)bundle("A",NULL,&box));

	(void)freeparcel(box);
	(void)freeparcel(crate);
	(void)freeparcel(container);

	package=unbundlepackage(NULL);
	cerrno((void *)package);
	(void)freepackage(package);

	cerrno((void *)initparcel(NULL));
	cerrno((void *)bundlepackage(string,NULL));
	cerrno((void *)bundlepackage(NULL,NULL));
	cerrno((void *)unbundle("A",NULL));
	cerrno((void *)unbundle(NULL,NULL));
	cerrno((void *)bundle("A","2",NULL));
	cerrno((void *)bundle(NULL,NULL,NULL));

	bufpool(stdout);
	kvpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
