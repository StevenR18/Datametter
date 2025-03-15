#include <Windows.h>
#include <stdio.h>
#include <wchar.h>
#include "..\..\Vt\includes\row.h"
#include "..\..\Vt\includes\process_keypress.h"
int GRunnigSerial=1;
// abrir un puerto COM usando la funcion CreateFile();
HANDLE openPortCOM(char * port,
		   int baudRate,
		   int dataBit,
		   int stopBit,
		   int parity,
		   int flowControl)
{
  
  HANDLE handle = CreateFileA(port,
			    GENERIC_READ|GENERIC_WRITE,
			    0
			    ,
			    NULL,
			    OPEN_EXISTING,
			    0,
			    NULL);
  
  if(handle == INVALID_HANDLE_VALUE)
    {
      printf("No se pudo abrir el puerto %s\n",port);
      return NULL;
    }
   printf("Puerto %s abierto correctamente\n",port);
   DCB serialSetting = {0}; // config
   serialSetting.BaudRate  = baudRate/*CBR_115200*/;   /// velocidad en baudios
   serialSetting.ByteSize  = dataBit;            /// un byte
   serialSetting.Parity    = parity/*NOPARITY*/;     /// no se usa bit de paridad
   serialSetting.StopBits  = stopBit/*ONESTOPBIT*/;   /// 1 bit de parada
   if (!SetCommState(handle, &serialSetting))
     {
       printf("Error configurando el puerto\n");
       CloseHandle(handle);
       return NULL;
    }
   printf("Puerto configurado correctamente %s\n",port);
   return handle;
}

void readPortCOM(HANDLE * pHSerial, int dataType)
{
  int sizeBuffer = dataType * 256;
  void * buffer = (void *) malloc(sizeBuffer);
  
  DWORD bytesRead;

  while(GRunnigSerial)
    {
      if(ReadFile(*pHSerial, buffer, sizeBuffer, &bytesRead, NULL))
	{
	  if(bytesRead > 0)
	    {
	      // datos recibidos
	      wchar_t tempBuffer[256]={L'\0'};
	      for(int x=0; x<(bytesRead/dataType); x++)
		{
		  if(sizeof(wchar_t) == dataType)
		    {
		      swprintf(tempBuffer,256,L"%lc",((wchar_t*)buffer)[x]);   
		    }
		  
		  /// aqui puedes agregar mas tipos de datos
		}
	      processChar(tempBuffer,(bytesRead/dataType));
	    }	  
	}
      else
	{
	  printf("Esperando datos\n");
	}
    }
  free(buffer);
}


/*
int main()
{
  char *puerto = "\\\\.\\COM5";
  HANDLE hSerial= CreateFileA(puerto,GENERIC_READ|GENERIC_WRITE,0, NULL,OPEN_EXISTING,0,NULL);
  if(hSerial == INVALID_HANDLE_VALUE)
    {
      printf("No se pudo abrir el puerto %s\n",puerto);
      return -1;
    }
   printf("Puerto %s abierto correctamente\n",puerto);

  DCB serialSetting = {0}; // config
  serialSetting.BaudRate  = CBR_115200;   /// velocidad en baudios
  serialSetting.ByteSize  = 8;            /// un byte
  serialSetting.Parity    = NOPARITY;     /// no se usa bit de paridad
  serialSetting.StopBits  = ONESTOPBIT;   /// 1 bit de parada
  if (!SetCommState(hSerial, &serialSetting))
    {
      printf("Error configurando el puerto\n");
      CloseHandle(hSerial);
      return -1;
    }

   printf("Puerto configurado correctamente %s\n",puerto);

   configurar los tiempos de espera
   char buffer[256]; // Buffer de lectura
   DWORD bytesRead;

while(ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL))
  {
    buffer[bytesRead] = '\0'; // Asegura que sea una cadena válida
    printf("Datos recibidos: %s\n", buffer);
  }
 
 
  CloseHandle(hSerial);
  printf("Puerto Cerrado correctamente  %s\n",puerto);
}
*/
