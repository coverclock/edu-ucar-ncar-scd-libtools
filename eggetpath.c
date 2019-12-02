/* @(#)eggetpath.c	4.6 94/05/18 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "getpath.h"

main()
	{
	extern char *getenv();
	char path[512];

	printf("%s\n",getpath(path,NULL,"%s/%s","dir","file"));
	printf("%s\n",getpath(path,NULL,"/%s/%s","dir","file"));
	printf("%s\n",getpath(path,NULL,"%s/%s","/dir","file"));
	printf("%s\n",getpath(path,"/root","%s/%s","dir","file"));
	printf("%s\n",getpath(path,"/root","/%s/%s","dir","file"));
	printf("%s\n",getpath(path,"/root","%s/%s","/dir","file"));
	printf("%s\n",getpath(path,"","%s/%s","dir","file"));
	printf("%s\n",getpath(path,"","/%s/%s","dir","file"));
	printf("%s\n",getpath(path,"","%s/%s","/dir","file"));
	printf("%s\n",getpath(path,getenv("HOME"),"%s/%s","dir","file"));
	printf("%s\n",getpath(path,getenv("HOME"),"/%s/%s","dir","file"));
	printf("%s\n",getpath(path,getenv("HOME"),"%s/%s","/dir","file"));

	exit(0);
	}
