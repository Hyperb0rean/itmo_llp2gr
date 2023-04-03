
#include <stdio.h>
#include <stddef.h>

#pragma once
#ifndef VECTOR_H
#define VECTOR_H

 struct vector {
    char **data;
    size_t  capacity;
    size_t size;
};

void vector_create_empty(struct vector *vector);
size_t vector_size(struct vector *vector);
size_t vector_capacity(struct vector *vector);
void vector_resize(struct vector *vector, size_t capacity);
void vector_add(struct vector *vector, char *p);
void vector_set(struct vector *vector, size_t i, char *p);
char vector_get(struct vector *vector, size_t i);
char *vector_at(struct vector *vector, size_t i);
void vector_delete(struct vector *vector, size_t i);
void vector_free(struct vector *vector);
void print_vector(struct vector* v, FILE *f);
#endif