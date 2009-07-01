/* Translatable strings extractor.
   Copyright © 2000 Progiciels Bourbeau-Pinard inc.
   François Pinard <pinard@iro.umontreal.ca>, 2000.  */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#if HAVE_STDBOOL_H
# include <stdbool.h>
#else
typedef enum {false = 0, true = 1} bool;
#endif
#include <error.h>

#include "xalloc.h"
#include "obstack.h"
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free
#include "hash.h"

#define _(String) String

/* Program options.  */

extern bool debug_bison_option;
extern bool debug_flex_option;
extern bool doc_strings_option;

extern Hash_table *hint_table;
extern Hash_table *symbol_table;
extern Hash_table *po_entry_table;
extern struct po_entry *po_entry_list;

/* Language hints.  */

enum language
{
  LANGUAGE_UNKNOWN,		/* not recognised yet */
  LANGUAGE_AWK,			/* awk script */
  LANGUAGE_C,			/* C source */
  LANGUAGE_ELISP,		/* Emacs LISP */
  LANGUAGE_PERL,		/* Perl script */
  LANGUAGE_PO,			/* translations */
  LANGUAGE_PYTHON,		/* Python script */
  LANGUAGE_SHELL		/* shell script */
};

struct hint
{
  const char *hint;		/* file extension, shell name, Emacs mark */
  enum language language;	/* associated language */
};

/* PO file entries.  */

struct reference
{
  struct reference *next;	/* chaining of references */
  const char *file_name;	/* file name of the reference */
  unsigned line_number;		/* line number of the reference */
  const char *comment;		/* associated source comment */
};

struct po_entry
{
  struct po_entry *next;	/* chaining of PO entries */
  const char *msgid;		/* string needing translation */
  struct reference *reference_list; /* wherever this string is used */
};

/* Scanner peculiarities.  */

#if YYTEXT_POINTER
extern char *yytext;
#else
extern yytext[];
#endif
extern int yyleng;

extern bool translatable_flag;
extern bool expect_semicolon_flag;

/* Parser peculiarities for Emacs LISP.  */

enum elisp_expr_class
{
  DEFINE_SYMBOL,		/* defun, defvar, defmacro, etc. */
  MARK_SYMBOL,			/* _, translate-string, message, etc. */
  OTHER_SYMBOL,			/* symbol other than those above */
  S_EXPRESSION			/* generic LISP expression */
};

struct symbol
{
  const char *string;		/* symbol or keyword */
  enum elisp_expr_class class;	/* classification for symbol */
};

struct elisp_expr
{
  enum elisp_expr_class class;	/* classification of expression */
};

struct elisp_expr_list
{
  enum elisp_expr_class class;	/* classification of first subexpression */
  short count;			/* count of how many exprs in list */
};
