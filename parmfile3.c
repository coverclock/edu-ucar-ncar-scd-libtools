/*
**	P A R M F I L E
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Parameter File
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Sep 26 15:44:57 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	The functions in this module implement the PARMFILE data
**	structure. A PARMFILE is a UNIX file contains human-readable
**	and editable parameters that can be imported into and exported
**	from a PARCEL.
**
**	See also parmfile2.c.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)parmfile3.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libtvalues.h"
#include <fcntl.h>
#include "libttypes.h"
#include "libtin.h"
#include "errors.h"
#include "libtstrings.h"
#include "nextparser.h"
#include "keyword.h"
#include "escapes.h"
#include "skiplists.h"
#include "skeyval.h"
#include "extract.h"
#include "buffers.h"
#include "nmalloc.h"
#include "portfolio.h"
#include "parmfile.h"

/*
**	stuffparmfile: read parcel from a file.
**
**	Arguments:
**		fd		open file descriptor
**		portfolio	pointer to portfolio
**
**	Returns:	0 if successful, <0 otherwise
*/
int
stuffparmfile(int fd, PORTFOLIO *portfolio)
	{
	int dupfd, rc, length;
	FILE *fp;
	char *lhs, *rhs;
	BUFFER word, lhsp, rhsp;

	if ((fd<0)||(portfolio==NULL))
		{
		seterrno(EINVAL);
		return(-1);
		}

	if ((dupfd=dup(fd))<0)
		return(-2);
	if ((fp=fdopen(dupfd,"r"))==NULL)
		{
		(void)close(dupfd);
		return(-3);
		}

	if ((word=bufalloc(PARMFILEWORD))==NULL)
		{
		(void)fclose(fp);
		return(-4);
		}
	length=buflength(word);

	while ((rc=nextfromfile(fp,word,length))>=0)
		if (rc>0)
			{

			rhs=keyword(word);
			lhs=(char *)word;

			if ((lhsp=esclapsenew(lhs))==NULL)
				{
				(void)buffree(word);
				(void)fclose(fp);
				return(-4);
				}
			if ((rhsp=esclapsenew(rhs))==NULL)
				{
				(void)buffree(word);
				(void)buffree(lhsp);
				(void)fclose(fp);
				return(-5);
				}

			if (stuffp(lhsp,rhsp,portfolio)==NULL)
				{
				(void)buffree(word);
				(void)buffree(lhsp);
				(void)buffree(rhsp);
				(void)fclose(fp);
				return(-6);
				}

			do
				rc=nextfromfile(fp,word,length);
			while (rc>0);

			if (rc<0)
				break;
			}

	(void)buffree(word);
	(void)fclose(fp);

	return(0);
	}

/*
**	unstuffparmfile: write portfolio to a file. It would be faster
**	if we just always quoted the keyword and value, but this
**	extra effort makes the resulting parmfile so much more
**	readable.
**
**	Arguments:
**		fd		open file descriptor
**		portfolio	pointer to portfolio
**
**	Returns:	0 if successful, <0 otherwise
*/
static int
exporter(SKEYVAL *n, void *vp)
	{
	char *kq, *vq;
	int length, kl, vl, rc;
	BUFFER key, val, buffer;
	FILE *fp;

	if ((n==NULL)||((fp=(FILE *)vp)==NULL))
		{
		seterrno(EINVAL);
		return(-4);
		}

	if ((key=escpandnew(n->skv_keyword))==NULL)
		return(-5);
	if (PORTFOLIOQUOTE(key))
		{
		kq="\"";
		kl=2;
		}
	else
		{
		kq="";
		kl=0;
		}

	if ((val=escpandnew(n->skv_value))==NULL)
		{
		(void)buffree(key);
		return(-6);
		}
	if (PORTFOLIOQUOTE(val))
		{
		vq="\"";
		vl=2;
		}
	else
		{
		vq="";
		vl=0;
		}

	/* 2=strlen("=")+1(for nul) */
	length=strlen(key)+kl+strlen(val)+vl+2;
	if ((buffer=bufalloc(length))==NULL)
		{
		(void)buffree(key);
		(void)buffree(val);
		return(-7);
		}
	
	(void)concats(buffer,kq,key,kq,"=",vq,val,vq,"\n",NULL);

	rc=(fputs(buffer,fp)==EOF)?(-8):0;

	(void)buffree(key);
	(void)buffree(val);
	(void)buffree(buffer);

	return(rc);
	}

int
unstuffparmfile(int fd, PORTFOLIO *portfolio)
	{
	int dupfd, rc;
	FILE *fp;

	if (fd<0)
		{
		seterrno(EINVAL);
		return(-1);
		}

	if ((dupfd=dup(fd))<0)
		return(-2);
	if ((fp=fdopen(dupfd,"w"))==NULL)
		{
		(void)close(dupfd);
		return(-3);
		}

	rc=travportfolio(portfolio,exporter,(void *)fp);

	(void)fclose(fp);

	return(rc);
	}
