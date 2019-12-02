/*
**	P A R C E L S
**
**	Copyright 1992 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)parcels.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
**	This header file is a shortcut to including the entire
**	parcel family of interrelated data structures.
*/

#ifndef H_PARCELS
#define H_PARCELS

#include "parcel.h"
#include "portfolio.h"
#include "pile.h"
#include "package.h"
#include "packet.h"
#include "parmfile.h"
#include "argvector.h"

#ifdef OLDPARCEL_LIBTOOLS
#define	wrap(p)			unbundlepackage(p)
#define	unwrap(b,pp)		bundlepackage((b),(pp))
#define	pack(p)			unbundlepacket(p)
#define unpack(d,pp)		bundlepacket((d),(pp))
#define envector(p)		unbundlevector(p)
#define devector(n,v,pp)	bundlevector((n),(v),(pp))
#define exportparcel(f,p)	unbundleparmfile((f),(p))
#define importparcel(f,pp)	bundleparmfile((f),(pp))
#define pile(p)			unbundlepile(p)
#define unpile(l,pp)		bundlepile((l),(pp))
#endif /* OLDPARCEL_LIBTOOLS */

#endif /* !H_PARCELS */
