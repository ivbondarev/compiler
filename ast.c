#include "lexer.h"
#include "tree.h"

void sc_ast_transform(struct node *n)
{
	if (N_ASSIGN == n->base_tok->type) {
		n->base_tok->type = ASSIGN;
		sc_vector_remove_elem(&n->nodes, 1);
		return;
	}

	if (N_IF == n->base_tok->type) {
		sc_vector_remove_elem(&n->nodes, 0);
		sc_vector_remove_elem(&n->nodes, 0);
		sc_vector_remove_elem(&n->nodes, 1);
		sc_vector_remove_elem(&n->nodes, 1);
		sc_vector_remove_elem(&n->nodes, 2);
		sc_vector_remove_elem(&n->nodes, 3);
	}

	for (size_t i = 0; i < n->nodes.size; i++)
		sc_ast_transform(n->nodes.elems[i]);

}
