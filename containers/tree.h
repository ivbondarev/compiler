#ifndef _TREE_H_
#define _TREE_H_

#include "../main.h"

struct node {
	struct vector nodes; /* Child nodes */
	struct node *parent; /* Parent node */
	void *data; /* Pointer to data */
};

/* Initialize tree structure */
void sc_tree_init(struct node *t, void *data);
/* Add sub-node to current node */
struct node *sc_tree_add_node(struct node *parent, void *data);

#endif /* !_TREE_H_ */
