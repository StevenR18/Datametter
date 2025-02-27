#include "..\..\Vt\includes\static_list.h"
#include <stdlib.h>
#include <string.h>
///List list={NULL,0};

/*

  [nodo *]-->[NODO][NODO][NODO][NODO][NODO][NODO][NODO]
  addes


 */


Nodo * createList(unsigned int size)
{
  Nodo *head = malloc(sizeof(Nodo)*size);
  for(int x= 0; x < size; x++)
  memset((char *)&head[x], 0, sizeof(Nodo));
  return head;
  
}

void clsList(List *lis)
{
  for(int x= 0; x < lis->count; x++)
    memset((char *)&lis->head[x], 0, sizeof(Nodo));
  lis->count=0;
    
}
void freeList(List *l)
{
  if(l->head!=NULL)
    {
      free(l->head);
      l->head=NULL;
    }
}

int listIsEmpty(List *l)
{
  return (l->count == 0);
  
}

Nodo createNodo(wchar_t *s ,unsigned int len)
{
  Nodo newNodo;
  wcsncpy(newNodo.data,s,len);
  newNodo.data[len]=L'\0';
  newNodo.len= len;
  return newNodo;
}

int appendNodoToList(List * list, wchar_t * s,unsigned int len)
{
  if(list->count > SIZE_LIST )return -1;
  Nodo newNodo = createNodo(s,len);
  list->head[list->count]=newNodo;
  list->count++;
  return 0;
}


/*

int main()
{
  list.head = createList(&list.head,SIZE_LIST);

  appendNodoToList(&list,L"Hola mundo", wcslen(L"Hola mundo"));
  appendNodoToList(&list,L"Hola mundo", wcslen(L"Hola mundo"));
  appendNodoToList(&list,L"Hola mundo", wcslen(L"Hola mundo"));
  appendNodoToList(&list,L"Hola mundo", wcslen(L"Hola mundo"));
  freeList(&list);
  
}
*/
