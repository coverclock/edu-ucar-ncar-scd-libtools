/* @(#)egqueues.c	4.2 94/05/18 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "queues.h"
#include "nmalloc.h"

#define N	10

struct item
	{
	QUEUENODE	next;
	int		value;
	} items[N];

static void
dump(q)
QUEUE *q;
	{
	QUEUENODE *c, *h, *t;
	int n;

	h=qhead(q);
	t=qtail(q);
	n=qsize(q);
	printf("Dump q=%#lx, h=%#lx, t=%#lx, n=%d\n",q,h,t,n);
	for (c=h, n=0; c!=NULL; c=qnext(c), n++)
		printf("Item n=%d, c=%#lx[%#lx,%#lx], v=%d\n",
			n,c,c->q_next,c->q_prev,((struct item *)c)->value);
	}

static int
f1(a,b)
struct item **a;
struct item **b;
	{
	int d;

	d=(*b)->value-(*a)->value; /* invert the sort */
	return(d);
	}

main()
	{
	QUEUE q;
	int n;

	printf("TEST qinit\n");
	for (n=0; n<N; n++)
		items[n].value=n;
	(void)qinit(&q);
	dump(&q);

	printf("TEST qinsert qtail 0-N, qorder invert, qdelete new 0-N\n");
	for (n=0; n<N; n++)
		(void)qinsert(&q,qtail(&q),&(items[n].next));
	dump(&q);
	(void)qorder(&q,f1);
	dump(&q);
	for (n=0; n<N; n++)
		(void)qdelete(&q,&(items[n].next));
	dump(&q);

	printf("TEST qinsert NULL 0-N, qdelete N-0\n");
	for (n=0; n<N; n++)
		(void)qinsert(&q,NULL,&(items[n].next));
	dump(&q);
	for (n=N; n>0; n--)
		(void)qdelete(&q,&(items[n-1].next));
	dump(&q);

	printf("TEST qinsert qtail 0-N, qdelete alt qhead/qtail\n");
	for (n=0; n<N; n++)
		(void)qinsert(&q,qtail(&q),&(items[n].next));
	dump(&q);
	for (n=0; n<N; n++)
		{
		if (n%2==0)
			(void)qdelete(&q,qhead(&q));
		else
			(void)qdelete(&q,qtail(&q));
		dump(&q);
		}

	printf("TEST qinsert alt qhead/qprev(qtail) 0-N, qdelete 4536271809\n");
	for (n=0; n<N; n++)
		{
		if (n%2==0)
			(void)qinsert(&q,qhead(&q),&(items[n].next));
		else
			(void)qinsert(&q,qprev(qtail(&q)),&(items[n].next));
		dump(&q);
		}

	for (n=0; n<N; n++)
		{
		if (n%2==0)
			(void)qdelete(&q,&(items[4-(n/2)].next));
		else
			(void)qdelete(&q,&(items[((n+1)/2)+4].next));
		dump(&q);
		}

	nmallocinfo(stdout);

	exit(0);
	}
