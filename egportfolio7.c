/*
** @(#)egportfolio7.c	4.3 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "portfolio.h"
#include "buffers.h"
#include "nmalloc.h"
#include "argvector.h"
#include "argvector.h"
#include "package.h"
#include "packet.h"
#include "parmfile.h"
#include "pile.h"

main(argc,argv,env)
int argc;
char **argv;
char **env;
	{
	PORTFOLIO args, envs;
	char **vector, **vp;

	argc--;
	argv++;
	initportfolio(&args);
	if (stuffvector(argc,argv,&args)==NULL)
		perror("stuffvector argv");
	printf("args: %d\n",countportfolio(&args));
	fflush(stdout);
	(void)unstuffparmfile(1,&args);

	initportfolio(&envs);
	if (stuffvector(99999,env,&envs)==NULL)
		perror("stuffvector env");
	printf("envs: %d\n",countportfolio(&envs));
	fflush(stdout);
	(void)unstuffparmfile(1,&envs);

	printf("vecs:\n");
	for (vector=vp=unstuffvector(&args); (vp!=NULL)&&(*vp!=NULL); vp++)
		puts(*vp);
	fflush(stdout);

	(void)freevector(vector);
	(void)freeportfolio(&args);
	(void)freeportfolio(&envs);

	bufpool(stdout);
	skvpool(stdout);
	sklpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
