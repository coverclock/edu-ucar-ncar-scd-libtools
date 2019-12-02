/* @(#)eglists.c	4.3 96/05/24 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "lists.h"
#include "nmalloc.h"

struct nodestruct
	{
	LISTNODE next;
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
	LIST list;
	LISTNODE *temp;
	struct nodestruct *node;

	linit(&list);

	for (n=10; n>0; n--)
		{
		node=(struct nodestruct *)lput(&list,&nodes[10-n].next);
		if (node==NULL)
			printf("lput=NULL\n");
		else
			printf("lput=%c\n",*lbody(&(node->next)));
		}

	for (n=11; n>0; n--)
		{
		node=(struct nodestruct *)lget(&list);
		if (node==NULL)
			printf("lget=NULL\n");
		else
			printf("lget=%c\n",*lbody(&(node->next)));
		}

	node=&nodes[0];
	temp=lget(NULL);
	printf("lget=0x%lx\n",temp);
	temp=lput(NULL,&(node->next));
	printf("lput=0x%lx\n",temp);
	temp=lput(&list,NULL);
	printf("lput=0x%lx\n",temp);
	temp=lput(NULL,NULL);
	printf("lput=0x%lx\n",temp);
	temp=(LISTNODE *)linit(NULL);
	printf("linit=0x%lx\n",temp);
	temp=(LISTNODE *)lbody(NULL);
	printf("lbody=0x%lx\n",temp);

	nmallocinfo(stdout);

	exit(0);
	}
