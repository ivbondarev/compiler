#include <stdlib.h>

#include "emit.h"
#include "vm.h"
#include "utils.h"
#include "front/lex.h"
#include "ir.h"
#include "bytecode.h"

#define EMIT_LONG_OP2(ins, op2) ((ins) |= op2 & 0xFFFF)

static void emit_instr(struct virtual_machine *vm, u32 instr)
{
	vm->bytecode[vm->pc++] = instr;
}

static void emit_push(struct virtual_machine *vm, u16 var_stack_id)
{
	u32 instr = 0;

	/* Opcode is PUSH */
	SET_OP(instr, PUSH);
	/* IMM16 is stack slot id relative to base */
	SET_IMM16(instr, var_stack_id);

	emit_instr(vm, instr);
}

static void emit_call(struct virtual_machine *vm, u8 global_func_id, u16 var_id)
{
	u32 instr = 0;

	SET_OP(instr, CALL);
	SET_IMM16(instr, global_func_id);
	emit_push(vm, var_id);
	emit_instr(vm, instr);
}

static void emit_mov(struct virtual_machine *vm, u8 mod, u8 dst, u8 src,
		     u32 imm32)
{
	u32 instr = 0;
	u16 imm = imm32 & 0xFFFF;

	instr |= MOV << 24;
	instr |= mod << 22;
	instr |= (dst & 0x3F) << 16;

	if (MOV_SLOT_SLOT == mod) {
		instr |= src << 8;
		emit_instr(vm, instr);
		return;
	}

	EMIT_LONG_OP2(instr, imm);
	emit_instr(vm, instr);
}

static void emit_jne(struct virtual_machine *vm, u16 imm16)
{
	u32 instr = JNE << 24;

	EMIT_LONG_OP2(instr, imm16 & 0xFFFF);
	emit_instr(vm, instr);
}

static void emit_cmp(struct virtual_machine *vm, u8 mod, u8 op1, u8 op2,
		     u32 imm32)
{
	u32 instr = 0;
	u16 imm;

	instr |= CMP << 24 | mod << 22;
	switch (mod) {
	case MOV_SLOT_SLOT:
		instr |= ((op1 & 0x3F) << 16) | op2 << 8;
		break;
	case MOV_SLOT_IMM32:
		instr |= ((op1 & 0x3F) << 16);
		imm = imm32 & 0xFFFF;
		EMIT_LONG_OP2(instr, imm);
		break;
	default:
		break;
	}

	emit_instr(vm, instr);
}

static void emit_jmp(struct virtual_machine *vm, u32 imm32)
{
	u32 instr = 0;

	instr |= JMP << 24;
	EMIT_LONG_OP2(instr, imm32 & 0xFFFF);
	emit_instr(vm, instr);
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
		case IR_INSTR_CMP:
			op1 = ir_ins->op1;
			op2 = ir_ins->op2;
			if (IR_OBJ_VAR == op2->type)
				emit_cmp(cs->vm, MOV_SLOT_SLOT, op1->global_id,
					 op2->global_id, 0);
			else
				emit_cmp(cs->vm, MOV_SLOT_IMM32, op1->global_id,
					 0, op2->val);
			total++;
			break;
		case IR_INSTR_JNE:
			emit_jne(cs->vm, ir_ins->go_to);
			total++;
			break;
		case IR_INSTR_CALL:
			/* Func */
			op1 = ir_ins->result;
			/* Var */
			op2 = ir_ins->op1;
			emit_call(cs->vm, op1->global_id, op2->global_id);
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
