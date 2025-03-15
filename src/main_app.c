/*
  Nombre del archivo: main_app
  este modulo cera la aplicacion principal englobara toda la aplicacion, con los siguiente
  modulo
  emulador_de_terminal.c
  otro modulo para procesar la imagenes
 */
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\keyboard.h"
#include "..\..\Vt\includes\main_app.h"
#include "..\..\Vt\includes\consol.h"

void mainApp(Win32ScreenBuffer *Buffer ,KeyboardEvent *k)
{
  /// estas funcion se ejecutan en el bucle princiapal
  // emula una terminal
  simulate_term(Buffer,k);
  // hace los dibujos vectoriales
  // vgDrawScreen();
}

/// esta funcion solo se utilizara para obtner las dimensiones de la ventana
//  si es que se hace un rezise en la ventana
void updateApp(int widthInPixel, int heightInPixel, HDC * hdc)
{
  // esta funcion se ejecuta cada vez que se redimensiona la pantalla  
}

void inicApp(Win32WindowDimension Dim, HWND *hWindow)
{
  // esta funcion solo se ejecuta una sola vez 

  inic_term(Dim);
  // inicVgrfics();

}
