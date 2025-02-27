#ifndef LIKED_LIST_H
#define LIKED_LIST_H
#include <wchar.h>
typedef struct Nodo {
  wchar_t *data;
  struct Nodo *next;
} Nodo;

typedef struct List
{
  Nodo *nodo;    // punteero al primer nodo de la lista
  Nodo * tail;   // apunta al ultimo nodo de la lista
  int count;     // cuenta el numero de de nodos que tiene la lista
}List;

int insert_nodo_end(Nodo **head, wchar_t *data, size_t len);
int free_lista(Nodo **head);

#endif
