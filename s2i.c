/*
**	S 2 I
**
**	Copyright 1993-1996 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		String-to-Integer/Long
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu Dec  2 12:15:50 MST 1993
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Profiling msscsim when the -z flag is used -- which disables
**	most of the cpu and memory intensive stuff -- shows that an
**	awesome amount of time is spend inside sscanf(3) which is
**	used many times for each record in Transaction(). Quick tests
**	have shown that the cpu time ratio is something like 2:3:14 for
**	atol:s2l:sscanf. Ouch!
**
**	On virtually all architectures I deal with, s2i (or s2l) is
**	redundant, since sizeof(int)==sizeof(long). However, this is
**	not the case for the DEC Alpha or the NEC SX-4, where
**	sizeof(int)==32 and sizeof(long)==64.
**
**	This code is only designed to work on architectures
**	which have "typical" integer binary encodings. Is is srictly
**	a kludge to get around the CPU consuming general purpose
**	code in sscanf(3) while offering some additional error checking
**	lacking in ato[il](3).
*/

static char copyright[]="Copyright 1993-1996 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)s2i.c	4.4 96/06/12 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "s2i.h"

#define	C_S2I

/*
**	s2l (string to long), s2i (string to integer), s2s (string to
**	short), s2c (string to character): try to do less work than sscanf,
**	yet more work than ato[il], yet basically accomplish the same thing.
*/
int
s2l(register char *string, long *pointer)
#define	S2I_TYPE	long
#include "s2ic.h"

int
s2i(register char *string, int *pointer)
#undef	S2I_TYPE
#define	S2I_TYPE	int
#include "s2ic.h"

int
s2s(register char *string, short *pointer)
#undef	S2I_TYPE
#define	S2I_TYPE	short
#include "s2ic.h"

int
s2c(register char *string, char *pointer)
#undef	S2I_TYPE
#define	S2I_TYPE	char
#include "s2ic.h"

/*
**	s2x: convert a string into some kind of number. This is
**	because types pre-defined by UNIX aren't portable. E.g.
**	on some systems off_t is int at 32 bits, on some it is
**	long at 64 bits, etc. so you can't know ahead of time which
**	function to use. PITA (Pain In The Ass).
*/
int
s2x(char *s, void *p, int n)
	{
	int rc;

	if (n==sizeof(int))
		rc=s2i(s,(int *)p);
	else if (n==sizeof(long))
		rc=s2l(s,(long *)p);
	else if (n==sizeof(short))
		rc=s2s(s,(short *)p);
	else if (n==sizeof(char))
		rc=s2c(s,(char *)p);
	else
		rc=0;
	return(rc);
	}
