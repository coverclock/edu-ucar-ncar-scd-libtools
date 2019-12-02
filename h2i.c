/*
**	H 2 I
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Hex-to-Integer/Long
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Jul 13 14:25:44 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Similar to s2i(3-libtools), but handles hex numbers instead.
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)h2i.c	1.2 96/06/10 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "h2i.h"

#define	C_H2I

/*
**	h2l (hex to long), h2i (hex to integer), h2s (hex to
**	short): try to do less work than sscanf, yet more work than ato[il],
**	yet basically accomplish the same thing.
*/
int
h2l(register char *string, long *pointer)
#define	H2I_TYPE	long
#include "h2ic.h"

int
h2i(register char *string, int *pointer)
#undef	H2I_TYPE
#define	H2I_TYPE	int
#include "h2ic.h"

int
h2s(register char *string, short *pointer)
#undef	H2I_TYPE
#define	H2I_TYPE	short
#include "h2ic.h"
