#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void prependChar(char ** pStr,char c, int len)
{
 *pStr = (char *) realloc(*pStr,sizeof(char)*(len+2));
  memmove(*pStr+1,*pStr,len+1);
  *(pStr)[0]=c;
}

void addSpace(int maxToken,char **arrToken)
{
  for(int x=2; x < maxToken; x++)
    {
      prependChar(&arrToken[x],' ',strlen(arrToken[x]));
    }
}
typedef struct
{
  char s[100];
}Row;

Row row[3]={"echo hola mundo","como estas"," Tercerafila"};

void extractString(Row *r, int cap, int y, char *ouput)
{
  char buffer[100]="";
  strncat(buffer,&r[y].s[0],strlen(r[y].s));
  for(int x=(y+1); x< cap; x++)strncat(buffer,&r[x].s[0],strlen(r[x].s));
  int len = strlen(buffer);
  memcpy(ouput, buffer,(len+1));
}


int main()
{
  #define MAX_PARAM 50
  
  char *p[MAX_PARAM];
  char message[]="echo hola mundo";
  char  *  token= strtok(message," ");
  char  *  copia=NULL;
  int x=0;
  char o[100];
  // extrae toda la cadena de la fila en la que esta el comando
  extractString(row, 3,0,o);
  printf("O = %s",o);
  while(token!=NULL)
    {
      p[x]=strdup(token);
      token=strtok(NULL," ");
      x++;
    }
  addSpace(x,p); // agrega un espacio al inicio de cada token a partir del 2
  
  char buffer[100]= "";
  for(int z=1; z < x; z++)strncat(buffer,p[z],100);
  
  for(int j=0; j<x; j++)printf("%s len=%d\n",p[j],strlen(p[j]));
  printf("Buffer\n%s\n len %d ",buffer,strlen(buffer));
  for(int i=0; i<x; i++)free(p[i]);
 
  getchar();
  return 0;
}
