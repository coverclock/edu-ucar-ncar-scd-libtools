/*
**	I N R A N G E
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		In Range
**	Program		TAGS libtools
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu Sep 14 10:51:27 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This package provides range checking for numeric values
**	and subparameter and string to number translation for
**	char values. It is intended to be used with the getparm(3l)
**	package, although it was designed with some independence
**	in mind.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)inrange.c	4.14 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libtvalues.h"
#include "errors.h"
#include "libtstrings.h"
#include "parcel.h"
#include "package.h"
#include "nmalloc.h"
#include "inrange.h"

#define IR_INT		"0123456789+-,:"
#define IR_SEP		':'
#define IR_LOW		0
#define IR_HIGH		MAXINT
#define	IR_MODE		0

/*
**	rangeint: parse a numeric range and stow the low and high
**	values in the RANGE object.
**
**	Arguments:
**		lhs	pointer to numeric range string
**		r	pointer to RANGE object
**
**	Returns:	r if success, NULL otherwise (errno set)
*/
static RANGE *
rangeint(char *lhs, RANGE *r)
	{
	char *rhs;

	if ((rhs=strchr(lhs,IR_SEP))==NULL)
		{
		rhs=lhs;
		lhs=rhs+strlen(rhs);
		}
	else
		*(rhs++)='\0';
	r->r_low=(strlen(lhs)>0)?atoi(lhs):IR_LOW;
	r->r_high=(strlen(rhs)>0)?atoi(rhs):IR_HIGH;
	return(r);
	}

/*
**	rangechar: parse a list of keyword=number pairs separated
**	by blanks, and stow the results in a keyval structure.
**
**	Arguments:
**		describe	pointer to string range string
**		r		pointer to RANGE object
*/
static RANGE *
rangechar(char *describe, RANGE *r)
	{
	if (bundlepackage(describe,&r->r_parms)==NULL)
		return(NULL);
	return(r);
	}

/*
**	getrange: parse a range description string and return
**	the corresponding RANGE object.
**
**	Arguments:
**		describe	range description string
**
**	Returns:		pointer to RANGE object if successful
**				NULL if error (errno set)
**
**	describe is in the format
**
**		lownumber:highnumber	where lownumber defaults to 0
**					and highnumber defaults to MAXINT
**	or
**		highnumber		implying 0:highnumber
**	or
**		keyword=value ...	where value is a number
**					associated with keyword
*/
RANGE *
getrange(char *describe)
	{
	RANGE *r, *rr;
	char *ptr;
	enum { r_int, r_char } r_type;

	if (describe==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}
	if ((r=(RANGE *)nmalloc(sizeof(RANGE)))==NULL)
		return(NULL);
	r->r_parms=NULL;
	r->r_low=IR_LOW;
	r->r_high=IR_HIGH;
	r_type=r_int;
	for (ptr=describe; *ptr; ptr++)
		if (strchr(IR_INT,*ptr)==NULL)
			{
			r_type=r_char;
			break;
			}
	switch (r_type)
		{
	case r_int:
		rr=rangeint(describe,r);
		break;
	case r_char:
		rr=rangechar(describe,r);
		break;
	default:
		rr=NULL;
		break;
		}
	if (rr==NULL)
		nfree((char *)r);
	r=rr;
	return((RANGE *)r);
	}

/*
**	inrange: return true (1) if the string is in range (either
**	within the numeric range or appears as a keyword). If in
**	range, the corresponding numeric value is returned.
**
**	Arguments:
**		string		string to be range checked
**		result		pointer to integer variable
**		r		pointer to RANGE object
**
**	Returns:		1 if string is in range, 0 if not
**				in range, -1 if error (errno set)
*/
int
inrange(char *string, int *result, RANGE *r)
	{
	int code, answer;
	char *value;

	if ((string==NULL)||(r==NULL))
		{
		seterrno(EINVAL);
		return(-1);
		}
	if (r->r_parms==NULL)
		{
		answer=atoi(string);
		code=((r->r_low<=answer)&&(answer<=r->r_high));
		}
	else
		{
		if ((value=unbundle(string,r->r_parms))==NULL)
			{
			answer=0;
			code=0;
			}
		else
			{
			answer=atoi(value);
			code=1;
			}
		}
	if ((result!=NULL)&&(code==1))
		*result=answer;
	return(code);
	}

/*
**	strange: return the address of the value portion
**	of the keyval pair if the string matches a key in
**	the tree. ("strange"=="string range")
**
**	Arguments:
**		string		string to be range checked
**		r		pointer to RANGE object
**
**	Returns:		pointer to value if string
**				matches a key, NULL otherwise
**				(if the key cannot be found,
**				errno is set to 0, otherwise
**				an error occurred and its set
**				appropriately).
*/
char *
strange(char *string, RANGE *r)
	{
	if ((string==NULL)||(r==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}
	if (r->r_parms==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}
	return(unbundle(string,r->r_parms));
	}

/*
**	freerange: free memory associated with a RANGE object.
**
**	Arguments:
**		r	pointer to RANGE object
**
**	Returns:	nothing useful
*/
int
freerange(RANGE *r)
	{
	(void)freeparcel(r->r_parms);
	nfree((char *)r);
	return(0);
	}
