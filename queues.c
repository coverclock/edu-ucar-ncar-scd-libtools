/*
**	Q U E U E S
**
**	Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Queue Handling Routines
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Sep  7 08:51:35 MDT 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This module contains routines that handle a queue data
**	structure. A queue in this implementation is a doubly
**	linked list into which items can be inserted into and
**	deleted from arbitrary positions in the queue. Compare
**	to the singly linked lists in the lists(3L) structure.
**
**	Although lists(3L) (FIFO) and stacks(3L) (LIFO) are
**	implemented seperately, queues can be used to implement
**	either, with some additional overhead.
**
**	Although the term "queue" is usually used to refer to a
**	FIFO structure, it is used here in a more specific sense,
**	intended to implement queues that a spooler might need.
*/

static char copyright[] = "Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)queues.c	4.12 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "nmalloc.h"
#include "queues.h"

/*
**	qinit: Initialize a queue header structure.
**
**	Arguments:
**		q	pointer to queue header
**
**	Returns:	pointer to queue header, NULL if error
*/
QUEUE *
qinit(QUEUE *q)
	{
	if (q!=NULL)
		{
		q->q_size=0;
		q->q_head=NULL;
		q->q_tail=NULL;
		}
	return(q);
	}

/*
**	qsize: Return the number of items in the queue.
**
**	Arguments:
**		q	pointer to queue header structure
**
**	Returns:	count of items in queue, <0 if error
*/
int
qsize(QUEUE *q)
	{
	return(q==NULL?(-1):q->q_size);
	}

/*
**	qbody: return the address of the body of a queue node.
**
**	Arguments:
**		b	address of a queue node
**
**	Returns:	address of body of node, NULL if error
*/
char *
qbody(QUEUENODE *b)
	{
	return(b==NULL?NULL:((char *)b)+(sizeof(QUEUENODE)));
	}

/*
**	qhead: Return address of first node in queue.
**
**	Arguments:
**		q	pointer to queue header structure
**
**	Returns:	pointer to queue node, NULL if empty or error
*/
QUEUENODE *
qhead(QUEUE *q)
	{
	return(q==NULL?NULL:q->q_head);
	}

/*
**	qtail: Return address of last node in queue.
**
**	Arguments:
**		q	pointer to queue header structure
**
**	Returns:	pointer to queue node, NULL if empty or error
*/
QUEUENODE *
qtail(QUEUE *q)
	{
	return(q==NULL?NULL:q->q_tail);
	}

/*
**	qnext: Return pointer to next node in queue.
**
**	Arguments:
**		b	pointer to queue node
**
**	Returns:	pointer to queue node or NULL if none or error
*/
QUEUENODE *
qnext(QUEUENODE *b)
	{
	return(b==NULL?NULL:b->q_next);
	}

/*
**	qprev: Return pointer to previous node in queue.
**
**	Arguments:
**		b	pointer to queue node
**
**	Returns:	pointer to queue node or NULL if none or error
*/
QUEUENODE *
qprev(QUEUENODE *b)
	{
	return(b==NULL?NULL:b->q_prev);
	}

/*
**	qinsert: Insert item b into queue q AFTER item a; if a is
**	null, b is placed at the head of the queue (as with a stack).
**	This can be combined with qtail() to place items at the
**	end of the queue (as with a list).
**
**	Arguments:
**		q	pointer to queue header structure
**		a	pointer to node after which to insert
**		b	pointer to node to insert
**
**	Returns:	pointer to inserted node, or NULL if error
*/
QUEUENODE *
qinsert(QUEUE *q, QUEUENODE *a, QUEUENODE *b)
	{
	if ((q==NULL)||(b==NULL))
		return(NULL);

	if (a==NULL)
		{
		if ((b->q_next=q->q_head)==NULL)
			q->q_tail=b;
		else
			q->q_head->q_prev=b;
		b->q_prev=NULL;
		q->q_head=b;
		}
	else
		{
		if ((b->q_next=a->q_next)==NULL)
			q->q_tail=b;
		else
			a->q_next->q_prev=b;
		b->q_prev=a;
		a->q_next=b;
		}

	q->q_size++;

	return(b);
	}

/*
**	qdelete: Delete item b from queue q.
**
**	Arguments:
**		q	pointer to queue header structure
**		b	pointer to node to be deleted
**
**	Returns:	pointer to deleted node, or NULL if error
*/
QUEUENODE *
qdelete(QUEUE *q, QUEUENODE *b)
	{
	if ((q==NULL)||(b==NULL))
		return(NULL);

	if (q->q_head==b)
		{
		if ((q->q_head=b->q_next)==NULL)
			q->q_tail=NULL;
		else
			q->q_head->q_prev=NULL;
		}
	else
		{
		if ((b->q_prev->q_next=b->q_next)==NULL)
			q->q_tail=b->q_prev;
		else
			b->q_next->q_prev=b->q_prev;
		}

	q->q_size--;
	b->q_next=NULL;
	b->q_prev=NULL;

	return(b);
	}

/*
**	qorder: Order a queue according to a comparison function.
**
**	Arguments:
**		q	pointer to queue header structure
**		fc	comparison function returning -1,0,1
**			if node1 is less than, equal to, greater
**			than node2.
**
**	Returns:	number of items in queue, <0 if error
*/
int
qorder(QUEUE *q, int (*fc)(/* QUEUENODE **a, QUEUENODE **b */))
	{
	QUEUENODE **vector;
	QUEUENODE *b, **v;
	int n;

	/*
	** Validate the arguments.
	*/

	if ((q==NULL)||(fc==NULL))
		return(-1);

	if (q->q_size<=1)
		return(q->q_size);

	/*
	** Allocate the sorting vector
	*/

	if ((vector=(QUEUENODE **)nmalloc((sizeof(QUEUENODE *))*(q->q_size)))==NULL)
		return(-2);

	/*
	** Step through the queue, filling the sorting vector.
	*/

	for (b=q->q_head, v=vector, n=0; b!=NULL; b=b->q_next, n++)
		*(v++)=b;

	/*
	** Sort the sorting vector.
	*/

	(void)qsort((char *)vector,n,sizeof(QUEUENODE *),fc);

	/*
	** Rebuild the queue in the new order.
	*/

	v=vector;
	b=(*(v++));
	q->q_head=b;
	q->q_size=n;
	b->q_prev=NULL;
	for (; n>1; n--)
		{
		b->q_next=(*v);
		(*v)->q_prev=b;
		b=(*(v++));
		}
	q->q_tail=b;
	b->q_next=NULL;

	/*
	** Free the sorting vector.
	*/

	(void)nfree(vector);

	return(q->q_size);
	}
