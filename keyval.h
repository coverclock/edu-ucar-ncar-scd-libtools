/*
**	K E Y V A L
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)keyval.h	4.15 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_KEYVAL
#define H_KEYVAL

#include <stdio.h>
#include "trees.h"
#include "buffers.h"
#include "libtproto.h"

struct keyval
	{
	TREE	kv_tree;
	BUFFER 	kv_keyword;
	BUFFER	kv_value;
	};

typedef struct keyval KEYVAL;

extern char *kvget LIBTPROTO((char *key, KEYVAL *tree));

extern KEYVAL **kvput LIBTPROTO((char *key, char *val, KEYVAL **treep));

extern KEYVAL **kvputp LIBTPROTO((BUFFER key, BUFFER val, KEYVAL **treep));

extern int kvmerge LIBTPROTO((KEYVAL *from, KEYVAL **top));

extern int kvaugment LIBTPROTO((KEYVAL *from, KEYVAL **top));

extern int kvcount LIBTPROTO((KEYVAL *tree));

extern int kvfree LIBTPROTO((KEYVAL *tree));

extern int kvdump LIBTPROTO((KEYVAL *tree, FILE *fp));

extern int kvcompare LIBTPROTO((KEYVAL *e, KEYVAL *p));

extern int kvpool LIBTPROTO((FILE *fp));

extern void kvshutdown LIBTPROTO((void));

#endif /* !H_KEYVAL */
