#include "lexer.h"
#include "parser.h"
#include "main.h"
#include "utils.h"

/*static void pop(struct vector *v)
{
	v->size--;
}*/

static void parser_match(const struct compiler_state *cs, size_t *tok_id,
			enum tokens expect_type)
{
	struct token *tok = cs->tokens.elems[*tok_id];

	if (tok->type != expect_type)
		sc_utils_die("parser_match failed: tok %u, type %u",
			     tok->type, expect_type);
	(*tok_id)++;
}

static void parser_prod(struct compiler_state *cs, size_t *tok_id,
			     enum tokens non_term)
{
	struct token *tok;

	switch (non_term) {
	case N_PROG:
		parser_prod(cs, tok_id, N_STATEMENT);
		break;
	case N_STATEMENT:
		tok = cs->tokens.elems[*tok_id];

		if (tok->type == ID) {
			/* ASSIGN */
			parser_prod(cs, tok_id, N_ASSIGN);
		} else if (tok->type == IF) {
			/* IF block */
			parser_prod(cs, tok_id, N_IF);
		} else if (tok->type == EOS) {
			(*tok_id)++;
			return;
		} else {
			break;
		}

		parser_prod(cs, tok_id, N_STATEMENT);
		break;
	case N_ASSIGN:
		parser_match(cs, tok_id, ID);
		parser_match(cs, tok_id, ASSIGN);

		tok = cs->tokens.elems[*tok_id];

		if (tok->type == ID || tok->type == NUM)
			(*tok_id)++;
		else
			sc_utils_die("Wrong ASSIGN rule");
		break;
	case N_IF:
		/* if */
		parser_match(cs, tok_id, IF);
		/* ( */
		parser_match(cs, tok_id, LPAR);
		/* COND */
		parser_prod(cs, tok_id, N_COND);
		/* ) */
		parser_match(cs, tok_id, RPAR);
		/* then */
		parser_match(cs, tok_id, THEN);
		/* STATEMENT */
		parser_prod(cs, tok_id, N_STATEMENT);
		/* else */
		parser_match(cs, tok_id, ELSE);
		/* STATEMENT */
		parser_prod(cs, tok_id, N_STATEMENT);
		/* end */
		parser_match(cs, tok_id, END);
		break;
	case N_COND:
		/* ID*/
		parser_match(cs, tok_id, ID);
		/* == */
		parser_match(cs, tok_id, EQ);
		/* NUM || ID */
		tok = cs->tokens.elems[*tok_id];

		if (tok->type == NUM || tok->type == ID)
			(*tok_id)++;
		else
			sc_utils_die("Wrong cond rule");
		break;
	case EOS:
		return;
	default:
		sc_utils_die("Wrong grammar: %u", non_term);
		break;
	}
}

void sc_parser_begin(struct compiler_state *cs)
{
	/* Now it's not LL1 :) */
	size_t tok_id = 0;

	parser_prod(cs, &tok_id, N_PROG);
	printf("Correct, read %lu tokens of %lu\n",
	       tok_id, cs->tokens.size);
}

