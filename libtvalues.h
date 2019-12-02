/*
**	L I B T V A L U E S
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)libtvalues.h	4.1 94/05/16 jsloan@ncar.ucar.edu
*/

#ifndef H_LIBTVALUES
#define H_LIBTVALUES

#ifndef MVS_LIBTOOLS
#include <values.h>
#endif /* !MVS_LIBTOOLS */

#include <stdlib.h>

#ifndef BITSPERBYTE
#define BITSPERBYTE     (8)
#endif /* !BITSPERBYTE */

#ifndef BITS
#define	BITS(type)	(BITSPERBYTE*(int)sizeof(type))
#endif /* !BITS */

#ifndef MAXINT
#define MAXINT		(~(1<<(BITS(int)-1)))
#endif /* !MAXINT */

#ifndef RAND_MAX
#define RAND_MAX        MAXINT
#endif /* !RAND_MAX */

#endif /* !H_LIBTVALUES */
