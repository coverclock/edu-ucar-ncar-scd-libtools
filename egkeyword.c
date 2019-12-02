/*
**	Test keyword()
**
**	@(#)egkeyword.c	4.7 94/05/18 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "nextparser.h"
#include "keyword.h"

#define LENGTH	512

main()
	{
	char *key, *val, buffer[LENGTH];
	int rc;

	while ((rc=nextfromfile(stdin,buffer,LENGTH))>=0)
		if (rc>0)
			{
			printf("<%s> : ",buffer);
			key=buffer;
			val=keyword(buffer);
			printf("<%s> = <%s>\n",key,val);
			}
		else
			printf("EOL\n");

	exit(0);
	}
