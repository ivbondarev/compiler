#ifndef _PARSER_H_
#define _PARSER_H_

#include "main.h"

/* Generate parse tree */
void sc_parser_begin(struct compiler_state *cs);
void sc_parser_dump_parse_tree(FILE *fp, struct node *root);

#endif /* !_PARSER_H_ */
