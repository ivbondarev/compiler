#include <assert.h>

#include "front/lex.h"
#include "containers/tree.h"
#include "parser.h"
#include "main.h"
#include "utils.h"
#include "postfix.h"

/* Recursive print info about all nodes */
static void parser_node_info(FILE *fp, struct node *n)
{
	const char *tok_info;
	struct token *tok;

	tok = n->data;
	if (tok->type == EXPRESSION)
		sc_postfix_make(n);

	for (size_t i = 0; i < n->nodes.size; i++)
		parser_node_info(fp, n->nodes.elems[i]);

	tok = (struct token *)n->data;
	tok_info = sc_lexer_token_info(tok);

	if (NULL != n->parent)
		fprintf(fp, "\t\"%p\" -> \"%p\";\n", n->parent, n);

	fprintf(fp, "\t\"%p\" [label=\"%s\"]\n", n, tok_info);
}

/* Dump tree to the file */
void sc_parser_dump_parse_tree(FILE *fp, struct node *root)
{
	fprintf(fp, "digraph G {\n");
	parser_node_info(fp, root);
	fprintf(fp, "}");
	fclose(fp);
}

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
			enum tokens non_term, struct node *parent)
{
	/* New node, which can be added to the root node */
	struct node *n;
	/* Current token */
	struct token *tok = sc_vector_get(&cs->tokens, *tok_id);
	struct token *next_tok;

	switch (non_term) {
	case STATEMENT:
		/* Is it a variable assignment? */
		if (tok->type == ID) {
			n = sc_tree_add_node(parent, sc_lexer_tok(ASSIGNMENT));
			parser_prod(cs, tok_id, ASSIGNMENT, n);
		}
		break;
	case ASSIGNMENT:
		/* id */
		sc_tree_add_node(parent, tok);
		parser_match(cs, tok_id, ID);
		/* = */
		sc_tree_add_node(parent, sc_lexer_tok(ASSIGN));
		parser_match(cs, tok_id, ASSIGN);
		n = sc_tree_add_node(parent, sc_lexer_tok(EXPRESSION));
		parser_prod(cs, tok_id, EXPRESSION, n);
		break;
	case EXPRESSION:
LOOP:
		tok = sc_vector_get(&cs->tokens, *tok_id);
		sc_tree_add_node(parent, tok);
		(*tok_id)++;
		next_tok = sc_vector_get(&cs->tokens, *tok_id);
		if ((tok->type == ID || tok->type == NUM)
			&& (next_tok->type != MINUS && next_tok->type != PLUS
			&& next_tok->type != MUL && next_tok->type != DIV
			&& next_tok->type != RPAR))
			break;
		goto LOOP;
		break;
	}
}

void sc_parser_begin(struct compiler_state *cs)
{
	/* Number of tokens processed in parsing (must be 100%) */
	size_t toks_num = 0;

	/* Start recursive descent parser, already has one node "Statement"*/
	parser_prod(cs, &toks_num, STATEMENT, cs->parse_tree);
	assert(toks_num + 1== cs->tokens.size); /* +1 is EOS */

	if (cs->dump.parse_tree != NULL)
		sc_parser_dump_parse_tree(cs->dump.parse_tree, cs->parse_tree);
}
