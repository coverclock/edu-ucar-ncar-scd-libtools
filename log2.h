/*
**	L O G 2
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)log2.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
**	Not all UNIX -lm math libraries contain log2(). If yours
**	does not, you can -DNOLOG2_LIBTOOLS and use this definition.
*/

#ifndef H_LOG2
#define H_LOG2

#include <math.h>

#ifdef NOLOG2_LIBTOOLS
#define log2(x)		(log10(x)/log10((double)2.0))
#endif /* NOLOG2_LIBTOOLS */

#endif /* !H_LOG2 */
