#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "front/lex.h"
#include "containers/tree.h"
#include "utils.h"
#include "state.h"
#include "postfix.h"
#include "vm.h"
#include "parser.h"
#include "ast.h"
#include "ir.h"
#include "emit.h"

int main(int argc, char *argv[])
{
	struct compiler_state cs;

	sc_state_init(&cs, argc, argv);
	sc_lexer_read_tokens(&cs);
	sc_parser_begin(&cs);
	/* Transfrom derivation tree to AST */
	//sc_ast_transform(cs.parse_tree);
	/* Dump AST */
	//sc_parser_dump(cs.parse_tree);
	/* Generate IR */
	//sc_ir_make(&cs, cs.parse_tree);
	/* Dump IR to stdout */
	//sc_ir_print_tac(cs.vm, cs.irs);
	/* Transform IR to bytecode */
	//sc_emit_tac(&cs);
	/* Show bytecode*/
	//sc_vm_dump_bytecode(&cs);

	/* Produce RPN for arithmetic operators */
	//sc_sort_station_make_postfix(cs);

	/* Now we can execute bytecode */
	//sc_vm_start(cs->vm);
	/* Result is the single bottom frame on stack */
	//sc_vm_print_stack_result(cs);

	sc_state_destroy(&cs);
	return 0;
}
