/* @(#)egescapes.c	4.3 94/09/29 jsloan@ncar.ucar.edu */

/*
**	E.g.:
**
**	original:	F\x4f\x4F\7\010\xc\xA\xd\11\xB\x7\007\x42\x41R
**	      or:	FOO\a\b\f\n\r\t\v\a\aBAR
**	esclapsed:	0x464f4f07080c0a0d090b070742415200
**	escpanded:	FOO\a\b\f\n\r       \v\a\aBAR
*/

#include <stdio.h>
#include "buffers.h"
#include "escapes.h"
#include "libtstrings.h"
#include "nmalloc.h"

static void
dump(s,l)
char *s;
int l;
	{
	printf("0x");
	while (l>0)
		{
		printf("%02x",*(s++));
		l--;
		}
	}

main()
	{
	BUFFER thing, another;
	int old, new;

	thing=bufalloc(128);
	another=bufalloc(128);

	while (gets(thing)!=NULL)
		{
		strcpy(another,thing);

		printf("original: ``%s'' ",thing);
		dump(thing,strlen(thing)); putchar('\n');

		thing=esclapse(thing);
		printf("esclapse: ``%s'' ",thing);
		dump(thing,strlen(thing)); putchar('\n');

		thing=escpand(thing);
		printf("escpand:  ``%s'' ",thing);
		dump(thing,strlen(thing)); putchar('\n');

		old=strlen(another);
		new=esclapsein(another,old);
		printf("esclapsein: %d %d ",old,new);
		dump(another,new); putchar('\n');
		}

	(void)buffree(thing);
	(void)buffree(another);

	bufpool(stdout);
	nmallocinfo(stdout);

	exit(0);
	}
