#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H
#include "..\..\Vt\includes\window64_platform.h"
float calculateStep(int widthInChar, int bufferWidthInChar);
float calculateScrollbarWidth(int widthInChar, int bufferWidthInChar);
void drawHScrollBar(HDC *hdcTerm,
		    int Y,
		    int X,
		    int W,
		    unsigned int colorref,
		    int widthInChar,
		    int widthBufferInChar);
#endif
