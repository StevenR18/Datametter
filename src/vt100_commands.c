/*
  ESTE ARCHIVO CONTENDRA LOS COMANDOS DE UNA TERMINAL VIRUTAL VT100
  ECHO "S" muestra una cadena en la terminal
  CLS      limpia la pantalla
  ..
  etc
 */

#include <wchar.h>
#include "..\..\Vt\includes\main_datametter.h"


void vt100Echo()
{
  // funcion para borrar el buffer
  
}

void procesingCommands(wchar_t *command,wchar_t **params)
{
  
      if(!wcsncmp(command, L"echo", 4) )
	{
	  appendRow(params[1],wcslen(params[0]));
	 
	}    	
    
}

