/*
**	P A C K E T
**
**	Copyright 1990, 1991, 1992, 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)packet.h	4.1 94/05/16 jsloan@ncar.ucar.edu
*/

#ifndef H_PACKET
#define H_PACKET

#include "parcel.h"
#include "portfolio.h"
#include "libtproto.h"

typedef char PACKET;

/*
** Starting size for a packet
*/
#define PACKETSIZE	(32)

/*
** Space at head of packet for length field
** (sizeof(netlong)/bitsperbyte or generally 32/8)
*/
#define PACKETLENGTH	(32/8)

extern PACKET *unbundlepacket LIBTPROTO((PARCEL parcel));

extern PARCEL *bundlepacket LIBTPROTO((PACKET *packet, PARCEL *parcelp));

extern PACKET *unstuffpacket LIBTPROTO((PORTFOLIO *portfolio));

extern PORTFOLIO *stuffpacket LIBTPROTO((PACKET *packet, PORTFOLIO *portfolio));

extern long packetlength LIBTPROTO((PACKET *packet));

extern int freepacket LIBTPROTO((PACKET *packet));

#endif /* !H_PACKET */
