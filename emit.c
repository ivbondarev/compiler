#include <stdlib.h>

#include "emit.h"
#include "vm.h"
#include "utils.h"
#include "lexer.h"
#include "ir.h"

static void emit_instr(struct virtual_machine *vm, u32 instr)
{
	vm->bytecode[vm->pc++] = instr;
}

static void emit_mov(struct virtual_machine *vm, u8 mod, u8 dst, u8 src,
		     u32 imm32)
{
	u32 instr = 0;

	instr |= MOV << 24;
	instr |= mod << 22;
	instr |= (dst & 0x3F) << 16;

	if (MOV_SLOT_SLOT == mod) {
		instr |= src << 8;
		emit_instr(vm, instr);
		return;
	}

	emit_instr(vm, instr);
	emit_instr(vm, imm32);
}

static void emit_jmp(struct virtual_machine *vm, u32 imm32)
{
	u32 instr = 0;

	instr |= JMP << 24;
	emit_instr(vm, instr);
	emit_instr(vm, imm32);
}

static void emit_hlt(struct virtual_machine *vm)
{
	u32 instr = HLT << 24;

	emit_instr(vm, instr);
}

void sc_emit_tac(struct compiler_state *cs)
{
	struct ir_instr *ir_ins;
	struct ir_obj *op1;
	struct ir_obj *op2;
	size_t total = 0;

	for (size_t i = 0; i < cs->irs->tac.size; i++) {
		ir_ins = cs->irs->tac.elems[i];

		switch (ir_ins->type) {
		case IR_INSTR_JMP:
			emit_jmp(cs->vm, ir_ins->go_to);
			total++;
			break;
		case IR_INSTR_ASSIGN:
			op1 = ir_ins->result;
			op2 = ir_ins->op1;
			if (IR_OBJ_VAR == op2->type)
				emit_mov(cs->vm, MOV_SLOT_SLOT, op1->global_id,
					 op2->global_id, 0);
			else
				emit_mov(cs->vm, MOV_SLOT_IMM32, op1->global_id,
					 0, op2->val);
			total++;
			break;
		default:
			break;
		}
	}

	emit_hlt(cs->vm);
	total++;

	printf("Total instructions: %lu\n", total);
}
