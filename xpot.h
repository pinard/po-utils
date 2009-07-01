#include "hash.h"

#define _(String) String

/* Program options.  */

extern int debug_lex;
extern int debug_yacc;

extern hash_table symbol_table;

/* PO file entries.  */

struct reference
{
  struct reference *next;
  const char *file_name;
  int line_number;
};

struct po_entry
{
  struct po_entry *next;
  struct reference *reference_list;
  const char *msgid;
};

extern struct po_entry *po_entry_list;

/* Scanner interface.  */

enum language
{
  LANGUAGE_AWK,			/* Awk script */
  LANGUAGE_BASH,		/* Shell script */
  LANGUAGE_C,			/* C source */
  LANGUAGE_ELISP,		/* Emacs LISP */
  LANGUAGE_PERL,		/* Perl script */
  LANGUAGE_PO			/* Translations */
};

#if YYTEXT_POINTER
extern char *yytext;
#else
extern yytext[];
#endif

extern int translatable_flag;

/* Parser interface.  */



enum elisp_expr_class
{
  DEFINE_SYMBOL,		/* defun, defvar, defmacro, etc. */
  MARK_SYMBOL,			/* _, translate-string, message, etc. */
  OTHER_SYMBOL,			/* symbol other than those above */
  S_EXPRESSION			/* generic LISP expression */
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
