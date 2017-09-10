#ifndef _VM_H_
#define _VM_H_

#include "main.h"

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

#endif /* !_VM_H_ */
