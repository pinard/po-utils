AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = xpot

noinst_HEADERS = error.h getopt.h hash.h obstack.h parser.h xpot.h
xpot_SOURCES = parser.y scanner.l error.c getopt.c getopt1.c hash.c \
obstack.c xmalloc.c xpot.c

xpot_LDADD = @LEXLIB@

CPPFLAGS = -DYYDEBUG
LFLAGS = -8pd
YFLAGS = -dt

check: xpot
	./xpot $(HOME)/po/po-mode.el
#	./xpot >/dev/null $(EMACSDIR)/lisp/*.el
