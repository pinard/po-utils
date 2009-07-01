/* Translatable strings extractor.
   Copyright © 2000 Progiciels Bourbeau-Pinard inc.
   François Pinard <pinard@iro.umontreal.ca>, 2000.  */

/* For all languages, the syntax is very minimalistic.  It skips over
   well-parenthetised expressions, and such things.  Most work is left to the
   scanner.  Here, we merely seek for predictably translatable strings.  */

%token STRING
%token AWK_STARTER
%token C_STARTER C_IDENTIFIER C_PYTHON_DOCSTRING C_OTHER
%token ELISP_STARTER ELISP_SYMBOL ELISP_OTHER_ATOM
%token PO_STARTER PO_MSGID PO_MSGSTR
%token PERL_STARTER
%token PYTHON_STARTER PYTHON_IDENTIFIER PYTHON_DEFINE PYTHON_OTHER
%token SHELL_STARTER

%{
#include "xpot.h"
static bool is_translatable (void);
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
  yydebug = debug_bison_option;
#endif
}
			  file
			;

file			:
			| AWK_STARTER awk_file
			| C_STARTER c_file
			| ELISP_STARTER elisp_file
			| PERL_STARTER perl_file
			| PO_STARTER po_file
			| PYTHON_STARTER python_file
			| SHELL_STARTER shell_file
			;

/*--------------------------------------------.
| Parsing AWK source is still unimplemented.  |
`--------------------------------------------*/

awk_file		:
			;

/*--------------------------------------------------------.
| Parsing C sources.  We only want _(STRING) constructs.  |
`--------------------------------------------------------*/

c_file			:
			| c_token_list
			;

c_token_list		: c_token
			| c_token_list c_token
			;

c_token			: C_IDENTIFIER
{
  if (is_translatable ())
    translatable_flag = true;
}
			  c_parenthetical
			| '(' ')'
			| '(' c_token_list ')'
			| '[' ']'
			| '[' c_token_list ']'
			| '{' '}'
			| '{' c_token_list '}'
			| C_PYTHON_DOCSTRING {
			    translatable_flag = true;
			    expect_semicolon_flag = true;
			  } c_python_docstring
			| STRING
			| C_OTHER
			;

c_parenthetical		: ')'
			| c_token_list ')'
			;

c_python_docstring	: c_token_list ';' { expect_semicolon_flag = false; }

/*---------------------------------------------------------------------------.
| ELISP parsing.  From atoms, we only need to distinguish symbols.  Opening  |
| delimiters for structured types (byte-code, text properties, etc.) are all |
| folded into either `(' when the closing delimiter is `)' or '[' when the   |
| closing delimiter is ']'.                                                  |
`---------------------------------------------------------------------------*/

elisp_file		: elisp_expr {}
			| elisp_file elisp_expr
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
	translatable_flag = true;
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
	translatable_flag = doc_strings_option;

    default:
      $$.class = $1.class;
    }
  $$.count = $1.count + 1;
}
			| elisp_expr_list error
			;

elisp_expr		: ELISP_SYMBOL
{
  struct symbol lookup;
  struct symbol *entry;

  lookup.string = yytext;
  entry = hash_lookup (symbol_table, &lookup);
  if (entry)
    $$.class = entry->class;
  else
    $$.class = OTHER_SYMBOL;
}
			| STRING
{
  $$.class = S_EXPRESSION;
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

/*----------------------------------------------.
| Parsing Perl sources is still unimplemented.  |
`----------------------------------------------*/

perl_file		:
			;

/*------------------.
| PO file parsing.  |
`------------------*/

po_file			: po_entry
			| po_file po_entry
			;

po_entry		: PO_MSGID
{
  translatable_flag = 1;
}
			  STRING PO_MSGSTR STRING
			;

/*---------------------------------------------------------------------.
| Python parsing.  We only want doc strings and _(STRING) constructs.  |
`---------------------------------------------------------------------*/

python_file		:
{
  translatable_flag = doc_strings_option;
}
			  python_docstring python_fragment_list
			;

python_fragment_list	:
			| python_token_list
			| python_fragment_list python_define
			| python_fragment_list python_define python_token_list
			;

python_define		: PYTHON_DEFINE ':'
{
  translatable_flag = doc_strings_option;
}
			  python_docstring
			| PYTHON_DEFINE '(' python_parenthetical ':'
{
  translatable_flag = doc_strings_option;
}
			  python_docstring
			;

python_docstring	:
			| STRING
			;

python_token_list	: python_token
			| python_token_list python_any_token
			;

python_any_token_list	: python_any_token
			| python_any_token_list python_any_token
			;

python_any_token	: STRING
			| python_token
			;

python_token		: PYTHON_IDENTIFIER
{
  if (is_translatable ())
    translatable_flag = true;
}
			  python_parenthetical
			| '(' ')'
			| '(' python_any_token_list ')'
			| '[' ']'
			| '[' python_any_token_list ']'
			| '{' '}'
			| '{' python_any_token_list '}'
			| PYTHON_OTHER
			| ':'
			;

python_parenthetical	: ')'
			| python_any_token_list ')'
			;

/*-----------------------------------------------.
| Parsing shell sources is still unimplemented.	 |
`-----------------------------------------------*/

shell_file		:
			;
%%
/* Service functions.  */

static bool
is_translatable (void)
{
  char *cursor = yytext + yyleng;
  char saved;
  struct symbol lookup;
  struct symbol *entry;

  /* Skip backwards over what would normally have been right context.  */
  if (cursor > yytext && cursor[-1] == '(')
    {
      cursor--;
      while (cursor > yytext && (cursor[-1] == ' ' || cursor[-1] == '\t'
				 || cursor[-1] == '\n'))
	cursor--;
    }

  /* Patch a NUL just for the time of the lookup.  */
  lookup.string = yytext;
  saved = *cursor;
  *cursor = '\0';
  entry = hash_lookup (symbol_table, &lookup);
  *cursor = saved;

  /* Tell if a mark symbol.  */
  return entry && entry->class == MARK_SYMBOL;
}
