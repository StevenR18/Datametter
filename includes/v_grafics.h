#ifndef V_GRAFICS_H
#define V_GRAFICS_H
#include <Windows.h>
typedef struct
{
  HDC  vgHdc;
  HBITMAP vghBitmap;
  int  width;         
  int height;
  POINT position;
}VGRAFIC;

// mi estructura propia para un bitmap
typedef struct
{
  HDC memDc;
  BITMAPINFO bmInfo;
  BITMAP bmp;
  int bufferPixelSize;
  unsigned char * pixels;
}Bmp;
#endif
