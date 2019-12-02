/*
**	E X T R A C T
**
**	Copyright 1991-1996 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)extract.h	4.13 96/06/10 jsloan@ncar.ucar.edu
*/

#ifndef H_EXTRACT
#define H_EXTRACT

/*
**	N.B.	These routines are designed to work with network
**		date types in network byte order. Any other use
**		is suspect (and this one is pretty shaky). Also,
**		most architectures won't actually be able to use
**		the 64-bit functions; you need a 64-bit integer
**		data type (e.g. long on the DEC Alpha, the CRAY
**		Y-MP, or the NEC SX-4).
*/

#include "libtproto.h"
#include "libtbase.h"

#ifdef LIBT_UNSIGNED64
extern libt_unsigned64 extract64 LIBTPROTO((char *net64));
extern char *insert64 LIBTPROTO((libt_unsigned64 host64, char *net64));
#endif /* LIBT_UNSIGNED64 */

#ifdef LIBT_SIGNED64
extern libt_signed64 extracts64 LIBTPROTO((char *net64));
extern char *inserts64 LIBTPROTO((libt_signed64 host64, char *net64));
#endif /* LIBT_SIGNED64 */

#ifdef LIBT_UNSIGNED32
extern libt_unsigned32 extract32 LIBTPROTO((char *net32));
extern char *insert32 LIBTPROTO((libt_unsigned32 host32, char *net32));
#endif /* LIBT_UNSIGNED32 */

#ifdef LIBT_SIGNED32
extern libt_signed32 extracts32 LIBTPROTO((char *net32));
extern char *inserts32 LIBTPROTO((libt_signed32 host32, char *net32));
#endif /* LIBT_SIGNED32 */

#ifdef LIBT_UNSIGNED16
extern libt_unsigned16 extract16 LIBTPROTO((char *net16));
extern char *insert16 LIBTPROTO((libt_unsigned16 host16, char *net16));
#endif /* LIBT_UNSIGNED16 */

#ifdef LIBT_SIGNED16
extern libt_signed16 extracts16 LIBTPROTO((char *net16));
extern char *inserts16 LIBTPROTO((libt_signed16 host16, char *net16));
#endif /* LIBT_SIGNED16 */

#endif /* !H_EXTRACT */
