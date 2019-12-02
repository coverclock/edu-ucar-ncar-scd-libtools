/*
**	P A C K E T
**
**	Copyright 1989-1996 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Packet
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Sep 26 15:44:57 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**      The functions in this module implement the PACKET data structure.
**      A PACKET is a data structure which contains the same information
**      as a PACKAGE but in a more concise machine-friendly (but
**      hopefully pretty machine independent) form. It's used mostly
**      for sending information across interprocess communication
**      links.
**
**	See also packet.c and packet3.c.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)packet2.c	4.3 96/06/10 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include "libtbase.h"
#include <stdio.h>
#include "libttypes.h"
#include "libtin.h"
#include "errors.h"
#include "libtstrings.h"
#include "trees.h"
#include "keyval.h"
#include "extract.h"
#include "buffers.h"
#include "nmalloc.h"
#include "parcel.h"
#include "packet.h"

struct Packing
	{
	int	P_length;
	BUFFER	P_string;
	};

/*
**	unbundlepacket: traverse a PARCEL structure, placing all the
**	parameters in a PACKET structure. packer() is the local recursive
**	routine used by unbundlepacket().
**
**	Arguments:
**		parcel		pointer to parcel structure
**
**	Returns:		pointer to PACKET string or NULL
**				if error (errno set)
*/
static int
packer(PARCEL n, void *vp)
	{
	int keylen, vallen, rc;
	struct Packing *p;
	BUFFER new;

	if ((n==NULL)||((p=(struct Packing *)vp)==NULL))
		{
		seterrno(EINVAL);
		return(-1);
		}

	keylen=strlen(n->kv_keyword)+1;
	vallen=strlen(n->kv_value)+1;
	if ((new=bufrealloc(p->P_string,p->P_length+keylen+vallen))==NULL)
		return(-2);

	bcopy(n->kv_keyword,new+p->P_length,keylen);
	p->P_length+=keylen;
	bcopy(n->kv_value,new+p->P_length,vallen);
	p->P_length+=vallen;

	p->P_string=new;

	return(0);
	}

PACKET *
unbundlepacket(PARCEL parcel)
	{
	struct Packing packing;

	/*
	** Initialize the PACKET, leaving enough room at
	** the beginning to store the total byte count (which
	** is stored in network byte order BTW).
	*/

	if ((packing.P_string=bufalloc(PACKETSIZE))==NULL)
		return(NULL);
	packing.P_length=PACKETLENGTH;

	/*
	** Traverse the PARCEL, building the PACKET recursively.
	*/

	if (travparcel(parcel,packer,(void *)&packing)!=0)
		{
		(void)buffree(packing.P_string);
		return(NULL);
		}

	/*
	** Terminate the PACKET by saving its length.
	*/

	(void)insert32(htonl((libt_unsigned32)packing.P_length),(char *)packing.P_string);

	return((PACKET *)packing.P_string);
	}

/*
**	bundlepacket: convert a PACKET into a PARCEL so that the
**	parameters can be accessed efficiently.
**
**	Arguments:
**		packet		pointer to input PACKET
**		parcelp		pointer to address of output PARCEL structure
**
**	Returns:		address of output PARCEL structure
**				or NULL if error (errno set)
*/
PARCEL *
bundlepacket(PACKET *packet, PARCEL *parcelp)
	{
	char *p, *e;
	char *key, *val;
	long len;

	if ((packet==NULL)||(parcelp==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	/*
	** Step through the PACKET, grabbing pairs of
	** strings and stuffing them into the PARCEL.
	*/

	len=packetlength(packet);
	p=(char *)packet;
	e=p+len;
	p+=PACKETLENGTH;

	while (p<e)
		{
		key=p;
		p+=strlen(key)+1;
		if (!(p<e))
			break;

		val=p;
		p+=strlen(val)+1;

		if (bundle(key,val,parcelp)==NULL)
			return(NULL);
		}

	return(parcelp);
	}
