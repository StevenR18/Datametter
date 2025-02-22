#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H
#include <wchar.h>
typedef struct Node
{
  wchar_t *s;  /// cadena
  struct Node *next;
}Node;

typedef struct
{
  int countNode;
  Node* head; /// cabeza de la lista
  Node* tail; // cola de la lista
}Buffer;

#define head  buffer.head
#define tail  buffer.tail
#define count buffer.countNode
#define splitBuffer wchar_t **

void appendBuffer(const wchar_t *s, int len);
void freeSplBuffer(splitBuffer splBuffer, int c);
#endif
