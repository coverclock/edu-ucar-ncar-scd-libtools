/*
**	L I S T S
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)lists.h	4.6 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_LISTS
#define H_LISTS

#include "libtproto.h"

struct listnode
	{
	struct listnode *	l_next;
	};

struct list
	{
	struct listnode *	l_head;
	struct listnode *	l_tail;
	};

typedef struct list LIST;
typedef struct listnode LISTNODE;

extern LISTNODE *lget LIBTPROTO((LIST *l));

extern LISTNODE *lnext LIBTPROTO((LISTNODE *n));

extern LISTNODE *lpeek LIBTPROTO((LIST *l));

extern LISTNODE *lput LIBTPROTO((LIST *l, LISTNODE *n));

extern LIST *linit LIBTPROTO((LIST *l));

extern char *lbody LIBTPROTO((LISTNODE *n));

#endif /* !H_LISTS */
