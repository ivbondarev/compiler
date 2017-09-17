#ifndef _AST_H_
#define _AST_H_

struct node n;

/* Makes AST from derivation tree */
void sc_ast_transform(struct node *n);

#endif /* !_AST_H_ */
