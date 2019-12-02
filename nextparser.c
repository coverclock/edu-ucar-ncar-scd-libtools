/*
**	N E X T P A R S E R
**
**	Copyright 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Next Parser
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Jun  1 11:28:04 MDT 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This is a replacement for the old libtools nextword and nextbufword
**	parsing functions. This package is _not_ compatible with the old
**	one. Quite intentional: the interface and implemention of the old
**	package was space and time inefficient, mostly due to the fact that
**	it was far more versatile then it needed to be. This version drives
**	tables via one-access array lookup. There are two interfaces:
**	one for parsing words from a user buffer, and one for parsing words
**	from a user file stream.
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)nextparser.c	4.15 94/05/18 jsloan@ncar.ucar.edu";

#ifndef EBCDIC_LIBTOOLS
static char use_ebcdic[]="@(#)nextparser.c	4.15 94/05/18 Using ASCII";
#else /* EBCDIC_LIBTOOLS */
static char use_ebcdic[]="@(#)nextparser.c	4.15 94/05/18 Using EBCDIC";
#endif /* EBCDIC_LIBTOOLS */

#include "libtools.h"
#include <stdio.h>
#include "transliterate.h"
#include "nextparser.h"

static TRANSLITERATE(table)=TRANSINITIALIZE; /* thread safe: sem_nextparser_libt */
static int first=1;			/* thread safe: sem_nextparser_libt */

/*
**	initialize: initializes parser state tables. This could
**	have been done in static storage at compile time, but this
**	method is easier to maintain.
*/
static void
initialize(void)
	{

	P(sem_nextparser_libt);

	if (!first)
		{
		V(sem_nextparser_libt);
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
	T_S(table,		' ',	ts_som,	ta_tral);
	T_S(table,		'!',	ts_som,	ta_char);
	T_S(table,		'"',	ts_som,	ta_dblq);
	T_S(table,		'#',	ts_som,	ta_comm);
	T_M(table,	'$',	'&',	ts_som,	ta_char);
	T_S(table,		'\'',	ts_som,	ta_sinq);
	T_M(table,	'(',	'/',	ts_som,	ta_char);
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
	T_M(table,	'!',	'/',	ts_non,	ta_char);
	T_M(table,	',',	'?',	ts_non,	ta_char);
	T_M(table,	'`',	':',	ts_non,	ta_char);
	T_S(table,		'#',	ts_non,	ta_comm);
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
	T_S(table,		' ',	ts_som,	ta_tral);
	T_M(table,	'.',	'&',	ts_som,	ta_char);
	T_M(table,	'!',	'/',	ts_som,	ta_char);
	T_M(table,	',',	'?',	ts_som,	ta_char);
	T_M(table,	'`',	':',	ts_som,	ta_char);
	T_S(table,		'#',	ts_som,	ta_comm);
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

	V(sem_nextparser_libt);

	}

/*
**	nextfrombuffer: parses out the next "word" in the user buffer.
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
nextfrombuffer(char *in, char *out, int len)
	{
	if ((in==NULL)||(out==NULL)||(len<=0))
		return(NULL);
	if (first)
		initialize();
	return(translitbuf(table,in,out,len));
	}

/*
**	nextfromfile: returns the next "word" in the user input stream.
**	A "word" may extend beyond physical lines by escaping terminating
**	newlines. It is assumed that the output buffer is large enough
**	to contain the complete word.
**
**	Note that the caller has full control over the position of the
**	seek pointer for the input stream; the caller can, for example,
**	skip all remaining data in the current line after retrieving the
**	first word IFF the functions returns a 1 (otherwise the newline
**	at the end as already been consumed by the parser).
**
**	Arguments:
**		fp	file pointer to input stream.
**		out	resulting output buffer containing word.
**		len	length of buffer out.
**
**	Returns:	1 if more data may exist on same line, -1 if
**			end of file, 0 if end of line. If 0 or -1
**			is returned, nothing useful is placed in
**			the `out' parameter.
*/
int
nextfromfile(FILE *fp, char *out, int len)
	{
	if ((out==NULL)||(len<=0))
		return(-1);
	if (first)
		initialize();
	return(translitfil(table,fp,out,len));
	}
