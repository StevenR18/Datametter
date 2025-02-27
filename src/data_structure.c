
#include "..\..\Vt\includes\static_list.h"
#include "..\..\Vt\includes\main_datametter.h"


int lenEscape(const wchar_t *s)
{
   int len=0;
   int x=0;
   while(s[x] != L'\0')
    {
      if(s[x] == 0x1b)
	{
	  len++;
	  x++; // contamos el caracter de incio de secuencia
	  if(s[x] == L'[')
	    {
	      len++; // contamos el corchete
	      x++;   // abanzamos en la secuencia
	    }
	  else
	    {
	      /// una secuencia sin parametros
	      x++; // abanzamos al siguiente caracter
	      continue;
	    }
	  while(!iswalpha(s[x]))
	    {
	      len++; // contamos el caracter
	      x++; // abanzamos al sigiente caracter
	    }
	  len++; // contamos el caracter de final de secuencia
	  x++; // abanazamos al siguiente caracter
	}
      else
	{
	   break;
	}
     
    }

   return len;
}
#define splitBuffer wchar_t **
#define MAX_ROW 50
#define MAX_CELL 1000

void spliteEcapeAndWord(const wchar_t *s,
			       int *countOut,
			       wchar_t splBuffer[MAX_ROW][MAX_CELL]
			       )
{
  //splitBuffer splBuffer = NULL;
  int splIndex = 0;
  int x = 0;
  int len = 0;
  while (s[x] != L'\0')
    {
      if (s[x] == 0x1b)
	{ // Inicio de una secuencia de escape
	  len = lenEscape(&s[x]);
	  #if 1
	  /*
	  splBuffer = (splitBuffer)myrealloc(splBuffer, sizeof(wchar_t *) * (splIndex + 1),__func__, __FILE__);
	  splBuffer[splIndex] = (wchar_t *)mymalloc(sizeof(wchar_t) * (len + 1),__func__, __FILE__);
	  */
	  #else
	  splBuffer = (splitBuffer)realloc(splBuffer, sizeof(wchar_t *) * (splIndex + 1));
	  splBuffer[splIndex] = (wchar_t *)malloc(sizeof(wchar_t) * (len + 1));
	  #endif


	  wcsncpy(splBuffer[splIndex], &s[x], len);
	  splBuffer[splIndex][len] = L'\0';
          x += len; // Avanzar el índice en la cadena original
          splIndex++;
	}
      else
	{
	  
	  // Es una palabra o frase
	  len = 0;
	  int start = x; // Marca el inicio de la palabra
	  while (iswprint(s[x]) && s[x] != L'\0' && s[x] != 0x1b)
	    {
                len++;
                x++;
            }
	  if (len > 0)
	    {
	      #if 1
	      /*
	      splBuffer = (splitBuffer)myrealloc(splBuffer, sizeof(wchar_t *) * (splIndex + 1),__func__, __FILE__);
	      splBuffer[splIndex] = (wchar_t *)mymalloc(sizeof(wchar_t) * (len + 1),__func__, __FILE__);
	      */
	      #else
	      splBuffer = (splitBuffer)realloc(splBuffer, sizeof(wchar_t *) * (splIndex + 1));
	      splBuffer[splIndex] = (wchar_t *)malloc(sizeof(wchar_t) * (len + 1));
	      #endif
	      wcsncpy(splBuffer[splIndex], &s[start], len);
	      splBuffer[splIndex][len] = L'\0';
	      splIndex++;
            }
	  else
	    {
	      len=0;
	      int start = x;
	      #if 1
	      /*
	      splBuffer = (splitBuffer)myrealloc(splBuffer, sizeof(wchar_t *) * (splIndex + 1),__func__, __FILE__);
	      splBuffer[splIndex] = (wchar_t *)mymalloc(sizeof(wchar_t) * (len + 1),__func__, __FILE__);
	      */
	      #else
	      splBuffer = (splitBuffer)realloc(splBuffer, sizeof(wchar_t *) * (splIndex + 1));
	      splBuffer[splIndex] = (wchar_t *)malloc(sizeof(wchar_t) * (len + 1));
	      #endif
	      wcsncpy(splBuffer[splIndex], &s[start], (len+2));
	      splBuffer[splIndex][len+1] = L'\0';
	      splIndex++;
	      x++;

	    }
        }
    }

    *countOut = splIndex;
}



// el len que se le pasa a esta funcion no devería tomar en cuenta las secuencias de escape
/*
void freeSplBuffer(splitBuffer splBuffer, int c)
{
  for(int x=0; x < c; x++)
    myfree(splBuffer[x]);
}
*/

void appendBuffer(const wchar_t *s, int len)
{
  List * lisString= getListOfString();
  int c;
  wchar_t spBuffer[MAX_ROW][MAX_CELL]={{L'\0'}};
  spliteEcapeAndWord(s, &c,spBuffer);
      for(int x=0; x < c; x++) appendNodoToList(lisString,spBuffer[x], wcslen(spBuffer[x]));
    //appendNode(splBuffer[x], wcslen(splBuffer[x]));
      /*
  #if 1
  freeSplBuffer(splBuffer, c);
  myfree(splBuffer);
  #else
  freeSplBuffer(splBuffer, c);
  free(splBuffer);
  #endif
  splBuffer=NULL;
      */
}


