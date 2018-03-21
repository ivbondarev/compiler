#ifndef _LEXER_H
#define _LEXER_H

#include "../main.h"

/*
 * PROG -> STATEMENT
 * STATEMENT -> ASSIGN STATEMENT | IF STATEMENT | WHILE STATEMENT |
	FUNCCALL STATEMENT | ASSIGN EOS | IF EOS| WHILE EOS | FUNCCALL EOS
 * FUNCCALL -> ID ( VAR )
 * ASSIGN -> ID = NUM | ID = ID
 * IF -> if ( COND ) then STATEMENT else STATEMENT end
 * WHILE -> while ( COND ) do STATEMENT end
 * COND -> ID == ID | ID == NUM
 * ID -> name1
 * NUM -> num1
 */

enum tokens {
	/* Non-terminals */
	ASSIGNMENT,
	EXPRESSION,
	STATEMENT,
	IFBLOCK,
	WHILELOOP,
	/* Terminals */
	LBRA, /* { */
	RBRA, /* } */
	LPAR, /* ( */
	RPAR, /* ) */
	IF, /* if */
	THEN, /* then */
	ELSE, /* else */
	END, /* end */
	ID,
	NUM,
	ASSIGN,
	CMP,
	EQ,
	PLUS,
	MINUS,
	DIV,
	MUL,
	/* Fake token */
	SPACE,
	EOS,
	NODE,
	TOK__MAX
};

struct token {
	enum tokens type;
	u32 val;
	char *str;
};

/* Scan and evaluate input stream */
void sc_lexer_read_tokens(struct compiler_state *cs);
/* Return info about given token */
char *sc_lexer_token_info(const struct token *tok);
/* Create token with given type */
struct token *sc_lexer_tok(u32 type);

#endif /* !_LEXER_H */
