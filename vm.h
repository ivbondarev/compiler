#ifndef _VM_H_
#define _VM_H_

#include "main.h"

enum instructions {
	MOV,
	HLT,
	JMP,
	CMP,
	JNE,
	CALL,
	PUSH,
	I__MAX
};

#define MOV_SLOT_SLOT 0
#define MOV_SLOT_IMM32 1

struct vm_func {
	void *callback;
	size_t nargs;
	const char *name;
};

struct virtual_machine {
	u64 pc; /* Number of instruction being executing */
	u64 sp; /* First free slot on stack */
	u32 *bp; /* Frame pointer of executing function */
	u32 status; /* Status register */
	u32 *bytecode; /* Pointer to allocated bytecode */
	u32 *stack; /* Pinter to stack base */
	struct vector global_funcs;
};

/* Initialize virtual machine */
void sc_vm_init(struct virtual_machine *vm);
/* Starts bytecode execution */
void sc_vm_start(struct virtual_machine *vm);
/* Show emitted bytecode */
void sc_vm_dump_bytecode(struct compiler_state *cs);

#endif /* !_VM_H_ */
