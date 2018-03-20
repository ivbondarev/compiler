#include "bytecode.h"
#include "vm.h"

#define MOD(x) (x >> 22) & 0x03
#define IMM16(ins) (ins & 0xFFFF)
#define SLOT_DST(x) (x >> 16) & 0x3F
#define SLOT_SRC(x) (x >> 8) & 0xFF

static void set_slot(struct virtual_machine *vm, u32 slot_id, u32 val)
{
	*(vm->bp + slot_id) = val;
}

static u32 read_slot(struct virtual_machine *vm, u32 slot_id)
{
	return *(vm->bp + slot_id);
}

void bcode_jmp(struct virtual_machine *vm, u32 ins)
{
	i16 rel = (i16)IMM16(ins);

	vm->pc += rel;
}

void bcode_mov(struct virtual_machine *vm, u32 ins)
{
	u8 mod = MOD(ins);
	u8 dst_slot = SLOT_DST(ins);
	u32 src_slot = SLOT_SRC(ins);
	u32 imm32;

	switch (mod) {
	case MOV_SLOT_SLOT:
		/* mov slot, slot */
		set_slot(vm, dst_slot, read_slot(vm, src_slot));
		break;
	case MOV_SLOT_IMM32:
		/* mov slot, imm32 */
		imm32 = (u32)IMM16(ins);
		set_slot(vm, dst_slot, imm32);
		break;
	}
}

int bcode_instr_info(struct virtual_machine *vm, u32 ins)
{
	u32 op = OP(ins);
	u8 mod = MOD(ins);
	u8 dst_slot = SLOT_DST(ins);
	u32 src_slot = SLOT_SRC(ins);
	u32 imm32;

	switch (op) {
	case MOV:
		if (MOV_SLOT_SLOT == mod) {
			printf("mov %u, %u\n", dst_slot, src_slot);
		} else if (MOV_SLOT_IMM32 == mod) {
			imm32 = ins & 0xFFFF;
			printf("movi %u, %" PRIu32 "\n", dst_slot, imm32);
		}
		break;

	case ICMP:
		if (MOV_SLOT_SLOT == mod) {
			printf("cmp %u, %u\n", dst_slot, src_slot);
		} else if (MOV_SLOT_IMM32 == mod) {
			imm32 = ins & 0xFFFF;
			printf("cpi %u, %" PRIu32 "\n", dst_slot, imm32);
		}
		break;
	case JMP:
		imm32 = ins & 0xFFFF;
		printf("jmp %" PRIi32 "\n", (i32)imm32);
		break;
	case HLT:
		printf("hlt\n");
		return 1;
		break;
	case JNE:
		printf("jne %d\n", (i16)(ins & 0xFFFF));
		break;
	case PUSH:
		printf("push %d\n", (i16)(ins & 0xFFFF));
		break;
	case CALL:
		printf("call %d\n", (u16)(ins & 0xFFFF));
		break;
	default:
		printf("Wrong OP: %u, pc = %" PRIu64 "\n", op, vm->pc);
		return 1;
	};

	return 0;
}
