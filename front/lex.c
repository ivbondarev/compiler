#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "lex.h"
#include "../utils.h"

#define DELIMETER " "

static int in_alphabet(const char ch)
{
	return isalpha(ch) || isdigit(ch);
}

static void lexer_add_tok(struct compiler_state *cs, struct token *tok)
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

char *sc_lexer_token_info(const struct token *tok)
{
	/* Temporary buffer for NUM terminal */
	char *buf = calloc(1, BUFSIZ);

	switch(tok->type) {
	case STATEMENT:
		return "STATEMENT";
	case ASSIGNMENT:
		return "ASSIGNMENT";
	case IFBLOCK:
		return "IFBLOCK";
	case EXPRESSION:
		return "EXPRESSION";
	case WHILELOOP:
		return "WHILELOOP";
	case NUM:
		sprintf(buf, "%u", tok->val);
		return buf;
	case ASSIGN:
		return "=";
	case IF:
		return "if";
	case LBRA:
		return "{";
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
	default:
	       assert(0);
       }
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
			fprintf(f, "[NUM : %s]", tok->str);
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
		case CMP:
			fprintf(f, "%s", "[==]");
			break;
		case EOS:
			break;
		default:
			assert(0);
		}
	}

	fprintf(f, "\n");
	fclose(f);
}

/* len is a length occupied by token*/
static int lexer_set_tok(struct token *tok, enum tokens type, char *str,
			 size_t len)
{
	tok->type = type;
	tok->str = str;
	return len;
}

static int lexer_get_token(struct compiler_state *cs, size_t pos,
			    struct token *tok)
{
	char *buf = &cs->buf[pos];

	if (in_alphabet(*buf)) {
		/* This must be a variable identificator or a constant */
		size_t i = 0;
		char *tok_str;

		for (;;) {
			if (!in_alphabet(buf[i]))
				break;
			i++;
		}

		tok_str = calloc(i + 1, 1);
		memcpy(tok_str, buf, i);
		return lexer_set_tok(tok, ID, tok_str, i);
	}

	if (*buf == '=') {
		/* Must be assing or comapre */
		if (buf[1] == '=')
			 /* '==' are 2 bytes*/
			return lexer_set_tok(tok, CMP, NULL, 2);
		else
			/* '=' is 1 byte*/
			return lexer_set_tok(tok, ASSIGN, NULL, 1);
	}

	if (*buf == '{')
		return lexer_set_tok(tok, LBRA, NULL, 1);

	if (*buf == '}')
		return lexer_set_tok(tok, RBRA, NULL, 1);

	if (*buf == '(')
		return lexer_set_tok(tok, LPAR, NULL, 1);

	if (*buf == ')')
		return lexer_set_tok(tok, RPAR, NULL, 1);

	if (*buf == '+')
		return lexer_set_tok(tok, PLUS, NULL, 1);

	if (*buf == '-')
		return lexer_set_tok(tok, MINUS, NULL, 1);

	if (*buf == '*')
		return lexer_set_tok(tok, MUL, NULL, 1);

	if (*buf == '/')
		return lexer_set_tok(tok, DIV, NULL, 1);

	if (*buf == ' ')
		return lexer_set_tok(tok, SPACE, NULL, 0);

	sc_utils_die("Wrong symbol [%s]", *buf);
	return 0;
}

static void num_or_id(struct token *tok)
{
	for (size_t i = 0; i < strlen(tok->str); i++)
		if (!isdigit(tok->str[i])) {
			tok->type = ID;
			return;
		}
	tok->type = NUM;
	tok->val = atoi(tok->str);
}

/* Determines, is token string or number */
static void lexer_correct_types(struct compiler_state *cs)
{
	for (size_t i = 0; i < sc_vector_size(&cs->tokens); i++) {
		struct token *tok = sc_vector_get(&cs->tokens, i);

		if (tok->type == ID)
			num_or_id(tok);
	}
}

void sc_lexer_read_tokens(struct compiler_state *cs)
{
	struct token *tok = NULL;
	size_t pos = 0;

	tok = malloc(sizeof(*tok));

	/* Remove new lines, tabs, etc */
	lexer_remove_bad_symbols(cs->buf);

	while (pos != strlen(cs->buf)) {
		size_t tok_len = lexer_get_token(cs, pos, tok);

		if (tok_len == 0) { /* space */
			pos++;
			continue;
		} else {
			lexer_add_tok(cs, tok);
			tok = malloc(sizeof(*tok));
			pos += tok_len;
		}
	}

	/* Fake EOS (end of stream) token */
	tok = malloc(sizeof(*tok));
	tok->type = EOS;
	lexer_add_tok(cs, tok);

	lexer_correct_types(cs);

	if (cs->dump.tokens != NULL)
		lexer_print_tokens(cs);
}
