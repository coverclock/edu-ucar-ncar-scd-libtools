/*
**	@(#)egatoargv.c	4.2 94/05/18 jsloan@ncar.ucar.edu
**
**	Test atoargv()
*/

#include <stdio.h>
#include <errno.h>
#include "buffers.h"
#include "nmalloc.h"
#include "atoargv.h"

main(argc,argv)
int argc;
char **argv;
	{
	char **nargv, **na;

	if (argc!=2)
		{
		fprintf(stderr,"usage: egatoargv string\n");
		exit(1);
		}

	printf("egatoargv: %s %s\n",argv[0],argv[1]);

	nargv=atoargv(argv[0],argv[1]);
	for (na=nargv; *na!=NULL; na++)
		printf("egatoargv: %s nargv[%d]=``%s''\n",
			argv[0],(na-nargv),*na);
	freeargv(nargv);

	nargv=atoargv(NULL,argv[1]);
	for (na=nargv; *na!=NULL; na++)
		printf("egatoargv: %s nargv[%d]=``%s''\n",
			"NULL",(na-nargv),*na);
	freeargv(nargv);

	bufpool(stdout);
        nmallocinfo(stdout);

	exit(0);
	}
