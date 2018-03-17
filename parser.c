#include "front/lex.h"
#include "containers/tree.h"
#include "parser.h"
#include "main.h"
#include "utils.h"

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
			enum tokens non_term, struct node *n)
{
	struct token *tok;
	/* New node to be add */
	struct node *nn;

	switch (non_term) {
	case N_PROG:
		nn = sc_tree_add_node(n, sc_lexer_tok(N_STATEMENT));
		parser_prod(cs, tok_id, N_STATEMENT, nn);
		break;
	case N_STATEMENT:
		tok = cs->tokens.elems[*tok_id];

		if (tok->type == ID) {
			struct token *next_tok = cs->tokens.elems[*tok_id + 1];

			if (next_tok->type == LPAR) {
				/* FUNCCALL */
				nn = sc_tree_add_node(n, sc_lexer_tok(N_FUNCCALL));
				parser_prod(cs, tok_id, N_FUNCCALL, nn);
			} else {
				/* ASSIGN */
				nn = sc_tree_add_node(n, sc_lexer_tok(N_ASSIGN));
				parser_prod(cs, tok_id, N_ASSIGN, nn);
			}
		} else if (tok->type == IF) {
			/* IF block */
			nn = sc_tree_add_node(n, sc_lexer_tok(N_IF));
			parser_prod(cs, tok_id, N_IF, nn);
		} else if (tok->type == EOS) {
			(*tok_id)++;
			return;
		} else {
			break;
		}

		parser_prod(cs, tok_id, N_STATEMENT, n);
		break;
	case N_FUNCCALL:
		tok = cs->tokens.elems[*tok_id];
		/* FUNC ID */
		parser_match(cs, tok_id, ID);
		sc_tree_add_node(n, tok);
		/* ( */
		parser_match(cs, tok_id, LPAR);
		tok = cs->tokens.elems[*tok_id];
		sc_tree_add_node(n, sc_lexer_tok(LPAR));
		/* var */
		parser_match(cs, tok_id, ID);
		sc_tree_add_node(n, tok);
		/* ) */
		parser_match(cs, tok_id, RPAR);
		sc_tree_add_node(n, sc_lexer_tok(RPAR));
		break;
	case N_ASSIGN:
		tok = cs->tokens.elems[*tok_id];
		parser_match(cs, tok_id, ID);
		sc_tree_add_node(n, tok);
		parser_match(cs, tok_id, ASSIGN);
		sc_tree_add_node(n, sc_lexer_tok(ASSIGN));

		tok = cs->tokens.elems[*tok_id];

		if (tok->type == ID || tok->type == NUM) {
			(*tok_id)++;
			sc_tree_add_node(n, tok);
		} else {
			sc_utils_die("Wrong ASSIGN rule");
		}
		break;
	case N_IF:
		/* if */
		parser_match(cs, tok_id, IF);
		sc_tree_add_node(n, sc_lexer_tok(IF));
		/* ( */
		parser_match(cs, tok_id, LPAR);
		sc_tree_add_node(n, sc_lexer_tok(LPAR));
		/* COND */
		nn = sc_tree_add_node(n, sc_lexer_tok(N_COND));
		parser_prod(cs, tok_id, N_COND, nn);
		/* ) */
		parser_match(cs, tok_id, RPAR);
		sc_tree_add_node(n, sc_lexer_tok(RPAR));
		/* then */
		parser_match(cs, tok_id, THEN);
		sc_tree_add_node(n, sc_lexer_tok(THEN));
		/* STATEMENT */
		nn = sc_tree_add_node(n, sc_lexer_tok(N_STATEMENT));
		parser_prod(cs, tok_id, N_STATEMENT, nn);
		/* else */
		sc_tree_add_node(n, sc_lexer_tok(ELSE));
		parser_match(cs, tok_id, ELSE);
		/* STATEMENT */
		nn = sc_tree_add_node(n, sc_lexer_tok(N_STATEMENT));
		parser_prod(cs, tok_id, N_STATEMENT, nn);
		/* end */
		sc_tree_add_node(n, sc_lexer_tok(END));
		parser_match(cs, tok_id, END);
		break;
	case N_COND:
		/* ID*/
		tok = cs->tokens.elems[*tok_id];
		sc_tree_add_node(n, tok);
		parser_match(cs, tok_id, ID);
		/* == */
		sc_tree_add_node(n, sc_lexer_tok(EQ));
		parser_match(cs, tok_id, EQ);
		/* NUM || ID */
		tok = cs->tokens.elems[*tok_id];

		if (tok->type == NUM || tok->type == ID) {
			(*tok_id)++;
			sc_tree_add_node(n, tok);
		} else {
			sc_utils_die("Wrong cond rule");
		}
		break;
	case EOS:
		return;
	default:
		sc_utils_die("Wrong grammar: %u", non_term);
		break;
	}
}

static void parser_node_info(FILE *fp, struct node *n)
{
	char *tok_info;
	struct token *tok;

	for (size_t i = 0; i < n->nodes.size; i++)
		parser_node_info(fp, n->nodes.elems[i]);

	tok = (struct token *)n->data;
	tok_info = sc_lexer_token_info(tok);

	if (NULL != n->parent)
		fprintf(fp, "\t\"%p\" -> \"%p\";\n", n->parent, n);

	fprintf(fp, "\t\"%p\" [label=\"%s\"]\n", n, tok_info);

	if (tok->type == NUM)
		free(tok_info);
}

void sc_parser_dump(FILE *fp, struct node *root)
{
	fprintf(fp, "digraph G {\n");
	parser_node_info(fp, root);
	fprintf(fp, "}");
	fclose(fp);
}

void sc_parser_begin(struct compiler_state *cs)
{
	size_t tok_id = 0;

	parser_prod(cs, &tok_id, N_PROG, cs->parse_tree);

	if (tok_id != cs->tokens.size)
		sc_utils_die("Wrong number of token parser");
	if (cs->dump.parse_tree != NULL)
		sc_parser_dump(cs->dump.parse_tree, cs->parse_tree);
}
