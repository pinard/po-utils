
/*  A Bison parser, made from ../../src/parser.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	STRING	258
#define	AWK_STARTER	259
#define	C_STARTER	260
#define	C_IDENTIFIER	261
#define	C_NUMBER	262
#define	ELISP_STARTER	263
#define	ELISP_SYMBOL	264
#define	ELISP_OTHER_ATOM	265
#define	PO_STARTER	266
#define	PO_MSGID	267
#define	PO_MSGSTR	268
#define	PERL_STARTER	269
#define	PYTHON_STARTER	270
#define	PYTHON_IDENTIFIER	271
#define	PYTHON_DEFINE	272
#define	PYTHON_OTHER	273
#define	SHELL_STARTER	274

#line 18 "../../src/parser.y"

#include "xpot.h"

#line 22 "../../src/parser.y"
typedef union
{
  struct elisp_expr elisp_expr;
  struct elisp_expr_list elisp_expr_list;
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		85
#define	YYFLAG		-32768
#define	YYNTBASE	29

#define YYTRANSLATE(x) ((unsigned)(x) <= 274 ? yytranslate[x] : 55)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    22,    23,
    25,     2,     2,     2,     2,    24,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    26,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    20,     2,    21,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    27,     2,    28,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     5,     8,    11,    14,    17,    20,    23,
    26,    27,    28,    30,    33,    34,    37,    40,    42,    44,
    48,    51,    57,    61,    62,    64,    67,    68,    74,    75,
    79,    80,    82,    85,    89,    90,    95,    96,   102,   103,
   105,   107,   110,   112,   115,   117,   119,   120,   124,   127,
   131,   134,   138,   141,   145,   147,   149,   152,   156
};

static const short yyrhs[] = {    -1,
    30,    31,     0,     0,     4,    32,     0,     5,    33,     0,
     8,    34,     0,    14,    37,     0,    11,    38,     0,    15,
    41,     0,    19,    54,     0,     0,     0,    36,     0,    34,
    36,     0,     0,    35,    36,     0,    35,     1,     0,     9,
     0,    10,     0,    20,    35,    21,     0,    22,    36,     0,
    23,    35,    24,    36,    25,     0,    23,    35,    25,     0,
     0,    39,     0,    38,    39,     0,     0,    12,    40,     3,
    13,     3,     0,     0,    42,    47,    43,     0,     0,    48,
     0,    43,    44,     0,    43,    44,    48,     0,     0,    17,
    26,    45,    47,     0,     0,    17,    53,    26,    46,    47,
     0,     0,     3,     0,    51,     0,    48,    50,     0,    50,
     0,    49,    50,     0,     3,     0,    51,     0,     0,    16,
    52,    53,     0,    23,    25,     0,    23,    49,    25,     0,
    20,    21,     0,    20,    49,    21,     0,    27,    28,     0,
    27,    49,    28,     0,    18,     0,    26,     0,    23,    25,
     0,    23,    49,    25,     0,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    33,    40,    42,    43,    44,    45,    46,    47,    48,    49,
    56,    63,    73,    74,    77,    81,   109,   112,   124,   128,
   132,   136,   140,   150,   157,   158,   161,   165,   173,   177,
   180,   181,   182,   183,   186,   191,   191,   196,   198,   199,
   202,   203,   206,   207,   210,   211,   214,   225,   225,   226,
   227,   228,   229,   230,   231,   232,   235,   236,   243
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","STRING",
"AWK_STARTER","C_STARTER","C_IDENTIFIER","C_NUMBER","ELISP_STARTER","ELISP_SYMBOL",
"ELISP_OTHER_ATOM","PO_STARTER","PO_MSGID","PO_MSGSTR","PERL_STARTER","PYTHON_STARTER",
"PYTHON_IDENTIFIER","PYTHON_DEFINE","PYTHON_OTHER","SHELL_STARTER","'['","']'",
"'''","'('","'.'","')'","':'","'{'","'}'","init","@1","file","awk_file","c_file",
"elisp_file","elisp_expr_list","elisp_expr","perl_file","po_file","po_entry",
"@2","python_file","@3","python_fragment_list","python_define","@4","@5","python_docstring",
"python_token_list","python_any_token_list","python_any_token","python_token",
"@6","python_parenthetical","shell_file", NULL
};
#endif

static const short yyr1[] = {     0,
    30,    29,    31,    31,    31,    31,    31,    31,    31,    31,
    32,    33,    34,    34,    35,    35,    35,    36,    36,    36,
    36,    36,    36,    37,    38,    38,    40,    39,    42,    41,
    43,    43,    43,    43,    45,    44,    46,    44,    47,    47,
    48,    48,    49,    49,    50,    50,    52,    51,    51,    51,
    51,    51,    51,    51,    51,    51,    53,    53,    54
};

static const short yyr2[] = {     0,
     0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
     0,     0,     1,     2,     0,     2,     2,     1,     1,     3,
     2,     5,     3,     0,     1,     2,     0,     5,     0,     3,
     0,     1,     2,     3,     0,     4,     0,     5,     0,     1,
     1,     2,     1,     2,     1,     1,     0,     3,     2,     3,
     2,     3,     2,     3,     1,     1,     2,     3,     0
};

static const short yydefact[] = {     1,
     3,    11,    12,     0,     0,    24,    29,    59,     2,     4,
     5,    18,    19,    15,     0,    15,     6,    13,    27,     8,
    25,     7,     9,    39,    10,     0,    21,     0,    14,     0,
    26,    40,    31,    17,    20,    16,     0,    23,     0,    47,
    55,     0,     0,    56,     0,    30,    32,    41,     0,     0,
     0,    45,    51,     0,    43,    46,    49,     0,    53,     0,
     0,    33,    42,    22,    28,     0,    48,    52,    44,    50,
    54,    35,     0,    34,    57,     0,    39,    37,    58,    36,
    39,    38,     0,     0,     0
};

static const short yydefgoto[] = {    83,
     1,     9,    10,    11,    17,    26,    36,    22,    20,    21,
    30,    23,    24,    46,    62,    77,    81,    33,    47,    54,
    55,    56,    51,    67,    25
};

static const short yypact[] = {-32768,
   160,-32768,-32768,    53,    -6,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,    53,-32768,    53,-32768,-32768,    -6,
-32768,-32768,-32768,     7,-32768,   140,-32768,    44,-32768,    19,
-32768,-32768,   162,-32768,-32768,-32768,    53,-32768,    11,-32768,
-32768,     0,    54,-32768,    16,     8,   120,-32768,     4,    34,
    17,-32768,-32768,    68,-32768,-32768,-32768,    80,-32768,    32,
   -19,   162,-32768,-32768,-32768,    94,-32768,-32768,-32768,-32768,
-32768,-32768,    15,   120,-32768,   106,     7,-32768,-32768,-32768,
     7,-32768,    46,    47,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768,    33,    -4,-32768,-32768,    31,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -72,    -1,   -28,
   -46,   -31,-32768,    -5,-32768
};


#define	YYLAST		189


static const short yytable[] = {    18,
    63,    48,    52,    66,    80,    19,    72,    69,    82,    32,
    27,    69,    29,    69,    58,    40,    60,    41,    52,    42,
    53,    39,    43,    50,    61,    44,    45,    63,    64,    69,
    48,    40,    49,    41,    52,    42,    65,    76,    43,    66,
    78,    44,    45,    59,    34,    84,    85,    40,    28,    41,
    31,    42,    12,    13,    43,    73,    52,    44,    45,    71,
    74,    12,    13,    14,     0,    15,    16,    37,    38,    40,
    52,    41,    14,    42,    15,    16,    43,     0,    57,    44,
    45,     0,    52,    40,     0,    41,     0,    42,    68,     0,
    43,     0,     0,    44,    45,    40,    52,    41,     0,    42,
     0,     0,    43,     0,    70,    44,    45,     0,    52,    40,
     0,    41,     0,    42,     0,     0,    43,     0,    75,    44,
    45,    40,    52,    41,     0,    42,     0,     0,    43,     0,
    79,    44,    45,     0,     0,    40,     0,    41,     0,    42,
    34,     0,    43,     0,     0,    44,    45,     0,    12,    13,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
    35,    15,    16,     2,     3,     0,     0,     4,     0,     0,
     5,     0,     0,     6,     7,     0,     0,    40,     8,    41,
     0,    42,     0,     0,    43,     0,     0,    44,    45
};

static const short yycheck[] = {     4,
    47,    33,     3,    23,    77,    12,    26,    54,    81,     3,
    15,    58,    17,    60,    43,    16,    45,    18,     3,    20,
    21,     3,    23,    13,    17,    26,    27,    74,    25,    76,
    62,    16,    37,    18,     3,    20,     3,    66,    23,    23,
    26,    26,    27,    28,     1,     0,     0,    16,    16,    18,
    20,    20,     9,    10,    23,    61,     3,    26,    27,    28,
    62,     9,    10,    20,    -1,    22,    23,    24,    25,    16,
     3,    18,    20,    20,    22,    23,    23,    -1,    25,    26,
    27,    -1,     3,    16,    -1,    18,    -1,    20,    21,    -1,
    23,    -1,    -1,    26,    27,    16,     3,    18,    -1,    20,
    -1,    -1,    23,    -1,    25,    26,    27,    -1,     3,    16,
    -1,    18,    -1,    20,    -1,    -1,    23,    -1,    25,    26,
    27,    16,     3,    18,    -1,    20,    -1,    -1,    23,    -1,
    25,    26,    27,    -1,    -1,    16,    -1,    18,    -1,    20,
     1,    -1,    23,    -1,    -1,    26,    27,    -1,     9,    10,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
    21,    22,    23,     4,     5,    -1,    -1,     8,    -1,    -1,
    11,    -1,    -1,    14,    15,    -1,    -1,    16,    19,    18,
    -1,    20,    -1,    -1,    23,    -1,    -1,    26,    27
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

#ifndef YYPARSE_RETURN_TYPE
#define YYPARSE_RETURN_TYPE int
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
YYPARSE_RETURN_TYPE yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

YYPARSE_RETURN_TYPE
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 34 "../../src/parser.y"
{
#if YYDEBUG
  yydebug = debug_bison_option;
#endif
;
    break;}
case 13:
#line 73 "../../src/parser.y"
{;
    break;}
case 15:
#line 78 "../../src/parser.y"
{
  yyval.elisp_expr_list.count = 0;
;
    break;}
case 16:
#line 82 "../../src/parser.y"
{
  switch (yyval.elisp_expr_list.count)
    {
    case 0:
      yyval.elisp_expr_list.class = yyvsp[0].elisp_expr.class;
      if (yyval.elisp_expr_list.class == MARK_SYMBOL)
	translatable_flag = true;
      break;

    case 1:
      if (yyvsp[-1].elisp_expr_list.class == DEFINE_SYMBOL && yyvsp[0].elisp_expr.class != OTHER_SYMBOL)
	/* For exemple, `defun' is not special if not defining a symbol.  */
	yyval.elisp_expr_list.class = OTHER_SYMBOL;
      else
	yyval.elisp_expr_list.class = yyvsp[-1].elisp_expr_list.class;
      break;

    case 2:
      yyval.elisp_expr_list.class = yyvsp[-1].elisp_expr_list.class;
      if (yyval.elisp_expr_list.class == DEFINE_SYMBOL)
	translatable_flag = doc_strings_option;

    default:
      yyval.elisp_expr_list.class = yyvsp[-1].elisp_expr_list.class;
    }
  yyval.elisp_expr_list.count = yyvsp[-1].elisp_expr_list.count + 1;
;
    break;}
case 18:
#line 113 "../../src/parser.y"
{
  struct symbol lookup;
  struct symbol *entry;

  lookup.string = yytext;
  entry = hash_lookup (symbol_table, &lookup);
  if (entry)
    yyval.elisp_expr.class = entry->class;
  else
    yyval.elisp_expr.class = OTHER_SYMBOL;
;
    break;}
case 19:
#line 125 "../../src/parser.y"
{
  yyval.elisp_expr.class = S_EXPRESSION;
;
    break;}
case 20:
#line 129 "../../src/parser.y"
{
  yyval.elisp_expr.class = S_EXPRESSION;
;
    break;}
case 21:
#line 133 "../../src/parser.y"
{
  yyval.elisp_expr.class = S_EXPRESSION;
;
    break;}
case 22:
#line 137 "../../src/parser.y"
{
  yyval.elisp_expr.class = S_EXPRESSION;
;
    break;}
case 23:
#line 141 "../../src/parser.y"
{
  yyval.elisp_expr.class = S_EXPRESSION;
;
    break;}
case 27:
#line 162 "../../src/parser.y"
{
  translatable_flag = 1;
;
    break;}
case 29:
#line 174 "../../src/parser.y"
{
  translatable_flag = doc_strings_option;
;
    break;}
case 35:
#line 187 "../../src/parser.y"
{
  translatable_flag = doc_strings_option;
;
    break;}
case 37:
#line 192 "../../src/parser.y"
{
  translatable_flag = doc_strings_option;
;
    break;}
case 47:
#line 215 "../../src/parser.y"
{
  struct symbol lookup;
  struct symbol *entry;

  lookup.string = yytext;
  entry = hash_lookup (symbol_table, &lookup);
  if (entry && entry->class == MARK_SYMBOL)
    translatable_flag = true;
;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 245 "../../src/parser.y"
