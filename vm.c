#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "vm.h"

#define VM_STACK_SIZE 100
#define VM_INSTRUCTIONS_SIZE 100
#define OP_MASK 0xFF

enum instructions {
	IPUSH,
	IPOP,
	IMUL,
	IDIV,
	IADD,
	IMINUS,
	IHALT,
	I__MAX
};

void sc_vm_init(struct virtual_machine *vm)
{
	memset(vm, 0, sizeof(*vm));
	vm->stack = malloc(VM_STACK_SIZE * sizeof(*vm->stack));
	vm->bytecode = malloc(VM_INSTRUCTIONS_SIZE * sizeof(*vm->bytecode));

}

static u32 vm_fetch_instr(struct virtual_machine *vm)
{
	return vm->bytecode[vm->pc++];
}

static void vm_exec_push(struct virtual_machine *vm, u32 instr)
{
	u32 num = (instr >> 16) & 0xFFFF;
	vm->stack[vm->sp++] = num;
}

static void *op_handler[I__MAX] = {
	vm_exec_push,
};

static u32 vm_decode_instr(struct virtual_machine *vm, u32 instr)
{
	u32 op = instr & OP_MASK;
	void (*handler)(struct virtual_machine *, u32);

	if (op >= I__MAX)
		sc_utils_die("Worng opcode: %" PRIu32 "\n", op);

	handler = op_handler[op];
	handler(vm, instr);
	return op;
}

void sc_vm_start(struct virtual_machine *vm)
{	
	u32 instr = vm_fetch_instr(vm);

	while (IHALT != vm_decode_instr(vm, instr))
		instr = vm_fetch_instr(vm);
}
