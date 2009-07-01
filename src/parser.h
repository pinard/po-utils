typedef union
{
  struct elisp_expr elisp_expr;
  struct elisp_expr_list elisp_expr_list;
} YYSTYPE;
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


extern YYSTYPE yylval;
