#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"
#include "state.h"

int main(int argc, char *argv[])
{
	struct compiler_state comp;
	struct compiler_state *cs = &comp;

	sc_state_init(cs, argc, argv);

	const char delim[2] = " ";
	char *tok;
	tok = strtok(cs->buf, delim);

	while (tok) {
		printf("%s\n", tok);
		tok = strtok(NULL, delim);
	}

	sc_state_destroy(cs);
	return 0;
}
