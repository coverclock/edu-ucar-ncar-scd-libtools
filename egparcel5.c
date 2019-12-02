/*
** @(#)egparcel5.c	4.13 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "parcels.h"
#include "buffers.h"
#include "nmalloc.h"

main()
	{
	PARCEL tree;
	char *format;
	char buffer[128];

	initparcel(&tree);
	(void)bundle("DARREL","Holley",&tree);
	(void)bundle("JOHN","Sloan",&tree);
	(void)bundle("MARK","Uris",&tree);
	(void)bundle("CRAIG","Ruff",&tree);
	(void)bundle("ED","Arnold",&tree);
	format="JOHN='John %s, ' CRAIG='Craig %s, ' ED='Ed %s, ' FOO='Foo %s'";
	printf("fmtparcel=%d\n",fmtparcel(tree,format,buffer));
	printf("%s\n",buffer);
	(void)freeparcel(tree);

	bufpool(stdout);
	kvpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
