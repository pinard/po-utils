AUTOMAKE_OPTIONS = gnits

bin_PROGRAMS = xpot

noinst_HEADERS = parser.h xpot.h
xpot_SOURCES = parser.y scanner.l xpot.c

INCLUDES = -I.. -I$(srcdir) -I$(top_srcdir)/lib
xpot_LDADD = @LEXLIB@ ../lib/libxpot.a

CPPFLAGS = -DYYDEBUG
LFLAGS = -8pd
YFLAGS = -dtv

## essai-elisp: xpot
## 	   xpot=`pwd`/xpot \
## 	     && (cd $(HOME)/po/po-mode && $$xpot po-mode.el) > po-mode.pot
## 	   xpot=`pwd`/xpot \
## 	     && (cd $(EMACSDIR)/lisp \
## 		 && find -name '*.el' | egrep -v '/(language|international)/' \
## 		   | xargs $$xpot) > emacs.pot

essai-python: xpot
	./xpot -ly using.py
