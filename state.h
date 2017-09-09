#ifndef _STATE_H_
#define _STATE_H_

#include "main.h"

/* Basic state initialization */
void sc_state_init(struct compiler_state *cs, int argc, char **argv);
/* Deallcoated memeory, etc */
void sc_state_destroy(const struct compiler_state *cs);

#endif /* !_STATE_H_ */
