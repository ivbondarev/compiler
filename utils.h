#ifndef _UTILS_H_
#define _UTILS_H_

#include "main.h"

/* Prints error message to stderr and exit */
void sc_utils_die(const char *fmt, ...);
/* Returns size of a given file */
size_t sc_utils_file_size(FILE *fp);

#endif /* !_UTILS_H_ */
