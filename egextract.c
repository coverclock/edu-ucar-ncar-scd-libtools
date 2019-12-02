/* @(#)egextract.c	4.13 96/06/10 jsloan@ncar.ucar.edu */

#include <stdio.h>
#include "extract.h"
#include "libttypes.h"
#include "libtin.h"
#include "libtvalues.h"
#include "hexprint.h"

#include "libtbase.h"
#ifndef LIBT_SIGNED64
typedef long libt_signed64;
typedef long libt_unsigned64;
#endif /* LIBT_SIGNED64 */

static libt_signed64 mask16=0;
static libt_signed64 mask32=0;

#define PEXTRACT(N,A,L,U,S,FU,FS)	do { \
				printf("%s[%d]=0x",(N),(L)); \
				(void)hexprint(stdout,(A),(L)); \
				U=FU(A); \
				S=FS(A); \
				fputs("=0x",stdout); \
				(void)hexprint(stdout,(char *)&(U),sizeof(U)); \
				printf("=0x%lx",(long)U); \
				fputs("=0x",stdout); \
				(void)hexprint(stdout,(char *)&(S),sizeof(S)); \
				printf("=0x%lx",(long)S); \
				fputc('\n',stdout); \
				} while (0)

#define PINSERT(N,V,A,L,FU,FS)	do { \
				printf("%s[%d]=0x",(N),sizeof(V)); \
				(void)hexprint(stdout,(char *)&(V),sizeof(V)); \
				printf("=0x%lx",(long)V); \
				(void)FU(V,A); \
				fputs("=0x",stdout); \
				(void)hexprint(stdout,(A),(L)); \
				(void)FS(V,A); \
				fputs("=0x",stdout); \
				(void)hexprint(stdout,(A),(L)); \
				fputc('\n',stdout); \
				} while (0)

static void
test16(libt_signed16 n)
	{
	libt_signed16 is, isr;
	char s[sizeof(libt_signed16)];

	is=n;
	isr=extracts16(inserts16(is,s));
	if ((isr&mask16)!=(is&mask16))
		printf("short %d (0x%x) != %d (0x%x) failed\n",
			is,is,isr,isr);
	isr=ntohs(extracts16(inserts16(htons(is),s)));
	if ((isr&mask16)!=(is&mask16))
		printf("netshort %d (0x%x) != %d (0x%x) failed\n",
			is,is,isr,isr);
	printf("16 %d (0x%*.*x) okay\n",
		(libt_signed16)n,
		sizeof(libt_signed16)*2, sizeof(libt_signed16)*2,
		(libt_signed16)n);
	}

static void
test32(libt_signed32 n)
	{
	libt_signed32 il, ilr;
	char l[sizeof(libt_signed32)];

	il=n;
	ilr=extracts32(inserts32(il,l));
	if ((ilr&mask32)!=(il&mask32))
		printf("long %ld (%lx) != %ld (%lx) failed\n",
			il,il,ilr,ilr);
	il=n;
	ilr=ntohl(extracts32(inserts32(htonl(il),l)));
	if ((ilr&mask32)!=(il&mask32))
		printf("netlong %ld (%lx) != %ld (%lx) failed\n",
			il,il,ilr,ilr);
	printf("32 %ld (0x%*.*lx) okay\n",
		(libt_signed32)n,
		sizeof(libt_signed32)*2,sizeof(libt_signed32)*2,
		(libt_signed32)n);
	}

#ifdef LIBT_SIGNED64
static void
test64(libt_signed64 n)
	{
	libt_signed64 ill, illr;
	char ll[sizeof(libt_signed64)];

	ill=n;
	illr=extracts64(inserts64(ill,ll));
	if (illr!=ill)
		printf("long64 %ld (%lx) != %ld (%lx) failed\n",
			ill,ill,illr,illr);
	printf("64 %ld (0x%*.*lx) okay\n",
		(libt_signed64)n,
		sizeof(libt_signed64)*2,sizeof(libt_signed64)*2,
		(libt_signed64)n);
	}
#endif /* LIBT_SIGNED64 */

main()
	{
	static unsigned long mustswap=1;

	static unsigned char c2n[]={0xfe, 0xdc};
	static unsigned char c2p[]={0x01, 0x23};
	static unsigned char c4n[]={0xfe, 0xdc, 0xba, 0x98};
	static unsigned char c4p[]={0x01, 0x23, 0x45, 0x67};
	static unsigned char c8n[]={0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
	static unsigned char c8p[]={0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

	static unsigned char c2ns[]={0xff, 0xfe};
	static unsigned char c2ps[]={0x00, 0x01};
	static unsigned char c4ns[]={0xff, 0xff, 0xff, 0xfe};
	static unsigned char c4ps[]={0x00, 0x00, 0x00, 0x01};
	static unsigned char c8ns[]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe};
	static unsigned char c8ps[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

	char cb[8];

	static libt_signed16 b2n, b2p;
	static libt_signed32 b4n, b4p;
	static libt_signed64 b8n, b8p;

	char s[sizeof(libt_signed16)];
	char l[sizeof(libt_signed32)];
	char ll[sizeof(libt_signed64)];

	libt_signed16 is, isr;
	libt_unsigned16 uis;
	libt_signed32 il, ilr;
	libt_unsigned32 uil;
	libt_signed64 ill, illr;
	libt_unsigned64 uill;

	long n;
	int b;

	/*
	** Architecture tests
	*/

	printf("sizeof(char)=%d\n",sizeof(char));
	printf("sizeof(short)=%d\n",sizeof(short));
	printf("sizeof(int)=%d\n",sizeof(int));
	printf("sizeof(long)=%d\n",sizeof(long));
	printf("sizeof(float)=%d\n",sizeof(float));
	printf("sizeof(double)=%d\n",sizeof(double));
	printf("sizeof(char *)=%d\n",sizeof(char *));
	printf("sizeof(void *)=%d\n",sizeof(void *));
	printf("sizeof(netshort)=%d\n",16/BITSPERBYTE);
	printf("sizeof(netlong)=%d\n",32/BITSPERBYTE);
	printf("sizeof(netlonglong)=%d\n",64/BITSPERBYTE);
	printf("sizeof(libt_signed16)=%d\n",sizeof(libt_signed16));
	printf("sizeof(libt_signed32)=%d\n",sizeof(libt_signed32));
#ifdef LIBT_SIGNED64
	printf("sizeof(libt_signed64)=%d\n",sizeof(libt_signed64));
#else /* LIBT_SIGNED64 */
	printf("sizeof(libt_signed64)=<undefined>\n");
#endif /* LIBT_SIGNED64 */

	fputc('\n',stdout);

	n=1;
	printf("%d=0x",n);
	hexprint(stdout,(char *)&n,sizeof(n));
	printf("=0x%x\n",n);
	fputc('\n',stdout);

	n=(-1);
	printf("%d=0x",n);
	hexprint(stdout,(char *)&n,sizeof(n));
	printf("=0x%x\n",n);
	fputc('\n',stdout);

	printf("swap=%s 0x%x 0x",(*(char *)&mustswap)?"yes":"no",mustswap);
	(void)hexprint(stdout,(char *)&mustswap,sizeof(mustswap));
	fputc('\n',stdout);
	fputc('\n',stdout);

	mask16=(libt_signed64)(~((libt_signed64)0));
	mask32=(libt_signed64)(~((libt_signed64)0));
	printf("mask16=0x%x mask32=0x%x\n",mask16,mask32);
	is=(-1);
	ilr=extracts16(inserts16(is,s));
	if ((ilr&0xffff)!=(is&0xffff))
		printf("low16 %d (0x%x) != %d (0x%x) failed\n",is,is,ilr,ilr);
	if ((ilr&0xffff)!=0xffff)
		printf("low16 %d (0x%x) != 0xffff failed\n",ilr,ilr);
	if (((ilr>>16)&0xffff)==0xffff)
		{
		printf("high16 %d (0x%x) sign extended\n",ilr,ilr);
		mask16=0xffff;
		mask32=0xffffffff;
		}
	else
		if (((ilr>>16)&0xffff)==0)
			printf("high16 %d (0x%x) zero extended\n",ilr,ilr);
		else
			printf("high16 %d (0x%x) failed\n",ilr,ilr);
	printf("mask16=0x%x mask32=0x%x\n",mask16,mask32);
	fputc('\n',stdout);

	/*
	** Extraction tests
	*/

	PEXTRACT("c2n",(char *)c2n,2,uis,is,extract16,extracts16);
	PEXTRACT("c2p",(char *)c2p,2,uis,is,extract16,extracts16);
	PEXTRACT("c4n",(char *)c4n,4,uil,il,extract32,extracts32);
	PEXTRACT("c4p",(char *)c4p,4,uil,il,extract32,extracts32);
#ifdef LIBT_SIGNED64
	PEXTRACT("c8n",(char *)c8n,8,uill,ill,extract64,extracts64);
	PEXTRACT("c8p",(char *)c8p,8,uill,ill,extract64,extracts64);
#endif /* LIBT_SIGNED64 */
	fputc('\n',stdout);

	PEXTRACT("c2ns",(char *)c2ns,2,uis,is,extract16,extracts16);
	PEXTRACT("c2ps",(char *)c2ps,2,uis,is,extract16,extracts16);
	PEXTRACT("c4ns",(char *)c4ns,4,uil,il,extract32,extracts32);
	PEXTRACT("c4ps",(char *)c4ps,4,uil,il,extract32,extracts32);
#ifdef LIBT_SIGNED64
	PEXTRACT("c8ns",(char *)c8ns,8,uill,ill,extract64,extracts64);
	PEXTRACT("c8ps",(char *)c8ps,8,uill,ill,extract64,extracts64);
#endif /* LIBT_SIGNED64 */
	fputc('\n',stdout);

	/*
	** Insertion tests
	*/

	b2n=0xf1f2;
	b2p=0x0102;
	b4n=0xf1f2f3f4;
	b4p=0x01020304;
	b8n=0xf1f2f3f4;
	b8p=0x01020304;
	if (sizeof(libt_signed64)>=8)
		{
		b8n=(b8n<<32)|0xf5f6f7f8;
		b8p=(b8p<<32)|0x05060708;
		}

	PINSERT("b2n",b2n,cb,2,insert16,inserts16);
	PINSERT("b2p",b2p,cb,2,insert16,inserts16);
	PINSERT("b4n",b4n,cb,4,insert32,inserts32);
	PINSERT("b4p",b4p,cb,4,insert32,inserts32);
#ifdef LIBT_SIGNED64
	PINSERT("b8n",b8n,cb,8,insert64,inserts64);
	PINSERT("b8p",b8p,cb,8,insert64,inserts64);
#endif /* LIBT_SIGNED64 */
	fputc('\n',stdout);

	b2n=((~(libt_signed16)0)<<8)|0xf1;
	b2p=0x01;
	b4n=((~(libt_signed32)0)<<8)|0xf1;
	b4p=0x01;
	b8n=((~(libt_signed64)0)<<8)|0xf1;
	b8p=0x01;

	PINSERT("b2ns",b2n,cb,2,insert16,inserts16);
	PINSERT("b2ps",b2p,cb,2,insert16,inserts16);
	PINSERT("b4ns",b4n,cb,4,insert32,inserts32);
	PINSERT("b4ps",b4p,cb,4,insert32,inserts32);
#ifdef LIBT_SIGNED64
	PINSERT("b8ns",b8n,cb,8,insert64,inserts64);
	PINSERT("b8ps",b8p,cb,8,insert64,inserts64);
#endif /* LIBT_SIGNED64 */
	fputc('\n',stdout);

	/*
	** Transitivity tests (non-exhaustive and assumes twos complement)
	*/

	is=1;
	for (b=16; b>0; is=(is<<1)|1, b--)
		test16(is);
	for (b=16; b>=0; is=is<<1, b--)
		test16(is);
	fputc('\n',stdout);

	il=1;
	for (b=32; b>0; il=(il<<1)|1, b--)
		test32(il);
	for (b=32; b>=0; il=il<<1, b--)
		test32(il);
	fputc('\n',stdout);

#ifdef LIBT_SIGNED64
	if (sizeof(libt_signed64)>4)
		{
		ill=1;
		for (b=64; b>0; ill=(ill<<1)|1, b--)
			test64(ill);
		for (b=64; b>=0; ill=ill<<1, b--)
			test64(ill);
		}
	fputc('\n',stdout);
#endif /* LIBT_SIGNED64 */

	exit(0);
	}
