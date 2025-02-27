#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H
#include <wchar.h>
/*
typedef struct Node
{
  wchar_t *s;  /// cadena
  struct Node *next;
}Node;

typedef struct
{
  int countNode;
  Node* he; /// cabeza de la lista
  Node* ta; // cola de la lista
}Buffer;
#define he  buffer.he
#define ta  buffer.ta
#define count buffer.countNode
*/
void appendBuffer(const wchar_t *s, int len);
void freeSplBuffer(wchar_t ** splBuffer, int c);
#endif
