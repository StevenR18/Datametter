#include <Windows.h>
#include "..\..\Vt\includes\window64_platform.h"

/*
  Nombre funcion: calculateDx();
  vPWidth     : El ancho de la ventana
  bufferWidth : Anho del buffer que puede ser mas grande que la ventana

         widtht=20             wBuffer= 47
  |--------------------|---------------------------|
  
            step = longitu de paso
            |--| -> 
  |-------|------------|
   visbelFr   rest
     42.55%  57.45%
       8      12
  Me indica cuanto del buffer se muestar en la venta
  visibleFraction = width/wBuffer  = 20/47  0.4255 o  42.55% de 47
  dx me dice la longitud de cada paaso que debo de moverme hasta mostrar el buffe completo
  dx= rest/(wBuffer-width) nos da una distancia de paso de 12/27 = 0.4444 con un numero de
  pasos  27
  En conclusion calcula la longitud de paso
  
 */

 
float calculateStep(int widthInChar, int bufferWidthInChar)
{
  
  float visibleFraction = (float)widthInChar / (float)bufferWidthInChar;
  float restante = (1.00f - visibleFraction)*widthInChar; // restante en caracteres
  float step = restante/(bufferWidthInChar - widthInChar);
  return step;
}

/*
  Nombre de la funcion calculateScrollbarWidth()
  descripcion: Calcula el ancho de la barra de escroll horizontal en caracteres

         widtht=20             wBuffer= 47
  |--------------------|---------------------------|
      
  |-------|
   visbelFr   rest
     42.55%  57.45%
       8      12

 visbleFraccion = (width/wBuffer)*width;

 visibleFraccion= Ancho de la barra de srcoll
  
 */


float calculateScrollbarWidth(int widthInChar, int bufferWidthInChar)
{
  
    if (bufferWidthInChar <= 0 || widthInChar <= 0)
    {
        return 0; // Error, no puede ser 0 o negativo
    }

    if (bufferWidthInChar <= widthInChar)
    {
        return widthInChar; // Todo está visible
    }

    float visibleFraction = (float)widthInChar / (float)bufferWidthInChar;
    float scrollbarWidth = (float)(widthInChar * visibleFraction);

    // Opcional: asegura que la barra tenga al menos 1 de ancho
    if (scrollbarWidth < 1)
    {
        scrollbarWidth = 1;
    }

    return scrollbarWidth;
}

/*
  Nombre funcion : drawHScrollBar()
  Parametros    :
  *b            : el HDC donde si dibujan los pixeles
   Y            : cordenada 'y' en codenadas de caracteres
   X            : cordenada 'x' en cordenads de caracteres
   W            : ancho de la barra de  escroll previamente calculada
   color        : color que tendra la barra de escroll
   widthViewpor : ancho de la ventana en caracteres
   widthBuffer  : ancho del buffer en caracteres

   Descripcion dibuja la barra de escroll horizontal

 */

void drawHScrollBar(HDC *hdcTerm,
		    int Y,
		    int X,
		    int W,
		    unsigned int colorref,
		    int widthInChar,
		    int widthBufferInChar)
{
  
  //rec represeta las cordenadas de la barra de escroll
  RECT rect;
  // aqui se calcula la longitud de paso en base al tamaño del buffer
  float step = calculateStep(widthInChar, widthBufferInChar);
  //cordenada x de la esquina superior isquierda y si multiplica por 8 para converir a pi
  rect.left = ((step * X)*8);
  //cordenada x de la esquina inferior derecha
  rect.right=(rect.left+(W*8));
  // cordenada y de la esquina superior isquierda
  rect.top = ((Y) * 16);
  // cordenada y de la esquina inferior drecha
  rect.bottom = (rect.top + 16);
  // HDC donde se ba a dibujar la barra de escroll horizontal
  HDC hdc = *hdcTerm;
  HBRUSH brush = CreateSolidBrush(colorref);
  HBRUSH oldBrush = SelectObject(hdc, brush);
  HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); // Color blanco para el borde
  HPEN oldPen = SelectObject(hdc, pen);
  int ok= RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 8, 8);
  if(!ok)
    {
     OutputDebugStringA("Error al renderizar la terminal virtual\n");
      return; 
    }
  SelectObject(hdc, oldBrush);
  SelectObject(hdc, oldPen);
  DeleteObject(brush);
  DeleteObject(pen);
}
