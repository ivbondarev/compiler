#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "utils.h"

void sc_vector_init(struct vector *v)
{
	memset(v, 0, sizeof(*v));
	v->capacity = 10;
	v->elems = malloc(v->capacity * sizeof(void *));

	if (v->elems == NULL)
		sc_utils_die("malloc failed");
}

void *sc_vector_add(struct vector *v, void *elem)
{
	size_t new_len;

	if (v->size != v->capacity)
		return (v->elems[v->size++ - 1] = elem);
		
	/* Not enought space */
	new_len = 2 * v->capacity * sizeof(void *);

	if (realloc(v->elems, new_len) == NULL)
		sc_utils_die("Realloc failed");

	return (v->elems[v->size++ - 1] = elem);
}

void sc_vector_free(const struct vector *v)
{
	free(v->elems);
}
