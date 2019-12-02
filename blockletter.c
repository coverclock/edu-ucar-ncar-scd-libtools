/*
**	B L O C K L E T T E R
**
**	Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Block Letter
**	Program		TAGS
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Mar 27 15:56:49 MST 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This module contains functions that handle character translation
**	using an ASCII translation table where each line is of the
**	form
**
**		ddd cccccccc...
**
**	where "ddd" is the decimal ASCII code for the input character,
**	and "ccc..." is the ASCII character string being substituted.
**	This function is not supposed to be really general purpose; it
**	is used by the hcgprint routine to generate instructions for
**	drawing block letters using the Dicomed model 48 Hardware
**	Character Generator.
*/

static char copyright[]="Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)blockletter.c	4.11 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "libtstrings.h"
#include "nmalloc.h"
#include "blockletter.h"

#define TRANSLATION	2048	/* longest target string in table */
#define TABLE		256	/* number of characters in trans table */
#define DEFCHAR		' '	/* default character if char undefined */

/*
**	BlockFree: free a dynamically acquired translation table.
*/
int
BlockFree(char **table)
	{
	int ndx;

	if (table!=NULL)
		for (ndx=0; ndx<TABLE; ndx++)
			if (table[ndx]!=NULL)
				(void)nfree(table[ndx]);
	(void)nfree((char *)table);
	return(0);
	}

/*
**	BlockDump: dump out a translation table.
*/
int
BlockDump(char **table, FILE *fp)
	{
	int ndx;

	if (table!=NULL)
		for (ndx=0; ndx<TABLE; ndx++)
			if (table[ndx]!=NULL)
				fprintf(fp,"%lx[%d]=``%s''\n",
					table,ndx,table[ndx]);
	return(0);
	}

/*
**	BlockTable: read in a translation table, probably
**	produced by fcap2hcg for making block letters.
*/
char **
BlockTable(char *path)
	{
	char line[TRANSLATION], *buf, **table;
	FILE *fp;
	int ch, rc, len;

	if ((fp=fopen(path,"r"))==NULL)
		return(NULL);
	if ((table=(char **)nmalloc(TABLE*(sizeof(char *))))==NULL)
		return(NULL);
	for (ch=0; ch<TABLE; ch++)
		table[ch]=NULL;
	for (;;)
		{
		if ((rc=fscanf(fp,"%d%s",&ch,line))==EOF)
			break;
		else if (rc!=2)
			{
			(void)BlockFree(table);
			seterrno(EINVAL);
			return(NULL);
			}
		if ((ch<0)||(ch>=TABLE))
			{
			(void)BlockFree(table);
			seterrno(ERANGE);
			return(NULL);
			}
		len=strlen(line);
		if (len>=TRANSLATION)
			{
			(void)BlockFree(table);
			seterrno(E2BIG);
			return(NULL);
			}
		if ((buf=(char *)namalloc(len+1))==NULL)
			{
			(void)BlockFree(table);
			return(NULL);
			}
		strcpy(buf,line);
		table[ch]=buf;
		}
	(void)fclose(fp);
	return(table);
	}
