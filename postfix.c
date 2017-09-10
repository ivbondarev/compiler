#include "main.h"
#include "vector.h"
#include "lexer.h"
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

static void sort_station_step(struct compiler_state *cs, size_t i,
			      struct vector *result, struct vector *stack)
{
	struct token *tok = cs->tokens.elems[i];

	switch (tok->type) {
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

void sc_sort_station_make_postfix(struct compiler_state *cs)
{
	struct vector stack;
	struct vector result;
	size_t i = 0;

	sc_vector_init(&stack);
	sc_vector_init(&result);

	while (i < cs->tokens.size) {
		sort_station_step(cs, i, &result, &stack);
		i++;
	}

	while (stack.size > 0) {
		sc_vector_add(&result, stack.elems[stack.size - 1]);
		stack.size--;
	}

	sc_lexer_token_chain_vector(&result);
	sc_emit_token_chain(cs, &result);
	sc_vm_dump_bytecode(cs);
}
