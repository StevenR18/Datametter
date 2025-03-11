
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\static_list.h"
#include "..\..\Vt\includes\font.h"
#include "..\..\Vt\includes\ringbuffer.h"
extern RingBuffer ring;
void drawGrid(HDC *hdcTerm,int width,int height)
{
  
  int widthCell      = 8;
  int heightCell     = 16;
  int pxWidth        = width * widthCell;
  int pxHeight       = height * heightCell;
  int maxVerticals   = (pxWidth/widthCell);
  int maxHorizontals = (pxHeight/heightCell);

  HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 120, 120)); // Línea roja de grosor 2
  HPEN pen2=CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Línea roja de grosor 2
  SelectObject(*hdcTerm, pen);

  for(int x=0; x <= maxVerticals; x++)
    {
      if((x == 74)|| (x == maxVerticals))SelectObject(*hdcTerm, pen2);
      else SelectObject(*hdcTerm, pen);
	
      MoveToEx(*hdcTerm,(x*widthCell), 0, NULL); // Punto inicial (x = 100, y = 50)
      LineTo(*hdcTerm,(x*widthCell), pxHeight);        // Punto final (x = 100, y = 200)
    }
  for(int y=0; y <= maxHorizontals; y++ )
    {
      SelectObject(*hdcTerm, pen);
      MoveToEx(*hdcTerm,0, (y*heightCell), NULL); // Punto inicial (x = 100, y = 50)
      LineTo(*hdcTerm,pxWidth,(y*heightCell));        // Punto final (x = 100, y = 200)
    }
  DeleteObject(pen);
  DeleteObject(pen2); 
}


typedef struct {
    wchar_t text[50];
    LARGE_INTEGER startTime;
} InputEntry;

#define MAX_ENTRIES 100

static InputEntry inputs[MAX_ENTRIES];
static int inputCount = 0;

void debugInputBuffer(HDC *hdcTerm,
                      List *listImp,
                      int width,
                      int height,
                      double delay)
{
    LARGE_INTEGER Hz, currentTime;
    QueryPerformanceFrequency(&Hz);
    QueryPerformanceCounter(&currentTime);

    // Agregar nuevas entradas desde listImp
    for (int r = 0; r < listImp->count; r++) {
        if (inputCount < MAX_ENTRIES) {
            wchar_t *text = (wchar_t *)listImp->head[r].data;
            wcsncpy(inputs[inputCount].text, text, 49);
            inputs[inputCount].text[49] = L'\0'; // Seguridad
            QueryPerformanceCounter(&inputs[inputCount].startTime);
            inputCount++;
        }
    }

    // Dibujar y limpiar entradas expiradas
    int y = 0;
    double delayMs = delay * 1000.0;

    for (int i = 0; i < inputCount; i++) {
        double elapsedMs = (((double)currentTime.QuadPart - (double)inputs[i].startTime.QuadPart) / (double)Hz.QuadPart) * 1000.0;

        if (elapsedMs < delayMs) {
            draw_font(hdcTerm, inputs[i].text, (width / 2), y++);
        } else {
            // Eliminar entrada vencida moviendo las siguientes hacia arriba
            memmove(&inputs[i], &inputs[i + 1], (inputCount - i - 1) * sizeof(InputEntry));
            inputCount--;
            i--; // Revalúa la nueva entrada en esta posición
        }
    }
}






