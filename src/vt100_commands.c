/*
  ESTE ARCHIVO CONTENDRA LOS COMANDOS DE UNA TERMINAL VIRUTAL VT100
  ECHO "S" muestra una cadena en la terminal
  CLS      limpia la pantalla
  ..
  etc
 */

#include <wchar.h>
#include "..\..\Vt\includes\main_datametter.h"
#define WIDTH_TERMV  60
#define HEIGHT_TERMV 20

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
void vt100Echo(const wchar_t * message, int len)
{
  wchar_t buffer[50][WIDTH_TERMV+1]={{L'\0'}};
  // wchar_t *ptr =(wchar_t *) message;
  int count =0;
  splitMessage(message,len,buffer,&count);
  for(int x=0; x < count; x++)
    {
       appendRow(buffer[x],wcslen(buffer[x]));
       //free(buffer[x]);
    }
  
}
void vt100Ls(wchar_t * str, int len)
{
  // TODO: implementar el comando para listar los directorios
  
  
  
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
  for(int x=0; x < count; x++)
    {
      appendRow(buffer[x],wcslen(buffer[x]));
      // free(buffer[x]);
      // buffer[x]=NULL;
    }
}
void trimLeadingSpaces(wchar_t *str)
{
  int count=0;
  while(str[count] == L' ')
    {
      count++;
    }
  

  //[ | | h | o | l | a ] count= 2
        
  int dx= (wcslen(&str[count]));
  memmove(str,&str[count],sizeof(wchar_t)*dx);
  str[wcslen(&str[count])]=L'\0';
}

void procesingCommands(wchar_t *command)
{
  //"echo hola mundo"
  wchar_t * ptr=NULL;
  trimLeadingSpaces(command);
  wchar_t *dupco =  wcsdup((const wchar_t *)command);
  wchar_t *com= wcstok(dupco, L" ",&ptr);
  
  wchar_t *dupc = malloc(sizeof(wchar_t)*(wcslen(command)+1));
  dupc[wcslen(command)]=L'\0';
  wmemcpy(dupc,command,wcslen(command));

  if(!wcsncmp(com, L"echo",wcslen(com) ))
    {
      wchar_t * message= &command[wcslen(com)];
      vt100Echo(message,wcslen(message));
    }
  else
    {
      comandNotValid(command,wcslen(command));
      
    }
  if((dupc!=NULL))
    {
      free(dupco);
      free(dupc);
      //free(dupc);
    }
  
}

