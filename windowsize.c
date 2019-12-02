/*
**	W I N D O W   S I Z E
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Window Size
**	Program		libtools
**	Project		Tools Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Apr 29 10:10:44 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)windowsize.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libtools.h"
#include <stdio.h>
#include "libttypes.h"
#include "s2i.h"
#include <unistd.h>
#ifdef WINDOWSIZE_TIOCGWINSZ
#include <sys/ioctl.h>
#endif /* WINDOWSIZE_TIOCGWINSZ */
#ifdef WINDOWSIZE_TERMCAP
#include <curses.h>
#endif /* WINDOWSIZE_TERMCAP */
#ifdef WINDOWSIZE_TERMINFO
#ifndef WINDOWSIZE_TERMCAP
#include <curses.h>
#endif /* !WINDOWSIZE_TERMCAP */
#endif /* WINDOWSIZE_TERMINFO */
#include "windowsize.h"

/*
**	windowsize: try to determine the size of the window that
**	this application is running on.
**
**	This code first looks in the environmental variables
**	LINES and COLUMNS for the screensize, then tries the driver
**	via an ioctl, then to the termcap, then to terminfo. This allows
**	the user to override everything, to resize the window,
**	or to use termcap/terminfo as a last resort. Lines and columns are
**	determined separately so they can be overridden
**	independently.
*/
void
windowsize(int *lp, int *cp)
	{
	char *s;
	int n;
	extern char *getenv();

#ifdef WINDOWSIZE_TIOCGWINSZ
	int irc;
	struct winsize w;
#endif /* WINDOWSIZE_TIOCGWINSZ */

#ifdef WINDOWSIZE_TERMCAP
	int trc;
	char *t;
	char tcap[1200];
#endif /* WINDOWSIZE_TERMCAP */

#ifdef WINDOWSIZE_TERMDEF
	extern char *termdef();
#endif /* WINDOWSIZE_TERMDEF */

#ifdef WINDOWSIZE_TERMINFO
	int frc, err;
#endif /* WINDOWSIZE_TERMINFO */

#ifdef WINDOWSIZE_TIOCGWINSZ
	irc=(-1);
#endif /* WINDOWSIZE_TIOCGWINSZ */

#ifdef WINDOWSIZE_TERMCAP
	trc=(-1);
	t=NULL;
#endif /* WINDOWSIZE_TERMCAP */

#ifdef WINDOWSIZE_TERMINFO
	frc=ERR;
	err=(-1);
#endif /* WINDOWSIZE_TERMINFO */

	if (cp!=NULL)
		if (((s=getenv("COLUMNS"))!=NULL)&&(s2i(s,&n)==1)&&(n>0))
			*cp=n;
#ifdef WINDOWSIZE_TIOCGWINSZ
		else if (((irc=ioctl(1,TIOCGWINSZ,&w))>=0)&&((n=w.ws_col)>0))
			*cp=n;
#endif /* WINDOWSIZE_TIOCGWINSZ */
#ifdef WINDOWSIZE_TERMCAP
		else if (((t=getenv("TERM"))!=NULL)&&((trc=tgetent(tcap,t))>0)&&((n=tgetnum("co"))>0))
			*cp=n;
#endif /* WINDOWSIZE_TERMCAP */
#ifdef WINDOWSIZE_TERMINFO
		else if (((t=getenv("TERM"))!=NULL)&&((frc=setupterm(t,1,&err))==OK)&&(err==1)&&((n=COLS)>0))
			*cp=n;
#endif /* WINDOWSIZE_TERMINFO */
#ifdef WINDOWSIZE_TERMDEF
		else if (((s=termdef(1,'c'))!=NULL)&&(s2i(s,&n)==1)&&(n>0))
			*cp=n;
#endif /* WINDOWSIZE_TERMDEF */
		else
			*cp=0;

	if (lp!=NULL)
		if (((s=getenv("LINES"))!=NULL)&&(s2i(s,&n)==1)&&(n>0))
			*lp=n;
#ifdef WINDOWSIZE_TIOCGWINSZ
		else if ((irc>=0)&&((n=w.ws_row)>0))
			*lp=n;
		else if (((irc=ioctl(1,TIOCGWINSZ,&w))>=0)&&((n=w.ws_row)>0))
			*lp=n;
#endif /* WINDOWSIZE_TIOCGWINSZ */
#ifdef WINDOWSIZE_TERMCAP
		else if ((trc>0)&&((n=tgetnum("li"))>0))
			*lp=n;
		else if (((t=getenv("TERM"))!=NULL)&&((trc=tgetent(tcap,t))>0)&&((n=tgetnum("li"))>0))
			*lp=n;
#endif /* WINDOWSIZE_TERMCAP */
#ifdef WINDOWSIZE_TERMINFO
		else if ((frc==OK)&&(err==1)&&((n=LINES)>0))
			*lp=n;
		else if (((t=getenv("TERM"))!=NULL)&&((frc=setupterm(t,1,&err))==OK)&&(err==1)&&((n=LINES)>0))
			*lp=n;
#endif /* WINDOWSIZE_TERMINFO */
#ifdef WINDOWSIZE_TERMDEF
		else if (((s=termdef(1,'l'))!=NULL)&&(s2i(s,&n)==1)&&(n>0))
			*lp=n;
#endif /* WINDOWSIZE_TERMDEF */
		else
			*lp=0;
	}
