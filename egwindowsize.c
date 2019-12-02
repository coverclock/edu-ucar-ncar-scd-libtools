/*
** @(#)egwindowsize.c	4.2 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "windowsize.h"

main()
	{
	int l1, l2, c1, c2;

	l1=(-1);
	l2=(-1);
	c1=(-1);
	c2=(-1);
	windowsize((int *)NULL,(int *)NULL);
	windowsize(&l1,(int *)NULL);
	windowsize((int *)NULL,&c1);
	windowsize(&l2,&c2);
	/* stderr is actually required here for stty to work in egwindowsize */
	fprintf(stderr,"lines=%d %d, columns=%d %d\n",l1,l2,c1,c2);
	}
