/*
**	G E N B A S E T Y P E S
**
**	Copyright 1995-1996 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Generate Base Types
**	Program		genbasetypes
**	Project		libtools
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu Jan 26 15:01:19 MST 1995
**	Organization	NCAR/SCD, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This application generates the basetype definitions which are
**	incorporated into the generic basetype header file. This program
**	should be independent of libtools, since it generates
**	information that is used to build the library.
*/

static char copyright[]="Copyright 1995-1996 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)genbasetypes.c	1.10 96/06/18 jsloan@ncar.ucar.edu";

#include <stdio.h>
#include <sys/utsname.h>
#include <values.h>
#include <stdlib.h>

#ifndef BITSPERBYTE
#define BITSPERBYTE     (8)
#endif /* !BITSPERBYTE */

#ifndef BITS
#define	BITS(type)	(BITSPERBYTE*(int)sizeof(type))
#endif /* !BITS */

#define	TS	(64)

static int
type(int size)
	{
	int bits, have;
	char *used;

	used=NULL;
	bits=size*BITSPERBYTE;
	have=0;

	if (BITS(char)>=bits)
		{
		used="char";
		have=BITS(char);
		}
	else if (BITS(short int)>=bits)
		{
		used="short int";
		have=BITS(short int);
		}
	else if (BITS(int)>=bits)
		{
		used="int";
		have=BITS(int);
		}
	else if (BITS(long int)>=bits)
		{
		used="long int";
		have=BITS(long int);
		}
#ifdef TYPE64_LIBTOOLS
	else if (BITS(TYPE64_LIBTOOLS)>=bits)
		{
		used=NAME64_LIBTOOLS;
		have=BITS(TYPE64_LIBTOOLS);
		}
#endif /* TYPE64_LIBTOOLS */

	if (have>=bits)
		{
		printf("typedef unsigned %s libt_unsigned%d;\n",used,bits);
		printf("#define LIBT_UNSIGNED%d \"%s\"\n",bits,used);
		printf("typedef %s libt_signed%d;\n",used,bits);
		printf("#define LIBT_SIGNED%d \"%s\"\n",bits,used);
		if (have>bits)
			fprintf(stderr,"Warning: BITS(%s)==%d>%d\n",
				used,have,bits);
		else
			fprintf(stderr,"Okay: BITS(%s)==%d==%d\n",
				used,have,bits);
		}
	else
		fprintf(stderr,"Warning: BITS(<undefined>)==%d\n",bits);

	return(used!=NULL);
	}

/*
**	M A I N   P R O G R A M
*/

main(int argc, char **argv)
	{
	int rc, n;
	char c;
	unsigned char *p;
        unsigned short swapped;
	char timestamp[TS], *ts;
	char *system, *node, *release, *version, *machine;
	FILE *fp;
	struct utsname u;
	long word;

	rc=0;
	c=0x80;
	swapped=1;
	word=(long)0xfedcba9876543210;	/* Ignore any constant range error */

	if ((fp=popen("date","r"))==NULL)
		ts="UNKNOWN";
	else
		(void)fgets(timestamp,TS,fp);
	(void)pclose(fp);

	if (uname(&u)>=0)
		{
		system=u.sysname;
		node=u.nodename;
		release=u.release;
		version=u.version;
		machine=u.machine;
		}
	else
		{
		system="UNKNOWN";
		node="UNKNOWN";
		release="UNKNOWN";
		version="UNKNOWN";
		machine="UNKNOWN";
		}

	printf("/*\n");
	printf("**\tProgram %s\n",argv[0]);
	printf("**\tDate    %s",timestamp);
	printf("**\n");
	printf("**\tSystem  %s\n",system);
	printf("**\tNode    %s\n",node);
	printf("**\tRelease %s\n",release);
	printf("**\tVersion %s\n",version);
	printf("**\tMachine %s\n",machine);
	printf("**\n");
	printf("**\tchars are %ssigned\n",c<0?"":"not ");
	printf("**\tbytes are %sswapped\n",(*(char *)&swapped)!=0?"":"not ");
	printf("**\n");
	printf("**\t0xfedcba9876543210=0x%lx=0x",word);
	for (p=(unsigned char *)&word, n=sizeof(word); n>0; n--)
		printf("%2.2x",*(p++));
	printf("\n");
	printf("**\n");
        printf("**\tMAXSHORT=0x%lx\n",MAXSHORT);
        printf("**\tMAXINT=0x%lx\n",MAXINT);
        printf("**\tMAXLONG=0x%lx\n",MAXLONG);
        printf("**\tMAXFLOAT=0x%lx\n",MAXFLOAT);
        printf("**\tMAXDOUBLE=0x%lx\n",MAXDOUBLE);
	printf("**\n");
	printf("**\tsizeof(char)=%d\n",sizeof(char));
	printf("**\tsizeof(unsigned char)=%d\n",sizeof(unsigned char));
	printf("**\tsizeof(short int)=%d\n",sizeof(short int));
	printf("**\tsizeof(unsigned short int)=%d\n",sizeof(unsigned short int));
	printf("**\tsizeof(int)=%d\n",sizeof(int));
	printf("**\tsizeof(unsigned int)=%d\n",sizeof(unsigned int));
	printf("**\tsizeof(long int)=%d\n",sizeof(long int));
	printf("**\tsizeof(unsigned long int)=%d\n",sizeof(unsigned long int));
#ifdef TYPE64_LIBTOOLS
	printf("**\tsizeof(%s)=%d\n",NAME64_LIBTOOLS,sizeof(TYPE64_LIBTOOLS));
	printf("**\tsizeof(unsigned %s)=%d\n",NAME64_LIBTOOLS,sizeof(unsigned TYPE64_LIBTOOLS));
#endif /* TYPE64_LIBTOOLS */
	printf("**\tsizeof(float)=%d\n",sizeof(float));
	printf("**\tsizeof(double)=%d\n",sizeof(double));
	printf("**\tsizeof(char *)=%d\n",sizeof(char *));
	printf("**\tsizeof(short *)=%d\n",sizeof(short *));
	printf("**\tsizeof(int *)=%d\n",sizeof(int *));
	printf("**\tsizeof(long *)=%d\n",sizeof(long *));
	printf("**\tsizeof(float *)=%d\n",sizeof(float *));
	printf("**\tsizeof(double *)=%d\n",sizeof(double *));
	printf("**\tsizeof(void *)=%d\n",sizeof(void *));
	printf("**\tsizeof(char **)=%d\n",sizeof(char **));
	printf("**\tsizeof(char[%d])=%d\n",TS,sizeof(timestamp));
	printf("**\tsizeof(0)=%d\n",sizeof(0));
	printf("**\tsizeof(0L)=%d\n",sizeof(0L));
	printf("**\tsizeof(0.0)=%d\n",sizeof(0.0));
	printf("**\tsizeof(0.0F)=%d\n",sizeof(0.0F));
	printf("**\tsizeof(0.0L)=%d\n",sizeof(0.0L));
	printf("**\tsizeof(' ')=%d\n",sizeof(' '));
	printf("**\tsizeof(\" \")=%d\n",sizeof(" "));
	printf("*/\n");

	if (!type(1))
		rc++;

	if (!type(2))
		rc++;

	if (!type(4))
		rc++;

	if (!type(8))
		rc++;

	printf("typedef char libt_boolean;\n");
	printf("#define LIBT_BOOLEAN \"char\"\n");
	printf("typedef void * libt_pointer;\n");
	printf("#define LIBT_POINTER \"void *\"\n");

	exit(rc);
	}
