#ifndef _LL1_H_
#define _LL1_H_

#include "main.h"

/*
 * Given simple LL1 grammar
 * S-> A | ( A + A )
 * A-> a
 *
 * #	Expression	    Predict
 * 1	S → A	              'id'
 * 2	S → '(' A '+' A ')'   '('
 * 3	A → id	              'id'
*/
void sc_ll1_parse(struct compiler_state *cs);

#endif /* !_LL1_H_ */
