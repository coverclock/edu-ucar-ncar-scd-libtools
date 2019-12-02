/*
**	@(#)egmaybenull.c	1.1 96/06/04 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "maybenull.h"

main()
	{
	fprintf(stderr,"egmaybenull: may core dump below\n");
	fprintf(stderr,"egmaybenull: non-NULL  %s\n",maybenull("non-NULL"));
	fprintf(stderr,"egmaybenull: NULL      %s\n",maybenull(NULL));
	fprintf(stderr,"egmaybenull: core dump %s\n",NULL);
	}
