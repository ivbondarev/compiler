#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"
#include "state.h"
#include "postfix.h"
#include "vm.h"
#include "parser.h"
#include "tree.h"

int main(int argc, char *argv[])
{
	struct compiler_state cs;

	/* Initialize compiler state */
	sc_state_init(&cs, argc, argv);
	/* Convert input stream into tokens */
	sc_lexer_tokenize(&cs);
	/* Print tokens */
	sc_lexer_print_tokens(&cs);
	/* Parse LL(k) grammar */
	sc_parser_begin(&cs);
	/* Dump parse tree to graphviz foramt */
	sc_tree_dump(cs.parse_tree);

	/* Produce RPN for arithmetic operators */
	//sc_sort_station_make_postfix(cs);

	/* Now we can execute bytecode */
	//sc_vm_start(cs->vm);
	/* Result is the single bottom frame on stack */
	//sc_vm_print_stack_result(cs);

	sc_state_destroy(&cs);
	return 0;
}
