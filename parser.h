typedef union
{
  struct elisp_expr elisp_expr;
  struct elisp_expr_list elisp_expr_list;
} YYSTYPE;
#define	AWK_IDENT	258
#define	BASH_IDENT	259
#define	C_IDENTIFIER	260
#define	C_NUMBER	261
#define	C_STRING	262
#define	ELISP_SYMBOL	263
#define	ELISP_OTHER_ATOM	264
#define	PO_MSGID	265
#define	PO_MSGSTR	266
#define	PO_STRING	267


extern YYSTYPE yylval;
