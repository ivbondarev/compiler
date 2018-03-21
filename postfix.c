#include "main.h"
#include "containers/vector.h"
#include "containers/tree.h"
#include "front/lex.h"
#include "emit.h"
#include "vm.h"

static int sort_station_priority(const struct token *tok)
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

static void sort_station_step(struct token *tok,
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
		int priority = sort_station_priority(tok);

		do {
			void *stack_top;

			if (0 == stack->size)
				break;

			stack_top = stack->elems[stack->size - 1];

			if (priority < sort_station_priority(stack_top))
				break;
			sc_vector_add(result, stack_top);
			stack->size--;

		} while (stack->size != 0);
		sc_vector_add(stack, tok);
		break;
	}
	case LPAR: {
		sc_vector_add(stack, tok);
		break;
	}
	case RPAR: {
		struct token *tok = stack->elems[stack->size - 1];

		while (LPAR != tok->type) {
			sc_vector_add(result, tok);
			stack->size--;
			tok = stack->elems[stack->size - 1];
		}
		stack->size--;
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

void sc_postfix_make(struct node *n)
{
	struct vector tokens;
	struct vector stack;
	struct vector result;
	size_t i = 0;

	sc_vector_init(&stack);
	sc_vector_init(&result);
	sc_vector_init(&tokens);

	postfix_node_2_vector(n, &tokens);

	while (i < tokens.size) {
		struct token *tok = sc_vector_get(&tokens, i);

		sort_station_step(tok, &result, &stack);
		i++;
	}

	while (stack.size > 0) {
		sc_vector_add(&result, stack.elems[stack.size - 1]);
		stack.size--;
	}

	postfix_print(&result);
}
