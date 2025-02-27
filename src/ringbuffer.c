
#include "..\..\Vt\includes\ringbuffer.h"

void inicRingBuffer(RingBuffer *ring)
{
  ring->head = 0;
  ring->tail = 0;
  ring->capacity = 0;
}
unsigned char ringBufferIsEmpty(RingBuffer *ring)
{
  return (ring->capacity == 0);
}

unsigned char ringBufferIsFull(RingBuffer *ring)
{
 return ((ring->tail + 1) % MAX_SIZE) == ring->head;
  
}

void enqueue(RingBuffer *ring,wchar_t * element, int leng)
{
  wchar_t * dest =(wchar_t *) &ring->data[(ring->tail)% MAX_SIZE];
  wcsncpy(dest, element,leng+1);
  ring->tail=(ring->tail+1)%MAX_SIZE;
  ring->capacity++;
  
}

wchar_t * dequeue(RingBuffer *ring)
{
  if(ringBufferIsEmpty(ring))return NULL;
  wchar_t * result = ring->data[(ring->head)% MAX_SIZE];
  ring->head= (ring->head+1)% MAX_SIZE;
  ring->capacity--;
  return result;
}


