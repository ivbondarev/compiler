#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"
#include "state.h"
#include "postfix.h"
#include "vm.h"
#include "ll1.h"

int main(int argc, char *argv[])
{
	struct compiler_state comp;
	struct compiler_state *cs = &comp;

	/* Initialize compiler state */
	sc_state_init(cs, argc, argv);

	/* Convert input stream into tokens */
	sc_lexer_tokenize(cs);
	/* Print tokens */
	sc_lexer_token_chain(cs);

	sc_ll1_parse(cs);

	/* Produce RPN for arithmetic operators */
	//sc_sort_station_make_postfix(cs);

	/* Now we can execute bytecode */
	//sc_vm_start(cs->vm);
	/* Result is the single bottom frame on stack */
	//sc_vm_print_stack_result(cs);

	sc_state_destroy(cs);
	return 0;
}
