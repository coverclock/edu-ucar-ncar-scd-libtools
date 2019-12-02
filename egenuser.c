/* @(#)egenuser.c	4.2 94/05/18 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "libttypes.h"

main(argc,argv)
int argc;
char **argv;
	{
	int uid, gid, rc;

	if (argc!=3)
		{
		fprintf(stderr,"usage: %s uid gid\n",argv[0]);
		exit(1);
		}

	uid=atoi(argv[1]);
	gid=atoi(argv[2]);

	printf("%s: ruid=%d rgid=%d euid=%d egid=%d\n",
		argv[0],getuid(),getgid(),geteuid(),geteuid());

	printf("%s: enuser(%d,%d)=%d\n",argv[0],uid,gid,enuser(uid,gid));

	printf("%s: ruid=%d rgid=%d euid=%d egid=%d\n",
		argv[0],getuid(),getgid(),geteuid(),geteuid());

	printf("%s: deuser()=%d\n",argv[0],deuser());

	printf("%s: ruid=%d rgid=%d euid=%d egid=%d\n",
		argv[0],getuid(),getgid(),geteuid(),geteuid());

	exit(0);
	}
