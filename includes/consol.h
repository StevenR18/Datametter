
#ifndef CONSOL_H
#define CONSOL_H
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\keyboard.h"
#include "..\..\Vt\includes\static_list.h"
#include "..\..\Vt\includes\row.h"
#include "..\..\Vt\includes\util_sprites.h"
#include <wchar.h>
#define MAX 100
typedef enum
  {
    MOD_FREE_CURSOR,
    MOD_TERM_CURSOR
  }CURSOR_TYPE;
typedef enum
  {
    LAYOUT_DEFECT,
    LAYOUT_2,
    LAYOUT_3
  }LAYOUT;
typedef struct
{
  HDC   hdcTerm;       // contexto propio de dibujo para la terminal
  HBITMAP hBitmapTerm; // bitmap propio de la terminal
  HBITMAP holdBitmap;  // bitmpa anterior
  POINT vCursor;
  POINT positionTerm; // para posicionar la terminal
  CURSOR_TYPE  cursorType; // modo del  curor
  LAYOUT layoutType;
  List buffercomand;
  List renderBuffer;
  List *Input;
  int  widthInChar;
  int  heightInChar;
  int  widthBufferInChar;
  int  heightBufferInChar;
  float widthBitmapTerm; 
  float heightBitmapTerm;
  int pixelPerChar;
  Row *row; // array del buffe principal
  ArrRow arrRow; ///array de filas
  int capacityRow;
  int ofsetYTop; // se utiliza para hacer escroll vertical
  int ofsetX; // se utilzia pra hacer escroll horizontal
  int ofsetYbottom; // ofset para la capacidad de la filas
  wchar_t currentDirectory[MAX];
  int lenDirectory;
  Entity Mario;
}View;
List * getListOfString();
void simulate_term(Win32ScreenBuffer *Buffer, KeyboardEvent * k);
void inic_term(Win32WindowDimension Dim);
void end_simulate();
void updateDimBitmap(int w, int h, HDC *backBuffer);
void modSerialTermiminal();
void exitModSerialTerminal();

#endif
