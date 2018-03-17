#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "lex.h"
#include "../utils.h"

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

static int is_lbra(char ch)
{
	return ch == '{';
}

static int is_rbra(char ch)
{
	return ch == '}';
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

struct token *sc_lexer_tok(u32 type)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = type;
	return tok;
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
	} else if (!strcmp(buf, "else")) {
		tok->type = ELSE;
		free(buf);
	} else if (!strcmp(buf, "end")) {
		tok->type = END;
		free(buf);
	} else if (!strcmp(buf, "then")) {
		tok->type = THEN;
		free(buf);
	} else {
		size_t dig_count = 0;

		for (size_t j = 0; j < i; j++)
			if (isdigit(buf[j]))
				dig_count++;

		if (dig_count == i) {
			tok->type = NUM;
			tok->val = atoi(buf);
			free(buf);
		}
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

static size_t lexer_read_lbra(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = LBRA;
	add_tok(cs, tok);
	return 1;
}

static size_t lexer_read_rbra(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = RBRA;
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

void lexer_print_tokens(const struct compiler_state *cs)
{
	FILE *f = cs->dump.tokens;

	for (size_t i = 0; i < cs->tokens.size; i++) {
		const struct token *tok = cs->tokens.elems[i];

		switch(tok->type) {
		case ASSIGN:
			fprintf(f, "%s", "['=']");
			break;
		case IF:
			fprintf(f, "%s", "[IF]");
			break;
		case LBRA:
			fprintf(f, "%s", "['{']");
			break;
		case RBRA:
			fprintf(f, "%s", "['}']");
			break;
		case EQ:
			fprintf(f, "%s", "['==']");
			break;
		case ID:
			fprintf(f, "[ID : %s]", tok->str);
			break;
		case PLUS:
			fprintf(f, "%s", "[+]");
			break;
		case MINUS:
			fprintf(f, "%s", "[-]");
			break;
		case DIV:
			fprintf(f, "%s", "[/]");
			break;
		case MUL:
			fprintf(f, "%s", "[*]");
			break;
		case LPAR:
			fprintf(f, "%s", "[(]");
			break;
		case RPAR:
			fprintf(f, "%s", "[)]");
			break;
		case NUM:
			fprintf(f, "[NUM : %u]", tok->val);
			break;
		case END:
			fprintf(f, "%s", "[END]");
			break;
		case THEN:
			fprintf(f, "%s", "[THEN]");
			break;
		case ELSE:
			fprintf(f, "%s", "[ELSE]");
			break;
		case EOS:
			fprintf(f, "%s", "[EOS]");
			break;
		default:
			sc_utils_die("Ivalid token type");
		}
	}

	fprintf(f, "\n");
	fclose(f);
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
		} else if (is_lbra(str[i])) {
			i += lexer_read_lbra(cs);
		} else if (is_rbra(str[i])) {
			i += lexer_read_rbra(cs);
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

	if (cs->dump.tokens != NULL)
		lexer_print_tokens(cs);
}


char *sc_lexer_token_info(const struct token *tok)
{
	/* Temporary buffer for NUM terminal */
	char *buf = calloc(1, BUFSIZ);

	switch(tok->type) {
	case N_PROG:
		return "N_PROG";
	case N_ASSIGN:
		return "N_ASSIGN";
	case N_IF:
		return "N_IF";
	case N_STATEMENT:
		return "N_STATEMENT";
	case N_COND:
		return "N_COND";
	case N_FUNCCALL:
		return "N_FUNCCALL";
	case NUM:
		sprintf(buf, "%u", tok->val);
		return buf;
	case ASSIGN:
		return "=";
	case IF:
		return "if";
	case LBRA:
		return ("{");
	case RBRA:
		return "}";
	case EQ:
		return "==";
	case ID:
		return tok->str;
	case PLUS:
		return "+";
	case MINUS:
		return "-";
	case DIV:
		return "/";
	case MUL:
		return "*";
	case LPAR:
		return "(";
	case RPAR:
		return ")";
	case THEN:
		return "then";
	case ELSE:
		return "else";
	case END:
		return "end";
	case EOS:
		return "[EOS]";
	default:
		break;
	}

	return " ";
}

void sc_lexer_token_chain(const struct compiler_state *cs)
{
	for (size_t i = 0; i < cs->tokens.size; i++)
		printf("%s\n", sc_lexer_token_info(cs->tokens.elems[i]));
	printf("\n");
}

void sc_lexer_token_chain_vector(const struct vector *v)
{
	for (size_t i = 0; i < v->size; i++)
		printf("%s\n", sc_lexer_token_info(v->elems[i]));
	printf("\n");
}
