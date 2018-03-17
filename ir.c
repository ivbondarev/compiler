#include <string.h>

#include "containers/tree.h"
#include "front/lex.h"
#include "ir.h"
#include "vm.h"
#include "utils.h"

void sc_ir_init(struct ir_state *irs)
{
	sc_vector_init(&irs->global_sym_table);
	sc_vector_init(&irs->tac);
}

static size_t symbol_to_global_symtab(struct ir_state *irs, char *sym)
{
	for (size_t i = 0; i < irs->global_sym_table.size; i++) {
		if (0 == strcmp(irs->global_sym_table.elems[i], sym))
			return i;
	}

	sc_vector_add(&irs->global_sym_table, sym);
	return irs->global_sym_table.size - 1;
}

static size_t sym_func_global_id(struct virtual_machine *vm, const char *sym)
{
	for (size_t i = 0; i < vm->global_funcs.size; i++) {
		struct vm_func *func = vm->global_funcs.elems[i];

		if (0 == strcmp(func->name, sym))
			return i;
	}

	sc_utils_die("Wrong global func %s\n", sym);
	return 0;
}

static struct ir_obj *ir_newobj(void)
{
	return calloc(1, sizeof(struct ir_obj));
}

static struct ir_instr *ir_newins(void)
{
	return calloc(1, sizeof(struct ir_instr));
}

static void ir_obj_info(struct virtual_machine *vm, const struct ir_state *irs,
			const struct ir_obj *obj)
{
	struct vm_func *func;

	switch (obj->type) {
	case IR_OBJ_NUM:
		printf("%u", obj->val);
		break;
	case IR_OBJ_VAR:
		printf("t%u", obj->global_id);
		break;
	case IR_OBJ_FUNC:
		func = vm->global_funcs.elems[obj->global_id];
		printf("%s", func->name);
		break;
	default:
		break;
	}
}

static void ir_demangle_var_info(const struct ir_state *irs,
				 const struct ir_obj *var)
{
	printf("t%u = %s", var->global_id,
	       (char *)irs->global_sym_table.elems[var->global_id]);
}


static void ir_instr_info(struct virtual_machine *vm,
			  const struct ir_state *irs,
			  const struct ir_instr *ins)
{
	struct ir_obj *obj1;
	struct ir_obj *obj2;

	switch (ins->type) {
	case IR_INSTR_HLT:
		printf("hlt\n");
		break;
	case IR_INSTR_JMP:
		printf("jmp %d", ins->go_to);
		printf("\n");
		break;
	case IR_INSTR_JNE:
		printf("jne %d", ins->go_to);
		printf("\n");
		break;
	case IR_INSTR_ASSIGN:
		obj1 = ins->result;
		obj2 = ins->op1;
		ir_obj_info(vm, irs, obj1);
		printf(" = ");
		ir_obj_info(vm, irs, obj2);
		printf("   # ");
		ir_demangle_var_info(irs, obj1);
		printf("\n");
		break;
	case IR_INSTR_CMP:
		obj1 = ins->op1;
		obj2 = ins->op2;
		printf("cmp ");
		ir_obj_info(vm, irs, obj1);
		printf(" ");
		ir_obj_info(vm, irs, obj2);
		printf("   # ");
		ir_demangle_var_info(irs, obj1);
		printf("\n");
		break;
	case IR_INSTR_CALL:
		obj1 = ins->result;
		obj2 = ins->op1;
		printf("call ");
		ir_obj_info(vm, irs, obj1);
		printf(" ");
		ir_obj_info(vm, irs, obj2);
		printf("   # ");
		ir_demangle_var_info(irs, obj1);
		printf("\n");
		break;
	default:
		sc_utils_die("Wrong ir\n");
		break;
	}
}

void sc_ir_print_tac(struct virtual_machine *vm, const struct ir_state *irs)
{
	for (size_t i = 0; i < irs->tac.size; i++) {
		printf("%lu) ", i);
		ir_instr_info(vm, irs, irs->tac.elems[i]);
	}
}

static struct token *node_tok(struct node *n, size_t i)
{
	struct node *ni = n->nodes.elems[i];

	return ni->base_tok;
}

void ir_gen(struct virtual_machine *vm, struct ir_state *irs,
	    struct node *ast_node)
{
	if (N_FUNCCALL == ast_node->base_tok->type) {
		struct node *n1 = ast_node->nodes.elems[0];
		struct node *n2 = ast_node->nodes.elems[1];
		struct token *func_name = n1->base_tok;
		struct token *var_name = n2->base_tok;
		struct ir_obj *func = ir_newobj();
		struct ir_obj *var = ir_newobj();
		struct ir_instr *ins = ir_newins();

		func->type = IR_OBJ_FUNC;
		func->global_id = sym_func_global_id(vm, func_name->str);

		var->type = IR_OBJ_VAR;
		var->global_id = symbol_to_global_symtab(irs, var_name->str);

		ins->type = IR_INSTR_CALL;
		ins->result = func;
		ins->op1 = var;

		sc_vector_add(&irs->tac, ins);
		return;
	}

	if (ASSIGN == ast_node->base_tok->type) {
		struct node *n1 = ast_node->nodes.elems[0];
		struct node *n2 = ast_node->nodes.elems[1];
		
		struct token *lhs_tok = n1->base_tok;
		struct token *rhs_tok = n2->base_tok;
		struct ir_obj *lhs = ir_newobj();
		struct ir_obj *rhs = ir_newobj();
		struct ir_instr *ins = ir_newins();

		lhs->type = IR_OBJ_VAR;
		lhs->global_id = symbol_to_global_symtab(irs, lhs_tok->str);

		if (rhs_tok->type == NUM) {
			rhs->type = IR_OBJ_NUM;
			rhs->val = rhs_tok->val;
		} else {
			rhs->type = IR_OBJ_VAR;
			rhs->global_id =
				symbol_to_global_symtab(irs, rhs_tok->str);
		}

		ins->type = IR_INSTR_ASSIGN;
		ins->result = lhs;
		ins->op1 = rhs;

		sc_vector_add(&irs->tac, ins);
		return;
	}

	if (N_COND == ast_node->base_tok->type) {
		/* Tokens */
		struct token *tok1 = node_tok(ast_node, 0);
		struct token *tok3 = node_tok(ast_node, 2);
		/* Left and rigth tokens */
		struct ir_obj *obj1 = ir_newobj();
		struct ir_obj *obj2 = ir_newobj();
		/* IR nstruction */
		struct ir_instr *ins = ir_newins();

		ins->type = IR_INSTR_CMP;

		obj1->type = IR_OBJ_VAR;
		obj1->global_id = symbol_to_global_symtab(irs, tok1->str);

		obj2->type = IR_OBJ_NUM;
		obj2->val = tok3->val;

		ins->op1 = obj1;
		ins->op2 = obj2;

		sc_vector_add(&irs->tac, ins);
		return;
	}

	if (N_IF == ast_node->base_tok->type) {
		struct ir_instr *ins_jne = ir_newins();
		struct ir_instr *ins_jmp = ir_newins();
		i32 rel_jump;

		ins_jne->type = IR_INSTR_JNE;
		ins_jmp->type = IR_INSTR_JMP;

		/* Condition */
		ir_gen(vm, irs, ast_node->nodes.elems[0]);
		/* Avoid true condition */
		sc_vector_add(&irs->tac, ins_jne);
		rel_jump = (i32)irs->tac.size;
		/* True */
		ir_gen(vm, irs, ast_node->nodes.elems[1]);
		/* Avoid else condition */
		sc_vector_add(&irs->tac, ins_jmp);
		ins_jne->go_to = (i32)(irs->tac.size - rel_jump);
		/* False */
		rel_jump = (i32)irs->tac.size;
		ir_gen(vm, irs, ast_node->nodes.elems[2]);
		ins_jmp->go_to = (i32)(irs->tac.size - rel_jump);
		return;
	}

	for (size_t i = 0; i < ast_node->nodes.size; i++)
		ir_gen(vm, irs, ast_node->nodes.elems[i]);
}


void sc_ir_make(struct compiler_state *cs, struct node *ast_node)
{
	struct ir_instr *ins_halt = ir_newins();

	ins_halt->type = IR_INSTR_HLT;
	ir_gen(cs->vm, cs->irs, ast_node);
	sc_vector_add(&cs->irs->tac, ins_halt);
}
