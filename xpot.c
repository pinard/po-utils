#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "getopt.h"
#include "xpot.h"

#define _(String) String

const char *program_name;	/* name of executing program */
static int show_help = 0;	/* display usage information and exit */
static int show_version = 0;	/* print the version and exit */

int debug_lex = 0;		/* debug Flex code */
int debug_yacc = 0;		/* debug Bison code */

hash_table symbol_table;	/* for saving symbols */

/* Domain (or package) and release (or version) identification.  */
static const char *domain_string = "PACKAGE";
static const char *release_string = "VERSION";

struct po_entry *po_entry_list;	/* accumulated PO entries */

/* Services.  */

static char *
creation_date ()
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

static void
register_define (char *symbol)
{
  insert_entry (&symbol_table, symbol, strlen (symbol), (void *) DEFINE_SYMBOL);
}

static void
register_keyword (char *symbol)
{
  insert_entry (&symbol_table, symbol, strlen (symbol), (void *) MARK_SYMBOL);
}

void
process_file (FILE *file, const char *name)
{
  initialize_scan (file, name, LANGUAGE_ELISP);
  yyparse ();
  terminate_scan ();
}

void
generate_po_file (void)
{
  struct po_entry *po_entry;

  /* Output the header entry.  */

  printf ("\
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

      putc ('\n', stdout);

      if (reference_list)
	{
	  struct reference *reference;

	  fputs ("#:", stdout);

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

	  /* Output all references to a line.  */

	  for (reference = reference_list;
	       reference;
	       reference = reference->next)
	    printf (" %s:%d", reference->file_name, reference->line_number);

	  putc ('\n', stdout);
	}

      /* Output msgid and msgstr.  */
      {
	const char *cursor = po_entry->msgid;
	int multi_line;

	/* An embedded newline triggers multi-line format.  */
	while (*cursor == '\n')
	  cursor++;
	while (*cursor && *cursor != '\n')
	  cursor++;
	while (*cursor == '\n')
	  cursor++;
	multi_line = *cursor;

	/* Output msgid.  */
	fputs (multi_line ? "msgid \"\"\n\"" : "msgid \"", stdout);
	for (cursor = po_entry->msgid; *cursor; cursor++)
	  switch (*cursor)
	    {
	    case '"':
	      fputs ("\\\"", stdout);
	      break;

	    case '\a':
	      fputs ("\\a", stdout);
	      break;

	    case '\b':
	      fputs ("\\b", stdout);
	      break;

	    case '\f':
	      fputs ("\\f", stdout);
	      break;

	    case '\n':
	      fputs (multi_line && cursor[1] ? "\\n\"\n\"" : "\\n", stdout);
	      break;

	    case '\t':
	      fputs ("\\t", stdout);
	      break;

	    case '\\':
	      fputs ("\\\\", stdout);
	      break;

	    default:
	      putchar (*cursor);
	    }

	/* Output msgstr.  */
	fputs ("\"\nmsgstr \"\"\n", stdout);
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
      printf ("(Not so :-) GNU %s %s\n", PACKAGE, VERSION);
      printf (_("\
\n\
Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Mandatory or optional arguments to long options are mandatory or optional\n\
for short options too.\n"),
	      stdout);
      fputs (_("\
  -d, --domain=DOMAIN     set domain, default is PACKAGE\n\
  -k, --keyword=SYMBOL    translate first argument if function is SYMBOL\n\
  -r, --release=RELEASE   set release, default is VERSION\n\
  -l, --scanner-debug     debug scanner\n\
  -y, --parser-debug      debug parser\n\
      --help              display this help and exit\n\
      --version           output version information and exit\n\
\n\
If no FILE is specified, process stdin.\n"),
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
  {"domain", required_argument, NULL, 'd'},
  {"help", no_argument, &show_help, 1},
  {"keyword", required_argument, NULL, 'k'},
  {"parser-debug", 0, NULL, 'y'},
  {"release", required_argument, NULL, 'r'},
  {"scanner-debug", 0, NULL, 'l'},
  {"version", no_argument, &show_version, 1},
  {0, 0, 0, 0},
};

void
main (int argc, char **argv)
{
  int option_char;

  program_name = argv[0];

  if (init_hash (&symbol_table, 20) < 0)
    error (EXIT_FAILURE, 0, _("Memory exhausted."));

  register_define ("defvar");
  register_define ("defun");
  register_define ("defmacro");
  register_keyword ("translate-string");
  register_keyword ("_");

  while (option_char = getopt_long (argc, argv, "d:k:lr:y", long_options, NULL),
	 option_char != EOF)
    switch (option_char)
      {
      default:
	usage (EXIT_FAILURE);

      case '\0':
	break;

      case 'd':
	domain_string = optarg;
	break;

      case 'k':
	register_keyword (optarg);
	break;

      case 'l':
	debug_lex = 1;
	break;

      case 'r':
	release_string = optarg;
	break;

      case 'y':
	debug_yacc = 1;
	break;
      }

  /* Process trivial options.  */

  if (show_version)
    {
      printf ("GNU %s %s\n", PACKAGE, VERSION);
      fputs (_("\
\n\
Copyright (C) 1996 Free Software Foundation, Inc.\n"),
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

  generate_po_file ();

  /* Clean up.  */

  exit (0);
}
