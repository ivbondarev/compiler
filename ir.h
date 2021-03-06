#ifndef _IR_H_
#define _IR_H_

#include "main.h"

/* Type of IR instructions */
enum {
	IR_INSTR_ASSIGN, /* a = b*/
	IR_INSTR_CMP, /* a == b */
	IR_INSTR_JMP, /* jmp xxx */
	IR_INSTR_JNE, /* jne xxx */
	IR_INSTR_PUSH, /* push slot1 */
	IR_INSTR_POP, /* pop */
	IR_INSTR_CALL, /* print(var1) */
	IR_INSTR_HLT
};

enum {
	IR_OBJ_VAR,
	IR_OBJ_NUM,
	IR_OBJ_FUNC
};

struct ir_obj {
	u32 type;
	u32 global_id; /* ID in global symbol table, for IR_OBJ_VAR */
	u32 val; /* For IR_OBJ_NUM*/
};

struct ir_instr {
	u32 type;
	i32 go_to; /* For j* instructions */
	struct ir_obj *op1;
	struct ir_obj *op2;
	struct ir_obj *result;
};

struct ir_state {
	struct vector global_sym_table;
	struct vector tac;
};

/* Base initialization */
void sc_ir_init(struct ir_state *irs);
/* Generate IR */
void sc_ir_make(struct compiler_state *cs, struct node *ast_node);
/* Print TAC */
void sc_ir_print_tac(struct virtual_machine *vm, const struct ir_state *irs);

#endif /* !_IR_H_ */
