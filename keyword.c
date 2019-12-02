/*
**	K E Y W O R D
**
**	Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Keyword
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Aug 18 13:02:57 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	This module is living proof that nothing is ever simple.
**	The original implementation just used strchr to find the
**	equal-sign that separated keywords and values, but then
**	to make it more general, it was desired that keywords (or
**	values) could contain quoted or escaped equal signs.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)keyword.c	4.18 94/05/18 jsloan@ncar.ucar.edu";

#ifndef EBCDIC_LIBTOOLS
static char use_ebcdic[]="@(#)keyword.c	4.18 94/05/18 using ASCII";
#else /* EBCDIC_LIBTOOLS */
static char use_ebcdic[]="@(#)keyword.c	4.18 94/05/18 using EBCDIC";
#endif /* EBCDIC_LIBTOOLS */

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "transliterate.h"
#include "buffers.h"
#include "keyword.h"

static TRANSLITERATE(table)=TRANSINITIALIZE; /* thread safe: sem_keyword_libt */
static int first=1;			/* thread safe: sem_keyword_libt */

/*
**	initialize: initializes parser state tables. This could
**	have been done in static storage at compile time, but this
**	method is easier to maintain.
*/
static void
initialize(void)
	{

	P(sem_keyword_libt);

	if (!first)
		{
		V(sem_keyword_libt);
		return;
		}

#ifndef EBCDIC_LIBTOOLS
	/*
	**	A S C I I   T A B L E
	*/

	/*
	** No character placed in output buffer yet
	*/

	T_S(table,		'\0',	ts_non,	ta_fini);
	T_S(table,		'\n',	ts_non,	ta_done);
	T_S(table,		'\r',	ts_non,	ta_done);
	T_S(table,		'!',	ts_non,	ta_char);
	T_S(table,		'"',	ts_non,	ta_dblq);
	T_S(table,		'#',	ts_non,	ta_comm);
	T_M(table,	'$',	'&',	ts_non,	ta_char);
	T_S(table,		'\'',	ts_non,	ta_sinq);
	T_M(table,	'(',	'/',	ts_non,	ta_char);
	T_M(table,	'0',	'9',	ts_non,	ta_char);
	T_M(table,	':',	'<',	ts_non,	ta_char);
	T_S(table,		'=',	ts_non,	ta_1chr);
	T_M(table,	'>',	'@',	ts_non,	ta_char);
	T_M(table,	'A',	'Z',	ts_non,	ta_char);
	T_S(table,		'[',	ts_non,	ta_char);
	T_S(table,		'\\',	ts_non,	ta_escp);
	T_M(table,	']',	'`',	ts_non,	ta_char);
	T_M(table,	'a',	'z',	ts_non,	ta_char);
	T_M(table,	'{',	'~',	ts_non,	ta_char);

	/*
	** Inside unquoted string
	*/

	T_S(table,		'\0',	ts_som,	ta_back);
	T_S(table,		'\t',	ts_som,	ta_tral);
	T_S(table,		'\n',	ts_som,	ta_back);
	T_S(table,		'\r',	ts_som,	ta_back);
	T_S(table,		' ',	ts_som,	ta_tral);
	T_S(table,		'!',	ts_som,	ta_char);
	T_S(table,		'"',	ts_som,	ta_dblq);
	T_S(table,		'#',	ts_som,	ta_comm);
	T_M(table,	'$',	'&',	ts_som,	ta_char);
	T_S(table,		'\'',	ts_som,	ta_sinq);
	T_M(table,	'(',	'/',	ts_som,	ta_char);
	T_M(table,	'0',	'9',	ts_som,	ta_char);
	T_M(table,	':',	'<',	ts_som,	ta_char);
	T_S(table,		'=',	ts_som,	ta_back);
	T_M(table,	'>',	'@',	ts_som,	ta_char);
	T_M(table,	'A',	'Z',	ts_som,	ta_char);
	T_S(table,		'[',	ts_som,	ta_char);
	T_S(table,		'\\',	ts_som,	ta_escp);
	T_M(table,	']',	'`',	ts_som,	ta_char);
	T_M(table,	'a',	'z',	ts_som,	ta_char);
	T_M(table,	'{',	'~',	ts_som,	ta_char);

	/*
	** Inside single quoted string
	*/

	T_S(table,		'\0',	ts_sin,	ta_back);
	T_S(table,		'\t',	ts_sin,	ta_qchr);
	T_S(table,		'\n',	ts_sin,	ta_back);
	T_S(table,		'\r',	ts_sin,	ta_back);
	T_S(table,		' ',	ts_sin,	ta_qchr);
	T_S(table,		'!',	ts_sin,	ta_qchr);
	T_S(table,		'"',	ts_sin,	ta_qchr);
	T_S(table,		'#',	ts_sin,	ta_qchr);
	T_M(table,	'$',	'&',	ts_sin,	ta_qchr);
	T_S(table,		'\'',	ts_sin,	ta_unqt);
	T_M(table,	'(',	'/',	ts_sin,	ta_qchr);
	T_M(table,	'0',	'9',	ts_sin,	ta_qchr);
	T_M(table,	':',	'@',	ts_sin,	ta_qchr);
	T_M(table,	'A',	'Z',	ts_sin,	ta_qchr);
	T_S(table,		'[',	ts_sin,	ta_qchr);
	T_S(table,		'\\',	ts_sin,	ta_escp);
	T_M(table,	']',	'`',	ts_sin,	ta_qchr);
	T_M(table,	'a',	'z',	ts_sin,	ta_qchr);
	T_M(table,	'{',	'~',	ts_sin,	ta_qchr);

	/*
	** Inside double quoted string
	*/

	T_S(table,		'\0',	ts_dbl,	ta_back);
	T_S(table,		'\t',	ts_dbl,	ta_qchr);
	T_S(table,		'\n',	ts_dbl,	ta_back);
	T_S(table,		'\r',	ts_dbl,	ta_back);
	T_S(table,		' ',	ts_dbl,	ta_qchr);
	T_S(table,		'!',	ts_dbl,	ta_qchr);
	T_S(table,		'"',	ts_dbl,	ta_unqt);
	T_S(table,		'#',	ts_dbl,	ta_qchr);
	T_M(table,	'$',	'&',	ts_dbl,	ta_qchr);
	T_S(table,		'\'',	ts_dbl,	ta_qchr);
	T_M(table,	'(',	'/',	ts_dbl,	ta_qchr);
	T_M(table,	'0',	'9',	ts_dbl,	ta_qchr);
	T_M(table,	':',	'@',	ts_dbl,	ta_qchr);
	T_M(table,	'A',	'Z',	ts_dbl,	ta_qchr);
	T_S(table,		'[',	ts_dbl,	ta_qchr);
	T_S(table,		'\\',	ts_dbl,	ta_escp);
	T_M(table,	']',	'`',	ts_dbl,	ta_qchr);
	T_M(table,	'a',	'z',	ts_dbl,	ta_qchr);
	T_M(table,	'{',	'~',	ts_dbl,	ta_qchr);

	/*
	** Next character should be escaped
	*/

	T_S(table,		'\0',	ts_esc,	ta_back);
	T_M(table,	'\001',	'\007',	ts_esc,	ta_lchr);
	T_S(table,		'\b',	ts_esc,	ta_lchr);
	T_S(table,		'\t',	ts_esc,	ta_lchr);
	T_S(table,		'\n',	ts_esc,	ta_rest);
	T_M(table,	'\013',	'\014',	ts_esc,	ta_lchr);
	T_S(table,		'\r',	ts_esc,	ta_lchr);
	T_M(table,	'\016',	'\037',	ts_esc,	ta_lchr);
	T_S(table,		' ',	ts_esc,	ta_lchr);
	T_S(table,		'!',	ts_esc,	ta_echr);
	T_S(table,		'"',	ts_esc,	ta_lchr);
	T_S(table,		'#',	ts_esc,	ta_lchr);
	T_M(table,	'$',	'&',	ts_esc,	ta_echr);
	T_S(table,		'\'',	ts_esc,	ta_lchr);
	T_M(table,	'(',	'~',	ts_esc,	ta_echr);
	T_S(table,		'\177',	ts_esc,	ta_lchr);

	/*
	** Inside comment body
	*/

	T_S(table,		'\0',	ts_com,	ta_cont);
	T_S(table,		'\n',	ts_com,	ta_cont);
	T_S(table,		'\r',	ts_com,	ta_cont);
	T_S(table,		'\\',	ts_com,	ta_ignr);

	/*
	** Ignore next character
	*/

	T_S(table,		'\0',	ts_ign,	ta_back);
	T_S(table,		'\t',	ts_ign,	ta_rest);
	T_S(table,		'\n',	ts_ign,	ta_rest);
	T_S(table,		'\r',	ts_ign,	ta_rest);
	T_M(table,	' ',	'~',	ts_ign,	ta_rest);

	/*
	** Skip trailing whitespace
	*/

	T_S(table,		'\0',	ts_aft,	ta_back);
	T_S(table,		'\n',	ts_aft,	ta_back);
	T_S(table,		'\r',	ts_aft,	ta_back);
	T_M(table,	'!',	'~',	ts_aft,	ta_back);
#else /* EBCDIC_LIBTOOLS */
	/*
	**	E B C D I C   T A B L E
	**
	**	Courtesy of Gene Harano, NCAR/SCD, for MVS
	*/

	/*
	** No character placed in output buffer yet
	*/

	T_S(table,		'\0',	ts_non,	ta_fini);
	T_S(table,		'\n',	ts_non,	ta_done);
	T_S(table,		'\r',	ts_non,	ta_done);
	T_M(table,	'.',	'&',	ts_non,	ta_char);
	T_M(table,	'!',	'/',	ts_non,	ta_char);
	T_M(table,	',',	'?',	ts_non,	ta_char);
	T_M(table,	'`',	':',	ts_non,	ta_char);
	T_S(table,		'#',	ts_non,	ta_comm);
	T_S(table,		'@',	ts_non,	ta_char);
	T_S(table,		'\'',	ts_non,	ta_sinq);
	T_S(table,		'=',	ts_non,	ta_1chr);
	T_S(table,		'"',	ts_non,	ta_dblq);
	T_M(table,	'a',	'i',	ts_non,	ta_char);
	T_M(table,	'j',	'r',	ts_non,	ta_char);
	T_M(table,	'~',	'z',	ts_non,	ta_char);
	T_S(table,		'[',	ts_non,	ta_char);
	T_S(table,		']',	ts_non,	ta_char);
	T_M(table,	'{',	'I',	ts_non,	ta_char);
	T_M(table,	'}',	'R',	ts_non,	ta_char);
	T_S(table,		'\\',	ts_non,	ta_escp);
	T_M(table,	'S',	'Z',	ts_non,	ta_char);
	T_M(table,	'0',	'9',	ts_non,	ta_char);

	/*
	** Inside unquoted string
	*/

	T_S(table,		'\0',	ts_som,	ta_back);
	T_S(table,		'\t',	ts_som,	ta_tral);
	T_S(table,		'\n',	ts_som,	ta_back);
	T_S(table,		'\r',	ts_som,	ta_back);
	T_S(table,		' ',	ts_som,	ta_tral);
	T_M(table,	'.',	'&',	ts_som,	ta_char);
	T_M(table,	'!',	'/',	ts_som,	ta_char);
	T_M(table,	',',	'?',	ts_som,	ta_char);
	T_M(table,	'`',	':',	ts_som,	ta_char);
	T_S(table,		'#',	ts_som,	ta_comm);
	T_S(table,		'@',	ts_som,	ta_char);
	T_S(table,		'\'',	ts_som,	ta_sinq);
	T_S(table,		'=',	ts_som,	ta_back);
	T_S(table,		'"',	ts_som,	ta_dblq);
	T_M(table,	'a',	'i',	ts_som,	ta_char);
	T_M(table,	'j',	'r',	ts_som,	ta_char);
	T_M(table,	'~',	'z',	ts_som,	ta_char);
	T_S(table,		'[',	ts_som,	ta_char);
	T_S(table,		']',	ts_som,	ta_char);
	T_M(table,	'{',	'I',	ts_som,	ta_char);
	T_M(table,	'}',	'R',	ts_som,	ta_char);
	T_S(table,		'\\',	ts_som,	ta_escp);
	T_M(table,	'S',	'Z',	ts_som,	ta_char);
	T_M(table,	'0',	'9',	ts_som,	ta_char);

	/*
	** Inside single quoted string
	*/

	T_S(table,		'\0',	ts_sin,	ta_back);
	T_S(table,		'\t',	ts_sin,	ta_qchr);
	T_S(table,		'\n',	ts_sin,	ta_back);
	T_S(table,		'\r',	ts_sin,	ta_back);
	T_S(table,		' ',	ts_sin,	ta_qchr);
	T_M(table,	'.',	'&',	ts_sin,	ta_qchr);
	T_M(table,	'!',	'/',	ts_sin,	ta_qchr);
	T_M(table,	',',	'?',	ts_sin,	ta_qchr);
	T_M(table,	'`',	':',	ts_sin,	ta_qchr);
	T_S(table,		'#',	ts_sin,	ta_qchr);
	T_S(table,		'@',	ts_sin,	ta_qchr);
	T_S(table,		'\'',	ts_sin,	ta_unqt);
	T_S(table,		'=',	ts_sin,	ta_qchr);
	T_S(table,		'"',	ts_sin,	ta_qchr);
	T_M(table,	'a',	'i',	ts_sin,	ta_qchr);
	T_M(table,	'j',	'r',	ts_sin,	ta_qchr);
	T_M(table,	'~',	'z',	ts_sin,	ta_qchr);
	T_S(table,		'[',	ts_sin,	ta_qchr);
	T_S(table,		']',	ts_sin,	ta_qchr);
	T_M(table,	'{',	'I',	ts_sin,	ta_qchr);
	T_M(table,	'}',	'R',	ts_sin,	ta_qchr);
	T_S(table,		'\\',	ts_sin,	ta_escp);
	T_M(table,	'S',	'Z',	ts_sin,	ta_qchr);
	T_M(table,	'0',	'9',	ts_sin,	ta_qchr);

	/*
	** Inside double quoted string
	*/

	T_S(table,		'\0',	ts_dbl,	ta_back);
	T_S(table,		'\t',	ts_dbl,	ta_qchr);
	T_S(table,		'\n',	ts_dbl,	ta_back);
	T_S(table,		'\r',	ts_dbl,	ta_back);
	T_S(table,		' ',	ts_dbl,	ta_qchr);
	T_M(table,	'.',	'&',	ts_dbl,	ta_qchr);
	T_M(table,	'!',	'/',	ts_dbl,	ta_qchr);
	T_M(table,	',',	'?',	ts_dbl,	ta_qchr);
	T_M(table,	'`',	':',	ts_dbl,	ta_qchr);
	T_S(table,		'#',	ts_dbl,	ta_qchr);
	T_S(table,		'@',	ts_dbl,	ta_qchr);
	T_S(table,		'\'',	ts_dbl,	ta_qchr);
	T_S(table,		'=',	ts_dbl,	ta_qchr);
	T_S(table,		'"',	ts_dbl,	ta_unqt);
	T_M(table,	'a',	'i',	ts_dbl,	ta_qchr);
	T_M(table,	'j',	'r',	ts_dbl,	ta_qchr);
	T_M(table,	'~',	'z',	ts_dbl,	ta_qchr);
	T_S(table,		'[',	ts_dbl,	ta_qchr);
	T_S(table,		']',	ts_dbl,	ta_qchr);
	T_M(table,	'{',	'I',	ts_dbl,	ta_qchr);
	T_M(table,	'}',	'R',	ts_dbl,	ta_qchr);
	T_S(table,		'\\',	ts_dbl,	ta_escp);
	T_M(table,	'S',	'Z',	ts_dbl,	ta_qchr);
	T_M(table,	'0',	'9',	ts_dbl,	ta_qchr);

	/*
	** Next character should be escaped
	*/

	T_S(table,		'\0',	ts_esc,	ta_back);
	T_M(table,	'\001',	'\077',	ts_esc,	ta_lchr);
	T_S(table,		'\b',	ts_esc,	ta_lchr);
	T_S(table,		'\t',	ts_esc,	ta_lchr);
	T_S(table,		'\n',	ts_esc,	ta_rest);
	T_S(table,		'\r',	ts_esc,	ta_lchr);
	T_S(table,		' ',	ts_esc,	ta_lchr);
	T_M(table,	'.',	'&',	ts_esc,	ta_echr);
	T_M(table,	'!',	'/',	ts_esc,	ta_echr);
	T_M(table,	',',	'?',	ts_esc,	ta_echr);
	T_M(table,	'`',	':',	ts_esc,	ta_echr);
	T_S(table,		'#',	ts_esc,	ta_lchr);
	T_S(table,		'@',	ts_esc,	ta_echr);
	T_S(table,		'\'',	ts_esc,	ta_lchr);
	T_S(table,		'=',	ts_esc,	ta_echr);
	T_S(table,		'"',	ts_esc,	ta_lchr);
	T_M(table,	'a',	'i',	ts_esc,	ta_echr);
	T_M(table,	'j',	'r',	ts_esc,	ta_echr);
	T_M(table,	'~',	'z',	ts_esc,	ta_echr);
	T_S(table,		'[',	ts_esc,	ta_echr);
	T_S(table,		']',	ts_esc,	ta_echr);
	T_M(table,	'{',	'I',	ts_esc,	ta_echr);
	T_M(table,	'}',	'R',	ts_esc,	ta_echr);
	T_S(table,		'\\',	ts_esc,	ta_echr);
	T_M(table,	'S',	'Z',	ts_esc,	ta_echr);
	T_M(table,	'0',	'9',	ts_esc,	ta_echr);

	/*
	** Inside comment body
	*/

	T_S(table,		'\0',	ts_com,	ta_cont);
	T_S(table,		'\n',	ts_com,	ta_cont);
	T_S(table,		'\r',	ts_com,	ta_cont);
	T_S(table,		'\\',	ts_com,	ta_ignr);

	/*
	** Ignore next character
	*/

	T_S(table,		'\0',	ts_ign,	ta_back);
	T_S(table,		'\t',	ts_ign,	ta_rest);
	T_S(table,		'\n',	ts_ign,	ta_rest);
	T_S(table,		'\r',	ts_ign,	ta_rest);
	T_M(table,	' ',	'9',	ts_ign,	ta_rest);

	/*
	** Skip trailing whitespace
	*/

	T_S(table,		'\0',	ts_aft,	ta_back);
	T_S(table,		'\n',	ts_aft,	ta_back);
	T_S(table,		'\r',	ts_aft,	ta_back);
	T_M(table,	'.',	'9',	ts_aft,	ta_back);
#endif /* EBCDIC_LIBTOOLS */

	first=0;

	V(sem_keyword_libt);

	}

/*
**	keyword: extract the value portion of a keyword=value pair.
**	No whitespace is expected to exist around the equal sign.
**	This is intended to be used to handle keyword=values on
**	the command line. If there is no value portion, the value
**	is a string of strlen() zero.
**
**	N.B.	This was once a trivial little function, but
**		was reimplemented to handle more complicated
**		tokenization correctly. Goes to show you,
**		nothing is ever simple.
**
**	Arguments:
**		string	pointer to keyword=value string
**
**	Returns:	pointer to null terminated value; also, the
**			keyword is null terminated in the original
**			string.
*/
char *
keyword(char *string)
	{
	char *value, *p;
	BUFFER buffer;
	int length;

	if (string==NULL)
		{
		seterrno(EINVAL);
		return(NULL);
		}

	if (first)
		initialize();

	/*
	** Technically, we really only need a buffer of length
	** two, and let translitbuf throw everything else away.
	** But depending on this behavior from translitbuf makes
	** me nervous. The word we're looking for will contain
	** only an equal sign and a terminating nil.
	*/

	if ((buffer=bufalloc(strlen(string)+1))==NULL)
		return(NULL);
	length=buflength(buffer);

	for (value=NULL, p=string; p!=NULL; )
		if ((p=translitbuf(table,p,buffer,length))==NULL)
			break;
		else if (strcmp(buffer,"=")==0)
			{
			value=p-1;
			break;
			}

	if (value!=NULL)
		*(value++)='\0';
	else
		value=string+strlen(string);

	(void)buffree(buffer);

	return(value);
	}
