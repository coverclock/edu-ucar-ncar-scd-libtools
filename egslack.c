/*
** @(#)egslack.c	4.3 96/06/11 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "slack.h"

static void
try(n1,s1,n2,s2)
char *n1;
int s1;
char *n2;
int s2;
	{
	printf("sizeof(%s)=%d sizeof(%s)=%d slack(%d,%d)=%d\n",
		n1,s1,n2,s2,s1,s2,SLACK(s1,s2));
	}

main(argc,argv)
int argc;
char **argv;
	{
	static char *names[]={"char","short","int","long","double"};
	static int sizes[]={sizeof(char),sizeof(short),sizeof(int),
			sizeof(long),sizeof(double)};
	int i, j;

	for (i=(sizeof(sizes)/sizeof(int))-1; i>=0; i--)
		for (j=(sizeof(sizes)/sizeof(int))-1; j>=0; j--)
			try(names[i],sizes[i],names[j],sizes[j]);
	}
