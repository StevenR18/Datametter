#include <Windows.h>
#include "..\..\Vt\includes\window64_platform.h"

 
float calculateDx(int vPWidth, int bufferWidth)
{
  float visibleFraction = (float)vPWidth / (float)bufferWidth;
  float restante = (1.00f - visibleFraction)*vPWidth; // restante en caracteres
  float dx = restante/(bufferWidth - vPWidth);
  return dx;
}


float calculateScrollbarWidth(int viewportWidth, int bufferWidth)
{
    if (bufferWidth <= 0 || viewportWidth <= 0)
    {
        return 0; // Error, no puede ser 0 o negativo
    }

    if (bufferWidth <= viewportWidth)
    {
        return viewportWidth; // Todo está visible
    }

    float visibleFraction = (float)viewportWidth / (float)bufferWidth;
    float scrollbarWidth = (float)(viewportWidth * visibleFraction);

    // Opcional: asegura que la barra tenga al menos 1 de ancho
    if (scrollbarWidth < 1)
    {
        scrollbarWidth = 1;
    }

    return scrollbarWidth;
}

void drawHScrollBar(Win32ScreenBuffer *b,
		    int Y,
		    int X,
		    int W,
		    unsigned int colorref,
		    int widthViewpor,
		    int widthBuffer)
{
  
  RECT rect; // dimensiones del rectangulo
  float dx = calculateDx(widthViewpor, widthBuffer);
  
  rect.left = ((dx * X)*8);
  rect.right=(rect.left+(W*8));
  rect.top = ((Y) * 16);
  rect.bottom = (rect.top + 16);
  HDC hdc = b->BackBuffer;
  HBRUSH brush = CreateSolidBrush(colorref);
  HBRUSH oldBrush = SelectObject(hdc, brush);
  HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); // Color blanco para el borde
  HPEN oldPen = SelectObject(hdc, pen);
  RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 8, 8);
  SelectObject(hdc, oldBrush);
  SelectObject(hdc, oldPen);
  DeleteObject(brush);
  DeleteObject(pen);
}
