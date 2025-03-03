/*
  ESTE ARCHIVO CONTENDRA LOS COMANDOS DE UNA TERMINAL VIRUTAL VT100
  ECHO "S" muestra una cadena en la terminal
  CLS      limpia la pantalla
  ..
  etc
 */
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "..\..\Vt\includes\row.h"
#include "..\..\Vt\includes\main_datametter.h"
extern size_t lenDirectory;
extern ArrRow arrRow;

void splitMessage(const wchar_t *message,int len ,wchar_t ouput[100][1000], int *count)
{
  wchar_t * ptr= (wchar_t *)message;
  int x=0;
  while((*ptr != L'\0') && (len > vEmu.widthBuffer))
    {
      if(len >vEmu.widthBuffer)
	{
	  //ouput[x]=(wchar_t *)malloc(sizeof(wchar_t)* (vEmu.width+1));
	  wcsncpy(ouput[x],ptr,(sizeof(wchar_t)*vEmu.widthBuffer));
	  ptr+=vEmu.widthBuffer;
	  ouput[x][vEmu.widthBuffer]=L'\0';
	}
      else
	{
	  // ouput[x]=(wchar_t *)malloc(sizeof(wchar_t)*(len+1));
	  wcsncpy(ouput[x],ptr,(len*sizeof(wchar_t)));
	  ptr+=len;
	  ouput[x][len]=L'\0';
	}    
      len= wcslen(ptr);
      x++;
    }
  *count=x;
}

void trimLeadingSpaces(wchar_t *str)
{
  int count=0;
  while(str[count] == L' ')
    {
      count++;
    }

  if(count == 0)return;
  //[ | | h | o | l | a ] count= 2    
  int dx= (wcslen(&str[count]));
  memmove(str,&str[count],sizeof(wchar_t)*dx);
  str[wcslen(&str[count])]=L'\0';
}

int isStringEmpty(wchar_t * str)
{
  return(str[0]== L'\0');
  
}
void toUperCase(wchar_t * str)
{
  if (str == NULL) return;
  for (int i = 0; str[i] != L'\0'; i++)
    {
        str[i] = towupper(str[i]);
    }  
}


int charToWchar(char *str, wchar_t *output)
{
    size_t converted_chars = mbstowcs(output, str, 256);
    
    if (converted_chars == (size_t)-1)
      {
        return -1;
      }
    return 0;
}

int wcharTOchar(wchar_t *str,char *ouput)
{
  size_t converted_chars = wcstombs(ouput, str, sizeof(ouput));
    
    if (converted_chars == (size_t)-1)
      {  
        return -1;
      }
    return 0;
}

/// funcion para limpiar la pantalla --> tenemos que borrar el buffer de linias
void Cls()
{
  freeRow(&vEmu.row,
	  &vEmu.capRow,
	  &vEmu.ofsetY,
	  &vEmu.ofsetCapRow);
}

void tokenize(wchar_t * arg, wchar_t ouput[10][80],int *count)
{
  //trimLeadingSpaces(arg);
  wchar_t a=0;
  wchar_t *ptr=&a;
  wchar_t *argc = wcsdup(arg);
  wchar_t * token = wcstok(argc, L" ", &ptr);
  int y=0;
  while(token != NULL)
    {
      wcsncpy(ouput[y],token,(sizeof(wchar_t)*(wcslen(token)+1)));
      token= wcstok(NULL, L" ", &ptr);
      y++;
    }
  *count=y;
  free(argc);
}
void chagenDirectory(const wchar_t * newDirectory,wchar_t *cDirectory)
{
  
  wchar_t ouput[100]={L'\0'};
  SetCurrentDirectoryW(newDirectory);
  size_t lennew = GetCurrentDirectoryW(100,ouput);
  size_t lenold = wcslen(cDirectory); 
  
  appendRow(arrRow,ouput,lennew,&vEmu.capRow);

  wmemset(cDirectory, L'\0', lenold);
  
  
  wcsncpy(cDirectory,ouput,(sizeof(wchar_t)*(lennew+1)));
  
  cDirectory[lennew+1]=L'\0';
  cDirectory[lennew]=L'>';
  lenDirectory=lennew+1;
  
}
void Cd(wchar_t * arg)
{
  wchar_t tokens[10][80];
  int countTokens=0;
  tokenize(arg,tokens,&countTokens);
  switch(countTokens)
    {
    case 1:
      {
	chagenDirectory(tokens[0],vEmu.currentDirectory);
      }break;
    case 2:
      {
	chagenDirectory(tokens[1],vEmu.currentDirectory);
      }break;
    default:
      {
	chagenDirectory(tokens[2],vEmu.currentDirectory);
      }break;
    }
}

void readLine(char ouput[1000][200], char input[4096], int inputLen, int *oCount)
{
  int countRow = 0;
  size_t inicioLinea = 0;
  // buffer[inputLen] = '\0';
  // TODO: hacer una funcion que permita leer cada linia del buffer
  for (int x = 0; x < inputLen; x++)
    {
      if (input[x] == '\n')
	{
	  if(input[x-1] == L'\r')input[x-1]=L'\0';
	  input[x] = '\0'; // Terminamos la línea
	  size_t rowLen = x - inicioLinea; // Largo de la línea
	  memcpy(ouput[countRow],&input[inicioLinea],rowLen+1);
	  countRow++;
	  inicioLinea = x + 1; // Saltamos al inicio de la próxima línea
	}
    }
  *oCount=countRow;
}


void procesingCommands(wchar_t * arg)
{
  ///echo hola
  //cmd.exe /c   -> esto tienes que agregarle a la cadena
  //cmd.exe /c echo hola cadena final

  //TODO: crear otro pipe para que el padre pueda escribir datos al hijo
  wchar_t argv[100];
  wchar_t auxDirectory[100]={L'\0'};
  wcsncpy(auxDirectory,vEmu.currentDirectory,(sizeof(wchar_t)*(wcslen(vEmu.currentDirectory)+1)));
  auxDirectory[wcslen(auxDirectory)-1]=L'\0';
  trimLeadingSpaces(arg); // quitamos los espacios al inicio del array
  if(wcscmp(arg, L"cls") == 0)
    {
      Cls();
      return;
    }
  else
    {
      wchar_t token[10][80];
      int count=0;
      tokenize(arg,token,&count);
      
      if(wcscmp(token[0], L"cd") == 0)
	{
	  if(count > 1)
	    {
	      if(wcscmp(token[1], L"/?")== 0)
		{
		  goto jump;
		}
	    }
	  Cd(arg);
	  return;
	}
    
    }
  jump:
  swprintf(argv,100,L"cmd.exe /c %s",arg);
  HANDLE readPipe,writePipe;
  SECURITY_ATTRIBUTES saAttr;
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;  // Permitir que el hijo herede los handles
  saAttr.lpSecurityDescriptor = NULL;

  ///// creacion e la tuberia
  if(!CreatePipe(&readPipe,&writePipe,&saAttr,0))
    {
      // printf("\nNo se pudo crear la tuberia");
      return;
    }
  // Asegurarse de que writePipe será heredado
  if (!SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0))
    {
      // printf("Fallo al configurar readPipe (%d).\n", GetLastError());
        return;
    }
  
  /// creacion del proceso 
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;
  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );
  si.dwFlags |= STARTF_USESTDHANDLES;
  si.hStdOutput = writePipe; // se redirigje la salida estandar al writepipe
  si.hStdError = writePipe;  // Opcional: si quieres también redirigir errores
  si.hStdInput = NULL;       // Si no necesitas entrada

  // Start the child process.
  // el poroceso hijo se ejecuta en el directorio de donde se encuentra
  if(!CreateProcessW( NULL,   // No module name (use command line)
		     (wchar_t *) argv,        // Command line
		     NULL,           // Process handle not inheritable
		     NULL,           // Thread handle not inheritable
		     TRUE,          // Set handle inheritance to FALSE
		     CREATE_NO_WINDOW,              // No creation flags
		     NULL,           // Use parent's environment block
		     auxDirectory,           // Use parent's starting directory
		     &si,            // Pointer to STARTUPINFO structure
		     &pi )           // Pointer to PROCESS_INFORMATION structure
    )
    {
      // printf( "CreateProcess failed (%d).\n", GetLastError() );
      return;
    }
  CloseHandle(writePipe); // el proceso padre no escribe
  WaitForSingleObject(pi.hProcess, INFINITE);
  char buffer[4096];
  char rows[1000][200];
  DWORD bytesRead;
  BOOL result;
  while (result = ReadFile(readPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0)
    {
      int countRow = 0;
      readLine(rows, buffer, bytesRead,&countRow);
      wchar_t wrow[1000][200];
      wchar_t ouput[100][1000];
      int count=0;
      for(int y=0; y < (countRow); y++)
	{
	  MultiByteToWideChar(CP_OEMCP, 0, rows[y], -1, wrow[y], 200);
	  ///SI LAS LINIAS SUPERAN EL ANCHO DE LA VENTATA splitline
	  splitMessage(wrow[y],wcslen(wrow[y]) ,ouput, &count);
	  if(count > 0)
	    {
	      for(int x=0; x < count; x++)
		{
		  appendRow(arrRow,ouput[x],wcslen(ouput[x]),&vEmu.capRow);
		}
	      countRow+=count;
	      y+=(count-1);
	      continue;
	    }
	  appendRow(arrRow,wrow[y],wcslen(wrow[y]),&vEmu.capRow);
	}
    }
  if (!result)
    {
      DWORD error = GetLastError();
      if (error != ERROR_BROKEN_PIPE)
	{
	  // printf("Fallo al leer la tubería (%d).\n", error);
	  return;
        }
    }
  CloseHandle(readPipe);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  
  // Wait until child process exits WaitForSingleObject( pi.hProcess, INFINITE );
  // printf("\nfinalizo el proceso");
  // Close process and thread handles.
  return;
}
