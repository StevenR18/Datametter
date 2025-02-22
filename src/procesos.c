#include <stdio.h>
#include <windows.h>
int main(int argc, char * argv[])
{
  HANDLE readPipe,writePipe;
  SECURITY_ATTRIBUTES saAttr;
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;  // Permitir que el hijo herede los handles
  saAttr.lpSecurityDescriptor = NULL;

  ///// creacion e la tuberia

  if(!CreatePipe(&readPipe,&writePipe,&saAttr,0))
    {
      printf("\nNo se pudo crear la tuberia");
      return;
    }
  // Asegurarse de que writePipe será heredado
  if (!SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0))
    {
        printf("Fallo al configurar readPipe (%d).\n", GetLastError());
        return 1;
    }
  
  /// creacion del proceso 
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );
  si.dwFlags |= STARTF_USESTDHANDLES;
  si.hStdOutput = writePipe; // se redirigje la salida estandar al writepipe
  si.hStdError = writePipe;  // Opcional: si quieres también redirigir errores
  si.hStdInput = NULL;       // Si no necesitas entrada
  if( argc != 2 )
    {
      printf("Usage: %s [cmdline]\n", argv[0]);
      return;
    }
  // Start the child process.
  if(!CreateProcess( NULL,   // No module name (use command line)
		     argv[1],        // Command line
		     NULL,           // Process handle not inheritable
		     NULL,           // Thread handle not inheritable
		     TRUE,          // Set handle inheritance to FALSE
		     0,              // No creation flags
		     NULL,           // Use parent's environment block
		     NULL,           // Use parent's starting directory
		     &si,            // Pointer to STARTUPINFO structure
		     &pi )           // Pointer to PROCESS_INFORMATION structure
    )
    {
      printf( "CreateProcess failed (%d).\n", GetLastError() );
      return;
    }
  CloseHandle(writePipe); // el proceso padre no escribe
  char buffer[4096];
  DWORD bytesRead;
  BOOL result;
  while (result = ReadFile(readPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

   if (!result)
    {
        DWORD error = GetLastError();
        if (error != ERROR_BROKEN_PIPE)
        {
            printf("Fallo al leer la tubería (%d).\n", error);
        }
    }

    CloseHandle(readPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  
  // Wait until child process exits WaitForSingleObject( pi.hProcess, INFINITE );
  printf("\nfinalizo el proceso");
  // Close process and thread handles.
  return 0;
}



///echo hola
//cmd.exe /c   -> esto tienes que agregarle a la cadena
//cmd.exe /c echo hola cadena final

void createArgv(const char * arg)
{
  char * pArg = (char *)arg;
  /// cmd echo "hola mundo" -p -m
  
  
}
