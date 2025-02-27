#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H
#include "..\..\Vt\includes\window64_platform.h"
float calculateDx(int vPWidth, int bufferWidth);
float calculateScrollbarWidth(int viewportWidth, int bufferWidth);
void drawHScrollBar(Win32ScreenBuffer *b,
		    int Y,
		    int X,
		    int W,
		    unsigned int colorref,
		    int widthViewpor,
		    int widthBuffer);
#endif
