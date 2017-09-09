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

static int is_lbra(const char ch)
{
	return ch == '(';
}

static int is_rbra(const char ch)
{
	return ch == ')';
}

static void add_tok(struct compiler_state *cs, struct token *tok)
{
	sc_vector_add(&cs->tokens, tok);
}

static void remove_newline(char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
		if (str[i] == '\n')
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
	if (!strcmp(buf, "if"))
		tok->type = IF;

	printf("ID [%s]", buf);
	add_tok(cs, tok);
	return i;
}

static size_t lexer_read_assign(struct compiler_state *cs, const char *str)
{
	size_t len = strlen(str);
	struct token *tok = malloc(sizeof(*tok));

	if (1 == len) {
		tok->type = ASSIGN;
		printf("ASSIGN");
	} else if (2 == len && !strcmp(str, "==")) {
		printf("EQ");
		tok->type = EQ;
	} else {
		sc_utils_die("Bad == operator");
	}
	add_tok(cs, tok);
	return len;
}

static size_t lexer_read_lbra(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = LBRA;
	add_tok(cs, tok);
	printf("LBRA");
	return 1;
}

static size_t lexer_read_rbra(struct compiler_state *cs)
{
	struct token *tok = malloc(sizeof(*tok));

	tok->type = RBRA;
	add_tok(cs, tok);
	printf("RBRA ");
	return 1;
}

static void lexer_get_tokens(struct compiler_state *cs, const char *str)
{
	size_t i = 0;
	size_t len = strlen(str);


	while (i < len) {
		if (is_id(str[i])) {
			i += lexer_read_id(cs, &str[i]);
			printf("\n");
		} else if (is_assign(str[i])) {
			printf("\n");
			break;
			i += lexer_read_assign(cs, &str[i]);
		} else if (is_lbra(str[i])) {
			i += lexer_read_lbra(cs);
			printf("\n");
		} else if (is_rbra(str[i])) {
			i += lexer_read_rbra(cs);
			printf("\n");
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

	str = strtok(cs->buf, DELIMETER);

	while (str) {
		remove_newline(str);
		printf("input [%s] | ", str);
		lexer_get_tokens(cs, str);
		str = strtok(NULL, DELIMETER);
	}
}
