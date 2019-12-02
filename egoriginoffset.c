/*
** @(#)egoriginoffset.c	4.3 95/09/14 jsloan@ncar.ucar.edu
**
** This puppy tests the macros ORIGIN and OFFSET, both of
** which rely on pointer arithmetic, which is not only illegal in C,
** but highly suspect in any circumstances, not to mention architecture
** specific. The need for this kind of stuff is generally a pain in the
** ass.
**
** N.B.	The check for OFFSET correctness notes that the reported
**	discrepancy may be a correct result. This shows up on
**	architectures with weird pointer formats, e.g. CRAY Y-MP.
**	It may or may not be cause for concern. Caveat Emptor. On
**	the other hand, an ORIGIN error IS a problem if you intend
**	to use the macro.
*/

#include <stdio.h>
#include "origin.h"
#include "offset.h"

struct foo
	{
	int a;
	char b;
	long c;
	double d;
	char *e;
	};

/*
**	Note: DISP does not work on word architectures like CRAY YMPs,
**	although OFF, which is the same as the macro defined in the
**	libtools offset.h if offsetof is not available, does work.
*/

#define DISP(s,f)	((int)(&(((s)0)->f)))
#define OFF(s,f)	((int)(((char *)&(((s)0)->f))-((char *)((s)0))))

main()
	{
	struct foo f, *fa, *fb, *fc, *fd, *fe;
	int da, db, dc, dd, de;
	int aa, ab, ac, ad, ae;
	int oa, ob, oc, od, oe;
	int *ap;
	char *bp;
	long *cp;
	double *dp;
	char **ep;

	ap=&f.a;
	bp=&f.b;
	cp=&f.c;
	dp=&f.d;
	ep=&f.e;

	printf("a %2d %2d %2d 0x%lx 0x%lx 0x%lx\n",
		da=OFFSET(struct foo,a),
		aa=DISP(struct foo *,a),
		oa=OFF(struct foo *,a),
		ap,
		fa=ORIGIN(ap,struct foo,a),
		&f);
	if (fa!=&f)
		printf("ORIGIN 0x%x != ADDRESS 0x%x failed!\n",fa,&f);
	if (da!=aa)
		printf("OFFSET %d != DISPLACEMENT %d (may be correct)\n",da,aa);
	if (da!=oa)
		printf("OFFSET %d != OLDOFFSET %d (may be correct)\n",da,oa);

	printf("b %2d %2d %2d 0x%lx 0x%lx 0x%lx\n",
		db=OFFSET(struct foo,b),
		ab=DISP(struct foo *,b),
		ob=OFF(struct foo *,b),
		bp,
		fb=ORIGIN(bp,struct foo,b),
		&f);
	if (fb!=&f)
		printf("ORIGIN 0x%x != ADDRESS 0x%x failed!\n",fb,&f);
	if (db!=ab)
		printf("OFFSET %d != DISPLACEMENT %d (may be correct)\n",db,ab);
	if (db!=ob)
		printf("OFFSET %d != OLDOFFSET %d (may be correct)\n",db,ob);

	printf("c %2d %2d %2d 0x%lx 0x%lx 0x%lx\n",
		dc=OFFSET(struct foo,c),
		ac=DISP(struct foo *,c),
		oc=OFF(struct foo *,c),
		cp,
		fc=ORIGIN(cp,struct foo,c),
		&f);
	if (fc!=&f)
		printf("ORIGIN 0x%x != ADDRESS 0x%x failed!\n",fc,&f);
	if (dc!=ac)
		printf("OFFSET %d != DISPLACEMENT %d (may be correct)\n",dc,ac);
	if (dc!=oc)
		printf("OFFSET %d != OLDOFFSET %d (may be correct)\n",dc,oc);

	printf("d %2d %2d %2d 0x%lx 0x%lx 0x%lx\n",
		dd=OFFSET(struct foo,d),
		ad=DISP(struct foo *,d),
		od=OFF(struct foo *,d),
		dp,
		fd=ORIGIN(dp,struct foo,d),
		&f);
	if (fd !=&f)
		printf("ORIGIN 0x%x != ADDRESS 0x%x failed!\n",fd,&f);
	if (dd !=ad)
		printf("OFFSET %d != DISPLACEMENT %d (may be correct)\n",dd,ad);
	if (dd !=od)
		printf("OFFSET %d != OLDOFFSET %d (may be correct)\n",dd,od);

	printf("e %2d %2d %2d 0x%lx 0x%lx 0x%lx\n",
		de=OFFSET(struct foo,e),
		ae=DISP(struct foo *,e),
		oe=OFF(struct foo *,e),
		ep,
		fe=ORIGIN(ep,struct foo,e),
		&f);
	if (fe!=&f)
		printf("ORIGIN 0x%x != ADDRESS 0x%x failed!\n",fe,&f);
	if (de!=ae)
		printf("OFFSET %d != DISPLACEMENT %d (may be correct)\n",de,ae);
	if (de!=oe)
		printf("OFFSET %d != OLDOFFSET %d (may be correct)\n",de,oe);

	exit(0);
	}
