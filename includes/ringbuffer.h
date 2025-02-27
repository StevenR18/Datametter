#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#define MAX_SIZE 250
#define MAX_CELL 100
#include <wchar.h>
typedef struct
{
  wchar_t data[MAX_SIZE][MAX_CELL];
  int head;
  int tail;
  int capacity;
}RingBuffer;

void inicRingBuffer(RingBuffer *ring);
unsigned char ringBufferIsEmpty(RingBuffer *ring);
unsigned char ringBufferIsFull(RingBuffer *ring);
void enqueue(RingBuffer *ring,wchar_t * element, int leng);
wchar_t * dequeue(RingBuffer *ring);

#endif
