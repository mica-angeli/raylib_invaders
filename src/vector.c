#include <stdlib.h>
#include "vector.h"

void vector_ctor(vector* v)
{
  v->capacity = 1;
  v->size = 0;
  v->items = malloc(sizeof(void *) * v->capacity);
}

void vector_dtor(vector* v)
{
  free(v->items);
}

int vector_size(vector* v)
{
  return v->size;
}

bool vector_empty(vector* v)
{
  return v->size == 0;
}

int vector_capacity(vector* v)
{
  return v->capacity;
}

void vector_resize(vector* v, int capacity)
{
  void **items = realloc(v->items, sizeof(void *) * capacity);
  if(items)
  {
    v->items = items;
    v->capacity = capacity;
  }
}

void vector_push_back(vector* v, void* item)
{
  if(v->capacity == v->size)
  {
    vector_resize(v, v->capacity * 2);
  }
  v->items[v->size++] = item;
}

void* vector_at(vector* v, int index)
{
  if(index >= 0 && index < v->size)
  {
    return v->items[index];
  }
  return NULL;
}

void* vector_back(vector* v)
{
  return vector_at(v, v->size - 1);
}

void* vector_front(vector* v)
{
  return vector_at(v, 0);
}

void vector_erase(vector* v, int index)
{
  if(index < 0 || index >= v->size)
  {
    return;
  }

  v->items[index] = NULL;

  // Shift items left
  for(int i = index; i < v->size - 1; i++)
  {
    v->items[index] = v->items[index + 1];
    v->items[index + 1] = NULL;
  }

  v->size--;

  // Adjust memory
  if(v->size > 0 && v->size == v->capacity / 4)
  {
    vector_resize(v, v->capacity / 2);
  }

}

void vector_clear(vector* v)
{
  vector_dtor(v);
  vector_ctor(v);
}
