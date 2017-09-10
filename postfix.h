#ifndef _SORT_STATION_H
#define _SORT_STATION_H

#include <stdio.h>

struct compiler_state;

/* Produce stack-based bytecode */
void sc_sort_station_make_postfix(struct compiler_state *cs);

#endif /* !_SORT_STATION_H */
