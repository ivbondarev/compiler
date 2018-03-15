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
	N_PROG,
	N_STATEMENT,
	N_ASSIGN,
	N_IF,
	N_WHILE,
	N_STNEXT,
	N_COND,
	N_FUNCCALL,

	/* Terminals */
	LBRA,
	RBRA,
	LPAR,
	RPAR,
	IF,
	THEN,
	ELSE,
	END,
	ID,
	NUM,
	ASSIGN,
	EQ,
	PLUS,
	MINUS,
	DIV,
	MUL,
	EOS,
	TOK__MAX
};

struct token {
	enum tokens type;
	union {
		u32 val;
		char *str;
	};
};

/* Scan and evaluate input stream */
void sc_lexer_tokenize(struct compiler_state *cs);
/* Print chain of scanned tokens */
void sc_lexer_token_chain(const struct compiler_state *cs);
void sc_lexer_token_chain_vector(const struct vector *v);
struct token *sc_lexer_tok(u32 type);
char *sc_lexer_token_info(const struct token *tok);
void sc_lexer_print_tokens(const struct compiler_state *cs);

#endif /* !_LEXER_H */
