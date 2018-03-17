#include <string.h>

#include "tree.h"
#include "../front/lex.h"
#include "../utils.h"

void sc_tree_init(struct node *root, void *data)
{
	sc_vector_init(&root->nodes);
	root->data = data;
	root->parent = NULL;
}

struct node *sc_tree_add_node(struct node *parent, void *data)
{
	struct node *n = calloc(1, sizeof(*n));

	sc_tree_init(n, data);
	n->parent = parent;
	sc_vector_add(&parent->nodes, n);
	return n;
}
