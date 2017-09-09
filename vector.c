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

	if (v->elems == NULL)
		sc_utils_die("malloc failed");
}

void *sc_vector_add(struct vector *v, void *elem)
{
	if (v->size != v->capacity) {
		v->elems[v->size++] = elem;
		return v->elems[v->size - 1];
	}
		
	/* Not enought space */
	v->capacity *= 2;
	if (NULL == (v->elems = realloc(v->elems,
					v->capacity * sizeof(void *))))
		sc_utils_die("realloc failed");

	v->elems[v->size++] = elem;
	return v->elems[v->size - 1];
}

void sc_vector_free(const struct vector *v)
{
	free(v->elems);
}
