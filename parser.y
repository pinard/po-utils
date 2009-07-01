%token AWK_IDENT
%token BASH_IDENT
%token C_IDENTIFIER C_NUMBER C_STRING
%token ELISP_SYMBOL ELISP_OTHER_ATOM
%token PO_MSGID PO_MSGSTR PO_STRING

%{
#include <stdio.h>
#include "xpot.h"

static void save_po_string (void);
%}

%union
{
  struct elisp_expr elisp_expr;
  struct elisp_expr_list elisp_expr_list;
}

%type <elisp_expr> elisp_expr
%type <elisp_expr_list> elisp_expr_list

%%

init			:
{
#if YYDEBUG
  yydebug = debug_yacc;
#endif
}
			  file
			;

file			:
			| awk_file
			| bash_file
			| c_file
			| elisp_file
			| po_file
			;

/* Parsing AWK source is still unimplemented.  */

awk_file		: AWK_IDENT
			;

/* Parsing bash sources is still unimplemented.  */

bash_file		: BASH_IDENT
			;

/* Parsing C sources is still unimplemented.  */

c_file			: C_IDENTIFIER
			;

/* ELISP parsing is kept as simple as possible.  Opening delimiters for
   structured types (byte-code, text properties, etc.) are all folded into
   either `(' when the closing delimiter is `)' or '[' when the closing
   delimiter is ']'.  From atoms, we only need to distinguish symbols.  */

elisp_file		: elisp_expr elisp_expr_list
{
  /* Preventing default action.  */
}
			;

elisp_expr_list		:
{
  $$.count = 0;
}
			| elisp_expr_list elisp_expr
{
  switch ($$.count)
    {
    case 0:
      $$.class = $2.class;
      if ($$.class == MARK_SYMBOL)
	translatable_flag = 1;
      break;

    case 1:
      if ($1.class == DEFINE_SYMBOL && $2.class != OTHER_SYMBOL)
	/* For exemple, `defun' is not special if not defining a symbol.  */
	$$.class = OTHER_SYMBOL;
      else
	$$.class = $1.class;
      break;

    case 2:
      $$.class = $1.class;
      if ($$.class == DEFINE_SYMBOL)
	translatable_flag = 1;

    default:
      $$.class = $1.class;
    }
  $$.count = $1.count + 1;
}
			| elisp_expr_list error
			;

elisp_expr		: ELISP_SYMBOL
{
  enum elisp_expr_class class;

  if (find_entry (&symbol_table, yytext, strlen (yytext), (void **) &class) < 0)
    $$.class = OTHER_SYMBOL;
  else
    $$.class = class;
}
			| ELISP_OTHER_ATOM
{
  $$.class = S_EXPRESSION;
}
			| '[' elisp_expr_list ']'
{
  $$.class = S_EXPRESSION;
}
			| '\'' elisp_expr
{
  $$.class = S_EXPRESSION;
}
			| '(' elisp_expr_list '.' elisp_expr ')'
{
  $$.class = S_EXPRESSION;
}
			| '(' elisp_expr_list ')'
{
  $$.class = S_EXPRESSION;
}
			;

po_file			: po_entry_list
			;

po_entry_list		: po_entry
			| po_entry_list po_entry
			| po_entry_list error
			;

po_entry		: PO_MSGID po_string_list PO_MSGSTR po_string_list
			;

po_string_list		: PO_STRING
			| po_string_list PO_STRING
			;

%%
