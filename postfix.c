#include <stddef.h>
#include <assert.h>

#include "containers/vector.h"
#include "containers/tree.h"
#include "front/lex.h"
#include "parser.h"

static int postfix_priority(const struct token *tok)
{
	switch (tok->type) {
	case MUL:
		return 0;
		break;
	case DIV:
		return 0;
		break;
	case PLUS:
		return 1;
		break;
	case MINUS:
		return 1;
		break;
	case LBRA:
		return 100;
		break;
	default:
		return 1000;
		break;
	};
}

static void postfix_step(struct token *tok,
			      struct vector *result, struct vector *stack)
{
	switch (tok->type) {
	case NUM:
	case ID:
		sc_vector_add(result, tok);
		break;
	case MUL:
	case DIV:
	case PLUS:
	case MINUS: {
		int priority = postfix_priority(tok);

		do {
			struct token *stack_top;

			if (sc_vector_size(stack) == 0)
				break;

			stack_top = sc_vector_get_last(stack);

			if (priority < postfix_priority(stack_top))
				break;

			sc_vector_add(result, stack_top);
			sc_vector_remove_last(stack);

		} while (sc_vector_size(stack) != 0);

		sc_vector_add(stack, tok);
		break;
	}
	case LPAR: {
		sc_vector_add(stack, tok);
		break;
	}
	case RPAR: {
		struct token *tok = sc_vector_get_last(stack);

		while (LPAR != tok->type) {
			sc_vector_add(result, tok);
			sc_vector_remove_last(stack);
			tok = sc_vector_get_last(stack);
		}

		sc_vector_remove_last(stack);
	}
		break;
	default:
		break;
	}
		
}	

static void postfix_print(struct vector *tokens)
{
	for (size_t i = 0; i < sc_vector_size(tokens); i++)
		printf("[%s] ", sc_lexer_token_info(sc_vector_get(tokens, i)));
	printf("\n");
}

void postfix_node_2_vector(struct node *n, struct vector *v)
{
	for (size_t i = 0; i < sc_vector_size(&n->nodes); i++) {
		struct node *child = sc_vector_get(&n->nodes, i);

		sc_vector_add(v, child->data);
	}
}

void postfix_build_tree(struct vector *rpn_toks, struct node *root)
{
	size_t i = 0;
	size_t root_nodes_sz = sc_vector_size(&root->nodes);
	struct node *new_root; /* Root node of the expression */

	while (sc_vector_size(rpn_toks) != 1) {
		struct token *tok = sc_vector_get(rpn_toks, i); /* current tok*/
		struct token *prev1; /* i - 1 */
		struct token *prev2; /* i - 2 */
		struct node *n; /* new node */
		struct node *l; /* left op */
		struct node *r; /* right op */
		struct token *faketok; /* Fake token (i.e. NODE) */

		if (tok->type == ID
		    || tok->type == NUM
		    || tok->type == NODE) {
			i++;
			continue;
		}

		n = malloc(sizeof(*n));
		sc_tree_init(n, tok);
		n->parent = NULL;

		prev2 = sc_vector_get(rpn_toks, i - 2);
		prev1 = sc_vector_get(rpn_toks, i - 1);

		if (prev2->type == NODE) {
			l = (struct node *)prev2->str;
		} else {
			l = malloc(sizeof(*l));
			sc_tree_init(l, prev2);
		}

		if (prev1->type == NODE) {
			r = (struct node *)prev1->str;
		} else {
			r = malloc(sizeof(*r));
			sc_tree_init(r, prev1);
		}

		l->parent = n;
		r->parent = n;

		sc_vector_add(&n->nodes, l);
		sc_vector_add(&n->nodes, r);

		/* Add fake Node token instead of 3 elements */
		faketok = sc_lexer_tok(NODE);
		rpn_toks->elems[i] = faketok;
		faketok->str = (char *)n;

		sc_vector_remove(rpn_toks, i - 1);
		sc_vector_remove(rpn_toks, i - 2);
		i = 0;
	}

	/* Remove old child nodes */
	while (root_nodes_sz != 0) {
		sc_vector_remove(&root->nodes, 0);
		root_nodes_sz--;
	}

	assert(sc_vector_size(&root->nodes) == 0);

	new_root = (struct node *)((struct token *)
			(sc_vector_get(rpn_toks, 0)))->str;
	new_root->parent = root;
	sc_vector_add(&root->nodes, new_root);
}

void sc_postfix_make(struct node *n)
{
	struct vector tokens;
	struct vector stack;
	struct vector result;
	size_t i = 0;
	size_t tokens_num = 0;

	sc_vector_init(&stack);
	sc_vector_init(&result);
	sc_vector_init(&tokens);

	postfix_node_2_vector(n, &tokens);
	tokens_num = sc_vector_size(&tokens);

	while (i < tokens_num) {
		struct token *tok = sc_vector_get(&tokens, i);

		postfix_step(tok, &result, &stack);
		i++;
	}

	while (sc_vector_size(&stack) > 0) {
		sc_vector_add(&result, sc_vector_get_last(&stack));
		sc_vector_remove_last(&stack);
	}

	postfix_build_tree(&result, n);
}
