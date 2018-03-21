#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#include "main.h"

size_t sc_utils_file_size(FILE *fp)
{
	size_t sz;

	fseek(fp, 0, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return sz;
}

void sc_utils_die(const char *fmt, ...)
{
	va_list argp;

	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	fputc('\n', stderr);
	assert(0);
	exit(1);
}
