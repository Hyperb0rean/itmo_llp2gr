#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

void vector_create_empty(struct vector *v)
{
    v->capacity = 1;
    v->size = 0;
    v->data = malloc(sizeof(char *) * v->capacity);
}

size_t vector_size(struct vector *v)
{
    return v->size;
}

size_t vector_capacity(struct vector *v)
{
    return v->capacity;
}

void vector_resize(struct vector *v, size_t capacity)
{
    void **new_data = realloc(v->data, sizeof(char *) * capacity);
    if (new_data) {
        v->data = new_data;
        v->capacity = capacity;
    }
}

void vector_add(struct vector *v, char *item)
{
    if (v->capacity == v->size) vector_resize(v, v->capacity * 2);
    char* temp = malloc(strlen(item) +1);
    v->data[v->size] = malloc(strlen(item) +1);
    strcpy(v->data[v->size++], temp);
}

void vector_set(struct vector *v, size_t i, char *item)
{
    if (i < v->size) v->data[i] = item;
}

char *vector_at(struct vector *v, size_t i)
{
    if (i < v->size) return v->data[i];
    return NULL;
}


char vector_get(struct vector *v, size_t i)
{
    if (i < v->size) return *(v->data[i]);
    return 0;
}

void vector_reduce(struct vector *v, size_t index)
{
    if (index >= v->capacity)
        return;

    for(size_t i =0; i< v->capacity; i++){
        free(vector_at(v,index));
    }

    v->capacity = index;
    if (index < v->size) v->size = index;

}

void vector_free(struct vector *v)
{
    for( size_t i =0; i< v->size;i++){
        free(vector_at(v,i));
    }
    free(v->data);
    free(v);
}


void print_vector(struct vector* v, FILE* f){
    for (int i = 0; i < vector_size(v); i++){
        fprintf(f, "%s ", vector_at(v,i));
    }
}