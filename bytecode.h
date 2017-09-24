#ifndef _BYTECODE_H_
#define _BYTECODE_H_

#include "main.h"

struct virtual_machine;

#define SET_OP(ins, op) ((ins) |= op << 24)
#define SET_IMM16(ins, imm16) (ins = (ins & (~0xFFFF)) | (imm16 & 0xFFFF))

/* MOV slot, slot | MOV slot, imm16 */
void bcode_mov(struct virtual_machine *vm, u32 ins);
/* IP-relative imm16 jump */
void bcode_jmp(struct virtual_machine *vm, u32 ins);
/* Information about one given bytecode instruction */
int bcode_instr_info(struct virtual_machine *vm, u32 ins);

#endif /* !_BYTECODE_H_ */
