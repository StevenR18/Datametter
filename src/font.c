#include <windows.h>
#include "..\..\Vt\includes\newwvt.h"

HFONT create_font(HDC *hdc, int fontHeight)
{
    // Obtén el contexto del dispositivo de la ventana (en este caso, un HDC)
    int dpi = GetDeviceCaps(*hdc, LOGPIXELSY);  // Obtener la densidad de píxeles de la pantalla

    // Calcula la altura de la fuente en función de los píxeles por pulgada (DPI)
    int pixelHeight = MulDiv(fontHeight, dpi, 72);  // 72 es la cantidad de puntos por pulgada en un font size de 1 (1 punto = 1/72 pulgadas)

    // Crear la fuente con tamaño fijo
    HFONT hFont = CreateFontW(
        -pixelHeight,        // Altura en píxeles
        0,                   // Ancho (0 para usar el valor predeterminado)
        0,                   // Ángulo de rotación (0 para texto horizontal)
        0,                   // Ángulo de rotación (0 para texto horizontal)
        FW_NORMAL,           // Grosor de la fuente
        FALSE,               // Sin cursiva
        TRUE,               // Sin subrayado
        FALSE,               // Sin tachado
        DEFAULT_CHARSET,     // Juego de caracteres (usamos DEFAULT_CHARSET para mayor compatibilidad con Unicode)
        OUT_DEFAULT_PRECIS,  // Precisión de salida
        CLIP_DEFAULT_PRECIS, // Precisión de recorte
        DEFAULT_QUALITY,     // Calidad de la fuente
        DEFAULT_PITCH | FF_SWISS, // Tipo de fuente (por ejemplo, sin ser monoespaciada)
        L"Consolas"     // Nombre de la fuente (puedes cambiarla a otra que soporte Unicode)
    );

    return hFont;
}


int draw_font(HDC *hdc, const wchar_t * text, int x, int y)
{
 
    // Establece la fuente en el contexto del dispositivo
    HFONT hFont = create_font(hdc, 12);  // 20 es el tamaño en puntos que desees
    HFONT hOldFont = (HFONT)SelectObject(*hdc, hFont);
    COLORREF bkColor=0;
    COLORREF fColor=RGB(188, 255, 0);
    int mode= TRANSPARENT;
    if(ISMODIFIED(TextAtri.fExtras))
      {
	bkColor=TextAtri.fBackground;
	fColor=TextAtri.fForeground;
	mode=0;
      }
    
    
    //   COLORREF holdColor= SetTextColor(*hdc,RGB(0xff,0x00,0x00));
    TEXTMETRIC tm;
    GetTextMetrics(*hdc, &tm); 
    int textLen =(unsigned int) wcslen(text);
    SetBkColor(*hdc,bkColor);
    if(mode != 0) SetBkMode(*hdc, mode);
    SetTextColor(*hdc, fColor); 
    if(TextOutW(*hdc,x*tm.tmAveCharWidth,y*tm.tmHeight,text,textLen))
      {
	 SelectObject(*hdc, hOldFont);
         DeleteObject(hFont);
	
      }
    else
      {
	///fallo
      }

    return textLen;    
}
