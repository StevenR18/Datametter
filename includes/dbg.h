#ifndef DBG_H
#define DBG_H
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\static_list.h"
void drawGrid(Win32ScreenBuffer *b,int width,int height);

void debugInputBuffer(Win32ScreenBuffer *b,
		      List *lisImp,
		      int width,
		      int height,
		      double delay);

#endif
