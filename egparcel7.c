/*
** @(#)egparcel7.c	4.3 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "parcels.h"
#include "buffers.h"
#include "nmalloc.h"
#include "argvector.h"

main(argc,argv,env)
int argc;
char **argv;
char **env;
	{
	PARCEL args, envs;
	char **vector, **vp;

	argc--;
	argv++;
	initparcel(&args);
	if (bundlevector(argc,argv,&args)==NULL)
		perror("bundlevector argv");
	printf("args: %d\n",countparcel(args));
	fflush(stdout);
	(void)unbundleparmfile(1,args);

	initparcel(&envs);
	if (bundlevector(99999,env,&envs)==NULL)
		perror("bundlevector env");
	printf("envs: %d\n",countparcel(envs));
	fflush(stdout);
	(void)unbundleparmfile(1,envs);

	printf("vec:\n");
	for (vector=vp=unbundlevector(args); (vp!=NULL)&&(*vp!=NULL); vp++)
		puts(*vp);
	fflush(stdout);

	(void)freevector(vector);
	(void)freeparcel(args);
	(void)freeparcel(envs);

	bufpool(stdout);
	kvpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
