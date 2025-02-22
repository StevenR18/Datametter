#ifndef MAIN_DATAMETTER_H
#define MAIN_DATAMETTER_H
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\keyboard.h"
#include "..\..\Vt\includes\liked_list.h"
#include "..\..\Vt\includes\row.h"



typedef struct
{
  POINT vCursor;
  Nodo *buffercomand;
  int  width;
  int  height;
  int  widthBuffer;
  int  heightBuffer;
  int pixelPerChar;
  Row *row; // array del buffe principal
  int capRow;
  int ofsetY; // se utiliza para hacer escroll vertical
  int ofsetX; // se utilzia pra hacer escroll horizontal
  int ofsetCapRow; // ofset para la capacidad del cursor 
}View;                  

extern View vEmu;

void main_app(Win32ScreenBuffer *Buffer, KeyboardEvent * k);
void inic_app(Win32WindowDimension Dim);
void end_app();
void updateDim(int w, int h);

#endif
