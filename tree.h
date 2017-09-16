#ifndef _TREE_H_
#define _TREE_H_

#include "main.h"

struct node {
	struct vector nodes;
	struct node *parent;
	struct token *base_tok;
};

void sc_tree_init(struct node *t, struct token *base_tok);
void sc_tree_dump(struct node *root);
struct node *sc_tree_add_node(struct node *n, struct token *tok);

#endif /* !_TREE_H_ */
