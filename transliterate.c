/*
**	T R A N S L I T E R A T E
**
**	Copyright 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Transliterate
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Nov  7 14:02:32 MST 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This is a simple transliterator useful for building parsers. It
**	drives tables via one-access array lookup. There are two interfaces:
**	one for parsing words from a user buffer, and one for parsing words
**	from a user file stream. The code may appear to be somewhat
**	redundant: the semantics of stream versus buffer parsing is
**	subtly different, and putting some of that code into a common
**	function would have added a function call per character. This may
**	be harder to maintain, but faster.
**
**	Transliterate is implemented as a finite state machine with a
**	stack... which I dimly recall is equivalent to a push down
**	automata. Gee, who'da thought it, a PDA as a parser. I also
**	dimly recall that this means that the syntax of the language it
**	parses is LL(1), but don't quote me on that. Those graduate
**	courses in compiler design, formal languages, and automata
**	theory were a long time ago.
**
**	Note: the original version of these functions didn't do any
**	checking for stack or buffer overflow. This one does. It's
**	a little slower, but is less prone to crash. Note that stack
**	overflow is virtually impossible (if I understand my own
**	code) with a stack larger than the number of possible
**	nested contexts.
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1993, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)transliterate.c	4.11 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include <ctype.h>
#include "transliterate.h"

/*
**	Size of context stack
*/
#define CONTEXTS	64

/*
**	Error operations
*/
#define CLEAR		(err=0)
#define ERROR		(err++)
#define INERROR		(err>0)

/*
**	Context stack operations
*/
#define PRIME(s)	(sp=stack, spp=stack+(CONTEXTS)-1, *sp=(s), CLEAR)
#define PUSH(s)		((sp<spp)?((*(++sp))=(s)):ERROR)
#define POP		(*((sp>stack)?(sp--):sp))
#define POKE(s)		((*sp)=(s))
#define PEEK		(*sp)

/*
**	Output buffer operations
*/
#define APPEND(c)	((l>0)?(*(t++)=((char)(c)), l--):ERROR)
#define INSERT(c)	((l>0)?(*t=((char)(c))):ERROR)

/*
**	translitbuf: parses out the next "word" in the user buffer.
**	The buffer must contain a C-style null-terminated ASCII string.
**	All parsing halts at the end of the string, and the state of
**	parsing is not maintained across calls. It is assumed that the
**	output buffer is large enough to hold the word. If not, the
**	word is silently truncated. (The caller can avoid this by
**	allocating an output buffer at least as large as the input
**	buffer.)
**
**	Arguments:
**		table	transliterator table
**		f	current pointer in input buffer.
**		t	resulting output buffer containing word.
**		l	maximum size of buffer t
**
**	Returns:	new pointer into input buffer, should 
**			replace `in' parameter on next invocation;
**			NULL if at end of buffer, in which case
**			the `out' parameter contains nothing useful.
*/
char *
translitbuf(TRANSLITERATE(table), char *f, char *t, int l)
	{
	t_state state, *sp, *spp, stack[CONTEXTS];
	int err;
	char *next;

	state=ts_non;
	PRIME(state);
	next=NULL;
	for (; state!=ts_xit; f++)
		{
#ifdef TRANSLITERATE_DEBUG
		fprintf(stderr,
"translitbuf: state=%d in=%c(%2.2x) len=%d next=0x%lx sp=%d spp=%d err=%d\n",
			state,iscntrl(*f)?' ':*f,*f,
			l,next,(sp-stack),(spp-stack),err);
#endif /* TRANSLITERATE_DEBUG */
		switch (table[state][((t_char)(*f))])
			{
		case ta_skip:
			break;
		case ta_tral:
			state=ts_aft;
			break;
		case ta_char:
			APPEND(*f);
			state=ts_som;
			break;
		case ta_1chr:
			APPEND(*(f++));
			INSERT('\0');
			next=f;
			state=ts_xit;
			break;
		case ta_qchr:
			APPEND(*f);
			POKE((PEEK==ts_non)?ts_som:PEEK);
			break;
		case ta_lchr:
			APPEND(*f);
			POKE((PEEK==ts_non)?ts_som:PEEK);
			state=POP;
			break;
		case ta_echr:
			APPEND('\\');
			APPEND(*f);
			POKE((PEEK==ts_non)?ts_som:PEEK);
			state=POP;
			break;
		case ta_sinq:
			PUSH(state);
			state=ts_sin;
			break;
		case ta_dblq:
			PUSH(state);
			state=ts_dbl;
			break;
		case ta_unqt:
			POKE((PEEK==ts_non)?ts_som:PEEK);
			state=POP;
			break;
		case ta_cont:
			--f;
			state=POP;
			break;
		case ta_rest:
			state=POP;
			break;
		case ta_escp:
			PUSH(state);
			state=ts_esc;
			break;
		case ta_comm:
			PUSH(state);
			state=ts_com;
			break;
		case ta_ignr:
			PUSH(state);
			state=ts_ign;
			break;
		case ta_back:
			next=f;
			INSERT('\0');
			state=ts_xit;
			break;
		case ta_done:
			next=f+1;
			INSERT('\0');
			state=ts_xit;
			break;
		case ta_fini:
			next=NULL;
			state=ts_xit;
			break;
			}
#ifdef TRANSLITERATE_DEBUG
		fprintf(stderr,
"translitbuf: state=%d out=%c(%2.2x) len=%d next=0x%lx sp=%d spp=%d err=%d\n",
			state,iscntrl(*(t-1))?' ':*(t-1),*(t-1),
			l,next,(sp-stack),(spp-stack),err);
#endif /* TRANSLITERATE_DEBUG */
		}
	return(next);
	}

/*
**	translitfil: returns the next "word" in the user input stream.
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
**		table	transliterator table
**		fp	file pointer to input stream.
**		t	resulting output buffer containing word.
**		l	maximum size of buffer t
**
**	Returns:	1 if more data may exist on same line, -1 if
**			end of file, 0 if end of line. If 0 or -1
**			is returned, nothing useful is placed in
**			the `out' parameter. If 2 is returned,
**			the internal stack or the output buffer
**			overflowed (the former is unlikely). This
**			error condition is indicated, unlike in
**			translitbuf, since here the caller cannot
**			a priori know how big the words in the file
**			are likely to be. 2 also means that something
**			was returned in the buffer (even if partial)
**			and more data may exist on the same line.
*/
int
translitfil(TRANSLITERATE(table), FILE *fp, char *t, int l)
	{
	t_state state, *sp, *spp, stack[CONTEXTS];
	int err;
	int c, rc;

	rc=T_EOF;
	state=ts_non;
	PRIME(state);
	c=getc(fp);
	for (; (state!=ts_xit)&&(c!=EOF); c=(state!=ts_xit)?(getc(fp)):0)
		{
#ifdef TRANSLITERATE_DEBUG
		fprintf(stderr,
"translitfil: state=%d in=%c(%2.2x) len=%d rc=%d sp=%d spp=%d err=%d\n",
			state,iscntrl(c)?' ':c,c,
			l,rc,(sp-stack),(spp-stack),err);
#endif /* TRANSLITERATE_DEBUG */
		switch (table[state][(t_char)c])
			{
		case ta_skip:
			break;
		case ta_tral:
			state=ts_aft;
			break;
		case ta_char:
			APPEND(c);
			state=ts_som;
			break;
		case ta_1chr:
			APPEND(c);
			INSERT('\0');
			rc=T_MORE;
			state=ts_xit;
			break;
		case ta_qchr:
			APPEND(c);
			POKE((PEEK==ts_non)?ts_som:PEEK);
			break;
		case ta_lchr:
			APPEND(c);
			POKE((PEEK==ts_non)?ts_som:PEEK);
			state=POP;
			break;
		case ta_echr:
			APPEND('\\');
			APPEND(c);
			POKE((PEEK==ts_non)?ts_som:PEEK);
			state=POP;
			break;
		case ta_sinq:
			PUSH(state);
			state=ts_sin;
			break;
		case ta_dblq:
			PUSH(state);
			state=ts_dbl;
			break;
		case ta_unqt:
			POKE((PEEK==ts_non)?ts_som:PEEK);
			state=POP;
			break;
		case ta_cont:
			(void)ungetc(c,fp);
			state=POP;
			break;
		case ta_rest:
			state=POP;
			break;
		case ta_escp:
			PUSH(state);
			state=ts_esc;
			break;
		case ta_comm:
			PUSH(state);
			state=ts_com;
			break;
		case ta_ignr:
			PUSH(state);
			state=ts_ign;
			break;
		case ta_back:
			(void)ungetc(c,fp);
			rc=T_MORE;
			INSERT('\0');
			state=ts_xit;
			break;
		case ta_done:
		case ta_fini:
			rc=T_EOL;
			state=ts_xit;
			break;
			}
#ifdef TRANSLITERATE_DEBUG
		fprintf(stderr,
"translitfil: state=%d out=%c(%2.2x) len=%d rc=%d sp=%d spp=%d err=%d\n",
			state,iscntrl(*(t-1))?' ':*(t-1),*(t-1),
			l,rc,(sp-stack),(spp-stack),err);
#endif /* TRANSLITERATE_DEBUG */
		}
	if ((INERROR)&&(rc==T_MORE))
		rc=T_ERR;
	return(rc);
	}
