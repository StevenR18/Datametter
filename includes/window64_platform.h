#ifndef WINDOW_64
#define WINDOW_64
#include <Windows.h>
#include <stdbool.h>
#define FRAMES 300.0
#define TIME_PER_FRAME_TARGET (1000.0 / FRAMES)

typedef struct
{
  int Width;         // ancho del bitmap  en pixeles
  int Height;        // alto del bitmpa   en pixeles 
  HDC BackBuffer;   
  HBITMAP HoldBitmap;
  HBITMAP Hbitmap;
}Win32ScreenBuffer;
typedef struct
{
  HDC primaryHdc; 
  HWND Window;
  int Running;
  double avarageFps;
  double avarageTimePerFrame;
  double sumdt;
  int frames;
}Win32Platform;

typedef struct
{
  int Width;
  int Height;
}Win32WindowDimension;

int Window32GetRunning();
double Window32GetAvarageFps();
double Window32GetAvarageTimePerFrame();
void Window32SetRunning(int value);
HWND Window32GetWindow();

#endif


