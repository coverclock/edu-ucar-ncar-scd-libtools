/*
**	@(#)egnextparser.c	4.2 94/05/18 jsloan@ncar.ucar.edu
**
**	Test nextfromfile() and nextfrombuffer()
*/

#include <stdio.h>
#include <errno.h>
#include "buffers.h"
#include "nmalloc.h"
#include "nextparser.h"

main(argc,argv)
int argc;
char **argv;
	{
	int rc, size, length;
	BUFFER buffer;
	char *next;

	argc--;
	argv++;
	if (argc<1)
		{
		fprintf(stderr,"usage: egnextparser bufsize\n");
		exit(1);
		}

	argc--;
	size=atoi(*(argv++));
	printf("egnextparser: size=%d\n",size);
	if (size<=0)
		exit(2);

	if ((buffer=bufalloc(size+1))==NULL)
		{
		perror("bufalloc");
		exit(3);
		}
	length=buflength(buffer);
	length--;
	*(buffer+length)='\0';
	printf("egnextparser: length=%d\n",length);

	for (; argc>0; argc--, argv++)
		{
		printf("egnextparser: argv=``%s''\n",*argv);
		next=*argv;
		while ((next=nextfrombuffer(next,buffer,length))!=NULL)
			printf("egnextparser: word=``%s''\n",buffer);
		}

	do
		{
		rc=nextfromfile(stdin,buffer,length);
		printf("%d: ``%s''\n",rc,rc>0?(char *)buffer:"");
		}
	while (rc>=0);

	bufpool(stdout);
        nmallocinfo(stdout);

	exit(0);
	}
