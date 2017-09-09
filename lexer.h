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
	ID,
	ASSIGN,
	EQ,
	LBRA,
	RBRA,
	NUM,
	IF
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

#endif /* !_LEXER_H */
