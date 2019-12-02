/*
**	T A B L E X L A T E
**
**	Copyright 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Table Translate
**	Program		TAGS libtools
**	Project		TAGS libtools
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Jul 17 10:14:37 MDT 1991
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Generic table driven byte translate code.
*/

static char copyright[]="Copyright 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)tablexlate.c	4.5 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"

/*
**	tablexlate: Generic table translation function. Translates
**	_len_ bytes at location _buf_ using 256-character translation
**	_table_. Returns the number of characters translated, which
**	should always be equal to len. Does not respect terminating
**	nuls, so len can be set to strlen(buf) for strings.
*/
int
tablexlate(unsigned char *buf, int len, unsigned char table[])
	{
	int cnt;

	for (cnt=0; len>0; len--, buf++, cnt++)
		*buf=table[(unsigned char)*buf];
	return(cnt);
	}
