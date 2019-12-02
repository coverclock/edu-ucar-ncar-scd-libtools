/*
**	I N R A N G E
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)inrange.h	4.6 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_INRANGE
#define H_INRANGE

#define IR_BUFLEN	256
#define IR_INT		"0123456789+-,:"
#define IR_SEP		':'
#define IR_LOW		0
#define IR_HIGH		MAXINT
#define	IR_MODE		0

#include "parcel.h"
#include "libtproto.h"

struct rangeroot
	{
	PARCEL	r_parms;
	int	r_low;
	int	r_high;
	};

typedef struct rangeroot RANGE;

extern RANGE *getrange LIBTPROTO((char *string));

extern int inrange LIBTPROTO((char *value, int *result, RANGE *r));

extern char *strange LIBTPROTO((char *value, RANGE *r));

extern int freerange LIBTPROTO((RANGE *r));

#endif /* !H_INRANGE */
