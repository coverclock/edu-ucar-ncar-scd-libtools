/*
**	P I L E
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Pile
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Dec  8 14:42:19 MST 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	The functions in this module implement the PILE data structure.
**	A PILE is an array of the form "char (*)[][2]" where the first
**	column of each row contains a pointer to a keyword string and
**	the second column contains a pointer to a value string. It
**	is an argv-like structure suitable for sequential access.
**
**	See also pile2.c and pile3.c.
*/

static char copyright[] = "Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)pile.c	4.24 96/05/22 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <stdlib.h>
#include "libtstrings.h"
#include "nmalloc.h"
#include "buffers.h"
#include "pile.h"

/*
**	freepile: free memory associated with a pile.
*/
int
freepile(char **list)
	{
	char **listp;

	if ((listp=list)!=NULL)
		{
		while (*listp)
			{
			(void)buffree(*(listp++));
			if (*listp)
				(void)buffree(*listp);
			listp++;
			}
		(void)nfree((char *)list);
		}

	return(0);
	}

/*
**	indexpile: return a key that is the ASCII numeric string of
**	the given number. Leading zeros are provided. This creates
**	a key which sorts numerically correctly in the ASCII collating
**	sequence.
*/
char *
indexpile(unsigned int ndx, char *buffer)
	{
	sprintf(buffer,"%012d",ndx);
	return(buffer);
	}

/*
**	countpile: return the number of items in the given pile.
*/
int
countpile(PILE list)
	{
	int count;
	PILE ptr;

	for (count=0, ptr=list; *ptr!=NULL; ptr+=2)
		count++;
	return(count);
	}

/*
**	sortpile: sorts a pile by keyword. In conjunction with
**	indexpile(), this is particularly useful for producing a
**	deliberately ordered list.
*/
static int
compare(const void *va, const void *vb)
	{
	char **a, **b;

	a=(char **)va;
	b=(char **)vb;
	return(strcmp(*a,*b));
	}

PILE
sortpile(PILE list)
	{
	int count;

	if ((count=countpile(list))>1)
		(void)qsort((void *)list,count,((sizeof(char *))*2),compare);
	return(list);
	}
