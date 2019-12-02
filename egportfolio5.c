/*
** @(#)egportfolio5.c	4.2 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "portfolio.h"
#include "buffers.h"
#include "nmalloc.h"
#include "argvector.h"
#include "package.h"
#include "packet.h"
#include "parmfile.h"
#include "pile.h"

main()
	{
	PORTFOLIO tree;
	char *format;
	char buffer[128];

	initportfolio(&tree);
	(void)stuff("DARREL","Holley",&tree);
	(void)stuff("JOHN","Sloan",&tree);
	(void)stuff("MARK","Uris",&tree);
	(void)stuff("CRAIG","Ruff",&tree);
	(void)stuff("ED","Arnold",&tree);
	format="JOHN='John %s, ' CRAIG='Craig %s, ' ED='Ed %s, ' FOO='Foo %s'";
	printf("fmtportfolio=%d\n",fmtportfolio(&tree,format,buffer));
	printf("%s\n",buffer);
	(void)freeportfolio(&tree);

	bufpool(stdout);
	skvpool(stdout);
	sklpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
