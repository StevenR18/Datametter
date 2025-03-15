#include <stdio.h>
#include <stdlib.h>     // Para malloc, free, realloc
#include <string.h>     // Para strncpy, strlen
#include <wctype.h>
#include <wchar.h>
#include <Windows.h>
//#include "..\..\Vt\includes\memory_traker.h"
#include "..\..\Vt\includes\font.h"
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\static_list.h"
#include "..\..\Vt\includes\keyboard.h"
#include "..\..\Vt\includes\consol.h"
#include "..\..\Vt\includes\newwvt.h"
#define debug 1
#define debugInfo __func__ ,__FILE__

////////////////////////////////////////////////////////////////////////////////////////////       VARIABLES GLOBALES
wchar_t cursorSymbols[4] = {
        L'█', // Bloque
	L' ',
        L'_', // Barra baja
        L'|' // Barra vertica
       
    };
//////////////////////////////////////////////////////////////////////////////////////////
Termv Emu ={
  {0,0},
  {0,0},
  cursorSymbols,
  0x00,
  {NULL,0}
};


unsigned char enableBlinkCursor=0; // se activa cada segundo

TextAtributes TextAtri={0};
  
List * getInput()
{
  return (&Emu.inputBuffer); 
}

wchar_t * getGlifo()
{
  return (Emu.glifo);
}

void inicInputBuffer(int size)
{
  Emu.inputBuffer.head = createList((unsigned int )size); 
}

#include "process_event.c" // se procesa los eventos del teclado
#include "process_escape.c" // se procesa las secuencias de escape

/*
int main()
{
  const wchar_t *testStr = L"\x1b[31mHello\x1b[0m World\x1b[1m!\x1b[0m This is a \x1b[4mtest\x1b[0m string.";

  while(1)
    {
  appendBuffer(testStr,wcslen(testStr));
  freeBuffer();// libera el buffer principal
  printList(); // imprimee la lista de seguimiento  de memoria para saber si esta vacia
  freeAll(); // libera la lista de seguimeinto de memoria
    }
  return 0;
}
*/
