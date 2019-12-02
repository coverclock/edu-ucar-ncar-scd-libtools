/*
** @(#)egtrees2.c	4.15 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <errno.h>
#include "trees.h"
#include "keyval.h"
#include "parcels.h"
#include "buffers.h"
#include "nmalloc.h"

static int
f(t)
TREE *t;
	{
	KEYVAL *r;

	r=(KEYVAL *)t;
	printf("%s=%s\n",r->kv_keyword,r->kv_value);
	return(0);
	}

main(argc,argv)
int argc;
char **argv;
	{
	PARCEL box;
	struct treestats s;

	initparcel(&box);

	printf("egtrees2: import=%d\n",bundleparmfile(0,&box));

	printf("egtrees2: before\n");
	dumparcel(&box,stderr);

	printf("egtrees2: tstats=%d\n",tstats(&(box->kv_tree),&s));
	printf(
"egtrees2: nodes=%d height=%d optimum=%d avl=%d average=%d width=%d optimum=%d\n",
        	s.t_numnodes,
		s.t_maxheight,s.t_optheight,s.t_avlheight,s.t_avgheight,
        	s.t_maxwidth,s.t_optwidth);

	printf("egtrees2: balancing\n");
	printf("egtrees2: balance=%d\n",balanceparcel((TREE **)&box));

	printf("egtrees2: after\n");
	dumparcel(&box,stderr);

	printf("egtrees2: tstats=%d\n",tstats(&(box->kv_tree),&s));
	printf(
"egtrees2: nodes=%d height=%d optimum=%d avl=%d average=%d width=%d optimum=%d\n",
        	s.t_numnodes,
		s.t_maxheight,s.t_optheight,s.t_avlheight,s.t_avgheight,
        	s.t_maxwidth,s.t_optwidth);

	printf("egtrees2: left to right\n");
	tleftright(&(box->kv_tree),f);
	printf("egtrees2: right to left\n");
	trightleft(&(box->kv_tree),f);

	printf("egtrees2: export=%d\n",unbundleparmfile(1,box));

	(void)freeparcel(box);

	bufpool(stdout);
	kvpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
