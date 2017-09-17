#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "vector.h"

struct node;
struct virtual_machine;
struct ir_state;

typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

struct compiler_state {
	char *buf;
	size_t size;
	struct vector tokens;
	struct virtual_machine *vm;
	struct node *parse_tree;
	struct ir_state *irs;
};

#endif /* !_MAIN_H */
