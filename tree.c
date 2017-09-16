#include <string.h>

#include "tree.h"
#include "lexer.h"
#include "utils.h"

static u32 internal_id = 10;

void sc_tree_init(struct node *n)
{
	memset(n, 0, sizeof(*n));
	sc_vector_init(&n->nodes);
	/* One base Non-terminal S */
	n->internal_id = internal_id++;
	n->base_tok = sc_lexer_tok(N_S);
	n->parent = NULL;
}

void sc_tree_add_node(struct node *t, struct token *tok)
{
	struct node *n = calloc(1, sizeof(*n));

	sc_tree_init(n);
	n->base_tok = tok;
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
	fprintf(graph, "}");

	fclose(graph);
}
