/* @(#)egbuffers.c	4.2 94/05/18 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "buffers.h"
#include "nmalloc.h"

main(argc,argv)
int argc;
char **argv;
        {
        int n, i, l, m;
	BUFFER c;
	char *s;

	nmalloclog(stdout);

	if (argc<2)
		{
		fprintf(stderr,"usage: %s count\n",argv[0]);
		exit(1);
		}

	m=atoi(argv[1]);

        for (n=0; n<m; n++)
                {
		if (n)
			{
			c=bufrealloc(c,n);
			s="bufrealloc";
			}
		else
			{
			c=bufalloc(n);
			s="bufalloc";
			}
                i=bufhash(n);
		l=buflength(c);
		printf("%s: string=0x%lx, request=%d, actual=%d, index=%d\n",
			s,c,n,l,i);
                }

	bufpool(stdout);
	nmallocinfo(stdout);

	exit(0);
        }
