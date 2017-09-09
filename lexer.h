#ifndef __LEXER_H
#define _LEXER_H

#include "main.h"

struct token {
	u32 type;
	u32 val;
	char *str;
};

#endif /* !_LEXER_H */
