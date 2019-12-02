/*
**	P O R T F O L I O
**
**	Copyright 1993, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)portfolio.h	4.1 94/05/16 jsloan@ncar.ucar.edu
*/

#ifndef H_PORTFOLIO
#define H_PORTFOLIO

#include <stdio.h>
#include "libtstrings.h"
#include "libtproto.h"

#include "skeyval.h"

typedef SKEYVALS PORTFOLIO;

#define PORTFOLIOQUOTE(string)       (strpbrk(string," =\t")!=NULL)
/* #define PORTFOLIOQUOTE(string)       ((strchr(string,' ')!=NULL)||(strchr(string,'\t')!=NULL)||(strchr(string,'=')!=NULL)) */

/*
** Largest word possible in a portfolio (for performance
** reasons in nextparser(3l), this is not dynamically
** resized as the string is filled, as is common elsewhere)
*/
#define PORTFOLIOWORD	(4096)

extern PORTFOLIO *initportfolio LIBTPROTO((PORTFOLIO *portfolio));
extern PORTFOLIO *stuff LIBTPROTO((char *parm, char *value, PORTFOLIO *portfolio));
extern PORTFOLIO *stuffp LIBTPROTO((BUFFER parm, BUFFER value, PORTFOLIO *portfolio));
extern char *unstuff LIBTPROTO((char *parm, PORTFOLIO *portfolio));
extern PORTFOLIO *restuff LIBTPROTO((PORTFOLIO *from, PORTFOLIO *to));
extern PORTFOLIO *enstuff LIBTPROTO((PORTFOLIO *from, PORTFOLIO *to));
extern int countportfolio LIBTPROTO((PORTFOLIO *portfolio));
extern int freeportfolio LIBTPROTO((PORTFOLIO *portfolio));
extern int travportfolio LIBTPROTO((PORTFOLIO *portfolio, int (*g)(), void *vp));

/*
** The stuff below can be found in the ancillary module portfolio2.
*/

extern int fmtportfolio LIBTPROTO((PORTFOLIO *portfolio, char *fmt, char *buffer));
extern int dumportfolio LIBTPROTO((PORTFOLIO *portfolio, FILE *fp));
extern PORTFOLIO *stufff LIBTPROTO((char *parm, PORTFOLIO *portfolio, char *buffer, char *format,...));
extern char *unstufff LIBTPROTO((char *parm, PORTFOLIO *portfolio, char *format, void *argp));

#endif /* !H_PORTFOLIO */
