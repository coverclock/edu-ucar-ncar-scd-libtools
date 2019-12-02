########################################################################
#
#			T O O L S   L I B R A R Y
#
#				4 . 5
#
#			jsloan@ncar.ucar.edu
#
#	Copyright 1990-1996 University Corporation for Atmospheric Research
#			All Rights Reserved
#
#	@(#)Makefile	4.123 96/06/18 jsloan@ncar.ucar.edu
#
#	Releases:
#		4	ANSI-C/POSIX compliant release w. DCE pthread support
#		3	MVS portable release
#		2	Thread-safe release
#		1	Cleaned-up "research" release
#		0	Original TAGS production release
#
#	Releases of this package have been tested on the following
#	platforms. However, because many of these machines were loaners
#	and therefore only available for a limited time, the portability
#	of the most recent release has not been established. In general,
#	however, portability has become less and less a concern as
#	vendors become more ANSI C and POSIX compliant. Current
#	platforms on which libtools is actively supported and being used
#	for projects are marked with a (*).
#
#		SMI Sun-3	SunOS 4.x
#		SMI Sun-4	SunOS 4.x w. GCC	(*)
#		DEC VAX		Ultrix-32 V3.x
#		DEC Alpha	OSF/1
#		IBM 370		AIX/370 1.x
#		IBM RS6000	AIX 3.x			(*)
#		H-P HP9000	HP-UX A.B8.05
#		IBM 3090	MVS/XA			(* subset)
#		CRAY Y-MP	UNICOS 8.x		(*)
#		SGI IP19	IRIX 5.3
#		Dell P133c XPS	Linux 2.1.13
#		SMI Ultra-2	Solaris 5.5.1
#		NEC SX-4	SUPER-UX 6.1
#
########################################################################

ROOT=		/crestone/home/jsloan
NAME=		libtools

########################################################################
#	Symbolic Definitions 
########################################################################

INCLUDE=	$(ROOT)/include
LIB=		$(ROOT)/lib
PRIVATE=	$(NAME).a
PUBLIC=		$(LIB)/$(PRIVATE)
LIBS=		$(PUBLIC) -lm -lcurses -ltermcap
SRC=		.
BIN=		.
MAN=		$(ROOT)/man
TMPDIR=		.

MVSCDIR=	./mvs_c
MVSHDIR=	./mvs_h
MVSPFILE=	./mvs_proc

DCENAME=	libtoolsdce
DCEPRIVATE=	$(DCENAME).a
DCEPUBLIC=	$(LIB)/$(DCEPRIVATE)
DCELIBS=	$(DCEPUBLIC) -lm -lcurses -ltermcap

########################################################################
#	File Definitions
########################################################################

# Notes:
#
# C files beginning with "eg" are examples and are used for regression
# testing when changes are made to the library.
#
# C files whose names end in a number (except for "eg" files), for
# example parcel2.c, are cases where the functions were split across
# multiple modules. This was usually done to keep a minimal set of
# functions in the main module (e.g. parcel.c) and the lesser used
# functions in the ancillary modules (e.g. parcel2.c). This can reduce
# the image sizes of binaries since frequently only the main module
# needs to be linked into the application.
#
# In the case of the ARGVECTOR, PACKAGE, PACKET, PARMFILE and PILE
# data types, there is a more systematic naming scheme:
#
#	foo.c:		general functions for FOO data type
#	foo2.c:		interfaces between FOOs and PARCEL data type
#	foo3.c:		interfaces between FOOs and PORTFOLIO data type

HEADERS=	argvector.h atoargv.h \
		blockletter.h buffers.h \
		cache.h concats.h \
		debug.h displacement.h \
		ebcdic.h errors.h escapes.h extract.h \
		getpath.h \
		h2i.h h2ic.h hexprint.h \
		inrange.h \
		keyval.h keyword.h \
		libtin.h libtparam.h libtools.h libtproto.h libtstrings.h \
		libttypes.h libtvalues.h \
		lists.h log2.h locksmith.h \
		maybenull.h \
		newmalloc.h nextfrompath.h nextparser.h nmalloc.h \
		offset.h origin.h \
		package.h packet.h parcel.h parcels.h parmfile.h \
		pile.h portfolio.h \
		queues.h \
		rings.h \
		s2i.h s2ic.h serialize.h skeyval.h skiplists.h sigfulldump.h \
		slack.h stacks.h \
		timeclock.h transliterate.h trees.h \
		windowsize.h

GENHEADERS=	libtbase.h

SOURCES=	argvector.c argvector2.c argvector3.c atoargv.c \
		backcopy.c basename.c blockletter.c buffers.c \
		cache.c concats.c \
		debug.c delenv.c dumpargs.c dup.c \
		enuser.c errtxt.c escapes.c extract.c \
		getpath.c \
		h2i.c hexprint.c \
		inrange.c \
		keyval.c keyword.c \
		libtools.c lists.c locksmith.c \
		maybenull.c \
		ncara2e.c ncare2a.c nextfrompath.c nextparser.c nmalloc.c \
		package.c package2.c package3.c packet.c packet2.c \
		packet3.c parcel.c parcel2.c parmfile2.c parmfile3.c \
		pile.c pile2.c pile3.c portfolio.c portfolio2.c \
		queues.c \
		readswrites.c rings.c \
		s2i.c serialize.c skeyval.c skiplists.c sigfulldump.c \
		stacks.c \
		tablexlate.c timeclock.c tobits.c transliterate.c \
		trees.c treesavl.c treesbreadth.c treesdump.c \
		treesstats.c \
		usr_errlist.c \
		windowsize.c

EGSOURCES=	egatoargv.c \
		egbuffers.c \
		egcache.c \
		egdup.c \
		egenuser.c egescapes.c egextract.c \
		eggetpath.c \
		egh2i.c \
		egkeyword.c \
		eglibtstrings.c eglists.c eglocksmith.c \
		egmaybenull.c \
		egnextfrompath.c egnextparser.c egnmalloc.c \
		egoriginoffset.c \
		egpacket.c \
		egparcel1.c egparcel2.c egparcel3.c egparcel4.c \
		egparcel5.c egparcel6.c egparcel7.c egparcel8.c \
		egportfolio1.c egportfolio2.c egportfolio3.c egportfolio4.c \
		egportfolio5.c egportfolio6.c egportfolio7.c egportfolio8.c \
		egqueues.c \
		egrings.c \
		egs2i.c egserialize.c egskiplists.c egslack.c \
		egstacks.c \
		egtimeclock.c egtrees2.c \
		egwindowsize.c

MVSOMIT=	delenv.c dup.c enuser.c locksmith.c readswrites.c \
		timeclock.c \
		egcache.c egenuser.c egh2i.c eglibtstrings.c \
		egparcel7.c egparcel8.c egportfolio7.c egportfolio8.c \
		egrings.c egstacks.c egs2i.c \
		egtimeclock.c egtrees2.c \
		egwindowsize.c

MANPAGES=	argvector.3l atoargv.3l \
		basename.3l buffers.3l \
		cache.3l concats.3l \
		displacement.3l \
		errtxt.3l escapes.3l \
		getpath.3l \
		inrange.3l \
		keyval.3l keyword.3l \
		lists.3l \
		nextfrompath.3l nextparser.3l nmalloc.3l \
		origin.3l \
		package.3l packet.3l parcel.3l parcel2.3l parmfile.3l \
		pile.3l portfolio.3l portfolio2.3l \
		queues.3l \
		readswrites.3l \
		skeyval.3l skiplists.3l stacks.3l \
		timeclock.3l tobits.3l trees.3l

MISC=		ABSTRACT README LICENSE ToDo Makefile genbasetypes.c

STORED=		$(MISC) $(HEADERS) $(SOURCES) $(EGSOURCES) $(MANPAGES) \
		$(SCRIPTS)

GENERATED=	$(GENHEADERS)

DISTRIBUTION=	$(STORED)

OBJECTS=	argvector.o argvector2.o argvector3.o atoargv.o \
		backcopy.o basename.o blockletter.o buffers.o \
		cache.o concats.o \
		debug.o delenv.o dumpargs.o dup.o \
		enuser.o errtxt.o escapes.o extract.o \
		getpath.o \
		h2i.o hexprint.o \
		inrange.o \
		keyval.o keyword.o \
		libtools.o lists.o locksmith.o \
		maybenull.o \
		ncara2e.o ncare2a.o nextfrompath.o nextparser.o nmalloc.o \
		package.o package2.o package3.o packet.o packet2.o \
		packet3.o parcel.o parcel2.o parmfile2.o parmfile3.o \
		pile.o pile2.o pile3.o portfolio.o portfolio2.o \
		queues.o \
		readswrites.o rings.o \
		s2i.o serialize.o skeyval.o skiplists.o sigfulldump.o \
		stacks.o \
		tablexlate.o timeclock.o tobits.o transliterate.o \
		trees.o treesavl.o treesbreadth.o treesdump.o treesstats.o \
		usr_errlist.o \
		windowsize.o

EXAMPLES=	egatoargv \
		egbuffers \
		egcache \
		egdup \
		egenuser egescapes egextract \
		eggetpath \
		egh2i \
		egkeyword \
		eglibtstrings eglists eglocksmith \
		egmaybenull \
		egnextfrompath egnextparser egnmalloc \
		egoriginoffset \
		egpacket \
		egparcel1 egparcel2 egparcel3 egparcel4 \
		egparcel5 egparcel6 egparcel7 egparcel8 \
		egportfolio1 egportfolio2 egportfolio3 egportfolio4 \
		egportfolio5 egportfolio6 egportfolio7 egportfolio8 \
		egqueues \
		egrings \
		egs2i egserialize egskiplists egslack egstacks \
		egtimeclock egtrees2 \
		egwindowsize

SCRIPTS=	cpdate sccsunbundle mvsconvert TestSuite.sh \
		install.rs6k install.sparc

EXAMPLE=	egdummy

TARGET=		install

########################################################################
#	Option Definitions
########################################################################
#
#	It is the stuff in this section that you may want to peruse
#	and modify in order to tailor libtools for your particular
#	configuration. Some combinations of options that have worked
#	on various platforms in the past are commented out. These
#	may offer some useful guidance.
#
#	The following compiler symbols may be defined while building
#	libtools to alter the resulting product. (For the most part,
#	the prefix of each symbol indicates which module it affects.)
#	If the symbol is in the form <MODULE>_<OPTION> then it affects
#	a single module and is probably used only in the .c file. If the
#	symbol is of the form <OPTION>_LIBTOOLS then it affects many
#	modules, may be used in .h files, and can be used when compiling
#	applications which include the .h files.
#
#	TYPE64_LIBTOOLS="foo"	64-bit arithmetic declared as "foo" [1]
#	NAME64_LIBTOOLS="\"foo\""	64-bit arithmetic declared as "foo" [1]
#	EBCDIC_LIBTOOLS		Use EBCDIC (vs. ASCII) character set
#	ERRNO_LIBTOOLS		Use errno functions instead of errno variable
#	INDEX_LIBTOOLS		Libc groks index, rindex (vs. strchr, strrchr)
#	MVS_LIBTOOLS		Use MVS include files (also set EBCDIC_LIBTOOLS)
#	NMALLOC_LIBTOOLS	Use libtools "new" malloc functions
#	NODUP_LIBTOOLS		Libc does not grok dup, dup2 (e.g. AIX OSF DCE)
#	NOLOG2_LIBTOOLS		Libm does not grok log2
#	OLDMALLOC_LIBTOOLS	Libc groks old (char *) malloc functions
#	PROTOTYPE_LIBTOOLS	C compiler groks function prototypes
#	PTHREADS_LIBTOOLS	Applications will use OSF DCE Threads
#	STRINGS_LIBTOOLS	Libc groks strings.h
#
#	BUFFERS_STATS		Maintain buffer statistics
#	KEYVAL_STATS		Maintain keyval statistics
#	SKEYVAL_STATS		Maintain skeyval statistics
#	SKIPLIST_STATS		Maintain skiplist statistics
#
#	CACHE_TRACE		Enable tracing option in cache
#	CPUUSECONDS_CLOCK	System groks clock(3) [2]
#	CPUUSECONDS_GETRUSAGE	System groks getrusage(2) [2]
#	CPUUSECONDS_TIMES	System groks times(2) [2]
#	ENUSER_SETEUID		Libc groks seteuid(2) (e.g. BSD)
#	ENUSER_SETUIDX		Libc groks setuidx(2) (e.g. AIX)
#	ESCAPES_NOESCA		C compiler does not grok '\a' (audible alarm)
#	ESCAPES_NOESCV		C compiler does not grok '\v' (vertical tab)
#	MEMORYBREAK_SBRKCHAR	Declare SBRK as char *
#	MEMORYBREAK_SBRKVOID	Declare SBRK as void *
#	TIMECLOCK_GETTIMEOFDAY	Libc groks gettimeofday
#	TIMECLOCK_NOTIMEZONE	Gettimeofday has no timezone as second parameter
#	WINDOWSIZE_TERMCAP	System groks termcap [3]
#	WINDOWSIZE_TERMDEF	System groks termdef [3]
#	WINDOWSIZE_TERMINFO	System groks terminfo [3]
#	WINDOWSIZE_TIOCGWINSZ	System groks TIOCGWINSZ [3]
#
#	Notes:
#
#	[1]	This is only necessary if your C compiler uses something
#		other than "long" to define a 64-bit integer. For
#		example, there have been systems that used "long long".
#		If used, both symbols have to be define, TYPE64_LIBTOOLS
#		as the C keyword, and NAME64_LIBTOOLS as the character
#		string. For example:
#
#		-DTYPE64_LIBTOOLS="long long"
#		-DNAME64_LIBTOOLS="\"long long\""
#
#	[2]	These definitions are mutually exclusive. Many systems
#		implement both. When you have a choice, choose the
#		POSIX compliant CPUUSECONDS_GETRUSAGE. Second choice
#		is CPUUSECONDS_TIMES. Use CPUUSECONDS_CLOCK only as a
#		last resort.
#
#	[3]	These definitions are _not_ mutually exclusive. If
#		the target system is capable of all of these
#		mechanisms, you can define all of them.
#
#	For the record, I normally try to write everything in ANSI C
#	with function prototypes, and stick to POSIX compliant stuff.
#	Libtools _must_ be installed using an ANSI C compiler that
#	supports function prototypes. However, applications using
#	libtools may or may not use function prototypes, depending on
#	the definition of PROTOTYPE_LIBTOOLS.
#
#	Below, CONDITIONALS are applied to applications built using
#	libtools, and CUSTOMIZATIONS are applied to the installation
#	of libtools itself.
#
########################################################################

CUSTOMIZATIONS=-DBUFFERS_STATS -DKEYVAL_STATS -DSKIPLIST_STATS -DSKEYVAL_STATS -DWINDOWSIZE_TIOCGWINSZ -DWINDOWSIZE_TERMCAP -DCACHE_TRACE -DTIMECLOCK_GETTIMEOFDAY -DENUSER_SETEUID -DMEMORYBREAK_SBRKCHAR -DNOLOG2_LIBTOOLS -DCPUUSECONDS_GETRUSAGE

CONDITIONALS=-DPROTOTYPE_LIBTOOLS

DEBUG=-g
#DEBUG=-O
CC=gcc ### Gnu CC
#CC=acc ### ANSI CC
CSYSTEM=
CFLAGS=$(DEBUG) $(CSYSTEM) $(CUSTOMIZATIONS)
CINCS=-I$(INCLUDE)

AR=ar
ARFLAGS=rv
RANLIB=ranlib
#RANLIB=ar tvs #IRIX, SUPER-UX

LDSYSTEM=
LDFLAGS=$(DEBUG) $(LDSYSTEM)
LDLIBS=$(LIBS)

COMPRESS=gzip -fv

########################################################################
#	default target
########################################################################

all:	$(PRIVATE)

########################################################################
#	Predefined Architecture Specific Targets
#
#	N.B.	Some of these configurations were added to this
#		Makefile after the ports were complete and the
#		platforms were no longer available to be tested
#		upon. Hence, there may be some syntax errors.
#		Also, later releases of the various operating
#		systems may require modifications.
########################################################################

STAT_DEFS=	-DBUFFERS_STATS -DKEYVAL_STATS -DSKIPLIST_STATS \
		-DSKEYVAL_STATS

##
##	SMI/SPARC/SunOS w/o DCE
##

SUNOS_DEFS=	-DWINDOWSIZE_TIOCGWINSZ -DWINDOWSIZE_TERMCAP -DCACHE_TRACE \
		-DTIMECLOCK_GETTIMEOFDAY -DENUSER_SETEUID \
		-DMEMORYBREAK_SBRKCHAR -DCPUUSECONDS_GETRUSAGE

sunos:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(SUNOS_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		CC=gcc \
		DEBUG="$(DEBUG)"

##
##	SMI/SPARC/SOLARIS w/o DCE
##

SOLARIS_DEFS=	-DWINDOWSIZE_TIOCGWINSZ -DWINDOWSIZE_TERMCAP -DCACHE_TRACE \
		-DTIMECLOCK_GETTIMEOFDAY -DENUSER_SETEUID \
		-DMEMORYBREAK_SBRKVOID -DCPUUSECONDS_GETRUSAGE \
		-DNOLOG2_LIBTOOLS

solaris:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(SOLARIS_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		CC="/opt/SUNWspro/bin/cc" \
		DEBUG="$(DEBUG)"

##
##	IBM/RS6K/AIX w/o DCE
##

AIX_DEFS=	-DWINDOWSIZE_TIOCGWINSZ \
		-DWINDOWSIZE_TERMCAP -DCACHE_TRACE \
		-DTIMECLOCK_GETTIMEOFDAY -DENUSER_SETEUID \
		-DNOLOG2_LIBTOOLS -DNODUP_LIBTOOLS \
		-DMEMORYBREAK_SBRKCHAR -DCPUUSECONDS_GETRUSAGE

aix:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(AIX_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		DEBUG="$(DEBUG)" \
		CC="cc -qlanglvl=ansi -qro -qroconst"

##
##	IBM/RS6K/AIX w. DCE
##

AIXDCE_DEFS=	$(AIX_DEFS) -DPTHREADS_LIBTOOLS

AIXDCE_LIBS=	 -lpthreads

aixdce:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(AIX_DEFS)" \
		NAME="$(DCENAME)" \
		LIBS="$(DCELIBS) $(AIXDCE_LIBS)" \
		EXAMPLE=$(EXAMPLE) \
		CC=cc \
		DEBUG="$(DEBUG)"

##
##	DEC/ALPHA/OSF1 w/o DCE
##

OSF1_DEFS=	-DWINDOWSIZE_TIOCGWINSZ \
		-DWINDOWSIZE_TERMCAP -DCACHE_TRACE \
		-DTIMECLOCK_GETTIMEOFDAY -DENUSER_SETEUID \
		-DMEMORYBREAK_SBRKCHAR -DCPUUSECONDS_GETRUSAGE

osf1:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="($STAT_DEFS) $(OSF1_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		CC=cc \
		DEBUG="$(DEBUG)"

##
##	DEC/ALPHA/OSF1 w. DCE
##

OSF1DCE_DEFS=	$(OSF1_DEFS) -DPTHREADS_LIBTOOLS

OSF1DCE_LIBS=	-lpthreads -lcmalib -lmach

osf1dce:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="($STAT_DEFS) $(OSF1DCE_DEFS)" \
		NAME="$(DCENAME)" \
		LIBS="$(DCELIBS) $(OSF1DCE_LIBS)" \
		EXAMPLE=$(EXAMPLE) \
		CC=cc \
		DEBUG="$(DEBUG)"

##
##	CRI/YMP/UNICOS w/o DCE
##
##	N.B.	Ignore warnings from the UNICOS C compiler of the form
##
##		cc-172 cc: WARNING File = <somefile>.c, Line = <someline>
##		No code is generated for a superfluous expression.
##
##	This is the result of some code that is no-op'ed out if
##	DCE is not enabled; the P and V semaphore operations
##	are conditionally compiled as "{}" to preserve correct
##	syntax. The compiler is doing exactly what is intended.
##

UNICOS_DEFS=	-DWINDOWSIZE_TERMCAP -DCACHE_TRACE \
		-DTIMECLOCK_GETTIMEOFDAY -DENUSER_SETEUID \
		-DMEMORYBREAK_SBRKCHAR -DCPUUSECONDS_TIMES \
		-DNOLOG2_LIBTOOLS

unicos:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(UNICOS_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		CC="cc -T," \
		RANLIB="ls -l" \
		DEBUG="$(DEBUG)"

##
##	SGI/IRIX w/o DCE
##

IRIX_DEFS=	-DWINDOWSIZE_TIOCGWINSZ \
		-DWINDOWSIZE_TERMCAP -DCACHE_TRACE \
		-DTIMECLOCK_GETTIMEOFDAY -DENUSER_SETEUID \
		-DMEMORYBREAK_SBRKVOID -DCPUUSECONDS_GETRUSAGE \
		-DNOLOG2_LIBTOOLS -DNODUP_LIBTOOLS

irix:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(IRIX_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		DEBUG="$(DEBUG)" \
		CC="cc" \
		RANLIB="ar tvs"

##
##	NEC/SX-4/SUPER-UX w/o DCE
##

SUX_DEFS=	-DWINDOWSIZE_TERMCAP -DCACHE_TRACE \
		-DTIMECLOCK_GETTIMEOFDAY -DTIMECLOCK_NOTIMEZONE \
		-DMEMORYBREAK_SBRKVOID -DCPUUSECONDS_TIMES \
		-DNOLOG2_LIBTOOLS

sux:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(SUX_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		DEBUG="$(DEBUG)" \
		CC="cc" \
		RANLIB="ar tvs"

##
##	P5/Linux w/o DCE
##

LINUX_DEFS=	-DWINDOWSIZE_TIOCGWINSZ -DWINDOWSIZE_TERMCAP -DCACHE_TRACE \
		-DTIMECLOCK_GETTIMEOFDAY \
		-DENUSER_SETEUID \
		-DMEMORYBREAK_SBRKVOID -DCPUUSECONDS_GETRUSAGE \
		-DNOLOG2_LIBTOOLS

linux:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(LINUX_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		DEBUG="$(DEBUG)" \
		CC="cc" \
		RANLIB="ar tvs"

########################################################################
#	application targets
########################################################################

install: all include $(PUBLIC)

examples:
	for F in $(EXAMPLES); do make example CUSTOMIZATIONS="$(CUSTOMIZATIONS) $(CONDITIONALS)" LIBS="$(LIBS)" EXAMPLE=$$F ROOT=$(ROOT) DEBUG="$(DEBUG)" CC="$(CC)"; done

example:
	make $(EXAMPLE) CUSTOMIZATIONS="$(CUSTOMIZATIONS) $(CONDITIONALS)" LIBS="$(LIBS)" EXAMPLE=$(EXAMPLE) ROOT=$(ROOT) DEBUG="$(DEBUG)" CC="$(CC)"

manpages:	unbundle $(MANPAGES)
	cp $(MANPAGES) $(MAN)/man3

distribution:	unbundle $(DISTRIBUTION)
	tar cvf - $(DISTRIBUTION) > $(TMPDIR)/$(NAME).tar
	$(COMPRESS) $(TMPDIR)/$(NAME).tar

test:	$(EXAMPLES) TestSuite.sh
	sh -x ./TestSuite.sh

libtbase.h: genbasetypes
	echo "/*" > libtbase.h
	echo "**	L I B T B A S E" >> libtbase.h
	echo "*/" >> libtbase.h
	echo "" >> libtbase.h
	echo "#ifndef H_LIBTBASE" >> libtbase.h
	echo "#define H_LIBTBASE" >> libtbase.h
	echo "" >> libtbase.h
	-genbasetypes >> libtbase.h
	echo "" >> libtbase.h
	echo "#endif /* H_LIBTBASE */" >> libtbase.h

genbasetypes:	genbasetypes.c
	$(CC) $(CFLAGS) $(CINCS) -o genbasetypes genbasetypes.c

########################################################################
#	utility targets
########################################################################

clean:
	rm -f $(PRIVATE) *.o a.out core
	-sccs clean

clean-examples:
	rm -f $(EXAMPLES)

depend:	makefile

makefile:	unbundle Makefile
	cat Makefile > makefile
	$(CC) $(CINCS) -M *.c >> makefile

unbundle:	sccsunbundle
	-sh sccsunbundle

lint:	unbundle
	lint -abchu $(CINCS) *.c

include:	$(HEADERS) $(GENHEADERS)
	-mkdir $(INCLUDE)
	for h in $(HEADERS) $(GENHEADERS);do make $(INCLUDE)/$$h HEADER=$$h ROOT=$(ROOT);done

lib:	$(PRIVATE)

########################################################################
#	maintenance targets
########################################################################

$(STORED):
	-test -f $@ || sccs get $@

$(INCLUDE)/$(HEADER):	cpdate $(HEADER)
	-mkdir $(INCLUDE)
	-chmod u+w $(INCLUDE)/$(HEADER)
	-sh cpdate $(INCLUDE)/$(HEADER)
	cp $(HEADER) $(INCLUDE)/$(HEADER)
	chmod u-w $(INCLUDE)/$(HEADER)

install-object: $(OBJECT)
	$(AR) $(ARFLAGS) $(PUBLIC) $(OBJECT)
	-$(RANLIB) $(PUBLIC)

$(PRIVATE):	$(GENHEADERS) $(OBJECTS)
	$(AR) $(ARFLAGS) $(PRIVATE) $(OBJECTS)
	-$(RANLIB) $(PRIVATE)

$(PUBLIC):	cpdate $(PRIVATE)
	-mkdir $(LIB)
	-sh cpdate $(PUBLIC)
	cp $(PRIVATE) $(PUBLIC)
	-$(RANLIB) $(PUBLIC)

$(EXAMPLE):	$(EXAMPLE).o
	$(CC) $(LDFLAGS) -o $(EXAMPLE) $(EXAMPLE).o $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) $(CINCS) -c $<

########################################################################
#	miscellaneous targets
########################################################################

get-manpages:	$(MANPAGES)

get-examples:	$(EGSOURCES)

get-sources:	$(SOURCES)

get-headers:	$(HEADERS) $(GENHEADERS)

get-mvs:	mvsconvert unbundle $(HEADERS) $(SOURCES) $(EGSOURCES)
	-mkdir $(MVSDIR)
	-sh $(BIN)/mvsconvert $(MVSCDIR) $(MVSHDIR) $(MVSPFILE) "$(HEADERS) $(SOURCES) $(EGSOURCES)" "$(MVSOMIT)"

########################################################################
#	Anything after this line is added by ``make depend''
#	and is not part of the production ``Makefile''. If you
#	are editing something past this message, then you are
#	probably editing the wrong file.
########################################################################

