#ifndef STATIC_LIST_H
#define STATIC_LIST_H
#define MAX_DATA 1000
#define SIZE_LIST 1000
#include <wchar.h>
typedef struct  Nodo
{
  wchar_t data[MAX_DATA];
  int len;
}Nodo;

typedef struct List
{
  Nodo *head;
  int count;
}List;

Nodo * createList(unsigned int size);
void freeList(List *l);
int listIsEmpty(List *l);
Nodo createNodo(wchar_t *s ,unsigned int len);
int appendNodoToList(List *list, wchar_t * s,unsigned int len);
void clsList(List *lis);
#endif
