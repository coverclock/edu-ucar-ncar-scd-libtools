/*
**	@(#)egnextfrompath.c	4.2 94/05/18 jsloan@ncar.ucar.edu
**
**	Test nextfrompath()
*/

#include <stdio.h>
#include <errno.h>
#include "buffers.h"
#include "nmalloc.h"
#include "nextfrompath.h"

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
		fprintf(stderr,"usage: egnextfrompath bufsize\n");
		exit(1);
		}

	argc--;
	size=atoi(*(argv++));
	printf("egnextfrompath: size=%d\n",size);
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
	printf("egnextfrompath: length=%d\n",length);

	for (; argc>0; argc--, argv++)
		{
		printf("egnextfrompath: argv=``%s''\n",*argv);
		next=*argv;
		while ((next=nextfrompath(next,buffer,length))!=NULL)
			printf("egnextfrompath: path=``%s''\n",buffer);
		}

	bufpool(stdout);
        nmallocinfo(stdout);

	exit(0);
	}
