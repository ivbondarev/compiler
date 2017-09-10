#ifndef _TREE_H_
#define _TREE_H_

#include "main.h"

struct node {
	struct vector nodes;
	struct node *parent;
	struct token *base_tok;
	u32 internal_id;
};

void sc_tree_init(struct node *t);
void sc_tree_dump(struct node *root);
void sc_tree_add_node(struct node *t, struct token *tok);

#endif /* !_TREE_H_ */
