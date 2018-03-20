#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "containers/vector.h"
#include "containers/tree.h"
#include "front/lex.h"
#include "main.h"
#include "utils.h"
#include "vm.h"
#include "ir.h"
#include "parser.h"

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
	char *file = NULL;
	char *parse_tree_dump = NULL;
	char *tokens_dump = NULL;
	int c;
	struct option longopts[] = {
		{ "parsetree", required_argument, NULL, 'p' },
		{ "file", required_argument, NULL, 'f' },
		{ "tokens", required_argument, NULL, 't' },
		{ 0, 0, 0, 0 }
	};

	while ((c = getopt_long(argc, argv, ":f:p:t:", longopts, NULL)) != -1) {
		switch (c) {
		case 'f':
			file = optarg;
			break;
		case 'p':
			parse_tree_dump = optarg;
			break;
		case 't':
			tokens_dump = optarg;
			break;
		case 0:
			break;
		default:
			sc_utils_die("Wrong argument");
		}
	}

	memset(cs, 0, sizeof(*cs));

	cs->dump.tokens = fopen(tokens_dump, "w+");
	cs->dump.parse_tree = fopen(parse_tree_dump, "w+");

	state_read_file(cs, file);

	sc_vector_init(&cs->tokens);

	cs->vm = malloc(sizeof(*cs->vm));
	cs->parse_tree = malloc(sizeof(*cs->parse_tree));
	sc_tree_init(cs->parse_tree, sc_lexer_tok(STATEMENT));
	cs->irs = calloc(1, sizeof(*cs->irs));
	sc_ir_init(cs->irs);
	sc_vm_init(cs->vm);
}

void sc_state_destroy(const struct compiler_state *cs)
{
	/* todo */
}
