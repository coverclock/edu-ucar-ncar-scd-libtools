/*
**	L O C K S M I T H
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)locksmith.h	4.1 94/05/16 jsloan@ncar.ucar.edu
*/

#ifndef H_LOCKSMITH
#define H_LOCKSMITH

#include "libtproto.h"

#define LOCKSMITH_EXCLUSIVE	(2)
#define LOCKSMITH_SHARED	(1)
#define LOCKSMITH_UNLOCK	(0)
#define	LOCKSMITH_NSHARED	(-LOCKSMITH_SHARED)
#define	LOCKSMITH_NEXCLUSIVE	(-LOCKSMITH_EXCLUSIVE)

#define LOCKSMITH_FAILED	(-1)
#define LOCKSMITH_INVALID	(-2)
#define LOCKSMITH_INTERRUPT	(-3)

extern int locksmith LIBTPROTO((int fd, int flag));

#endif /* !H_LOCKSMITH */
