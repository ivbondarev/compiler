#ifndef _VM_H_
#define _VM_H_

#include "main.h"

enum instructions {
	IPUSH,
	IPOP,
	IMUL,
	IDIV,
	IADD,
	ISUB,
	IHALT,
	I__MAX
};

struct virtual_machine {
	u64 pc;
	u64 sp;
	u32 status;
	u32 *bytecode;
	u32 *stack;
};

/* Initialize virtual machine */
void sc_vm_init(struct virtual_machine *vm);
/* Starts bytecode execution */
void sc_vm_start(struct virtual_machine *vm);
void sc_vm_dump_bytecode(struct compiler_state *cs);
void sc_vm_print_stack_result(struct compiler_state *cs);

#endif /* !_VM_H_ */
