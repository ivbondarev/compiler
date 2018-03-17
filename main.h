#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "misc/vector.h"

struct node;
struct virtual_machine;
struct ir_state;

typedef uint32_t u32;
typedef int32_t i32;
typedef int16_t i16;
typedef uint16_t u16;
typedef uint64_t u64;
typedef int64_t i64;
typedef uint8_t u8;
typedef int8_t i8;

struct dumper {
	FILE *bc;
	FILE *tokens;
	FILE *parse_tree;
	FILE *ast;
};

struct compiler_state {
	char *buf;
	size_t size;
	struct vector tokens;
	struct virtual_machine *vm;
	struct node *parse_tree;
	struct ir_state *irs;
	struct dumper dump;
};

#endif /* !_MAIN_H */
