/*
**	T R A N S L I T E R A T E
**
**	Copyright 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)transliterate.h	4.11 94/07/18 jsloan@ncar.ucar.edu
**
*/

#ifndef H_TRANSLITERATE
#define H_TRANSLITERATE

#include <stdio.h>
#include "libtvalues.h"
#include "libtproto.h"

typedef unsigned char t_state;
typedef unsigned char t_action;
typedef unsigned char t_char;

/*
**	State table operations
**	(Once built the state tables are stable and static)
**
**	T_S(table, character, state, action)
**	T_M(table, lowcharacter, highcharacter, state, action)
*/

#define T_S(t,c,q,s)	(t)[((t_state)(q))][((t_char)(c))]=((t_action)(s))
#define T_M(t,l,h,q,s)	do { \
			unsigned int t_ch; \
			for (t_ch=(l); t_ch<=(h); t_ch++) \
				T_S(t,t_ch,q,s); \
			} while (0)

/*
**	translitfil() return codes
*/

#define	T_EOF		(-1)
#define T_EOL		(0)
#define T_MORE		(1)
#define T_ERR		(2)

/*
**	Transliterator States
*/

#define ts_non		((t_state)0)	/* no characters in output buffer yet */
#define ts_som		((t_state)1)	/* some characters in output buffer */
#define ts_sin		((t_state)2)	/* these characters single quoted */
#define ts_dbl		((t_state)3)	/* these characters double quoted */
#define ts_esc		((t_state)4)	/* this character escaped */
#define ts_com		((t_state)5)	/* skip past comment to end of line */
#define ts_ign		((t_state)6)	/* ignore next character */
#define ts_aft		((t_state)7)	/* skip whitespace up to next token */
#define ts_xit		((t_state)8)	/* output buffer complete */
#define ts_count	9		/* LAST: number of items */

/*
**	Transliterator Actions
*/

#define	ta_skip		((t_action)0)	/* skip white space */
#define	ta_char		((t_action)1)	/* capture characters */
#define	ta_1chr		((t_action)2)	/* capture single character */
#define	ta_sinq		((t_action)3)	/* single quote */
#define	ta_dblq		((t_action)4)	/* double quote */
#define	ta_unqt		((t_action)5)	/* matching unquote */
#define	ta_rest		((t_action)6)	/* restore prev. context w. next char */
#define	ta_cont		((t_action)7)	/* restore prev. context w. this char */
#define	ta_escp		((t_action)8)	/* escape character */
#define	ta_qchr		((t_action)9)	/* quote the following characters */
#define	ta_lchr		((t_action)10)	/* take the next character literally */
#define	ta_echr		((t_action)11)	/* escape the next character */
#define	ta_comm		((t_action)12)	/* comment */
#define	ta_tral		((t_action)13)	/* skip trailing whitespace */
#define	ta_ignr		((t_action)14)	/* ignore next character */
#define	ta_back		((t_action)15)	/* exit, pick up on this character */
#define	ta_done		((t_action)16)	/* exit, pick up on next character */
#define	ta_fini		((t_action)17)	/* exit, input buffer exhausted */
#define	ta_count	18		/* LAST: number of items */

/*
**	State table definition
**
**	e.g.	TRANSLITERATE(table_name)=TRANSINITIALIZE;
*/

#define TRANSLITERATE(t)	t_action t[ts_count][(1<<BITS(char))]
#define	TRANSINITIALIZE		{ta_skip}

/*
**	Entry points, one for buffers, another for files.
*/

extern char *translitbuf LIBTPROTO((TRANSLITERATE(table), char *in, char *out, int length));

extern int   translitfil LIBTPROTO((TRANSLITERATE(table), FILE *fp, char *out, int length));

#endif /* !H_TRANSLITERATE */
