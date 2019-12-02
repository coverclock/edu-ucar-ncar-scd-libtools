/*
**	S K E Y V A L
**
**	Copyright 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)skeyval.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
**	This is an implementation of keyvals using skiplists instead of
**	binary trees. Binary trees are simpler and more space efficient
**	for small numbers of nodes. Skiplists are more time efficient
**	for large numbers of nodes unless the underlying binary trees
**	are always balanced (which the TREES module does not guarantee).
*/

#ifndef H_SKEYVAL
#define H_SKEYVAL

#include <stdio.h>
#include "lists.h"
#include "skiplists.h"
#include "buffers.h"
#include "libtproto.h"

/*
** Unlike keyvals, skeyvals have a separate structure for the nodes
** and the header. SKEYVAL is the node, SKEYVALS (plural) is the
** header.
*/
struct skeyval
	{
	LISTNODE	skv_next;
	BUFFER	 	skv_keyword;
	BUFFER		skv_value;
	};

typedef struct skeyval SKEYVAL;

typedef SKIPLIST SKEYVALS;

extern SKEYVALS *skvinit LIBTPROTO((SKEYVALS *list));

extern char *skvget LIBTPROTO((char *key, SKEYVALS *list));

extern SKEYVALS *skvput LIBTPROTO((char *key, char *val, SKEYVALS *list));

extern SKEYVALS *skvputp LIBTPROTO((BUFFER key, BUFFER val, SKEYVALS *list));

extern int skvmerge LIBTPROTO((SKEYVALS *from, SKEYVALS *to));

extern int skvaugment LIBTPROTO((SKEYVALS *from, SKEYVALS *to));

extern int skvcount LIBTPROTO((SKEYVALS *list));

extern int skvfree LIBTPROTO((SKEYVALS *list));

extern int skvdump LIBTPROTO((SKEYVALS *list, FILE *fp));

extern int skvpool LIBTPROTO((FILE *fp));

extern void skvshutdown LIBTPROTO((void));

#endif /* !H_SKEYVAL */
