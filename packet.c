/*
**	P A C K E T
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
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
**	See also packet2.c and packet3.c.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)packet.c	4.2 94/06/03 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libttypes.h"
#include "libtin.h"
#include "errors.h"
#include "buffers.h"
#include "packet.h"

/*
**	packetlength: returns the length of the specified packet.
**	The length is in bytes and includes the length header at
**	the beginning.
**
**	Arguments:
**		packet		pointer to the packet
**
**	Returns:		length of packet in bytes
*/
long
packetlength(PACKET *packet)
	{
	if (packet==NULL)
		{
		seterrno(EINVAL);
		return(-1);
		}

	return((long)ntohl(extract32(packet)));
	}

/*
**	freepacket: free memory associated with a packet.
*/
int
freepacket(PACKET *packet)
	{
	return(packet==NULL?0:buffree((BUFFER)packet));
	}
