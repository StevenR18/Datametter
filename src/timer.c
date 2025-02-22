#include <windows.h>
#include <stdio.h>
#include "..\..\Vt\includes\timer.h"
void inic_clock(Clock * clock)
{
  LARGE_INTEGER *hz = &clock->frequency;
  LARGE_INTEGER *startT = &clock->startTime;
  if( hz->QuadPart == 0)
    {
    QueryPerformanceFrequency(hz); // numero de cuentas por segundo
    }
  QueryPerformanceCounter(startT); // por que cuenta va

}

double stop_clock(Clock *clock)
{
  QueryPerformanceCounter(&clock->endTime); // cuenta final
  LONGLONG elapsedTicks = clock->endTime.QuadPart - clock->startTime.QuadPart;
  double deltaT= (double)elapsedTicks /(double)clock->frequency.QuadPart;
  return deltaT;
}




