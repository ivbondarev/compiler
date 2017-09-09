#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <inttypes.h>

#include "vector.h"

typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

struct compiler_state {
	char *buf;
	size_t size;
	struct vector tokens;
};

#endif /* !_MAIN_H */
