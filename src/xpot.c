/* Translatable strings extractor.
   Copyright © 2000 Progiciels Bourbeau-Pinard inc.
   François Pinard <pinard@iro.umontreal.ca>, 2000.  */

#include "xpot.h"
#include <time.h>

#include <errno.h>
#ifndef errno
extern int errno;
#endif errno
#include "error.h"

#include "getopt.h"

/* Name of executing program.  */
const char *program_name;
/* Display usage information and exit.  */
static bool show_help = false;
/* Print the version and exit.  */
static bool show_version = false;

/* Extract documentation strings.  */
bool doc_strings_option = false;
/* Debug Flex code.  */
bool debug_flex_option = false;
/* Debug Bison code.  */
bool debug_bison_option = false;

/* Do not generate PO file header.  */
static bool no_header_option = false;

/* File to receive the output.  */
static FILE *output_file;

/* Relating various hints to languages.  */
Hash_table *hint_table;
/* Relating symbols to their class.  */
Hash_table *symbol_table;
/* Relating msgids to references.  */
Hash_table *po_entry_table;
/* Accumulated PO entries.  */
struct po_entry *po_entry_list;

/* Domain (or package) and release (or version) identification.  */
static const char *domain_string = "PACKAGE";
static const char *release_string = "VERSION";

/* Services.  */

/*---------------------------------------.
| Return a value for POT-Creation-Date.	 |
`---------------------------------------*/

static char *
creation_date (void)
{
  time_t current_time;
  static char buffer[21];
  struct tm *tm;

  /* But how do I proceed to get the zone?  */

  time (&current_time);
  tm = localtime (&current_time);
  sprintf (buffer, "%4d-%02d-%02d %02d:%02d+ZONE",
	   tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
	   tm->tm_hour, tm->tm_min);
  return buffer;
}

/*---------------------------------------------.
| Take note of some HINT implying a LANGUAGE.  |
`---------------------------------------------*/

static unsigned
hint_hash (const void *void_data, unsigned table_size)
{
  const struct hint *data = void_data;

  return hash_string (data->hint, table_size);
}

static bool
hint_compare (const void *void_first, const void *void_second)
{
  const struct hint *first = void_first;
  const struct hint *second = void_second;

  return strcmp (first->hint, second->hint) == 0;
}

static void
register_hint (char *hint, enum language language)
{
  struct hint *extension = xmalloc (sizeof (struct hint));

  extension->hint = hint;
  extension->language = language;
  if (!hash_insert (hint_table, extension))
    error (EXIT_FAILURE, 0, _("Memory exhausted."));
}

/*-------------------------------------------------------------------------.
| Take note that a symbol, having STRING fot its name, pertains to CLASS.  |
`-------------------------------------------------------------------------*/

static unsigned
symbol_hash (const void *void_data, unsigned table_size)
{
  const struct symbol *data = void_data;

  return hash_string (data->string, table_size);
}

static bool
symbol_compare (const void *void_first, const void *void_second)
{
  const struct symbol *first = void_first;
  const struct symbol *second = void_second;

  return strcmp (first->string, second->string) == 0;
}

static void
register_symbol (char *string, enum elisp_expr_class class)
{
  struct symbol *symbol = xmalloc (sizeof (struct symbol));

  symbol->string = string;
  symbol->class = class;

  if (!hash_insert (symbol_table, symbol))
    error (EXIT_FAILURE, 0, _("Memory exhausted."));
}

/*------------------------.
| Initialise all tables.  |
`------------------------*/

static void
initialize_tables (void)
{
  if (hint_table = hash_initialize (20, NULL, hint_hash,
					 hint_compare, NULL),
      !hint_table)
    error (EXIT_FAILURE, 0, _("Memory exhausted."));

  register_hint (".awk", LANGUAGE_AWK);
  register_hint ("awk", LANGUAGE_AWK);
  register_hint ("gawk", LANGUAGE_AWK);
  register_hint ("gnuawk", LANGUAGE_AWK);
  register_hint ("mawk", LANGUAGE_AWK);
  register_hint ("nawk", LANGUAGE_AWK);

  register_hint (".C", LANGUAGE_C);
  register_hint (".c", LANGUAGE_C);
  register_hint (".c++", LANGUAGE_C);
  register_hint (".cc", LANGUAGE_C);
  register_hint (".h", LANGUAGE_C);
  register_hint (".l", LANGUAGE_C);
  register_hint (".y", LANGUAGE_C);
  register_hint ("bison", LANGUAGE_C);
  register_hint ("byacc", LANGUAGE_C);
  register_hint ("c", LANGUAGE_C);
  register_hint ("c++", LANGUAGE_C);
  register_hint ("flex", LANGUAGE_C);
  register_hint ("lex", LANGUAGE_C);
  register_hint ("yacc", LANGUAGE_C);

  register_hint (".el", LANGUAGE_ELISP);
  register_hint (".elc", LANGUAGE_ELISP);
  register_hint ("emacs-lisp", LANGUAGE_ELISP);

  register_hint (".pl", LANGUAGE_PERL);
  register_hint ("perl", LANGUAGE_PERL);

  register_hint (".po", LANGUAGE_PO);
  register_hint (".pot", LANGUAGE_PO);
  register_hint (".pox", LANGUAGE_PO);
  register_hint ("po", LANGUAGE_PO);

  register_hint (".py", LANGUAGE_PYTHON);
  register_hint ("python", LANGUAGE_PYTHON);

  register_hint (".sh", LANGUAGE_SHELL);
  register_hint ("ash", LANGUAGE_SHELL);
  register_hint ("bash", LANGUAGE_SHELL);
  register_hint ("csh", LANGUAGE_SHELL);
  register_hint ("mcsh", LANGUAGE_SHELL);
  register_hint ("sh", LANGUAGE_SHELL);
  register_hint ("shell", LANGUAGE_SHELL);
  register_hint ("shell-script", LANGUAGE_SHELL);
  register_hint ("tcsh", LANGUAGE_SHELL);
  register_hint ("zsh", LANGUAGE_SHELL);

  if (symbol_table = hash_initialize (20, NULL, symbol_hash,
				      symbol_compare, NULL),
      !symbol_table)
    error (EXIT_FAILURE, 0, _("Memory exhausted."));

  register_symbol ("defconst", DEFINE_SYMBOL);
  register_symbol ("defcustom", DEFINE_SYMBOL);
  register_symbol ("defmacro", DEFINE_SYMBOL);
  register_symbol ("defun", DEFINE_SYMBOL);
  register_symbol ("defvar", DEFINE_SYMBOL);

  register_symbol ("translate-string", MARK_SYMBOL);
  register_symbol ("_", MARK_SYMBOL);
}

/*------------------------------------------------------.
| Process an already opened FILE, also given its NAME.  |
`------------------------------------------------------*/

void
process_file (FILE *file, const char *name)
{
  initialize_scan (file, name);
  yyparse ();
  terminate_scan ();
}

/*----------------------------------------------.
| Generate all accumulated PO entries on FILE.  |
`----------------------------------------------*/

void
generate_po_file (FILE *file)
{
  struct po_entry *po_entry;

  /* Output the header entry.  */

  if (!no_header_option)
    fprintf (file, "\
# SOME DESCRIPTIVE TITLE.\n\
# Copyright (C) YEAR Free Software Foundation, Inc.\n\
# FIRST AUTHOR <EMAIL@ADDRESS>, YEAR.\n\
#\n\
#, fuzzy\n\
msgid \"\"\n\
msgstr \"\"\n\
\"Project-Id-Version: %s %s\\n\"\n\
\"POT-Creation-Date: %s\\n\"\n\
\"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\\n\"\n\
\"Last-Translator: FULL NAME <EMAIL@ADDRESS>\\n\"\n\
\"Language-Team: LANGUAGE <LL@li.org>\\n\"\n\
\"MIME-Version: 1.0\\n\"\n\
\"Content-Type: text/plain; charset=CHARSET\\n\"\n\
\"Content-Transfer-Encoding: ENCODING\\n\"\n",
	     domain_string, release_string, creation_date ());

  /* Reverse PO_ENTRY_LIST.  */
  {
    struct po_entry *reversed = NULL;
    struct po_entry *next;

    while (po_entry_list)
      {
	next = po_entry_list->next;
	po_entry_list->next = reversed;
	reversed = po_entry_list;
	po_entry_list = next;
      }
    po_entry_list = reversed;
  }

  /* Process all PO entries.  */

  for (po_entry = po_entry_list; po_entry; po_entry = po_entry->next)
    {
      struct reference *reference_list = po_entry->reference_list;

      putc ('\n', file);

      if (reference_list)
	{
	  struct reference *reference;

	  /* Reverse REFERENCE_LIST.  */
	  {
	    struct reference *reversed = NULL;
	    struct reference *next;

	    while (reference_list)
	      {
		next = reference_list->next;
		reference_list->next = reversed;
		reversed = reference_list;
		reference_list = next;
	      }
	    reference_list = reversed;
	  }
	  po_entry->reference_list = reference_list;

	  /* Output all hard comments.  */

	  for (reference = reference_list;
	       reference;
	       reference = reference->next)
	    if (reference->comment)
	      {
		bool left_margin = true;
		const char *cursor;

		for (cursor = reference->comment; *cursor; cursor++)
		  {
		    if (left_margin)
		      {
			fputs ("#. ", file);
			left_margin = false;
		      }
		    putc (*cursor, file);
		    if (*cursor == '\n')
		      left_margin = true;
		  }
		if (!left_margin)
		  putc ('\n', file);
	      }

	  /* Output all references to a line.  */

	  fputs ("#:", file);
	  for (reference = reference_list;
	       reference;
	       reference = reference->next)
	    fprintf (file, " %s:%d",
		     reference->file_name, reference->line_number);

	  putc ('\n', file);
	}

      /* Output msgid and msgstr.  */
      {
	const char *cursor = po_entry->msgid;
	bool multi_line;

	/* An embedded newline triggers multi-line format.  */
	while (*cursor == '\n')
	  cursor++;
	while (*cursor && *cursor != '\n')
	  cursor++;
	while (*cursor == '\n')
	  cursor++;
	multi_line = *cursor != '\0';

	/* Output msgid.  */
	fputs (multi_line ? "msgid \"\"\n\"" : "msgid \"", file);
	for (cursor = po_entry->msgid; *cursor; cursor++)
	  switch (*cursor)
	    {
	    case '"':
	      fputs ("\\\"", file);
	      break;

	    case '\a':
	      fputs ("\\a", file);
	      break;

	    case '\b':
	      fputs ("\\b", file);
	      break;

	    case '\f':
	      fputs ("\\f", file);
	      break;

	    case '\n':
	      fputs (multi_line && cursor[1] ? "\\n\"\n\"" : "\\n", file);
	      break;

	    case '\t':
	      fputs ("\\t", file);
	      break;

	    case '\\':
	      fputs ("\\\\", file);
	      break;

	    default:
	      putc (*cursor, file);
	    }

	/* Output msgstr.  */
	fputs ("\"\nmsgstr \"\"\n", file);
      }
    }
}

/* Main control of execution.  */

/*-----------------------------------------------.
| Explain how to use the program, then get out.	 |
`-----------------------------------------------*/

void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    fprintf (stderr, _("Try `%s --help' for more information.\n"),
	     program_name);
  else
    {
      printf ("Free %s %s\n", PACKAGE, VERSION);
      printf (_("\
\n\
Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Mandatory or optional arguments to long options are mandatory or optional\n\
for short options too.\n"),
	      stdout);
      fputs (_("\
\n\
Documentation:\n\
      --help               display this help and exit\n\
      --version            output version information and exit\n\
\n\
Control:\n\
  -d, --doc-strings        translate so-called documentation strings\n\
  -k, --keyword=SYMBOL     translate first argument if function is SYMBOL\n\
  -o, --output-file=NAME   generate file on NAME rather than stdout\n\
\n\
POT file identification:\n\
  -n, --no-header          do not generate the PO file header\n\
  -p, --domain=DOMAIN      set domain, default is \"PACKAGE\"\n\
  -v, --release=RELEASE    set release, default is \"VERSION\"\n\
\n\
Debugging:\n\
  -l, --scanner-debug      debug scanner\n\
  -y, --parser-debug       debug parser\n\
\n\
If no FILE is specified, process stdin (and fail: no extension!).\n"),
	     stdout);
      fputs (_("\
\n\
Report bugs to <pinard@iro.umontreal.ca>.\n"),
	       stdout);
    }
  exit (status);
}

/*----------------------------------------------------------------------.
| Main program.  Decode ARGC arguments passed through the ARGV array of |
| strings, then launch execution.				        |
`----------------------------------------------------------------------*/

/* Long options equivalences.  */
static const struct option long_options[] =
{
  {"doc-strings", no_argument, NULL, 'd'},
  {"domain", required_argument, NULL, 'p'},
  {"help", no_argument, (int *) &show_help, 1},
  {"keyword", required_argument, NULL, 'k'},
  {"no-header", no_argument, NULL, 'n'},
  {"output_file", required_argument, NULL, 'o'},
  {"parser-debug", 0, NULL, 'y'},
  {"release", required_argument, NULL, 'v'},
  {"scanner-debug", 0, NULL, 'l'},
  {"version", no_argument, (int *) &show_version, 1},
  {0, 0, 0, 0},
};

int
main (int argc, char **argv)
{
  int option_char;

  program_name = argv[0];
  initialize_tables ();
  output_file = stdout;

  while (option_char = getopt_long (argc, argv, "dk:lno:p:v:y",
				    long_options, NULL),
	 option_char != EOF)
    switch (option_char)
      {
      default:
	usage (EXIT_FAILURE);

      case '\0':
	break;

      case 'd':
	doc_strings_option = true;
	break;

      case 'k':
	register_symbol (optarg, MARK_SYMBOL);
	break;

      case 'l':
	debug_flex_option = true;
	break;

      case 'n':
	no_header_option = true;
	break;

      case 'o':
	output_file = fopen (optarg, "w");
	if (!output_file)
	  error (EXIT_FAILURE, errno, optarg);
	break;

      case 'p':
	domain_string = optarg;
	break;

      case 'v':
	release_string = optarg;
	break;

      case 'y':
	debug_bison_option = true;
	break;
      }

  /* Process trivial options.  */

  if (show_version)
    {
      printf ("Free %s %s\n", PACKAGE, VERSION);
      fputs (_("\
\n\
Copyright (C) 1996, 2000 Progiciels Bourbeau-Pinard inc.\n"),
	     stdout);
      fputs (_("\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"),
	     stdout);
      fputs (_("\
\n\
Written by Franc,ois Pinard <pinard@iro.umontreal.ca>.\n"),
	     stdout);
      exit (EXIT_SUCCESS);
    }

  if (show_help)
    usage (EXIT_SUCCESS);

  /* Process all files.  */

  po_entry_list = NULL;

  if (optind == argc)
    process_file (stdin, "-");
  else
    for (; optind < argc; optind++)
      {
	FILE *file = fopen (argv[optind], "r");

	if (file)
	  {
	    process_file (file, argv[optind]);
	    fclose (file);
	  }
	else
	  error (0, errno, argv[optind]);
      }

  generate_po_file (output_file);

  /* Clean up.  */

  if (output_file != stdout)
    fclose (output_file);

  exit (0);
}
