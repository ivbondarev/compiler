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

	sc_lexer_tokenize(cs);
	sc_lexer_token_chain(cs);

	sc_state_destroy(cs);
	return 0;
}
