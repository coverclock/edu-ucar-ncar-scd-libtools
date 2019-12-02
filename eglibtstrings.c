/* @(#)eglibtstrings.c	4.2 94/05/18 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "libtstrings.h"

main()
	{
	char *a, *b, *c;

	a="Foo Bar";
	b="";
	c=NULL;
	printf("``%s'' ``%s'' ``%s''\n",
		STRINGPRINT(a),
		STRINGPRINT(b),
		STRINGPRINT(c));
	}
