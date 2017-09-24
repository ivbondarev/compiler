#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "vm.h"
#include "bytecode.h"

#define VM_STACK_SIZE 100
#define VM_INSTRUCTIONS_SIZE 100

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

static void *op_handler[I__MAX] = {
	bcode_mov,
	NULL,
	bcode_jmp
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

void sc_vm_dump_bytecode(struct compiler_state *cs)
{
	cs->vm->pc = 0;

	while (!bcode_instr_info(cs->vm, cs->vm->bytecode[cs->vm->pc++]))
		;
}
