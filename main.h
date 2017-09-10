#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "vector.h"

struct virtual_machine;

typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

struct compiler_state {
	char *buf;
	size_t size;
	struct vector tokens;
	struct virtual_machine *vm;
};

#endif /* !_MAIN_H */
