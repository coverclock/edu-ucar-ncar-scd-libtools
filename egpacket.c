/*
**	@(#)egpacket.c	1.1 94/06/03 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include <parcel.h>
#include <packet.h>
#include <parmfile.h>

main()
	{
	PARCEL par1, par2;
	PACKET *pkt;
	char *p;
	long length, l;

	initparcel(&par1);
	initparcel(&par2);

	(void)bundleparmfile(0,&par1);
	printf("Original:\n");
	(void)unbundleparmfile(1,par1);

	pkt=unbundlepacket(par1);

	(void)bundlepacket(pkt,&par2);
	printf("Duplicate:\n");
	(void)unbundleparmfile(1,par2);

	printf("Check: %d\n",length=packetlength(pkt));
	for (p=(char *)pkt+PACKETLENGTH, l=length-PACKETLENGTH; l>0; l--, p++)
		fputc(*p=='\0'?'\n':*p,stdout);

	(void)freepacket(pkt);
	(void)freeparcel(par1);
	(void)freeparcel(par2);
	}
