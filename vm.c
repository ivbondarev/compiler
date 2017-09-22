#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "vm.h"

#define VM_STACK_SIZE 100
#define VM_INSTRUCTIONS_SIZE 100
#define OP(x) (x >> 24) & 0xFF
#define MOD(x) (x >> 22) & 0x03
#define SLOT_DST(x) (x >> 16) & 0x3F
#define SLOT_SRC(x) (x >> 8) & 0xFF

/*
 * Instruction format:
 * OP | MOD | REG | REG | 0 |       |
 * OP | MOD | 0   | 0   | 0 | IMM32 |
 * OP | MOD | REG | 0   | 0 | IMM32 |
 * ---|-----|-----|-----|---|-------|
 * 8  | 2   | 6   | 8   | 8 |   32  |
 *
 * STATUS32:
 * | 0 | Z |
 * |---|---|
 * |31 | 1 |
 */

enum {
	FLG_ZERO,
	FLG_CARRY
};

static void g_func_print(u64 val)
{
	printf("%" PRIu64 "\n", val);
}

static void add_global_func(struct virtual_machine *vm, const char *name,
			    size_t nargs, void *clbck)
{
	struct vm_func *func = calloc(1, sizeof(*func));

	func->name = name;
	func->nargs = nargs;
	func->callback = clbck;
	sc_vector_add(&vm->global_funcs, func);
}

void sc_vm_init(struct virtual_machine *vm)
{
	memset(vm, 0, sizeof(*vm));
	vm->stack = calloc(1, VM_STACK_SIZE * sizeof(*vm->stack));
	vm->bytecode = calloc(1, VM_INSTRUCTIONS_SIZE * sizeof(*vm->bytecode));

	/* Global functions */
	add_global_func(vm, "print", 1, g_func_print);
}

static u32 vm_fetch_instr(struct virtual_machine *vm)
{
	return vm->bytecode[vm->pc++];
}

static void vm_reset_status(struct virtual_machine *vm)
{
	vm->status = 0;
}

static void vm_set_flag(struct virtual_machine *vm, u32 flag, u32 val)
{
	if (val)
		vm->status |= 1 << flag;
	else
		vm->status &= (~(1 << flag));
}

static void vm_set_slot(struct virtual_machine *vm, u32 slot_id, u32 val)
{
	*(vm->bp + slot_id) = val;
}

static u32 vm_read_slot(struct virtual_machine *vm, u32 slot_id)
{
	return *(vm->bp + slot_id);
}

static void vm_instr_mov(struct virtual_machine *vm, u32 instr)
{
	u8 mod = MOD(instr);
	u8 dst_slot = SLOT_DST(instr);
	u32 src_slot = SLOT_SRC(instr);
	u32 imm32;

	switch (mod) {
	case MOV_SLOT_SLOT:
		/* mov slot, slot */
		vm_set_slot(vm, dst_slot, vm_read_slot(vm, src_slot));
		break;
	case MOV_SLOT_IMM32:
		/* mov slot, imm32 */
		imm32 = vm_fetch_instr(vm);
		vm_set_slot(vm, dst_slot, imm32);
		break;
	}
}

static void vm_instr_jmp(struct virtual_machine *vm, u32 instr)
{
	i32 imm32 = (i32)vm_fetch_instr(vm);

	vm->pc += imm32;
}

static void *op_handler[I__MAX] = {
	vm_instr_mov,
	NULL,
	vm_instr_jmp
};

static u32 vm_decode_instr(struct virtual_machine *vm, u32 instr)
{
	u32 op = OP(instr);
	void (*handler)(struct virtual_machine *, u32);

	if (op >= I__MAX)
		sc_utils_die("Worng opcode: %" PRIu32 "\n", op);

	if (NULL != (handler = op_handler[op]))
		handler(vm, instr);
	return op;
}

void sc_vm_start(struct virtual_machine *vm)
{
	u32 instr;
	vm->pc = 0;	
	
	instr = vm_fetch_instr(vm);

	while (HLT != vm_decode_instr(vm, instr))
		instr = vm_fetch_instr(vm);
}

static int vm_print_instr_info(struct virtual_machine *vm, u32 instr)
{
	u32 op = OP(instr);
	u8 mod = MOD(instr);
	u8 dst_slot = SLOT_DST(instr);
	u32 src_slot = SLOT_SRC(instr);
	u32 imm32;

	switch (op) {
	case MOV:
		if (MOV_SLOT_SLOT == mod) {
			printf("mov %u, %u\n", dst_slot, src_slot);
		} else if (MOV_SLOT_IMM32 == mod) {
			imm32 = instr & 0xFFFF;
			printf("movi %u, %" PRIu32 "\n", dst_slot, imm32);
		}
		break;

	case CMP:
		if (MOV_SLOT_SLOT == mod) {
			printf("cmp %u, %u\n", dst_slot, src_slot);
		} else if (MOV_SLOT_IMM32 == mod) {
			imm32 = instr & 0xFFFF;
			printf("cpi %u, %" PRIu32 "\n", dst_slot, imm32);
		}
		break;
	case JMP:
		imm32 = instr & 0xFFFF;
		printf("jmp %" PRIi32 "\n", (i32)imm32);
		break;
	case HLT:
		printf("hlt\n");
		return 1;
		break;
	case JNE:
		printf("jne %d\n", (i16)(instr & 0xFFFF));
		break;
	case PUSH:
		printf("push %d\n", (i16)(instr & 0xFFFF));
		break;
	case CALL:
		printf("call %d\n", (u16)(instr & 0xFFFF));
		break;
	default:
		printf("Wrong OP: %u, pc = %" PRIu64 "\n", op, vm->pc);
		return 1;
	};

	return 0;
}

void sc_vm_dump_bytecode(struct compiler_state *cs)
{
	cs->vm->pc = 0;

	while (!vm_print_instr_info(cs->vm, cs->vm->bytecode[cs->vm->pc++]))
		;
}
