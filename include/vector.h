#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

typedef struct vector {
  void **items;
  int capacity;
  int size;
} vector;

void vector_ctor(vector* v);
void vector_dtor(vector* v);
int vector_size(vector* v);
bool vector_empty(vector* v);
int vector_capacity(vector* v);
void vector_resize(vector* v, int capacity);
void vector_push_back(vector* v, void* item);
void* vector_at(vector* v, int index);
void* vector_back(vector* v);
void* vector_front(vector* v);
void vector_erase(vector* v, int index);
void vector_clear(vector* v);

#endif // VECTOR_H
