#include "front/lex.h"
#include "containers/tree.h"

void sc_ast_transform(struct node *n)
{
	/*struct token *tok = (struct token *)n->data;


	if (IF == tok->type) {
		sc_vector_remove_elem(&n->nodes, 0);
		sc_vector_remove_elem(&n->nodes, 0);
		sc_vector_remove_elem(&n->nodes, 1);
		sc_vector_remove_elem(&n->nodes, 1);
		sc_vector_remove_elem(&n->nodes, 2);
		sc_vector_remove_elem(&n->nodes, 3);
	}

	if (FUNCCALL == tok->type) {
		sc_vector_remove_elem(&n->nodes, 1);
		sc_vector_remove_elem(&n->nodes, 2);
	}

	for (size_t i = 0; i < n->nodes.size; i++)
		sc_ast_transform(n->nodes.elems[i]);*/
}
