/* Translatable strings extractor.
   Copyright (C) 1996 Free Software Foundation, Inc.
   François Pinard <pinard@iro.umontreal.ca>, 1996.  */

%{
#include <stdio.h>

#include "obstack.h"
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free
void *xmalloc (int);

#include "xpot.h"
#include "parser.h"

/* Set to nonzero by parser when next string should be considered at
   translatable.  Always reset to zero by scanner before any return.  */
int translatable_flag;

/* Error reporting routine.  */
void yyerror (const char *);

/* Pool for all saved comment characters.  */
static struct obstack comment_stack;

/* Pool for all saved string characters.  */
static struct obstack string_stack;

/* Table used for making all strings unique.  */
static hash_table string_table;

/* Location for next scanning.  */
static const char *current_file_name;
static int current_line_number;

/* Location for last scanned string.  */
static const char *string_file_name;
static int string_line_number;

static int token_seen;		/* non-string token seen recently */
static int meta_flag;		/* if M- has been seen */
static int control_flag;	/* if C- has been seen */
static int escape_value;	/* character value of escaped sequence */
static char *last_comment;	/* comment preceeding the string */

static void save_elisp_comment (void);
static void begin_string_scan (int);
static void save_one_character (int);
static void unexpected_character (void);

#define ELISP_RETURN(Token) \
  do {				\
    token_seen = 1;		\
    translatable_flag = 0;	\
    return Token;		\
  } while (0)

%}

%x STATE_AWK
%x STATE_BASH
%x STATE_C STATE_C_COMMENT STATE_C_STRING
%x STATE_ELISP STATE_ELISP_CHARACTER STATE_ELISP_STRING
%x STATE_PERL
%x STATE_PO

white			[\t\f ]

%%

<<EOF>>			ELISP_RETURN (0);

<STATE_C>{			/* C language */

  {white}+
  \n{white}*		current_line_number++;
  [_A-Za-z][_A-Za-z0-9]* token_seen = 1; return C_IDENTIFIER;
  -?[0-9]+		token_seen = 1; return C_NUMBER;
  [()\[\]\{\}]		return yytext[0];
  \/\*			BEGIN STATE_C_COMMENT;
  \"			BEGIN STATE_C_STRING;

  .			unexpected_character ();

}

<STATE_C_COMMENT>{		/* C - comment */

  \*\/			BEGIN STATE_C;
  \n[^*\n]*		current_line_number++;
  [^*\n]*
  \*

}

<STATE_C_STRING>{		/* C - string */

  \"			BEGIN STATE_C; return C_STRING;
  \\\n			current_line_number++;
  \\.
  \n[^\"\\\n]*		current_line_number++;
  [^\"\\\n]+

}

<STATE_ELISP>{			/* Emacs LISP language */

  {white}+
  \n{white}*		current_line_number++;
  \;.*			save_elisp_comment ();
  \;.*\n		save_elisp_comment (); current_line_number++;
  [(.)\[\]\']		ELISP_RETURN (yytext[0]);

  `|,|,@|,\.		/* Just ignore quasi-quotation, save for... */
  \',			ELISP_RETURN (ELISP_OTHER_ATOM);

  \#\^\[		ELISP_RETURN ('['); /* char-table */
  \#\[			ELISP_RETURN ('['); /* byte-code */
  \#\(			ELISP_RETURN ('('); /* text properties */

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

  \#\$			ELISP_RETURN (ELISP_OTHER_ATOM); /* docum from file */
  \#\'			ELISP_RETURN ('\''); /* function */

  [-+]?[0-9]+		|
  [-+]?[0-9]+\.[0-9]*	|
  [-+]?\.[0-9]+		ELISP_RETURN (ELISP_OTHER_ATOM); /* number */

  \?			BEGIN STATE_ELISP_CHARACTER;

  \"			begin_string_scan (0); /* simple string */
  \#\"			begin_string_scan (1); /* translated string */
  \#&\"			begin_string_scan (0); /* boolean vector */

  ([^\000-\040\"\';?()\[\]#]|\\.)+  ELISP_RETURN (ELISP_SYMBOL);

  .			unexpected_character ();

}

<STATE_ELISP_CHARACTER>{	/* Emacs LISP - single character */

  \\(\^|[ACHMSs]-)

  \\[0-7]{1,3}		|
  \\x[0-9a-fA-F]{1,2}	|
  \\.			|
  .			BEGIN STATE_ELISP; ELISP_RETURN (ELISP_OTHER_ATOM);

}

<STATE_ELISP_STRING>{		/* Emacs LISP - string of characters */

  \"			{
    if (token_seen)
      {
	/* Ignore a comment not immediately preceeding the string.  */
	obstack_free (&comment_stack, obstack_finish (&comment_stack));
	last_comment = NULL;
      }
    else
      {
	/* Complete the accumulated comment block.  */
	obstack_1grow (&comment_stack, '\0');
	last_comment = obstack_finish (&comment_stack);
	if (*last_comment == '\0')
	  {
	    /* Disregard the comment block if it contains nothing.  */
	    obstack_free (&comment_stack, obstack_finish (&comment_stack));
	    last_comment = NULL;
	  }
      }
    if (translatable_flag)
      {
	struct reference *reference = (struct reference *)
	  xmalloc (sizeof (struct reference));
	struct po_entry *po_entry;
	char *string;
	int length;

	obstack_1grow (&string_stack, '\0');
	string = obstack_finish (&string_stack);
	length = strlen (string);
	reference->file_name = string_file_name;
	reference->line_number = string_line_number;

	if (find_entry (&string_table, string, length, (void **) &po_entry) < 0)
	  {
	    reference->next = NULL;
	    po_entry = (struct po_entry *) xmalloc (sizeof (struct po_entry));
	    po_entry->next = po_entry_list;
	    po_entry->reference_list = reference;
	    po_entry->msgid = string;
	    po_entry_list = po_entry;
	    insert_entry (&string_table, string, length, po_entry);
	  }
	else
	  {
	    obstack_free (&string_stack, string);
	    reference->next = po_entry->reference_list;
	    po_entry->reference_list = reference;
	  }
      }
    BEGIN STATE_ELISP;
    ELISP_RETURN (ELISP_OTHER_ATOM);
  }

  \\\n			current_line_number++;
  \\(\^|C-)		control_flag = 1;
  \\M-			meta_flag = 1;
  \\[AHSs]-


  \\[0-7]{1,3}		{
    int value = yytext[1] - '0';
    char *cursor = yytext + 2;

    while (*cursor)
      value = 8 * value + *cursor++ - '0';
    save_one_character (value);
  }

  \\x[0-9a-fA-F]{1,2}	{
    int value = 0;
    char *cursor = yytext + 2;

    while (*cursor)
      if (*cursor >= 'a' && *cursor <= 'f')
	value = 16 * value + *cursor++ - 'a' + 10;
      else if (*cursor >= 'A' && *cursor <= 'F')
	value = 16 * value + *cursor++ - 'A' + 10;
      else
	value = 16 * value + *cursor - '0';
    save_one_character (value);
  }

  \\a			save_one_character ('\007');
  \\b			save_one_character ('\b');
  \\d			save_one_character (127);
  \\e			save_one_character (27);
  \\f			save_one_character ('\f');
  \\n			save_one_character ('\n');
  \\r			save_one_character ('\r');
  \\t			save_one_character ('\t');
  \\v			save_one_character ('\v');
  \\.			save_one_character (yytext[1]);

  \n			save_one_character ('\n'); current_line_number++;

  [^\"\\\n]+		{
    if (control_flag || meta_flag)
      {
	save_one_character (yytext[0]);
	if (yyleng > 1 && translatable_flag)
	  obstack_grow (&string_stack, yytext + 1, yyleng - 1);
      }
    else if (translatable_flag)
      obstack_grow (&string_stack, yytext, yyleng);
  }

}

%%

/* Functions proper.  */

/*----------------------------------------------------------------------.
| Initialize the scanning of a FILE, which is already opened, and which |
| have NAME as its file name.					        |
`----------------------------------------------------------------------*/

void
initialize_scan (FILE *file, const char *name, enum language language)
{
  static int first_time = 1;

  if (first_time)
    {
      first_time = 0;

      if (!obstack_init (&comment_stack))
	error (EXIT_FAILURE, 0, _("Memory exhausted."));

      if (!obstack_init (&string_stack))
	error (EXIT_FAILURE, 0, _("Memory exhausted."));

      if (init_hash (&string_table, 1000) < 0)
	error (EXIT_FAILURE, 0, _("Memory exhausted."));
    }

#ifdef FLEX_DEBUG
  yy_flex_debug = debug_lex;
#endif
  yyin = file;
  current_file_name = name;
  current_line_number = 1;

  switch (language)
    {
    case LANGUAGE_AWK:
      BEGIN STATE_AWK;
      break;

    case LANGUAGE_BASH:
      BEGIN STATE_BASH;
      break;

    case LANGUAGE_C:
      BEGIN STATE_C;
      break;

    case LANGUAGE_ELISP:
      BEGIN STATE_ELISP;
      break;

    case LANGUAGE_PERL:
      BEGIN STATE_PERL;
      break;

    case LANGUAGE_PO:
      BEGIN STATE_PO;
      break;
    }

  last_comment = NULL;
  translatable_flag = 0;
  token_seen = 1;
}

/*------------------------.
| Terminate the scanner.  |
`------------------------*/

void
terminate_scan (void)
{
}

/*-------------------------------------------.
| Save or accumulate an Emacs LISP comment.  |
`-------------------------------------------*/

static void
save_elisp_comment (void)
{
  if (last_comment)
    {
      /* This is the first comment to appear after a string, which comment
	 the caller did not choose to save.  */
      obstack_free (&comment_stack, last_comment);
      last_comment = NULL;
    }
  else if (token_seen)
    /* This is the start of a new comment block.  */
    obstack_free (&comment_stack, obstack_finish (&comment_stack));
  /* Else, this is the continuation for an already started comment block.  */

  obstack_grow (&comment_stack, yytext, yyleng);
  token_seen = 0;
}

/*--------------------------------------------------------------.
| Prepare to scan string, knowing if it is to be TRANSLATABLE.  |
`--------------------------------------------------------------*/

static void
begin_string_scan (int translatable)
{
    string_file_name = current_file_name;
    string_line_number = current_line_number;
    if (translatable)
      translatable_flag = 1;
    meta_flag = 0;
    control_flag = 0;
    BEGIN STATE_ELISP_STRING;
}

/*-------------------------------------------------------------------.
| Accumulate a single character, subject to meta and control flags.  |
`-------------------------------------------------------------------*/

static void
save_one_character (int character)
{
  if (control_flag)
    {
      if (character == '?')
	character = ~(~0 << 7);
      else
	character &= ~(~0 << 5);
      control_flag = 0;
    }
  if (meta_flag)
    {
      character |= 1 << 7;
      meta_flag = 0;
    }
  if (translatable_flag)
    obstack_1grow (&string_stack, character);
}

/*--------------------------------.
| Diagnose an unknown character.  |
`--------------------------------*/

static void
unexpected_character (void)
{
  char message[80];

  sprintf (message, "Unexpected character: %d (%c)", yytext[0], yytext[0]);
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
