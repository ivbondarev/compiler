#ifndef _EMIT_H_
#define _EMIT_H_

#include "main.h"

void sc_emit_push(struct compiler_state *cs, u32 val);
void sc_emit_token_chain(struct compiler_state *cs, struct vector *tokens);

#endif /* !_EMIT_H_ */
