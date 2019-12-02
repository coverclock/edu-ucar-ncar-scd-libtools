/*
**	@(#)egserialize.c	1.6 94/05/31 jsloan@ncar.ucar.edu
**
**	This program tests the P/V synchronization primitives and
**	the counting semaphore data type as implemented using
**	(currently) OSF DCE pthreads and mutexen. It implements
**	a classic little producer/consumer collaboration where
**	the producer reads input from stdin and places it in a
**	ring buffer, and the consumer takes data from the ring
**	buffer, does some trivial processing, and displays it
**	to stdout.
*/

#include "libtools.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef PTHREADS_LIBTOOLS

#include <assert.h>
#include <ctype.h>

#define RING	(8)

#define END	(0x80)

static unsigned char ring[RING];
static unsigned char *producing;
static unsigned char *consuming;
static SEMAPHORE production;
static SEMAPHORE consumption;

static pthread_addr_t
producer(pthread_addr_t vp)
	{
	int ch;

#ifdef DEBUG
	fprintf(stderr,"egserialize: producer begins\n");
#endif /* DEBUG */

	do
		{
		P(&production);
		ch=getchar();
#ifdef DEBUG
		fprintf(stderr,"egserialize: producer ``%c'' 0x%x\n",
			isascii(ch)&&isprint(ch)?ch:'.',(unsigned)ch);
#endif /* DEBUG */
		*(producing++)=(ch==EOF)?END:ch;
		if (producing>=(ring+RING))
			producing=ring;
		V(&consumption);
		/* pthread_yield(); */
		}
	while (ch!=EOF);

#ifdef DEBUG
	fprintf(stderr,"egserialize: producer ends\n");
#endif /* DEBUG */

	pthread_exit(0);
	}

static pthread_addr_t
consumer(pthread_addr_t vp)
	{
	unsigned char ch;

#ifdef DEBUG
	fprintf(stderr,"egserialize: consumer begins\n");
#endif /* DEBUG */

	do
		{
		P(&consumption);
		ch=*(consuming++);
#ifdef DEBUG
		fprintf(stderr,"egserialize: consumer ``%c'' 0x%x\n",
			isascii(ch)&&isprint(ch)?ch:'.',(unsigned)ch);
#endif /* DEBUG */
		if (consuming>=(ring+RING))
			consuming=ring;
		if (ch!=END)
			{
			if (isascii(ch)&&islower(ch))
				ch=toupper(ch);
			putchar((int)ch);
			}
		V(&production);
		}
	while (ch!=END);

#ifdef DEBUG
	fprintf(stderr,"egserialize: consumer ends\n");
#endif /* DEBUG */

	pthread_exit(0);
	}

main(int argc, char **argv)
	{
	int rc, xc;
	pthread_t produce, consume;

	libtools();

	semInit(&production,RING);
	producing=ring;
	semInit(&consumption,0);
	consuming=ring;

	rc=pthread_create(&produce,pthread_attr_default,producer,
		(pthread_addr_t)NULL);
	assert(rc>=0);
	rc=pthread_create(&consume,pthread_attr_default,consumer,
		(pthread_addr_t)NULL);
	assert(rc>=0);

	rc=pthread_join(produce,(pthread_addr_t)&xc);
	assert(rc>=0);
	rc=pthread_detach(&produce);
	assert(rc>=0);
	rc=pthread_join(consume,(pthread_addr_t)&xc);
	assert(rc>=0);
	rc=pthread_detach(&consume);
	assert(rc>=0);

	exit(0);
	}

#else /* PTHREADS_LIBTOOLS */

main()
	{
	fprintf(stderr,"egserialize: not built with pthreads\n");
	}

#endif /* PTHREADS_LIBTOOLS */
