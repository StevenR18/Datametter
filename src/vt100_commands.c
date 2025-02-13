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
#include "..\..\Vt\includes\main_datametter.h"
#define WIDTH_TERMV  60
#define HEIGHT_TERMV 25

void splitMessage(const wchar_t *message,int len ,wchar_t ouput[][WIDTH_TERMV+1], int *count)
{
  wchar_t * ptr= (wchar_t *)message;
  int x=0;
  while(*ptr != L'\0')
    {
      if(len >WIDTH_TERMV)
	{
	  //ouput[x]=(wchar_t *)malloc(sizeof(wchar_t)* (WIDTH_TERMV+1));
	  wcsncpy(ouput[x],ptr,(sizeof(wchar_t)*WIDTH_TERMV));
	  ptr+=WIDTH_TERMV;
	  ouput[x][WIDTH_TERMV]=L'\0';
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

void echoHelp()
{
  wchar_t help[]=L"\x1b[38;2;0;255;0mECHO [ON | OFF]";
  wchar_t help2[]=L"ECHO [message]\x1b[0m";
  appendRow(help,wcslen(help));
  appendRow(help2,wcslen(help2));
}

void vt100Echo(wchar_t * message, int len)
{
  static int switchEcho=1;
  wchar_t * dup= wcsdup((const wchar_t *)message);
  trimLeadingSpaces(dup);
  toUperCase(dup);
  if((wcslen(dup) > 0)&&
     (wcsncmp(dup, L"ON",wcslen(dup))== 0))
    {
      switchEcho=1;
      free(dup);
      return;
    }
  else
    {
      if((wcslen(dup) > 0)&&
	 (wcsncmp(dup, L"OFF",wcslen(dup))== 0))
	{
	   switchEcho=0;
	   free(dup);
	   return;
	}
      if((wcslen(dup) > 0)&&
	 (wcsncmp(dup, L"/?",wcslen(dup))== 0))
	{
	  echoHelp();
	  return;
	  
	}
    }
  
  if(isStringEmpty((wchar_t *)dup))
    {
      wchar_t enable[50]=L"ECHO esta activado";
      wchar_t disable[50]=L"ECHO esta desactivado";
      wchar_t *defec = enable;
      if(switchEcho != 1) defec=disable;
      appendRow(defec,wcslen(defec));
      free(dup);
      return;
    }
  
  wchar_t buffer[50][WIDTH_TERMV+1]={{L'\0'}};
  // wchar_t *ptr =(wchar_t *) message;
  int count =0;
  splitMessage(message,len,buffer,&count);
  for(int x=0; x < count; x++)
    {
       appendRow(buffer[x],wcslen(buffer[x]));
       //free(buffer[x]);
    }
  free(dup);
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

/*incompleta*/
int listdir(wchar_t *path) 
{
  // mejora esta funcio  createProcess
  trimLeadingSpaces(path);
  char ouput[200]={'\0'};
  size_t count = wcharTOchar(path,ouput);
  if(count != 0)return -1;
  const char *pDir= "dir ";
  size_t dirLen   = strlen(pDir);
  size_t ouputLen = strlen(ouput);
  memmove(&ouput[dirLen], ouput, ouputLen+1);
  memcpy(ouput, pDir,dirLen);
  
  /* ouput[s|r|c|\|v|t|0] len7  pero el buffer tiene 200 espacios
   */              
  FILE *Pipe=NULL;
  if ((Pipe = _popen(ouput,"rt")) == NULL)
    {
      OutputDebugStringA("No se pudo abir el pipe");
      return -1;
    }
  char buffer[50][200]={{'\0'}};
  wchar_t obuffer[50][200]={{'\0'}};
  int x=0;
  while (fgets(buffer[x], 200, Pipe))
    {
      size_t len = strlen(buffer[x]);
      if (len > 0 && buffer[x][len - 1] == '\n')
	{
	  buffer[x][len - 1] = '\0';
	}
      x++;
    }
  
  for(int i=0; i < x ; i++)
    {
      charToWchar(buffer[i], obuffer[i]);
      appendRow(obuffer[i],wcslen(obuffer[i]));
    }

  int endOfFileVal = feof(Pipe);
  int closeReturnVal = _pclose(Pipe);
  return 1;
}


void vt100Dir(wchar_t * path)
{
  // TODO: implementar el comando para listar los directorios
  listdir(path);
  
}


void comandNotValid(wchar_t *message, int len)
{
  wchar_t buffer[50][WIDTH_TERMV+1]={{L'\0'}};
  wchar_t messerror[100]=L" No ser reconoce como un comando interno o externo,";
  int lene= wcslen(messerror);
  wchar_t mess[500]={L'\0'};
  mess[0]=L'"';
  mess[len+1]=L'"';
  wmemcpy((mess+1),message,len);
  wmemcpy(&mess[len+2],messerror,lene);
  mess[len+lene+2]=L'\0';
  int currentlen = len+lene; 
  int count =0;
  splitMessage(mess,currentlen,buffer,&count);
  for(int x=0; x < count; x++) appendRow(buffer[x],wcslen(buffer[x]));
      
    
}

void procesingCommands(wchar_t * arg)
{
  ///echo hola
  //cmd.exe /c   -> esto tienes que agregarle a la cadena
  //cmd.exe /c echo hola cadena final

  wchar_t argv[100];
  trimLeadingSpaces(arg); // quitamos los espacios al inicio del array
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
  if(!CreateProcessW( NULL,   // No module name (use command line)
		     (wchar_t *) argv,        // Command line
		     NULL,           // Process handle not inheritable
		     NULL,           // Thread handle not inheritable
		     TRUE,          // Set handle inheritance to FALSE
		     CREATE_NO_WINDOW,              // No creation flags
		     NULL,           // Use parent's environment block
		     NULL,           // Use parent's starting directory
		     &si,            // Pointer to STARTUPINFO structure
		     &pi )           // Pointer to PROCESS_INFORMATION structure
    )
    {
      // printf( "CreateProcess failed (%d).\n", GetLastError() );
      return;
    }
  CloseHandle(writePipe); // el proceso padre no escribe
  char buffer[4096];
  char rows[50][200];
  DWORD bytesRead;
  BOOL result;
  while (result = ReadFile(readPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0)
    {
      int countRow = 0;
      size_t inicioLinea = 0;
      buffer[bytesRead] = '\0';
      // TODO: hacer una funcion que permita leer cada linia del buffer
      for (int x = 0; x < bytesRead; x++)
	{
	  if (buffer[x] == '\n')
	    {
	      buffer[x] = '\0'; // Terminamos la línea
	      size_t rowLen = x - inicioLinea; // Largo de la línea
	      // Copiar la línea a rows[firstRow]
	      memcpy(rows[countRow], &buffer[inicioLinea], rowLen + 1); // Incluimos el '\0'
	      countRow++;
	      inicioLinea = x + 1; // Saltamos al inicio de la próxima línea
	    }
	}
      
	
        wchar_t wrow[50][200];
	for(int y=0; y < countRow; y++)
	  {
	     MultiByteToWideChar(CP_OEMCP, 0, rows[y], -1, wrow[y], 200);
	     appendRow(wrow[y],wcslen(wrow[y]));
	  }
	// printf("%s", buffer);
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
