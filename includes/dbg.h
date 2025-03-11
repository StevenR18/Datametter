#ifndef DBG_H
#define DBG_H
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\static_list.h"
void drawGrid(HDC *hdcTerm,int width,int height);

void debugInputBuffer(HDC *hdcTerm,
		      List *lisImp,
		      int width,
		      int height,
		      double delay);

#endif
