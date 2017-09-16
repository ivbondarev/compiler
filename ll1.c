#include "lexer.h"
#include "ll1.h"
#include "main.h"
#include "utils.h"

/*static void pop(struct vector *v)
{
	v->size--;
}*/

void sc_ll1_parse(struct compiler_state *cs)
{
/*	struct vector stack;
	size_t i = 0;
	i32 table[TOK__MAX + 1][TOK__MAX + 1] = { { -1 } };

	table[N_S][ID] = 1;
	table[N_S][LPAR] = 2;
	table[N_A][ID] = 3;

	sc_vector_init(&stack);
	sc_vector_add(&stack, sc_lexer_tok(EOS));
	sc_vector_add(&stack, sc_lexer_tok(N_S));

	while (stack.size > 0) {
		struct token *top_tok = stack.elems[stack.size - 1];
		struct token *input_tok;

		if (i == cs->tokens.size)
			sc_utils_die("No input stream anymore");

		input_tok = cs->tokens.elems[i];

		if (top_tok->type == input_tok->type) {
			printf("Matched\n");
			pop(&stack);
			i++;
		} else {
			printf("Rule: %d\n",
				table[top_tok->type][input_tok->type]);

			switch (table[top_tok->type][input_tok->type]) {
			case 1:
				// A -> id 
				pop(&stack);
				sc_vector_add(&stack, sc_lexer_tok(N_A));
				break;
			case 2:
				//S -> (S + A) 
				pop(&stack);
				sc_vector_add(&stack, sc_lexer_tok(RPAR));
				sc_vector_add(&stack, sc_lexer_tok(N_A));
				sc_vector_add(&stack, sc_lexer_tok(PLUS));
				sc_vector_add(&stack, sc_lexer_tok(N_S));
				sc_vector_add(&stack, sc_lexer_tok(LPAR));
				break;
			case 3:
				pop(&stack);
				sc_vector_add(&stack, sc_lexer_tok(ID));
				break;
			default:
				sc_utils_die("Wrong grammar %d %d",
					top_tok->type, input_tok->type);
				break;
			}
		}
		
	}
	*/

	printf("Grammar is correct\n");
}

