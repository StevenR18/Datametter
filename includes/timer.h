#ifndef TIMER_H
#define TIMER_H
#include <windows.h>
typedef struct
{
  LARGE_INTEGER frequency;
  LARGE_INTEGER startTime;
  LARGE_INTEGER endTime;
}Clock;

void inic_clock(Clock * clock);
double stop_clock(Clock *clock);


#endif
