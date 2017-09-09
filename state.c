#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "utils.h"
#include "vector.h"

static void state_read_file(struct compiler_state *cs, const char *path)
{
	FILE *fp = fopen(path, "r");
	char *buf;
	size_t size;

	if (NULL == fp)
		sc_utils_die("Cant't open %s file", path);

	size = sc_utils_file_size(fp);
	buf = calloc(size + 1, sizeof(char));
	if (NULL == buf)
		sc_utils_die("malloc failed");

	fread(buf, size, 1, fp);
	fclose(fp);

	cs->size = size + 1;
	cs->buf = buf;
}

void sc_state_init(struct compiler_state *cs, int argc, char **argv)
{
	if (2 != argc)
		sc_utils_die("Bad number of args");

	memset(cs, 0, sizeof(*cs));
	state_read_file(cs, argv[1]);

	sc_vector_init(&cs->tokens);
}

void sc_state_destroy(const struct compiler_state *cs)
{
	/* todo */
}
