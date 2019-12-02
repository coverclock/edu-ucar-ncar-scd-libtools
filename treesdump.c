/*
**	T R E E S D U M P
**
**	U N D E R   D E V E L O P M E N T
**
**	Copyright 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Trees Dump
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu May 28 12:32:31 MDT 1992
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This module contains additional functions used to manipulate
**	trees (3l), but which are not part of the base trees package.
*/

static char copyright[]="Copyright 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)treesdump.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "lists.h"
#include "trees.h"

/*
**	tdump: dump out the contents of a tree. Since we don't
**	have any idea of the context in which the tree is being
**	used, the best we can do is print addresses.
**
**	tdumpn: dumps a single node of a tree.
*/
int
tdumpn(TREE **nodep, FILE *fp)
	{
	if (nodep==NULL)
		{
		fprintf(fp,"0x%lx",nodep);
		return(-1);
		}
	if (*nodep==NULL)
		{
		fprintf(fp,"0x%lx:0x%lx",nodep,*nodep);
		return(0);
		}
	fprintf(fp,"0x%lx:0x%lx[0x%lx:0x%lx,0x%lx:0x%lx]",
		nodep,*nodep,
		&((*nodep)->t_left),(*nodep)->t_left,
		&((*nodep)->t_right),(*nodep)->t_right);
	return(1);
	}

static void
tprint(FILE *fp, TREE **nodep, int level)
	{
	int n;

	fputs("tdump: ",fp);
	for (n=level; n>0; n--)
		fputc('.',fp);
	(void)tdumpn(nodep,fp);
	fputc('\n',fp);

	/*
	** The left and right branches have seperate if's
	** so it's easier for me to move them around. I
	** can never quite decider whither tis nobler to
	** dump in-order, or post-order, or what have you.
	*/

	if (nodep!=NULL)
		if ((*nodep)!=NULL)
			tprint(fp,&((*nodep)->t_left),level+1);

	if (nodep!=NULL)
		if ((*nodep)!=NULL)
			tprint(fp,&((*nodep)->t_right),level+1);
	}

int
tdump(TREE **treep, FILE *fp)
	{
	fprintf(fp,"tdump(0x%lx,0x%lx)\n",treep,fp);
	tprint(fp,treep,0);
	return(0);
	}
