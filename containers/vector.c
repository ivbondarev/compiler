#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "../utils.h"

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

void *sc_vector_get(const struct vector *v, size_t i)
{
	return v->elems[i];
}

size_t sc_vector_size(const struct vector *v)
{
	return v->size;
}

void sc_vector_free(const struct vector *v)
{
	free(v->elems);
}

void vector_move_elems_left(struct vector *v, size_t del)
{
	size_t num = v->size - del - 1;

	for (size_t i = 0; i < num; i++) {
		v->elems[del] = v->elems[del + 1];
		del++;
	}
}

void sc_vector_remove_elem(struct vector *v, size_t i)
{
	free(v->elems[i]);
	if (i != v->size - 1)
		vector_move_elems_left(v, i);
	v->size--;
}
