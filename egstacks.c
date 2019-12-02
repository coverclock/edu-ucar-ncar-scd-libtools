/* @(#)egstacks.c	4.3 96/05/24 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "stacks.h"
#include "nmalloc.h"

struct nodestruct
	{
	STACKNODE next;
	char datum;
	} nodes[10] =
		{
		NULL, '0',
		NULL, '1',
		NULL, '2',
		NULL, '3',
		NULL, '4',
		NULL, '5',
		NULL, '6',
		NULL, '7',
		NULL, '8',
		NULL, '9',
		};
main()
	{
	int n;
	STACK stack;
	STACKNODE *temp;
	struct nodestruct *node;

	stack=NULL;

	for (n=10; n>0; n--)
		{
		node=(struct nodestruct *)spush(&stack,&(nodes[10-n].next));
		if (node==NULL)
			printf("spush=NULL\n");
		else
			printf("spush=%c\n",*sbody(&(node->next)));
		}

	for (n=11; n>0; n--)
		{
		node=(struct nodestruct *)sget(&stack);
		if (node==NULL)
			printf("sget=NULL\n");
		else
			printf("sget=%c\n",*sbody(&(node->next)));
		node=(struct nodestruct *)spop(&stack);
		if (node==NULL)
			printf("spop=NULL\n");
		else
			printf("spop=%c\n",*sbody(&(node->next)));
		}

	node=&nodes[0];
	temp=sget(NULL);
	printf("sget=0x%lx\n",temp);
	temp=spop(NULL);
	printf("spop=0x%lx\n",temp);
	temp=spush(NULL,&(node->next));
	printf("spush=0x%lx\n",temp);
	temp=spush(&stack,NULL);
	printf("spush=0x%lx\n",temp);
	temp=spush(NULL,NULL);
	printf("spush=0x%lx\n",temp);
	temp=(STACKNODE *)sbody(NULL);
	printf("sbody=0x%lx\n",temp);

	nmallocinfo(stdout);

	exit(0);
	}
