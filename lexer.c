#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "main.h"
#include "vector.h"
#include "lexer.h"
#include "utils.h"

#define DELIMETER " "

static int is_id(const char ch)
{
	return isalpha(ch) || isdigit(ch);
}

static int is_assign(const char ch)
{
	return ch == '=';
}

static int is_lpar(const char ch)
{
	return ch == '(';
}

static int is_rpar(const char ch)
{
	return ch == ')';
}

static int is_plus(const char ch)
{
	return ch == '+';
}

static int is_minus(const char ch)
{
	return ch == '-';
}

static int is_div(const char ch)
{
	return ch == '/';
}

static int is_mul(const char ch)
{
	return ch == '*';
}

static void add_tok(struct compiler_state *cs, struct token *tok)
{
	sc_vector_add(&cs->tokens, tok);
}

static void lexer_remove_bad_symbols(char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
		if (str[i] == '\n' || str[i] == '\t')
			str[i] = ' ';
}

static size_t lexer_read_id(struct compiler_state *cs, const char *str)
{
	size_t i = 0;
	char *buf = calloc(1, strlen(str) + 1);
	struct token *tok = malloc(sizeof(*tok));

	while (is_id(str[i])) {
		buf[i] = str[i];
		i++;
	}

	tok->type = ID;
	tok->str = buf;
	if (!strcmp(buf, "if")) {
		tok->type = IF;
		free(buf);
	}

	add_tok(cs, tok);
	return i;
}

static size_t lexer_read_assign(struct compiler_state *cs, const char *str)
{
	size_t len = strlen(str);
	struct token *tok = malloc(sizeof(*tok));
	char *buf = calloc(1, len);
	size_t i = 0;

	while ('=' == str[i]) {
		buf[i] = str[i];
		i++;
	}

	if (1 == i) {
		tok->type = ASSIGN;
	} else if (2 == i) {
		tok->type = EQ;
	} else {
		sc_utils_die("Bad {=}+ operator");
	}

	add_tok(cs, tok);
	return i;
}

static size_t lexer_read_lpar(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = LPAR;
	add_tok(cs, tok);
	return 1;
}

static size_t lexer_read_rpar(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = RPAR;
	add_tok(cs, tok);
	return 1;
}

static size_t lexer_read_plus(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = PLUS;
	add_tok(cs, tok);
	return 1;
}

static size_t lexer_read_minus(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = MINUS;
	add_tok(cs, tok);
	return 1;
}

static size_t lexer_read_mul(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = MUL;
	add_tok(cs, tok);
	return 1;
}
static size_t lexer_read_div(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = DIV;
	add_tok(cs, tok);
	return 1;
}

static void lexer_get_tokens(struct compiler_state *cs, const char *str)
{
	size_t i = 0;
	size_t len = strlen(str);

	while (i < len) {
		if (is_id(str[i])) {
			i += lexer_read_id(cs, &str[i]);
		} else if (is_assign(str[i])) {
			i += lexer_read_assign(cs, &str[i]);
		} else if (is_lpar(str[i])) {
			i += lexer_read_lpar(cs);
		} else if (is_rpar(str[i])) {
			i += lexer_read_rpar(cs);
		} else if (is_plus(str[i])) {
			i += lexer_read_plus(cs);
		} else if (is_minus(str[i])) {
			i += lexer_read_minus(cs);
		} else if (is_div(str[i])) {
			i += lexer_read_div(cs);
		} else if (is_mul(str[i])) {
			i += lexer_read_mul(cs);
		} else if (str[i] == 0) {
			break;
		} else if (str[i] == ' ') {
			i++;
		} else {
			sc_utils_die("Wrong symbol [%s]", str[i]);
		}
	}
}

void sc_lexer_tokenize(struct compiler_state *cs)
{
	char *str;
	struct token *tok = malloc(sizeof(*tok));

	str = strtok(cs->buf, DELIMETER);
	while (str) {
		lexer_remove_bad_symbols(str);
		lexer_get_tokens(cs, str);
		str = strtok(NULL, DELIMETER);
	}

	tok->type = EOS;
	add_tok(cs, tok);
}

static void lexer_token_info(const struct token *tok)
{
	switch(tok->type) {
	case ASSIGN:
		printf("['=']");
		break;
	case IF:
		printf("[IF]");
		break;
	case LBRA:
		printf("['{']");
		break;
	case RBRA:
		printf("['}']");
		break;
	case EQ:
		printf("['==']");
		break;
	case ID:
		printf("[ID: %s]", tok->str);
		break;
	case PLUS:
		printf("[+]");
		break;
	case MINUS:
		printf("[-]");
		break;
	case DIV:
		printf("[/]");
		break;
	case MUL:
		printf("[*]");
		break;
	case LPAR:
		printf("[(]");
		break;
	case RPAR:
		printf("[)]");
		break;
	case EOS:
		printf("[EOS]");
		break;
	default:
		break;
	}

	printf(" ");
}

void sc_lexer_token_chain(const struct compiler_state *cs)
{
	for (size_t i = 0; i < cs->tokens.size; i++)
		lexer_token_info(cs->tokens.elems[i]);
	printf("\n");
}

void sc_lexer_token_chain_vector(const struct vector *v)
{
	for (size_t i = 0; i < v->size; i++)
		lexer_token_info(v->elems[i]);
	printf("\n");
}
