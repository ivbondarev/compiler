#include <stdlib.h>

#include "emit.h"
#include "vm.h"
#include "utils.h"
#include "lexer.h"

static void emit_instr(struct compiler_state *cs, u32 instr)
{
	cs->vm->bytecode[cs->vm->pc++] = instr;
}

void sc_emit_push(struct compiler_state *cs, u32 val)
{
	u32 instr = IPUSH;

	instr |= (val & 0xFFFF) << 16;
	emit_instr(cs, instr);
}

void sc_emit_pop(struct compiler_state *cs)
{
	u32 instr = IPOP;

	emit_instr(cs, instr);
}

void sc_emit_mul(struct compiler_state *cs)
{
	u32 instr = IMUL;

	emit_instr(cs, instr);
}

void sc_emit_div(struct compiler_state *cs)
{
	u32 instr = IDIV;

	emit_instr(cs, instr);
}

void sc_emit_add(struct compiler_state *cs)
{
	u32 instr = IADD;

	emit_instr(cs, instr);
}

void sc_emit_sub(struct compiler_state *cs)
{
	u32 instr = ISUB;

	emit_instr(cs, instr);
}

void sc_emit_halt(struct compiler_state *cs)
{
	u32 instr = IHALT;

	emit_instr(cs, instr);
}

void sc_emit_token_chain(struct compiler_state *cs, struct vector *tokens)
{
	for (size_t i = 0; i < tokens->size; i++) {
		struct token *tok = tokens->elems[i];
		u32 val;

		switch (tok->type) {
		case ID:
			val = (u32)atoi(tok->str) & 0xFFFF;
			sc_emit_push(cs, val);
			break;
		case PLUS:
			sc_emit_add(cs);
			break;
		case MINUS:
			sc_emit_sub(cs);
			break;
		case MUL:
			sc_emit_mul(cs);
			break;
		case DIV:
			sc_emit_div(cs);
			break;
		default:
			break;
		}
	}

	sc_emit_halt(cs);
}
