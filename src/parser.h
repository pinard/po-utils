typedef union
{
  struct elisp_expr elisp_expr;
  struct elisp_expr_list elisp_expr_list;
} YYSTYPE;
#define	STRING	258
#define	AWK_STARTER	259
#define	C_STARTER	260
#define	C_IDENTIFIER	261
#define	C_PYTHON_DOCSTRING	262
#define	C_OTHER	263
#define	ELISP_STARTER	264
#define	ELISP_SYMBOL	265
#define	ELISP_OTHER_ATOM	266
#define	PO_STARTER	267
#define	PO_MSGID	268
#define	PO_MSGSTR	269
#define	PERL_STARTER	270
#define	PYTHON_STARTER	271
#define	PYTHON_IDENTIFIER	272
#define	PYTHON_DEFINE	273
#define	PYTHON_OTHER	274
#define	SHELL_STARTER	275


extern YYSTYPE yylval;
