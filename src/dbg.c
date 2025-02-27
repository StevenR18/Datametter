
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\static_list.h"
#include "..\..\Vt\includes\font.h"
#include "..\..\Vt\includes\ringbuffer.h"
extern RingBuffer ring;
void drawGrid(Win32ScreenBuffer *b,int width,int height)
{
  
  int widthCell      = 8;
  int heightCell     = 16;
  int pxWidth        = width * widthCell;
  int pxHeight       = height * heightCell;
  int maxVerticals   = (pxWidth/widthCell);
  int maxHorizontals = (pxHeight/heightCell);

  HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 120, 120)); // Línea roja de grosor 2
  HPEN pen2=CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Línea roja de grosor 2
  SelectObject(b->BackBuffer, pen);

  for(int x=0; x <= maxVerticals; x++)
    {
      if((x == 74)|| (x == maxVerticals))SelectObject(b->BackBuffer, pen2);
      else SelectObject(b->BackBuffer, pen);
	
      MoveToEx(b->BackBuffer,(x*widthCell), 0, NULL); // Punto inicial (x = 100, y = 50)
      LineTo(b->BackBuffer,(x*widthCell), pxHeight);        // Punto final (x = 100, y = 200)
    }
  for(int y=0; y <= maxHorizontals; y++ )
    {
      SelectObject(b->BackBuffer, pen);
      MoveToEx(b->BackBuffer,0, (y*heightCell), NULL); // Punto inicial (x = 100, y = 50)
      LineTo(b->BackBuffer,pxWidth,(y*heightCell));        // Punto final (x = 100, y = 200)
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

void debugInputBuffer(Win32ScreenBuffer *b,
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
            draw_font(&b->BackBuffer, inputs[i].text, (width / 2), y++);
        } else {
            // Eliminar entrada vencida moviendo las siguientes hacia arriba
            memmove(&inputs[i], &inputs[i + 1], (inputCount - i - 1) * sizeof(InputEntry));
            inputCount--;
            i--; // Revalúa la nueva entrada en esta posición
        }
    }
}


/*
void debugInputBuffer(Win32ScreenBuffer *b,
                      List *listImp,
                      int width,
                      int height,
                      double delay)
{
    static unsigned char Drawing = 1;      // Persistencia entre fotogramas
    static int y = 0;                      // Línea actual de texto
    static double elapsedTime = 0;         // Tiempo acumulado
    static wchar_t temp[100][50];          // Almacena los textos extraídos
    static int x = 0;                      // Índice del texto actual en temp[]
    static LARGE_INTEGER t0 = {0};         // Marca de tiempo inicial

    LARGE_INTEGER Hz, tf;
    QueryPerformanceFrequency(&Hz);

    // Si hay un nuevo input, lo encolamos
    for (int r=0; r < listImp->count;r++)
    {
        wchar_t *text = (wchar_t *)listImp->head[r].data;
        int len = wcslen(text);
        enqueue(&ring, text, len); // len + 1 para incluir '\0'
    }

    // Si `temp` está vacío, llenarlo con los datos del ringBuffer
    if (!ringBufferIsEmpty(&ring) && temp[0][0] == L'\0')
    {
        int countr = 0;
        while (!ringBufferIsEmpty(&ring))
        {
            wchar_t *text = dequeue(&ring);
            int len = wcslen(text);
            wcsncpy(temp[countr], text, len);
            temp[countr][len] = L'\0'; // Asegurar terminación
            countr++;
        }
        Drawing = 1;
        x = 0; // Reiniciar el índice de dibujo
        y = 0; // Reiniciar la posición vertical
        QueryPerformanceCounter(&t0); // Reiniciar el tiempo
    }

    // Mostrar texto si `Drawing` está activado y hay datos en `temp`
    if (Drawing && temp[0][0] != L'\0')
    {
        for (int i = 0; temp[i][0] != L'\0'; i++)
        {
            draw_font(&b->BackBuffer, temp[i], (width / 2), i); // Aumentar 'y' para cada línea
        }
    }

    // Calcular el tiempo transcurrido
    QueryPerformanceCounter(&tf);
    double dtms = (((double)tf.QuadPart - (double)t0.QuadPart) / (double)Hz.QuadPart) * 1000.0;

    // Si pasa el delay, avanzar al siguiente texto
    if (dtms > delay*1000)
    {
        x++;      // Pasar al siguiente texto en `temp`
        t0 = tf;  // Reiniciar el tiempo

        // Si ya mostramos todos los textos, vaciar `temp`
       if (temp[x][0] == L'\0')
	 {
	   memset(temp, 0, sizeof(temp)); // Inicializa todo el buffer a '\0'
	   Drawing = 0;                   // Desactivar dibujo temporalmente
	   ///totalLines = 0;
	   x = 0;
	 }

    }
}

*/



