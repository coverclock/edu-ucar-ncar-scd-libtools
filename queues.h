/*
**	Q U E U E S
**
**	Copyright 1990-1996 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)queues.h	4.7 96/05/22 jsloan@ncar.ucar.edu
**
**	N.B.	Beginning with LIBTOOLS 4.3, the data structures
**		queuenode and queue have been renamed quenode and que
**		respectively to prevent collision with the queue
**		structure that is part of the SOLARIS (System V)
**		STREAMS package. The typedefs remain the same.
*/

#ifndef H_QUEUES
#define H_QUEUES

#include "libtproto.h"

struct quenode
	{
	struct quenode *	q_next;		/* next item on queue */
	struct quenode *	q_prev;		/* previous item on queue */
	};

struct que
	{
	int			q_size;		/* number of items on queue */
	struct quenode *	q_head;		/* first item on queue */
	struct quenode *	q_tail;		/* last item on queue */
	};

typedef struct que QUEUE;
typedef struct quenode QUEUENODE;

extern QUEUENODE *qhead LIBTPROTO((QUEUE *q));

extern QUEUENODE *qtail LIBTPROTO((QUEUE *q));

extern QUEUENODE *qinsert LIBTPROTO((QUEUE *q, QUEUENODE *a, QUEUENODE *b));

extern QUEUENODE *qdelete LIBTPROTO((QUEUE *q, QUEUENODE *b));

extern QUEUENODE *qnext LIBTPROTO((QUEUENODE *b));

extern QUEUENODE *qprev LIBTPROTO((QUEUENODE *b));

extern QUEUE *qinit LIBTPROTO((QUEUE *q));

extern int qsize LIBTPROTO((QUEUE *q));

extern int qorder LIBTPROTO((QUEUE *q, int (*fc)()));

extern char *qbody LIBTPROTO((QUEUENODE *b));

#endif /* !H_QUEUES */
