/*
**	E S C A P E S
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Collapse/Expand Escape Sequences
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Aug 18 10:38:02 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Esclapse collapses all C-style escape sequences in a string,
**	returning a buffer with the collapsed string.
**
**	Escpand provides the complementary function, returning a buffer
**	containing a collapse-able string.
**
**	All parsing is done in a nextparser(3l) and esctoi(3l) compatible
**	manner.
**
**	N.B.	Note the weirdness to try and handle non-ANSI compilers
**		which do not recognize some escape sequences like the
**		"audible alarm" (\a), which has a different encoding in
**		ASCII and EBCDIC.
**
**	N.B.	All the "unsigned" stuff is mostly unneccessary,
**		but merely a prophalactic to prevent some weirdness
**		observed during comparisons and printing octal escapes.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)escapes.c	4.17 94/09/29 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <ctype.h>
#include "errors.h"
#include "buffers.h"
#include "libtstrings.h"
#include "escapes.h"

/*
**	esclapseto: collapse the escape sequences from one
**	string into another.
**
**	esclapse: same as esclapseto, except returns a buffers(3l)
**	string that is guaranteed to be large enough. May deallocate
**	the original string.
**
**	Argument:
**		string	the original string
**		buffer	output buffer
**
**	Returns:	pointer to the new string
*/
char *
esclapseto(char *string, char *buffer)
	{
	unsigned char *from, *to, value, word;
	int ndx;

	if ((string==NULL)||(buffer==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	for (from=(unsigned char *)string, to=(unsigned char *)buffer; *from!='\0'; )
		{
		switch (*from)
			{
		case '\\':
			from++;
			switch(*from)
				{
			case '\0':
				break;
			case '\n':
				from++;
				break;
			case 'a':
				from++;
#ifdef ESCAPES_NOESCA
#ifdef EBCDIC_LIBTOOLS
				*(to++)='\057';
#else /* EBCDIC_LIBTOOLS */
				*(to++)='\007';
#endif /* EBCDIC_LIBTOOLS */
#else /* ESCAPES_NOESCA */
				*(to++)='\a';
#endif /* ESCAPES_NOESCA */
				break;
			case 'b':
				from++;
				*(to++)='\b';
				break;
			case 'f':
				from++;
				*(to++)='\f';
				break;
			case 'n':
				from++;
				*(to++)='\n';
				break;
			case 'r':
				from++;
				*(to++)='\r';
				break;
			case 't':
				from++;
				*(to++)='\t';
				break;
			case 'v':
				from++;
#ifdef ESCAPES_NOESCV
#ifdef EBCDIC_LIBTOOLS
				*(to++)='\013';
#else /* EBCDIC_LIBTOOLS */
				*(to++)='\013';
#endif /* EBCDIC_LIBTOOLS */
#else /* ESCAPES_NOESCV */
				*(to++)='\v';
#endif /* ESCAPES_NOESCV */
				break;
			case 'x':
				from++;
				for (word=0,ndx=2; ndx>0; ndx--)
					{
					switch (*from)
						{
					case '0':
						value=0x0;
						break;
					case '1':
						value=0x1;
						break;
					case '2':
						value=0x2;
						break;
					case '3':
						value=0x3;
						break;
					case '4':
						value=0x4;
						break;
					case '5':
						value=0x5;
						break;
					case '6':
						value=0x6;
						break;
					case '7':
						value=0x7;
						break;
					case '8':
						value=0x8;
						break;
					case '9':
						value=0x9;
						break;
					case 'a':
					case 'A':
						value=0xA;
						break;
					case 'b':
					case 'B':
						value=0xB;
						break;
					case 'c':
					case 'C':
						value=0xC;
						break;
					case 'd':
					case 'D':
						value=0xD;
						break;
					case 'e':
					case 'E':
						value=0xE;
						break;
					case 'f':
					case 'F':
						value=0xF;
						break;
					default:
						ndx=0;
						break;
						}
					if (ndx>0)
						{
						from++;
						word<<=4;
						word|=value;
						}
					}
				*(to++)=word;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
				for (word=0,ndx=3; ndx>0; ndx--)
					{
					switch (*from)
						{
					case '0':
						value=00;
						break;
					case '1':
						value=01;
						break;
					case '2':
						value=02;
						break;
					case '3':
						value=03;
						break;
					case '4':
						value=04;
						break;
					case '5':
						value=05;
						break;
					case '6':
						value=06;
						break;
					case '7':
						value=07;
						break;
					default:
						ndx=0;
						break;
						}
					if (ndx>0)
						{
						from++;
						word<<=3;
						word|=value;
						}
					}
				*(to++)=word;
				break;
			default:
				*(to++)=(*(from++));
				break;
				}
			break;
		default:
			*(to++)=(*(from++));
			}
		}
	*to='\0';

	return((char *)buffer);
	}

BUFFER
esclapsenew(char *old)
	{
	BUFFER new;

	if (old==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	if ((new=bufalloc(strlen(old)+1))==NULL)
		return(NULL);

	if (esclapseto(old,(char *)new)==NULL)
		{
		(void)buffree(new);
		return(NULL);
		}

	return(new);
	}

BUFFER
esclapse(BUFFER old)
	{
	BUFFER new;

	if ((new=esclapsenew(old))!=NULL)
		(void)buffree(old);
	return(new);
	}

/*
**	escpand: return a copy of a string modified to conform to
**	collapse-able format.
**
**	escpandto: same as escpand() but caller supplies output
**	buffer.
**
**	Arguments:
**		string	original string
**		buffer	output buffer
**
**	Returns:	new string in a (static) buffer
*/
char *
escpandto(char *string, char *buffer)
	{
	unsigned char *from, *to;

	if ((string==NULL)||(buffer==NULL))
		{
		seterrno(EINVAL);
		return(NULL);
		}

	for (from=(unsigned char *)string, to=(unsigned char *)buffer; *from!='\0'; )
		/*
		** Note using strchr(3) in reverse to see if
		** the character is one from a particular set.
		** Pretty damn clever, eh? :-)
		*/
		if (strchr("\"\'\\#=?",*from)!=NULL)
			{
			*(to++)='\\';
			*(to++)=(*(from++));
			}
		/*
		** Foolish isprint(3) doesn't recognize a tab as
		** printable. Inconvenient. If there's other
		** characters that isprint(3) rejects, that you
		** don't want escaped, add them to the string in
		** strchr(3) immediately below.
		*/
		else if (strchr("	",*from)!=NULL)
			*(to++)=(*(from++));
		else if (isprint(*from))
			*(to++)=(*(from++));
		else
			{
			switch (*from)
				{
#ifdef ESCAPES_NOESCA
#ifdef EBCDIC_LIBTOOLS
			case '\057':
#else /* EBCDIC_LIBTOOLS */
			case '\007':
#endif /* EBCDIC_LIBTOOLS */
#else /* ESCAPES_NOESCA */
			case '\a':
#endif /* ESCAPES_NOESCA */
				*(to++)='\\';
				*(to++)='a';
				break;
			case '\b':
				*(to++)='\\';
				*(to++)='b';
				break;
			case '\f':
				*(to++)='\\';
				*(to++)='f';
				break;
			case '\n':
				*(to++)='\\';
				*(to++)='n';
				break;
			case '\r':
				*(to++)='\\';
				*(to++)='r';
				break;
			case '\t': /* Never happens: tabs handled above */
				*(to++)='\\';
				*(to++)='t';
				break;
#ifdef ESCAPES_NOESCV
#ifdef EBCDIC_LIBTOOLS
			case '\013':
#else /* EBCDIC_LIBTOOLS */
			case '\013':
#endif /* EBCDIC_LIBTOOLS */
#else /* ESCAPES_NOESCV */
			case '\v':
#endif /* ESCAPES_NOESCV */
				*(to++)='\\';
				*(to++)='v';
				break;
			default:
				(void)sprintf((char *)to,"\\%3.3o",*from);
				to+=4;
				break;
				}
			from++;
			}
	*to='\0';

	return((char *)buffer);
	}

BUFFER
escpandnew(char *old)
	{
	BUFFER new;

	if (old==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	if ((new=bufalloc((strlen(old)*(sizeof("\\000")-1))+1))==NULL)
		return(NULL);

	if (escpandto(old,(char *)new)==NULL)
		{
		(void)buffree(new);
		return(NULL);
		}

	return(new);
	}

BUFFER
escpand(BUFFER old)
	{
	BUFFER new;

	if ((new=escpandnew(old))!=NULL)
		(void)buffree(old);
	return(new);
	}

/*
**	esclapsein: special version of esclapse that collapses an
**	escape sequence in place, and can handle embedded null
**	(\0) sequences. Returns the length of the resulting string,
**	which should be of equal or shorter length than the original.
*/
int
esclapsein(char *string, int length)
	{
	unsigned char *from, *past, *to, value, word;
	int ndx;

	from=(unsigned char *)string;
	to=(unsigned char *)string;
	past=from+length;
	while (from<past)
		{
		switch (*from)
			{
		case '\\':
			from++;
			if (from>=past)
				break;
			switch(*from)
				{
			case '\n':
				from++;
				break;
			case 'a':
				from++;
#ifdef ESCAPES_NOESCA
#ifdef EBCDIC_LIBTOOLS
				*(to++)='\057';
#else /* EBCDIC_LIBTOOLS */
				*(to++)='\007';
#endif /* EBCDIC_LIBTOOLS */
#else /* ESCAPES_NOESCA */
				*(to++)='\a';
#endif /* ESCAPES_NOESCA */
				break;
			case 'b':
				from++;
				*(to++)='\b';
				break;
			case 'f':
				from++;
				*(to++)='\f';
				break;
			case 'n':
				from++;
				*(to++)='\n';
				break;
			case 'r':
				from++;
				*(to++)='\r';
				break;
			case 't':
				from++;
				*(to++)='\t';
				break;
			case 'v':
				from++;
#ifdef ESCAPES_NOESCV
#ifdef EBCDIC_LIBTOOLS
				*(to++)='\013';
#else /* EBCDIC_LIBTOOLS */
				*(to++)='\013';
#endif /* EBCDIC_LIBTOOLS */
#else /* ESCAPES_NOESCV */
				*(to++)='\v';
#endif /* ESCAPES_NOESCV */
				break;
			case 'x':
				from++;
				for (word=0,ndx=2; ndx>0; ndx--)
					{
					if (from>=past)
						break;
					switch (*from)
						{
					case '0':
						value=0x0;
						break;
					case '1':
						value=0x1;
						break;
					case '2':
						value=0x2;
						break;
					case '3':
						value=0x3;
						break;
					case '4':
						value=0x4;
						break;
					case '5':
						value=0x5;
						break;
					case '6':
						value=0x6;
						break;
					case '7':
						value=0x7;
						break;
					case '8':
						value=0x8;
						break;
					case '9':
						value=0x9;
						break;
					case 'a':
					case 'A':
						value=0xA;
						break;
					case 'b':
					case 'B':
						value=0xB;
						break;
					case 'c':
					case 'C':
						value=0xC;
						break;
					case 'd':
					case 'D':
						value=0xD;
						break;
					case 'e':
					case 'E':
						value=0xE;
						break;
					case 'f':
					case 'F':
						value=0xF;
						break;
					default:
						ndx=0;
						break;
						}
					if (ndx>0)
						{
						from++;
						word<<=4;
						word|=value;
						}
					}
				*(to++)=word;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
				for (word=0,ndx=3; ndx>0; ndx--)
					{
					if (from>=past)
						break;
					switch (*from)
						{
					case '0':
						value=00;
						break;
					case '1':
						value=01;
						break;
					case '2':
						value=02;
						break;
					case '3':
						value=03;
						break;
					case '4':
						value=04;
						break;
					case '5':
						value=05;
						break;
					case '6':
						value=06;
						break;
					case '7':
						value=07;
						break;
					default:
						ndx=0;
						break;
						}
					if (ndx>0)
						{
						from++;
						word<<=3;
						word|=value;
						}
					}
				*(to++)=word;
				break;
			default:
				*(to++)=(*(from++));
				break;
				}
			break;
		default:
			*(to++)=(*(from++));
			}
		}

	return(to-(unsigned char *)string);
	}
