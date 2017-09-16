#include <string.h>

#include "tree.h"
#include "lexer.h"
#include "utils.h"

void sc_tree_init(struct node *n, struct token *base_tok)
{
	sc_vector_init(&n->nodes);
	n->base_tok = base_tok;
	n->parent = NULL;
}

void sc_tree_add_node(struct node *t, struct token *tok)
{
	struct node *n = calloc(1, sizeof(*n));

	sc_tree_init(n, tok);
	n->parent = t;
	sc_vector_add(&t->nodes, n);
}

static void tree_print_node_info(FILE *fp, struct node *n)
{
	for (size_t i = 0; i < n->nodes.size; i++)
		tree_print_node_info(fp, n->nodes.elems[i]);

	/* printf("NODE [%p] : parent [%p]\n", n, n->parent); */
	fprintf(fp, "\t\"%p\" -> \"%p\";\n", n->parent, n);
	fprintf(fp, "\t\"%p\" [label=\"%s\"]\n", n,
		sc_lexer_token_info(n->base_tok));
}

void sc_tree_dump(struct node *root)
{
	FILE *graph = fopen("graph", "w");

	if (NULL == graph)
		sc_utils_die("Can't open graph");

	fprintf(graph, "digraph G {\n");
	tree_print_node_info(graph, root);
	fprintf(graph, "\t\"(nil)\" [label=\"Parse tree\"]\n");
	fprintf(graph, "}");

	fclose(graph);
}
