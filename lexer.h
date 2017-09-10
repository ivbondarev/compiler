#ifndef _LEXER_H
#define _LEXER_H

#include "main.h"

/*
 * BNF grammar:
 * PROG = CHUNK
 * CHUNK = DEF | IFBLOCK | FUNC | CHUNK
 * DEF = VAR ASSIGN VAR | VAR ASSIGN NUM
 * VAR = ID
 * ID = 'a' .. 'z' | 'A' .. 'Z' | '0' .. '9' | ID
 * NUM = '0' .. '9'
 * IFBLOCK = 'if' '(' COND ')' CHUNK+
 * COND = VAR EQ VAR | VAR EQ VAR
 * EQ = '=='
 * FUNC = FUNCNAME '(' VAR ')'
 * FUNCNAME = ID
 *
 * Simple example:
 * var1 = 10
 * var2 = 4
 * var3 = var1
 * if (var == 10)
 *     var2 = 15
 * print(var2)
 */

enum tokens {
	ID,      /* 0 */
	ASSIGN,  /* 1 */
	EQ,      /* 2 */
	LBRA,    /* 3 */
	RBRA,    /* 4 */
	NUM,     /* 5 */
	IF,      /* 6 */
	PLUS,    /* 7 */
	MINUS,   /* 8 */
	MUL,     /* 9 */
	DIV,     /* 10 */
	LPAR,    /* 11 */
	RPAR,    /* 12 */
	EOS,     /* 13 */
	N_S,     /* 14 */
	N_A,     /* 15 */
	TOK__MAX /* 16 */
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

#endif /* !_LEXER_H */
