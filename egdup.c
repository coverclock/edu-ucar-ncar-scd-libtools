/*
**	@(#)egdup.c	1.2 94/05/19 jsloan@ncar.ucar.edu
*/

#include "libtools.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

main(int argc, char **argv)
	{
	int fd3, fd4, fd9;

#ifdef NODUP_LIBTOOLS
	fprintf(stderr,"egdup: assuming using libtools dup, dup2\n");
#else /* NODUP_LIBTOOLS */
	fprintf(stderr,"egdup: assuming using libc dup, dup2\n");
#endif /* NODUP_LIBTOOLS */

	if ((fd3=dup(0))<0)
		perror("fd3 failed");
	else if (fd3!=3)
		fprintf(stderr,"fd3=%d differ\n",fd3);
	else
		fprintf(stderr,"fd3=%d\n",fd3);

	if ((fd4=dup(1))<0)
		perror("fd4 failed");
	else if (fd4!=4)
		fprintf(stderr,"fd4=%d differ\n",fd4);
	else
		fprintf(stderr,"fd4=%d\n",fd4);

	if ((fd9=dup2(fd3,9))<0)
		perror("fd9 failed");
	else if (fd9!=9)
		fprintf(stderr,"fd9=%d differ\n",fd9);
	else
		fprintf(stderr,"fd9=%d\n",fd9);

	exit(0);
	}
