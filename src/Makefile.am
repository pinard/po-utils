AUTOMAKE_OPTIONS = gnits

bin_PROGRAMS = xpot
noinst_HEADERS = parser.h xpot.h
lisp_LISP = po-mode.el

EXTRA_DIST = po-mode.el

xpot_SOURCES = parser.y scanner.l xpot.c
xpot_LDADD = @LEXLIB@ ../lib/libxpot.a

CPPFLAGS = -DYYDEBUG
INCLUDES = -I.. -I$(srcdir) -I$(top_srcdir)/lib
LFLAGS = -8pd
YFLAGS = -dtv

essai-elisp: xpot
#	 xpot=`pwd`/xpot \
#	   && (cd $(HOME)/po/po-mode && $$xpot po-mode.el) > po-mode.pot
#	 xpot=`pwd`/xpot \
#	   && (cd $(EMACSDIR)/lisp/international && find -name '*.el' | xargs $$xpot -d) \
#	     > emacs.pot
	find $(EMACSDIR)/lisp/international -name '*.el' | xargs ./xpot -d \
	  > emacs.pot

essai-python: xpot
	./xpot -ly using.py
