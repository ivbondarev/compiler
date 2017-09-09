#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

struct compiler_state comp;

static void die(const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	fputc('\n', stderr);
	exit(1);
}

static size_t get_file_size(FILE *fp)
{
	size_t ret;

	fseek(fp, 0, SEEK_END);
	ret = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return ret;
}

static void sc_utils_init(struct compiler_state *cs, int argc, char **argv)
{
	FILE *fp;
	char *buf;
	size_t size;

	fp = fopen(argv[1], "r");
	if (!fp)
		die("ERROR: cant't open %s file", argv[1]);

	size = get_file_size(fp);
	buf = calloc(size + 1, sizeof(char));
	if (!buf)
		die("ERROR: malloc failed");

	fread(buf, size, 1, fp);
	fclose(fp);

	cs->size = size + 1;
	cs->buf = buf;
}
static void sc_utils_destroy(struct compiler_state *cs)
{
	/* todo */
}

int main(int argc, char *argv[])
{
	struct compiler_state *cs = &comp;
	if (argc != 2)
		die("ERROR: bad number of args");

	sc_utils_init(cs, argc, argv);

	const char delim[2] = " ";
	char *tok;
	tok = strtok(cs->buf, delim);

	while (tok) {
		printf("%s\n", tok);
		tok = strtok(NULL, delim);
	}

	sc_utils_destroy(cs);
	return 0;
}
