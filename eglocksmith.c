/*
** @(#)eglocksmith.c	4.3 94/05/19 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <fcntl.h>
#include "locksmith.h"

#define WRITEWAIT(fd,f)	Try("WRITEWAIT",(fd),LOCKSMITH_EXCLUSIVE,(f))
#define READWAIT(fd,f)	Try("READWAIT",(fd),LOCKSMITH_SHARED,(f))
#define UNLOCK(fd,f)	Try("UNLOCK",(fd),LOCKSMITH_UNLOCK,(f))
#define READTRY(fd,f)	Try("READTRY",(fd),LOCKSMITH_NSHARED,(f))
#define WRITETRY(fd,f)	Try("WRITETRY",(fd),LOCKSMITH_NEXCLUSIVE,(f))

static void
Try(name,fd,type,fail)
char *name;
int fd;
int type;
int fail;
	{
	long pid;
	int result;

	pid=getpid();
	fprintf(stderr,"locksmith %d: %s\n",pid,name);
	result=locksmith(fd,type);
	fprintf(stderr,
		"locksmith %d: %s %s %d (%s)\n",
		pid,name,
		result==0?"success":"failure",
		result,
		fail?(result==0?"unexpected":"expected"):
			(result==0?"expected":"unexpected"));
	}

static void
Wait(fd)
int fd;
	{
	char ch;

	if (read(fd,&ch,sizeof(ch))<=0)
		exit(10);
	}

static void
Post(fd)
int fd;
	{
	char ch;

	if (write(fd,&ch,sizeof(ch))<=0)
		exit(11);
	}

main(argc,argv)
int argc;
char **argv;
	{
	int fd, rc, pipeline[2], cw, cp, pw, pp;
	long pid;

	if (argc!=2)
		{
		fprintf(stderr,"usage: eglocksmith filename\n");
		exit(1);
		}

	fprintf(stderr,"eglocksmith: %s\n",argv[1]);

	if ((fd=open(argv[1],O_RDWR|O_CREAT))<0)
		{
		perror("open");
		exit(2);
		}

	if (pipe(pipeline)<0)
		{
		perror("pipe");
		exit(3);
		}
	cw=pipeline[0];
	pp=pipeline[1];

	if (pipe(pipeline)<0)
		{
		perror("pipe");
		exit(4);
		}
	pw=pipeline[0];
	cp=pipeline[1];

	if ((pid=fork())<0)
		{
		perror("fork");
		exit(5);
		}

	if (pid==0)
		{
		Wait(cw);
		READTRY(fd,1);
		WRITETRY(fd,1);
		READWAIT(fd,0);
		sleep(10);
		WRITEWAIT(fd,0);
		sleep(10);
		UNLOCK(fd,0);
		READTRY(fd,0);
		WRITETRY(fd,1);
		UNLOCK(fd,0);
		exit(0);
		}
	else
		{
		WRITEWAIT(fd,0);
		Post(pp);
		UNLOCK(fd,0);
		sleep(10);
		READWAIT(fd,0);
		sleep(10);
		UNLOCK(fd,0);
		sleep(10);
		READWAIT(fd,0);
		exit(0);
		}
	}
