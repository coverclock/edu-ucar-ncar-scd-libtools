/*
** @(#)egskiplists.c	4.2 94/05/18 jsloan@ncar.ucar.edu
**
** See skiplists.c for credits and acknowledgements.
*/

#include <stdio.h>
#include "libtvalues.h"
#include "skiplists.h"
#include "errors.h"

static int
dump(ep,vp,fp)
void *ep;
void *vp;
FILE *fp;
	{
	long *lpv, *lpc;

	lpv=(long *)ep;
	lpc=(long *)vp;

	if (lpv==NULL)
		fprintf(fp,"%ld=(null)\n",*lpc);
	else
		fprintf(fp,"%ld=%ld\n",*lpc,*lpv);
	(*lpc)++;
	}

static int
flong(ap,bp)
long *ap;
long *bp;
	{
	return(((*ap)<(*bp))?(-1):(((*ap)>(*bp))?(1):(0)));
	}

static int 
test(size,f)
int size;
int (*f)();
	{
	SKIPLIST list;
	int i, k, rc;
	long *kp, *keys, counter;

	if ((keys=(long *)malloc(size*sizeof(long)))==NULL)
		{
		fprintf(stderr,"egskiplists: malloc(%d) failed\n",
			size*sizeof(long));
		return(1);
		}

	if (sklinit(&list)==NULL)
		{
		fprintf(stderr,"egskiplists: sklinit failed\n");
		return(2);
		}

	fprintf(stderr,"egskiplists: sklinit\n");
	sklpool(stderr);

	for (k=0; k<size; k++)
		{
		keys[k]=(*f)(k,size);
        	kp=(long *)sklput(&list,(void *)&keys[k],flong,(void *)0);
		if (kp==NULL)
			fprintf(stderr,
				"egskiplists: %d sklput failed (%ld)\n",
				k,keys[k]);
		else if (kp!=&keys[k])
			fprintf(stderr,
				"egskiplists: %d sklput differ (%ld!=%ld)\n",
				k,keys[k],*kp);
        	}

	fprintf(stderr,"egskiplists: sklput sklcount(%d)=%d\n",
		size,sklcount(&list));
	counter=0;
	skldump(&list,dump,(void *)&counter,stderr);

	for(i=0; i<4; i++)
		{
		for(k=0; k<size; k++)
			{
	    		kp=sklget(&list,(void *)&keys[k],flong,(void *)0);
			if (kp==NULL)
				fprintf(stderr,
				"egskiplists: %d sklget failed (%ld)\n",
					k,keys[k]);
			else if (kp!=&keys[k])
				fprintf(stderr,
				"egskiplists: %d sklget differ (%ld!=%ld)\n",
					k,keys[k],*kp);
			else if (*kp!=keys[k])
				fprintf(stderr,
				"egskiplists: %d sklget differ *(%ld!=%ld)\n",
					k,keys[k],*kp);
			}

		fprintf(stderr,"egskiplists: sklget sklcount(%d)=%d\n",
			size,sklcount(&list));
		counter=0;
		skldump(&list,dump,(void *)&counter,stderr);

		for(k=0; k<size; k++)
			{
	    		kp=(long *)skldel(&list,(void *)&keys[k],flong,(void *)0);
			if (kp==NULL)
				fprintf(stderr,
				"egskiplists: %d skldel failed (%ld)\n",
					k,keys[k]);
			else if (kp!=&keys[k])
				fprintf(stderr,
				"egskiplists: %d skldel differ (%ld!=%ld)\n",
					k,keys[k],*kp);
			else if (*kp!=keys[k])
				fprintf(stderr,
				"egskiplists: %d skldel differ *(%ld!=%ld)\n",
					k,keys[k],*kp);
			}

		fprintf(stderr,"egskiplists: skldel sklcount(%d)=%d\n",
			size,sklcount(&list));
		counter=0;
		skldump(&list,dump,(void *)&counter,stderr);

		for (k=0; k<size; k++)
			{
        		kp=(long *)skladd(&list,(void *)&keys[k],flong,(void *)0);
			if (kp==NULL)
				fprintf(stderr,
				"sgskiplists: %d skladd failed (%ld)\n",
					k,keys[k]);
			else if (kp!=&keys[k])
				fprintf(stderr,
				"sgskiplists: %d skladd differ (%ld!=%ld)\n",
					k,keys[k],*kp);
        		}

		fprintf(stderr,"egskiplists: skladd sklcount(%d)=%d\n",
			size,sklcount(&list));
		counter=0;
		skldump(&list,dump,(void *)&counter,stderr);
		}

	sklpool(stderr);
	if ((rc=sklfree(&list,(int (*)())NULL,(void *)NULL))<0)
		fprintf(stderr,"egskiplists: sklfree=%d\n",rc);
	sklpool(stderr);

	return(0);
	}

static int
fs(k,size)
int k;
int size;
	{
	return(k);
	}

static int
fr(k,size)
int k;
int size;
	{
	return((((double)rand())/((double)(RAND_MAX)))*(10*size));
	}

main(argc,argv)
int argc;
char **argv;
	{
	int size, rc, randy, (*fp)();

	if (argc<2)
		{
		fprintf(stderr,"usage: %s samplesize\n",argv[0]);
		exit(1);
		}

	size=atoi(argv[1]);
	if (size<0)
		{
		randy=1;
		fp=fr;
		size=(-size);
		}
	else
		{
		randy=0;
		fp=fs;
		}

	fprintf(stderr,"egskiplists: samplesize=%d keys=%s\n",
		size,randy?"random":"unique");

	rc=test(size,fp);

	sklshutdown();
	sklpool(stderr);

	exit(rc!=0);
	}
