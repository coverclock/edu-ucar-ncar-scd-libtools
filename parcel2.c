/*
**	P A R C E L 2
**
**	Copyright 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Additional Parcel Functions
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Jul  1 15:23:24 MDT 1992
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This module contains additional parcel-handling functions that
**	are extensions of those in the basic parcel(3l) module.
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)parcel2.c	4.21 96/06/04 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <stdarg.h>
#include "errors.h"
#include "nextparser.h"
#include "buffers.h"
#include "keyword.h"
#include "trees.h"
#include "maybenull.h"
#include "parcel.h"

/*
**	fmtparcel: format parcel stuff using the provided control
**	string which is in the format "KEYWORD=printfmt ...".
*/
int
fmtparcel(PARCEL p, char *f, char *b)
	{
	BUFFER buffer;
	char *key, *fmt, *val;
	int length;

	if ((buffer=bufalloc(strlen(f)+1))==NULL)
		return(-1);
	length=buflength(buffer);

	/*
	** March through the format string, getting each keyword
	** and printf-string pair, fetch the keyword's value
	** from the parcel, and format it into the output string.
	*/

	*b='\0';

	while ((f=nextfrombuffer(f,buffer,length))!=NULL)
		{

		key=(char *)buffer;
		fmt=keyword(buffer);

		if ((val=unbundle(key,p))==NULL)
			val="";

		(void)sprintf(b,fmt,val);

		b+=strlen(b);

		}

	(void)buffree(buffer);

	return(0);
	}

/*
**	dumparcel: dump a parcel in a form that gives a programmer
**	some prayer of determining the structure of the underlying
**	tree.
*/
static void
pdump(PARCEL *pp, FILE *fp)
	{
	if (pp==NULL)
		fprintf(fp,"0x%lx",pp);
	else if (*pp==NULL)
		fprintf(fp,"0x%lx:0x%lx",pp,*pp);
	else
		fprintf(fp,
			"0x%lx:0x%lx[0x%lx,0x%lx,0x%lx:``%s'',0x%lx:``%s'']",
			pp,*pp,
			(*pp)->kv_tree.t_left,
			(*pp)->kv_tree.t_right,
			(*pp)->kv_keyword,
			maybenull((*pp)->kv_keyword),
			(*pp)->kv_value,
			maybenull((*pp)->kv_value)
			);
	}

static void
pprint(FILE *fp, PARCEL *pp, int level)
	{
	int n;

	fputs("dumparcel: ",fp);
	for (n=level; n>0; n--)
		fputc('.',fp);
	(void)pdump(pp,fp);
	fputc('\n',fp);

	/*
	** The left and right branches have seperate if's
	** so it's easier for me to move them around. I
	** can never quite decider whither tis nobler to
	** dump in-order, or post-order, or what have you.
	*/

	if (pp!=NULL)
		if ((*pp)!=NULL)
			pprint(fp,(PARCEL *)&((*pp)->kv_tree.t_left),level+1);

	if (pp!=NULL)
		if ((*pp)!=NULL)
			pprint(fp,(PARCEL *)&((*pp)->kv_tree.t_right),level+1);
	}

void
dumparcel(PARCEL *parcelp, FILE *fp)
	{
	pprint(fp,parcelp,0);
	}

/*
**	bundlef: create a value using a printf-style format and
**	a list of arguments and bundle(3l) it into a parcel.
**
**	Arguments:
**		parm		keyword string
**		parcelp		pointer to parcel
**		buffer		buffer large enough to contain value
**		format		format string
**		va_list		argument list for format string
**
**	Returns:		value of bundle()
*/
PARCEL *
bundlef(char *parm, PARCEL *parcelp, char *buffer, char *format,...)
	{
	va_list args;
	
	va_start(args,format);

	(void)vsprintf(buffer,format,args);

	va_end(args);

	return(bundle(parm,buffer,parcelp));
	}

/*
**	unbundlef: extract a value from a parcel in an
**	unbundle(3l)-like fashion, and convert it into another
**	type via sscanf(3) using a printf-style format.
**
**	Arguments:
**		parm		keyword string
**		parcel		parcel
**		format		format string
**		argp		pointer to output variable
**
**	Returns:		same as unbundle(3l)
*/
char *
unbundlef(char *parm, PARCEL parcel, char *format, void *argp)
	{
	char *value;

	if ((value=unbundle(parm,parcel))==NULL)
		return(value);

	if (sscanf(value,format,argp)!=1)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	return(value);
	}
