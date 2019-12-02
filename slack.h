/*
**	S L A C K
**
**	Copyright 1993 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)slack.h	4.1 94/05/16 jsloan@ncar.ucar.edu
**
**	This macro returns the number of bytes it would take to align
**	an object of size "s" bytes to the alignment of "t" bytes. It's
**	a trivial little formula, but I can never remember it. There's
**	also probably some simpler formula.
**
**	e.g. on Sun-4, SLACK(sizeof(int),sizeof(double))==4
*/

#ifndef H_SLACK
#define H_SLACK

#define SLACK(s,t)	(((t)-((s)%(t)))%(t))

#endif /* !H_SLACK */
