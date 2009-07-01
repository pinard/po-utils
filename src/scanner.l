/* Translatable strings extractor.
   Copyright (C) 1996, 2000 Progiciels Bourbeau-Pinard inc.
   François Pinard <pinard@iro.umontreal.ca>, 1996.  */

%{
#include "xpot.h"
#include "parser.h"
#include <ctype.h>
#include <string.h>

/* Set to true by either the parser or scanner when next string should be
   considered translatable.  Reset to false when the scanner returns.  */
bool translatable_flag;

/* Error reporting routine.  */
void yyerror (const char *);

/* Pool for all saved comment characters.  */
static struct obstack comment_stack;

/* Pool for all saved string characters.  */
static struct obstack string_stack;

/* Location for next scanning.  */
static const char *current_file_name;
static unsigned current_line_number;

/* Language of file being scanned.  */
static enum language current_language;

/* Location for last scanned string.  */
static const char *string_file_name;
static unsigned  string_line_number;

/* Saving reference lists while reading PO files.  */
static const char *current_reference_file;
static struct reference *current_reference_list = NULL;

static bool starter_sent;	/* if starter token has already been sent */
static bool comment_seen;	/* comment accumulation started */
static bool string_seen;	/* a string is being seen, maybe accumulated */
static bool meta_flag;		/* if M- has been seen */
static bool control_flag;	/* if C- has been seen */
static char escape_value;	/* character value of escaped sequence */

/* Returns.  */
static int prepare_for_return (int, int, int);

#define C_RETURN(Token, Start) \
  return prepare_for_return ((Token), (Start), C_STARTER)
#define ELISP_RETURN(Token, Start) \
  return prepare_for_return ((Token), (Start), ELISP_STARTER)
#define PO_RETURN(Token, Start) \
  return prepare_for_return ((Token), (Start), PO_STARTER)
#define PYTHON_RETURN(Token, Start) \
  return prepare_for_return ((Token), (Start), PYTHON_STARTER)

/* Comments and strings.  */
static void grow_comment (char *, size_t, char);
static struct reference *new_reference (const char *, unsigned);
static void begin_string (bool, int);
static void grow_string (char *, size_t);
static void grow_string_with_character (char);
static void grow_string_with_octal (const char *);
static void grow_string_with_hexadecimal (const char *);

/* Miscellaneous.  */
static void try_setting_language (const char *, size_t);
static void unexpected_character (const char *);
%}

%option noyywrap

%x STATE_AWK
%x STATE_C
%x   STATE_C_PREPROCESSOR STATE_C_COMMENT STATE_C_STRING
%x STATE_ELISP
%x   STATE_ELISP_CHARACTER STATE_ELISP_STRING
%x STATE_PERL
%x STATE_PO
%x   STATE_PO_FLAGS STATE_PO_COMMENT STATE_PO_REFERENCES STATE_PO_STRING
%x STATE_SHELL
%x STATE_PYTHON
%x   STATE_PYTHON_SINGLE_STRING STATE_PYTHON_SINGLE_STRING_RAW
%x   STATE_PYTHON_DOUBLE_STRING STATE_PYTHON_DOUBLE_STRING_RAW
%x   STATE_PYTHON_TRIPLE_STRING STATE_PYTHON_TRIPLE_STRING_RAW
%x   STATE_PYTHON_SEXTUPLE_STRING STATE_PYTHON_SEXTUPLE_STRING_RAW

white			[\t\f ]
word			[_A-Za-z0-9]+
path			[/_A-Za-z0-9]+
identifier		[_A-Za-z][_A-Za-z0-9]*

%%

<<EOF>>			{
  return prepare_for_return (0, YY_START, 0);
}

<INITIAL>{			/* Language still unknown */

  \#!{white}*{path}\/env{white}+{path}  |
  \#!{white}*{path}	{
    char *limit = yytext + yyleng;
    char *cursor = limit;

    while (cursor[-1] != '!' && cursor[-1] != '/'
	   && cursor[-1] != ' ' && cursor[-1] != '\t')
      cursor--;
    try_setting_language (cursor, limit - cursor);
  }

  .*-\*-{white}*{word}  {
    char *limit = yytext + yyleng;
    char *cursor = limit;

    while (cursor[-1] != '-' && cursor[-1] != ' ' && cursor[-1] != '\t')
      cursor--;
    try_setting_language (cursor, limit - cursor);
  }

  .*-\*-.*mode:{white}*{word}  {
    char *limit = yytext + yyleng;
    char *cursor = limit;

    while (cursor[-1] != ':' && cursor[-1] != ' ' && cursor[-1] != '\t')
      cursor--;
    try_setting_language (cursor, limit - cursor);
  }

  .*\n			{
    current_line_number++;
    if (current_line_number > 2)
      {
	yyerror ("Unrecognized language");
	return 0;
      }
  }

  .			{
    yyerror ("Unrecognized language");
    return 0;
  }
}

<STATE_C>{			/* C language */

  {white}+
  \n{white}*		current_line_number++;
  \/\/.*\n		{
    /* C++ comment.  */
    grow_comment (yytext, yyleng, '/');
    current_line_number++;
  }
  \/\/.*		grow_comment (yytext, yyleng, '/');
  {identifier}{white}*\(  {
    /* Identifier.  */
    C_RETURN (C_IDENTIFIER, YY_START);
  }
  {identifier}		|
  [0-9]+		|
  \'\\.\'		|
  \'.\'			|
  [!%&+,-./:<=>?^|~;]+	|
  \*  			{
    C_RETURN (C_OTHER, YY_START);
  }
  [()\[\]\{\}]		{
    /* Bracketing delimiter.  */
    C_RETURN (yytext[0], YY_START);
  }
  \#			BEGIN STATE_C_PREPROCESSOR;
  \/\*			BEGIN STATE_C_COMMENT;
  \"			begin_string (false, STATE_C_STRING);

  .			unexpected_character ("C source");
}

<STATE_C_PREPROCESSOR>{		/* C - preprocessor directive */
  .*\\\n		current_line_number++;
  .*			BEGIN STATE_C;
}

<STATE_C_COMMENT>{		/* C - comment */

  {white}*\*\/		BEGIN STATE_C;
  \n			{
    current_line_number++;
    grow_comment (yytext, yyleng, '\0');
  }
  \*			|
  [^*\n]+		grow_comment (yytext, yyleng, '\0');
}

<STATE_C_STRING>{		/* C - string */

  \"			BEGIN STATE_C;
  \\\n			current_line_number++;
  \\[0-7]{1,3}		grow_string_with_octal (yytext + 1);
  \\x[0-9a-fA-F]{1,2}	grow_string_with_hexadecimal (yytext + 2);
  \\a			grow_string_with_character ('\007');
  \\b			grow_string_with_character ('\b');
  \\d			grow_string_with_character (127);
  \\e			grow_string_with_character (27);
  \\f			grow_string_with_character ('\f');
  \\n			grow_string_with_character ('\n');
  \\r			grow_string_with_character ('\r');
  \\t			grow_string_with_character ('\t');
  \\v			grow_string_with_character ('\v');
  \\\\			grow_string_with_character ('\\');
  \\\"			grow_string_with_character ('"');
  \\.			unexpected_character ("C string escape");
  \n			{
    grow_string_with_character ('\n');
    current_line_number++;
  }
  [^\"\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("C string");
}

<STATE_ELISP>{			/* Emacs LISP language */
				/* FIXME: Broken by MULE files!  */
  {white}+
  \n{white}*		current_line_number++;
  \;.*\n		{
    grow_comment (yytext, yyleng, ';');
    current_line_number++;
  }
  \;.*			grow_comment (yytext, yyleng, ';');

  [(.)\[\]\']		ELISP_RETURN (yytext[0], YY_START);

  `|,|,@|,\.		/* Just ignore quasi-quotation, save for... */
  \',			ELISP_RETURN (ELISP_OTHER_ATOM, YY_START);

  \#\^\[		{
    /* Char-table.  */
    ELISP_RETURN ('[', YY_START);
  }
  \#\[			{
    /* Byte-code.  */
    ELISP_RETURN ('[', YY_START);
  }
  \#\(			{
    /* Text properties.  */
    ELISP_RETURN ('(', YY_START);
  }

  \#@[0-9]+		{
    int counter;
    int character;

    for (counter = atoi (yytext + 2); counter > 0; counter--)
      {
	character = input ();
	if (character == '\n')
	  current_line_number++;
      }
  }

  \#\$			{
    /* Docum from file.  */
    ELISP_RETURN (ELISP_OTHER_ATOM, YY_START);
  }
  \#\'			{
    /* Function.  */
    ELISP_RETURN ('\'', YY_START);
  }
  [-+]?[0-9]+		|
  [-+]?[0-9]+\.[0-9]*	|
  [-+]?\.[0-9]+		{
    /* Number.  */
    ELISP_RETURN (ELISP_OTHER_ATOM, YY_START);
  }

  \?			BEGIN STATE_ELISP_CHARACTER;

  \"			{
    /* Simple string.  */
    begin_string (false, STATE_ELISP_STRING);
  }
  \#\"			{
    /* Translated string.  */
    begin_string (true, STATE_ELISP_STRING);
  }
  \#&\"			{
    /* Boolean vector.  */
    begin_string (false, STATE_ELISP_STRING);
  }

  ([^\000-\040\"\';?()\[\]#]|\\.)+  {
    ELISP_RETURN (ELISP_SYMBOL, YY_START);
  }

  \006			/* Only once in progmodes/cperl-mode.el.  */

  .			unexpected_character ("Emacs LISP source");
}

<STATE_ELISP_CHARACTER>{	/* Emacs LISP - single character */

  \\(\^|[ACHMSs]-)

  \\[0-7]{1,3}		|
  \\x[0-9a-fA-F]{1,2}	|
  \\.			|
  .			{
    /* Single character.  */
    ELISP_RETURN (ELISP_OTHER_ATOM, STATE_ELISP);
  }
  \n			{
    /* Occurs only once in textmodes/texinfmt.el.  */
    current_line_number++;
    ELISP_RETURN (ELISP_OTHER_ATOM, STATE_ELISP);
  }
}

<STATE_ELISP_STRING>{		/* Emacs LISP - string of characters */

  \"			BEGIN STATE_ELISP;
  \\\n			current_line_number++;
  \\(\^|C-)		control_flag = true;
  \\M-			meta_flag = true;
  \\[AHSs]-
  \\[0-7]{1,3}		grow_string_with_octal (yytext + 1);
  \\x[0-9a-fA-F]{1,2}	grow_string_with_hexadecimal (yytext + 2);
  \\a			grow_string_with_character ('\007');
  \\b			grow_string_with_character ('\b');
  \\d			grow_string_with_character (127);
  \\e			grow_string_with_character (27);
  \\f			grow_string_with_character ('\f');
  \\n			grow_string_with_character ('\n');
  \\r			grow_string_with_character ('\r');
  \\t			grow_string_with_character ('\t');
  \\v			grow_string_with_character ('\v');
  \\.			grow_string_with_character (yytext[1]);
  \n			{
    grow_string_with_character ('\n');
    current_line_number++;
  }

  [^\"\\\n]+		{
    if (control_flag || meta_flag)
      {
	grow_string_with_character (yytext[0]);
	if (yyleng > 1)
	  grow_string (yytext + 1, yyleng - 1);
      }
    else
      grow_string (yytext, yyleng);
  }

  .			unexpected_character ("Emacs LISP string");
}

<STATE_PO>{			/* Portable Object (PO) file */

  {white}+
  \n{white}+		current_line_number++;
  ^\#\..*		grow_comment (yytext + 2, yyleng - 2, '\0');
  ^\#,			BEGIN STATE_PO_FLAGS;
  ^\#:			{
    current_reference_file = NULL;
    BEGIN STATE_PO_REFERENCES;
  }
  ^\#			BEGIN STATE_PO_COMMENT;
  ^msgid{white}+	PO_RETURN (PO_MSGID, YY_START);
  ^msgstr{white}+	PO_RETURN (PO_MSGSTR, YY_START);
  \"			begin_string (false, BEGIN STATE_PO_STRING);
}

<STATE_PO_COMMENT>{		/* PO file - translator comment */
  .*\n			{
    current_line_number++;
    BEGIN STATE_PO;
  }
}

<STATE_PO_FLAGS>{		/* PO file - entry flags */
  .*\n			{	/* FIXME: Ignored, should do better...  */
    current_line_number++;
    BEGIN STATE_PO;
  }
}

<STATE_PO_REFERENCES>{		/* PO file - list of references */
  \n			{
    current_line_number++;
    BEGIN STATE_PO;
  }
  {white}+
  [^:]+			current_reference_file = xstrdup (yytext);
  :[0-9]+		{
    if (current_reference_file)
      {
	unsigned value = 0;
	const char *cursor;
	struct reference *reference;

	for (cursor = yytext + 1; *cursor; cursor++)
	  value = 10 * value + *cursor - '0';
	reference = new_reference (current_reference_file, value);
	reference->next = current_reference_list;
	current_reference_list = reference;
      }
  }
}

<STATE_PO_STRING>{		/* PO file - message string */

  \"			BEGIN STATE_PO;
  \\[0-7]{1,3}		grow_string_with_octal (yytext + 1);
  \\x[0-9a-fA-F]{1,2}	grow_string_with_hexadecimal (yytext + 2);
  \\f			grow_string_with_character ('\f');
  \\n			grow_string_with_character ('\n');
  \\r			grow_string_with_character ('\r');
  \\t			grow_string_with_character ('\t');
  \\v			grow_string_with_character ('\v');
  \\\\			grow_string_with_character ('\\');
  \\\"			grow_string_with_character ('"');
  \\.			unexpected_character ("PO string escape");
  \n			unexpected_character ("PO string line");
  [^\"\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("PO string");
}

<STATE_PYTHON>{			/* Python language */

  {white}+
  \n{white}*		current_line_number++;
  \\\n{white}*		current_line_number++;
  \#.*\n			{
    grow_comment (yytext, yyleng, '#');
    current_line_number++;
  }
  \#.*			grow_comment (yytext, yyleng, '#');
  {identifier}{white}*\(  {
    PYTHON_RETURN (PYTHON_IDENTIFIER, YY_START);
  }
  (class|def){white}+{identifier}  {
    PYTHON_RETURN (PYTHON_DEFINE, YY_START);
  }
  [:()\[\]\{\}]		PYTHON_RETURN (yytext[0], YY_START);
  [-+*/%!<>=^~!&|`,.;]+	{
    PYTHON_RETURN (PYTHON_OTHER, YY_START);
  }
  [_A-Za-z0-9]+		{
    PYTHON_RETURN (PYTHON_OTHER, YY_START);
  }

  \'			{	/* 1 */
    begin_string (false, STATE_PYTHON_SINGLE_STRING);
  }
  [rR]\'		{	/* 1r */
    begin_string (false, STATE_PYTHON_SINGLE_STRING_RAW);
  }
  \"			{	/* 2 */
    begin_string (false, STATE_PYTHON_DOUBLE_STRING);
  }
  [rR]\"		{	/* 2r */
    begin_string (false, STATE_PYTHON_DOUBLE_STRING_RAW);
  }
  \'\'\'		{	/* 3 */
    begin_string (false, STATE_PYTHON_TRIPLE_STRING);
  }
  [rR]\'\'\'		{	/* 3r */
    begin_string (false, STATE_PYTHON_TRIPLE_STRING_RAW);
  }
  \'\'\"		{	/* 4 */
    begin_string (true, STATE_PYTHON_DOUBLE_STRING);
  }
  \'\'[rR]\"		{	/* 4r */
    begin_string (true, STATE_PYTHON_DOUBLE_STRING_RAW);
  }
  \"\"\'		{	/* 5 */
    begin_string (true, STATE_PYTHON_SINGLE_STRING);
  }
  \"\"[rR]\'		{	/* 5r */
    begin_string (true, STATE_PYTHON_SINGLE_STRING_RAW);
  }
  \"\"\"		{	/* 6 */
    begin_string (false, STATE_PYTHON_SEXTUPLE_STRING);
  }
  [rR]\"\"\"		{	/* 6r */
    begin_string (false, STATE_PYTHON_SEXTUPLE_STRING_RAW);
  }
  \"\"\'\'\'		{	/* 7 */
    begin_string (true, STATE_PYTHON_TRIPLE_STRING);
  }
  \"\"[rR]\'\'\'	{	/* 7r */
    begin_string (true, STATE_PYTHON_TRIPLE_STRING_RAW);
  }
  \'\'\"\"\"		{	/* 8 */
    begin_string (true, STATE_PYTHON_SEXTUPLE_STRING);
  }
  \'\'[rR]\"\"\"	{	/* 8r */
    begin_string (true, STATE_PYTHON_SEXTUPLE_STRING_RAW);
  }

  .			unexpected_character ("Python source");
}

<STATE_PYTHON_SINGLE_STRING>{	/* 1 */

  \'			BEGIN STATE_PYTHON;
  \\\n			current_line_number++;
  \\[0-7]{1,3}		grow_string_with_octal (yytext + 1);
  \\x[0-9a-fA-F]{1,2}	grow_string_with_hexadecimal (yytext + 2);
  \\a			grow_string_with_character ('\007');
  \\b			grow_string_with_character ('\b');
  \\d			grow_string_with_character (127);
  \\e			grow_string_with_character (27);
  \\f			grow_string_with_character ('\f');
  \\n			grow_string_with_character ('\n');
  \\r			grow_string_with_character ('\r');
  \\t			grow_string_with_character ('\t');
  \\v			grow_string_with_character ('\v');
  \\.			grow_string_with_character (yytext[1]);
  \n			{
    grow_string_with_character ('\n');
    current_line_number++;
  }

  [^\'\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("Python single string");
}

<STATE_PYTHON_SINGLE_STRING_RAW>{ /* 1r */

  \'			BEGIN STATE_PYTHON;
  \n|\\\n		{
    grow_string (yytext, yyleng);
    current_line_number++;
  }
  \\[0-7]{1,3}		|
  \\x[0-9a-fA-F]{1,2}	|
  \\.			|
  [^\'\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("Python single string raw");
}

<STATE_PYTHON_DOUBLE_STRING>{	/* 2 */

  \"			BEGIN STATE_PYTHON;
  \\\n			current_line_number++;
  \\[0-7]{1,3}		grow_string_with_octal (yytext + 1);
  \\x[0-9a-fA-F]{1,2}	grow_string_with_hexadecimal (yytext + 2);
  \\a			grow_string_with_character ('\007');
  \\b			grow_string_with_character ('\b');
  \\d			grow_string_with_character (127);
  \\e			grow_string_with_character (27);
  \\f			grow_string_with_character ('\f');
  \\n			grow_string_with_character ('\n');
  \\r			grow_string_with_character ('\r');
  \\t			grow_string_with_character ('\t');
  \\v			grow_string_with_character ('\v');
  \\.			grow_string_with_character (yytext[1]);
  \n			{
    grow_string_with_character ('\n');
    current_line_number++;
  }

  [^\"\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("Python double string");
}

<STATE_PYTHON_DOUBLE_STRING_RAW>{ /* 2r */

  \"			BEGIN STATE_PYTHON;
  \n|\\\n		{
    grow_string (yytext, yyleng);
    current_line_number++;
  }
  \\[0-7]{1,3}		|
  \\x[0-9a-fA-F]{1,2}	|
  \\.			|
  [^\"\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("Python double string raw");
}

<STATE_PYTHON_TRIPLE_STRING>{	/* 3 */

  \'\'\'		BEGIN STATE_PYTHON;
  \\\n			current_line_number++;
  \\[0-7]{1,3}		grow_string_with_octal (yytext + 1);
  \\x[0-9a-fA-F]{1,2}	grow_string_with_hexadecimal (yytext + 2);
  \\a			grow_string_with_character ('\007');
  \\b			grow_string_with_character ('\b');
  \\d			grow_string_with_character (127);
  \\e			grow_string_with_character (27);
  \\f			grow_string_with_character ('\f');
  \\n			grow_string_with_character ('\n');
  \\r			grow_string_with_character ('\r');
  \\t			grow_string_with_character ('\t');
  \\v			grow_string_with_character ('\v');
  \\.			grow_string_with_character (yytext[1]);
  \n			{
    grow_string_with_character ('\n');
    current_line_number++;
  }

  \'			|
  [^\'\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("Python triple string");
}

<STATE_PYTHON_TRIPLE_STRING_RAW>{ /* 3r */

  \'\'\'		BEGIN STATE_PYTHON;
  \n|\\\n		{
    grow_string (yytext, yyleng);
    current_line_number++;
  }
  \\[0-7]{1,3}		|
  \\x[0-9a-fA-F]{1,2}	|
  \\.			|
  \'			|
  [^\'\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("Python triple string raw");
}

<STATE_PYTHON_SEXTUPLE_STRING>{	/* 6 */

  \"\"\"		BEGIN STATE_PYTHON;
  \\\n			current_line_number++;
  \\[0-7]{1,3}		grow_string_with_octal (yytext + 1);
  \\x[0-9a-fA-F]{1,2}	grow_string_with_hexadecimal (yytext + 2);
  \\a			grow_string_with_character ('\007');
  \\b			grow_string_with_character ('\b');
  \\d			grow_string_with_character (127);
  \\e			grow_string_with_character (27);
  \\f			grow_string_with_character ('\f');
  \\n			grow_string_with_character ('\n');
  \\r			grow_string_with_character ('\r');
  \\t			grow_string_with_character ('\t');
  \\v			grow_string_with_character ('\v');
  \\.			grow_string_with_character (yytext[1]);
  \n			{
    grow_string_with_character ('\n');
    current_line_number++;
  }

  \"			|
  [^\"\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("Python sextuple string");
}

<STATE_PYTHON_SEXTUPLE_STRING_RAW>{ /* 6r */

  \"\"\"		BEGIN STATE_PYTHON;
  \n|\\\n		{
    grow_string (yytext, yyleng);
    current_line_number++;
  }
  \\[0-7]{1,3}		|
  \\x[0-9a-fA-F]{1,2}	|
  \\.			|
  \"			|
  [^\"\\\n]+		grow_string (yytext, yyleng);

  .			unexpected_character ("Python sextuple string raw");
}

%%

/* Processing done just before returning a token.  */

static unsigned
po_entry_hash (const void *void_data, unsigned table_size)
{
  const struct po_entry *data = void_data;

  return hash_string (data->msgid, table_size);
}

static bool
po_entry_compare (const void *void_first, const void *void_second)
{
  const struct po_entry *first = void_first;
  const struct po_entry *second = void_second;

  return strcmp (first->msgid, second->msgid) == 0;
}

static bool
prepare_for_rescan (void)
{
  char *copy = alloca (yyleng);
  char *cursor = copy + yyleng;

  /* Make so the whole last token will be rescanned.  */
  memcpy (copy, yytext, yyleng);
  while (cursor > copy)
    {
      cursor--;
      unput (*cursor);
    }
}

/*--------------------------------------------------------------------------.
| Normally return TOKEN and prepare for a possibly new Flex START state,    |
| correctly adjusting the stack of comments, the stack of strings and the   |
| list of PO file entries.                                                  |
|                                                                           |
| However, if no token has been returned yet for this file, return STARTER  |
| instead and prepare to rescan the last token.  If a string has been seen, |
| return a STRING token and prepare to rescan the last token.               |
`--------------------------------------------------------------------------*/

static int
prepare_for_return (int token, int start, int starter)
{
#if 0
  printf ("** started %d string %d translatable %d token %d start %d\n",
	  starter_sent, string_seen, translatable_flag, token, start);
#endif
  if (!starter_sent)
    {
      starter_sent = true;
      if (token)
	/* If not at end of file, then `unput' is usable.  */
	prepare_for_rescan ();
      return starter;
    }

  if (string_seen)
    {
      string_seen = false;
      if (token)
	/* If not at end of file, then `unput' is usable.  */
	prepare_for_rescan ();
      if (translatable_flag)
	{
	  struct po_entry lookup;
	  struct po_entry *entry;
	  char *string;

	  /* Check if this string has already been seen.  */
	  obstack_1grow (&string_stack, '\0');
	  string = obstack_finish (&string_stack);
	  lookup.msgid = string;
	  entry = hash_lookup (po_entry_table, &lookup);
	  if (entry)
	    /* Discard a duplicate string.  */
	    obstack_free (&string_stack, string);
	  else
	    {
	      /* This is a new string, so generate a new PO file entry.  */
	      entry = (struct po_entry *) xmalloc (sizeof (struct po_entry));
	      entry->msgid = string;
	      entry->next = po_entry_list;
	      po_entry_list = entry;
	      entry->reference_list = NULL;
	      if (!hash_insert (po_entry_table, entry))
		error (EXIT_FAILURE, 0, _("Memory exhausted."));
	    }
	  if (current_reference_list)
	    {
	      struct reference *reference = current_reference_list;

	      /* Prepend the already accumulated list of references.  */
	      while (reference->next)
		reference = reference->next;
	      reference->next = entry->reference_list;
	      entry->reference_list = current_reference_list;
	      current_reference_list = NULL;
	    }
	  else
	    {
	      struct reference *reference
		= new_reference (string_file_name, string_line_number);

	      /* Chain a single reference built for this circumstance.  */
	      reference->next = entry->reference_list;
	      entry->reference_list = reference;
	    }
	}
      return STRING;
    }

  if (comment_seen)
    {
      /* No string, then just get rid of the accumulated comment.  */
      obstack_free (&comment_stack, obstack_finish (&comment_stack));
      comment_seen = false;
    }

  BEGIN start;
  translatable_flag = false;
  return token;
}

/* Comment and string processing.  */

/*-----------------------------------------------------------------------.
| Start a new comment, or continue accumulating one, from given TEXT and |
| SIZE.  If DELIMITER is not NUL, beginning occurrences are excluded, as |
| well as the first space following them (a TAB is preserved).           |
`-----------------------------------------------------------------------*/

static void
grow_comment (char *text, size_t length, char delimiter)
{
  if (delimiter)
    {
      while (length > 0 && *text == delimiter)
	{
	  text++;
	  length--;
	}
      if (length > 0 && *text == ' ')
	{
	  text++;
	  length--;
	}
    }
  obstack_grow (&comment_stack, text, length);
  comment_seen = true;
}

/*------------------------------------------------------------------------.
| Build a reference pointing to FILE_NAME and LINE_NUMBER.  The reference |
| includes the comment currently being grown, if any.  The caller should  |
| then include the returned reference in some linked chain.               |
`------------------------------------------------------------------------*/

static struct reference *
new_reference (const char *file_name, unsigned line_number)
{
  struct reference *reference = (struct reference *)
    xmalloc (sizeof (struct reference));

  reference->file_name = file_name;
  reference->line_number = line_number;
  if (comment_seen)
    {
      obstack_1grow (&comment_stack, '\0');
      reference->comment = obstack_finish (&comment_stack);
      comment_seen = false;
    }
  else
    reference->comment = NULL;

  return reference;
}

/*-------------------------------------------------------------------------.
| Prepare to scan a new string, possibly known to be TRANSLATABLE from its |
| lexical properties, then read it using the START Flex state.  The parser |
| might also have already decided that the next string is translatable.    |
`-------------------------------------------------------------------------*/

static void
begin_string (bool translatable, int start)
{
  if (!string_seen)
    {
      string_file_name = current_file_name;
      string_line_number = current_line_number;
      string_seen = true;
    }
  if (translatable)
    translatable_flag = true;
  BEGIN start;
  meta_flag = false;
  control_flag = false;
}

/*---------------------------------------------------.
| Start a new string, or continue accumulating one.  |
`---------------------------------------------------*/

static void
grow_string (char *text, size_t length)
{
  if (translatable_flag)
    obstack_grow (&string_stack, text, length);
}

/*-------------------------------------------------------------------.
| Accumulate a single character, subject to meta and control flags.  |
`-------------------------------------------------------------------*/

static void
grow_string_with_character (char character)
{
  if (control_flag)
    {
      if (character == '?')
	character = ~(~0 << 7);
      else
	character &= ~(~0 << 5);
      control_flag = false;
    }
  if (meta_flag)
    {
      character |= 1 << 7;
      meta_flag = false;
    }
  if (translatable_flag)
    obstack_1grow (&string_stack, character);
}

/*---------------------------------.
| Decode and save an octal value.  |
`---------------------------------*/

static void
grow_string_with_octal (const char *text)
{
  unsigned value = *text++ - '0';

  while (*text)
    value = 8 * value + *text++ - '0';
  grow_string_with_character (value);
}

/*---------------------------------------.
| Decode and save an hexadecimal value.	 |
`---------------------------------------*/

static void
grow_string_with_hexadecimal (const char *text)
{
  unsigned value = 0;

  while (*text)
    if (*text >= 'a' && *text <= 'f')
      value = 16 * value + *text++ - 'a' + 10;
    else if (*text >= 'A' && *text <= 'F')
      value = 16 * value + *text++ - 'A' + 10;
    else
      value = 16 * value + *text++ - '0';
  grow_string_with_character (value);
}

/* Miscellaneous processing.  */

/*---------------------------.
| Set the language to scan.  |
`---------------------------*/

static void
set_language (enum language language)
{
  current_language = language;
  switch (current_language)
    {
    case LANGUAGE_UNKNOWN: BEGIN INITIAL; break;
    case LANGUAGE_AWK: BEGIN STATE_AWK; break;
    case LANGUAGE_C: BEGIN STATE_C; break;
    case LANGUAGE_ELISP: BEGIN STATE_ELISP; break;
    case LANGUAGE_PERL: BEGIN STATE_PERL; break;
    case LANGUAGE_PO: BEGIN STATE_PO; break;
    case LANGUAGE_PYTHON: BEGIN STATE_PYTHON; break;
    case LANGUAGE_SHELL: BEGIN STATE_SHELL; break;
    }
  comment_seen = false;
  string_seen = false;
  translatable_flag = false;
}

/*------------------------------------------------------------------.
| Try setting the language, given a HINT string of a given LENGTH.  |
`------------------------------------------------------------------*/

static void
try_setting_language (const char *hint, size_t length)
{
  char *string = alloca (length + 1);
  char *copy;
  const char *cursor;
  struct hint lookup;
  struct hint *entry;
  enum language language;

  for (copy = string, cursor = hint; *cursor; copy++, cursor++)
    *copy = tolower (*cursor);
  *copy = '\0';
  lookup.hint = string;
  entry = hash_lookup (hint_table, &lookup);
  if (entry)
    {
      prepare_for_rescan ();
      set_language (entry->language);
    }
}

/*----------------------------------------------------------------------.
| Initialize the scanning of a FILE, which is already opened, and which |
| have NAME as its file name.					        |
`----------------------------------------------------------------------*/

void
initialize_scan (FILE *file, const char *name)
{
  static int first_time = 1;
  enum language language = LANGUAGE_UNKNOWN;
  struct hint lookup;
  struct hint *entry;

  if (first_time)
    {
      first_time = 0;

      if (!obstack_init (&comment_stack))
	error (EXIT_FAILURE, 0, _("Memory exhausted."));

      if (!obstack_init (&string_stack))
	error (EXIT_FAILURE, 0, _("Memory exhausted."));

      if (po_entry_table = hash_initialize (1000, NULL, po_entry_hash,
					    po_entry_compare, NULL),
	  !po_entry_table)
	error (EXIT_FAILURE, 0, _("Memory exhausted."));
    }

#ifdef FLEX_DEBUG
  yy_flex_debug = debug_flex_option;
#endif
  yyin = file;
  if (name[0] == '.' && name[1] == '/')
    current_file_name = name + 2;
  else
    current_file_name = name;
  current_line_number = 1;
  starter_sent = false;

  lookup.hint = strrchr (name, '.');
  if (lookup.hint)
    {
      entry = hash_lookup (hint_table, &lookup);
      if (entry)
	language = entry->language;
    }
  set_language (language);
}

/*------------------------.
| Terminate the scanner.  |
`------------------------*/

void
terminate_scan (void)
{
}

/*--------------------------------.
| Diagnose an unknown character.  |
`--------------------------------*/

static void
unexpected_character (const char *container)
{
  char message[80];

  sprintf (message, "Unexpected in %s: #%d `%c'", container,
	   yytext[0], yytext[0]);
  yyerror (message);
}

/*-----------------------------------------------.
| Report MESSAGE as a lexical or parsing error.	 |
`-----------------------------------------------*/

void
yyerror (const char *message)
{
  fprintf (stderr, "%s:%d: %s\n",
	   current_file_name, current_line_number, message);
}
