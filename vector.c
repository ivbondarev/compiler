#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "utils.h"

#define DEFAULT_ELEMS_NUM 2

void sc_vector_init(struct vector *v)
{
	memset(v, 0, sizeof(*v));
	v->capacity = DEFAULT_ELEMS_NUM;
	v->elems = malloc(v->capacity * sizeof(void *));

	printf("Allocated %p size %lu\n", v->elems, v->capacity * sizeof(void *));

	if (v->elems == NULL)
		sc_utils_die("malloc failed");
}

void *sc_vector_add(struct vector *v, void *elem)
{
	size_t new_len;
	void *mem;

	if (v->size != v->capacity)
		return (v->elems[v->size++ - 1] = elem);
		
	/* Not enought space */
	new_len = 2 * v->capacity * sizeof(void *);
	mem  = realloc(v->elems, new_len);

	if (NULL == mem)
		sc_utils_die("realloc failed");
	v->elems = mem;

	return (v->elems[v->size++ - 1] = elem);
}

void sc_vector_free(const struct vector *v)
{
	free(v->elems);
}
