/*
**	S T A C K S
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)stacks.h	4.6 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_STACKS
#define H_STACKS

#include "libtproto.h"

struct stacknode
	{
	struct stacknode *	s_next;
	};

typedef struct stacknode * STACK;
typedef struct stacknode STACKNODE;

extern STACKNODE *sget LIBTPROTO((STACK *s));

extern STACKNODE *spop LIBTPROTO((STACK *s));

extern STACKNODE *spush LIBTPROTO((STACK *s, STACKNODE *n));

extern char *sbody LIBTPROTO((STACKNODE *n));

#endif /* !H_STACKS */
