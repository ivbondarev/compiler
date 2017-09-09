/*
 * Simple vector. Contains only pointers to allocated objects
 * i.e. size of each element is sizeof(void *)
 */

#ifndef _VECTOR_H
#define _VECTOR_H

struct vector {
	size_t size;
	size_t capacity;
	void **elems;
};

/* Initializing vector */
void sc_vector_init(struct vector *v);
/* Adds element to vector */
void *sc_vector_add(struct vector *v, void *elem);
/* Destroys memory, belongs to vector */
void sc_vector_free(const struct vector *v);

#endif /* !_VECTOR_H */
