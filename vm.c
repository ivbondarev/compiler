#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "vm.h"

#define VM_STACK_SIZE 100
#define VM_INSTRUCTIONS_SIZE 100
#define OP_MASK 0xFF

void sc_vm_init(struct virtual_machine *vm)
{
	memset(vm, 0, sizeof(*vm));
	vm->stack = calloc(1, VM_STACK_SIZE * sizeof(*vm->stack));
	vm->bytecode = calloc(1, VM_INSTRUCTIONS_SIZE * sizeof(*vm->bytecode));

}

static void vm_print_stack(struct virtual_machine *vm)
{
	printf("* bot stack *\n");
	for (size_t i = 0; i < vm->sp; i++) {
		printf("----------\n|   %" PRIi32 "   |\n----------\n",
			vm->stack[i]);
	}
}

static u32 vm_fetch_instr(struct virtual_machine *vm)
{
	return vm->bytecode[vm->pc++];
}

static void vm_exec_push(struct virtual_machine *vm, u32 instr)
{
	u32 num = (instr >> 16) & 0xFFFF;
	vm->stack[vm->sp++] = num;
	vm_print_stack(vm);
}

static void vm_exec_add(struct virtual_machine *vm)
{
	vm->stack[vm->sp - 2] = (u32)((i32)vm->stack[vm->sp - 1]
		+ (i32)vm->stack[vm->sp - 2]);
	vm->sp--;
	vm_print_stack(vm);
}

static void vm_exec_mul(struct virtual_machine *vm)
{
	vm->stack[vm->sp - 2] = (u32)((i32)vm->stack[vm->sp - 1]
		* (i32)vm->stack[vm->sp - 2]);
	vm->sp--;
	vm_print_stack(vm);
}

static void vm_exec_sub(struct virtual_machine *vm)
{
	vm->stack[vm->sp - 2] = (u32)((i32)vm->stack[vm->sp - 2]
		- (i32)vm->stack[vm->sp - 1]);
	vm->sp--;
	vm_print_stack(vm);
}

static void *op_handler[I__MAX] = {
	vm_exec_push,
	NULL,
	vm_exec_mul,
	NULL,
	vm_exec_add,
	vm_exec_sub,
	NULL
};

static u32 vm_decode_instr(struct virtual_machine *vm, u32 instr)
{
	u32 op = instr & OP_MASK;
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

	while (IHALT != vm_decode_instr(vm, instr))
		instr = vm_fetch_instr(vm);
}

static int vm_print_instr_info(u32 instr)
{
	u32 op = instr & 0xFF;
	u32 val = instr >> 16;

	switch (op) {
	case IPUSH:
		printf("IPUSH %" PRIi32 "\n", val);
		break;
	case IMUL:
		printf("IMUL\n");
		break;
	case IADD:
		printf("IADD\n");
		break;
	case IDIV:
		printf("IDIV\n");
		break;
	case ISUB:
		printf("ISUB\n");
		break;
	case IPOP:
		printf("IPOP\n");
		break;
	case IHALT:
		printf("IHALT\n");
		return 1;
	default:
		return 1;
	};

	return 0;
}

void sc_vm_dump_bytecode(struct compiler_state *cs)
{
	u32 pc = 0;

	while (!vm_print_instr_info(cs->vm->bytecode[pc]))
		pc++;
}

void sc_vm_print_stack_result(struct compiler_state *cs)
{
	printf("Computation result: %" PRIi32 "\n", (i32)cs->vm->stack[0]);
}
