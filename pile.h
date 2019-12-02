/*
**	P I L E
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)pile.h	4.12 94/05/16 jsloan@ncar.ucar.edu
**
*/

#ifndef H_PILE
#define H_PILE

#include "parcel.h"
#include "portfolio.h"
#include "libtproto.h"

typedef char ** PILE;

extern PILE unbundlepile LIBTPROTO((PARCEL parcel));

extern PARCEL *bundlepile LIBTPROTO((PILE pile, PARCEL *parcelp));

extern PILE unstuffpile LIBTPROTO((PORTFOLIO *portfolio));

extern PORTFOLIO *stuffpile LIBTPROTO((PILE pile, PORTFOLIO *portfolio));

extern int freepile LIBTPROTO((PILE pile));

extern char *indexpile LIBTPROTO((unsigned int ndx, char *buffer));

extern int countpile LIBTPROTO((PILE list));

extern PILE sortpile LIBTPROTO((PILE list));

#endif /* !H_PILE */
