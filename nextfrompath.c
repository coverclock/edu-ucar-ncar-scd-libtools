/*
**	N E X T F R O M P A T H
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Next From Path
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed May 11 09:43:35 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	This module uses the transliterator to pick out '/' seperated
**	path components.
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)nextfrompath.c	4.2 94/05/18 jsloan@ncar.ucar.edu";

#ifndef EBCDIC_LIBTOOLS
static char use_ebcdic[]="@(#)nextfrompath.c	4.2 94/05/18 using ASCII";
#else /* EBCDIC_LIBTOOLS */
static char use_ebcdic[]="@(#)nextfrompath.c	4.2 94/05/18 using EBCDIC";
#endif /* EBCDIC_LIBTOOLS */

#include "libtools.h"
#include <stdio.h>
#include "errors.h"
#include "transliterate.h"
#include "buffers.h"
#include "nextfrompath.h"

static TRANSLITERATE(table)=TRANSINITIALIZE; /* thread safe: sem_nextfrompath_libt */
static int first=1;			/* thread safe: sem_nextfrompath_libt */

/*
**	initialize: initializes parser state tables. This could
**	have been done in static storage at compile time, but this
**	method is easier to maintain.
*/
static void
initialize(void)
	{

	P(sem_nextfrompath_libt);

	if (!first)
		{
		V(sem_nextfrompath_libt);
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
	T_S(table,		'#',	ts_non,	ta_char);
	T_M(table,	'$',	'&',	ts_non,	ta_char);
	T_S(table,		'\'',	ts_non,	ta_sinq);
	T_M(table,	'(',	'.',	ts_non,	ta_char);
	T_S(table,		'/',	ts_non,	ta_1chr);
	T_M(table,	'0',	'9',	ts_non,	ta_char);
	T_M(table,	':',	'@',	ts_non,	ta_char);
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
	T_S(table,		' ',	ts_som,	ta_char);
	T_S(table,		'!',	ts_som,	ta_char);
	T_S(table,		'"',	ts_som,	ta_dblq);
	T_S(table,		'#',	ts_som,	ta_char);
	T_M(table,	'$',	'&',	ts_som,	ta_char);
	T_S(table,		'\'',	ts_som,	ta_sinq);
	T_M(table,	'(',	'.',	ts_som,	ta_char);
	T_S(table,		'/',	ts_som,	ta_back);
	T_M(table,	'0',	'9',	ts_som,	ta_char);
	T_M(table,	':',	'@',	ts_som,	ta_char);
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
	T_M(table,	'!',	'-',	ts_non,	ta_char);
	T_S(table,		'/',	ts_non,	ta_1chr);
	T_M(table,	',',	'?',	ts_non,	ta_char);
	T_M(table,	'`',	':',	ts_non,	ta_char);
	T_S(table,		'#',	ts_non,	ta_char);
	T_S(table,		'@',	ts_non,	ta_char);
	T_S(table,		'\'',	ts_non,	ta_sinq);
	T_S(table,		'=',	ts_non,	ta_char);
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
	T_S(table,		' ',	ts_som,	ta_char);
	T_M(table,	'.',	'&',	ts_som,	ta_char);
	T_M(table,	'!',	'-',	ts_som,	ta_char);
	T_S(table,		'/',	ts_som,	ta_back);
	T_M(table,	',',	'?',	ts_som,	ta_char);
	T_M(table,	'`',	':',	ts_som,	ta_char);
	T_S(table,		'#',	ts_som,	ta_char);
	T_S(table,		'@',	ts_som,	ta_char);
	T_S(table,		'\'',	ts_som,	ta_sinq);
	T_S(table,		'=',	ts_som,	ta_char);
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
**	nextfrompath: parses out the next "subpath" in the user buffer.
**	The buffer must contain a C-style null-terminated ASCII string.
**	All parsing halts at the end of the string, and the state of
**	parsing is not maintained across calls. It is assumed that the
**	output buffer is large enough to hold the word.
**
**	Arguments:
**		in	current pointer in input buffer.
**		out	resulting output buffer containing word.
**		len	length of buffer out.
**
**	Returns:	new pointer into input buffer, should 
**			replace `in' parameter on next invocation;
**			NULL if at end of buffer, in which case
**			the `out' parameter contains nothing useful.
*/
char *
nextfrompath(char *in, char *out, int len)
	{
	if ((in==NULL)||(out==NULL)||(len<=0))
		return(NULL);
	if (first)
		initialize();
	return(translitbuf(table,in,out,len));
	}
