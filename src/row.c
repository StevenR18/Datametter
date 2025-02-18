#include "..\..\Vt\includes\row.h"
#include <stdlib.h>
#define ROW_MAIN 1
#define isZero(x)((x) <= 0)
#define rowisEmpty(r, index)((*r)[index].len == 0)
#define ISROWMAIN(r,index) ((*r)[index].rowType == ROW_MAIN)
#define xIsEqualRowLen(r,x,index)(x == (*r)[index].len)
extern size_t lenDirectory;


//addres[row ** ]--> adrres[row *]--> addres[row]
//adrres[row *]--> addres[row]
//                 addres[row]
//		   addres[row]
                    

/*
  typedef struct
  {

  char *s;
  int len
  
  }Row

  
Row *row;

Row **r= &row;


adres[row **]-->adres[row *]-->adres[row]
                   

(*r)->s;
(*r)->len;
 */

void appendRow(Row **row, const wchar_t * s, int len, int *capr)
{
 
  if(s == NULL)return;
  *row = (Row *)realloc(*row,sizeof(Row)*(*capr+1));
  (*row)[*capr].s = (wchar_t *)malloc(sizeof(wchar_t)*(len+1));
  wcsncpy((*row)[*capr].s, s,len); 
  (*row)[*capr].len=len;
  (*row)[*capr].s[len]=L'\0';
  (*capr)++;
}

void deleteRow(Row **row,int indexRow, int *capr)
{
  if(!((indexRow >= 0) &&(indexRow <= *capr)))return;
  free((*row)[indexRow].s); 
  // free((*row)[indexRow]);
  (*capr)--;
}


void freeRow(Row ** row, int *capr, int *ofset1, int *ofset2)
{
  if(row == NULL)return;
  for(int r =0; r < (*capr); r++)
    {
      free((*row)[r].s);
    }
  free((*row));
  (*row)=NULL;
  *capr=0;
  *ofset1=0;
  *ofset2=0;
}

int isYiRowisEmpty(Row **row ,int indexRow,int *capr)
{
  int f=0;
  if((*row)[indexRow].len == 0)
    {
      deleteRow(row,indexRow,capr);
      if((indexRow-1) >= 0)
	{
	  if(isZero(indexRow))
	    {
	      f=1;
	      goto is_zero;
	      
	    }
	  (*row)[indexRow-1].s[(*row)[indexRow-1].len - 1]=L'\0';
	  (*row)[indexRow-1].len--;
	  f=1;
	}
    }
 is_zero:
   return f;
}

int findLastRowmain(Row **row, int cursor_y/*sumar el ofset_y*/)
{
  int position=0;
  for(int y= (cursor_y); y > 0; y--)
    {
      if((*row)[y].rowType== ROW_MAIN)
	{
	  position= y;
	  break;
	}
    }
  return position;
}


/*
  
  Nombre de la funcion: appendChar()

  Parametros de entrada
    *c     : puntero a una cadena de caracteres
    yIndex : indice de fila row[0] row[1].. row[yIndex]
    len    : longitud de *c
    
  Descripcion: agrega un caracter al final de una fila y agrega el cero de terminacion
               de cadena '\0'
    row[len-1]=*c row[len]='\0'  row[a]
  Retorna) (void)
 */
void appendCharToRow(Row **row,const wchar_t *c, int yIndex, int len)
{
  int prevLen= (*row)[yIndex].len;
  (*row)[yIndex].s= (wchar_t *) realloc((*row)[yIndex].s,sizeof(wchar_t)*(prevLen+len+1));
  (*row)[yIndex].len++;
  (*row)[yIndex].s[(*row)[yIndex].len]=L'\0';
  wmemcpy(&(*row)[yIndex].s[(*row)[yIndex].len-1], c, len);
}

/*
  Nombre de la funcion : reservMemory()

  parametros de entrada
     yIndex : indice de fila row[0] row[1].. row[yIndex]
     len    : longitud de la cadena que ba a alojar
  Descripcion : reserva memoria para una fila, toma encuenta la cantidad de memoria que
                tenia reservada anteriormete y le suma el espacio adicional para el cara-
		ter nuevo que se va ingresar + el cero de terminacion (prevLen +len+ 1),-
                agrega tambien el cero de terminacion de cadena.
 */
void reservMemoryRow(Row **row,int yIndex,int len)
{
  int prevLen= (*row)[yIndex].len;
  (*row)[yIndex].s=(wchar_t *)realloc((*row)[yIndex].s,sizeof(wchar_t)*(prevLen+len+1));
  (*row)[yIndex].len++;
  (*row)[yIndex].s[(*row)[yIndex].len]=L'\0';
}

/*
  Nombre de la funcion : shiftAndInserChar()
  Parametros de entrada.
   c         : caracter que se ba a ingresar en el hueco
   len       : longitud del caracter (pueden ser varios)
   yIndex    : indice de fila row[0] row[1].. row[yIndex]
   dest      : posicion donde se moveran los caracteres
   origin    : origen de los datos que seran movidos
   moveCount : cantidad de caracters que se ban a mover
  Descripcion: mueve una cantidad (moveCount) de caracteres desde un origen (origin),hac-               un destino (dest) e ingresa un caracter (c) en el hueco que queda.
  t0  s[ a | b | c | d ] moveCount= 4
         o   d
       
  t1 s[ a | a | b | c ] moveCount= 4
        o   d
      
  t3 s[ c | a | b | c ] moveCount= 4
        o   d
  Retorna (void)
*/
void shiftAndInserChar(Row **row,
		       wchar_t c ,
		       int len,
		       int yIndex,
		       int dest,
		       int origin,
		       int moveCount)
{
  wmemmove(&(*row)[yIndex].s[dest],&(*row)[yIndex].s[origin] ,moveCount-1);
  wmemcpy(&(*row)[yIndex].s[origin], &c, len);
  (*row)[yIndex].s[(*row)[yIndex].len]=L'\0'; ///finalizamos la cadena
}

/*
  Nombre de la funcion : appendCharsRow()

*/

void appendCharsRow(Row **row,
		    const wchar_t *c,
		    int cursorX,
		    int cursorY,
		    int ofsetY,
		    int len,
		    int *capExceeded,
		    int *capr,
		    int width,
		    int height)
  
{
  if(rowisEmpty(row,cursorY))
    {
      appendCharToRow(row,c,cursorY,len);
      // cursor->x++; // aumenta el cursor
      return;
    }
  cursorX+=(ISROWMAIN(row,cursorY))? lenDirectory : 0;
  
  if(xIsEqualRowLen(row,cursorX,cursorY))
    {
      int prevLen= (*row)[cursorY].len;
      appendCharToRow(row,c,cursorY,len);
      //cursorX++; // aumenta el cursor
      if((cursorX+1) == width)appendRow(row,L"",0,capr);
      return;
    }  
  wchar_t  prevChar[2]={L'\0',L'\0'};
  wchar_t  currentChar[2]={L'\0',L'\0'};
  int xDest=0;
  int xOrigin=0;
  int size=0;
  
  for(int r=cursorY; r< *capr; r++)
    {
      xDest   = ( cursorX +1);
      xOrigin = cursorX;
      size= (((*row)[r].len - xOrigin));
      reservMemoryRow(row,r,len);
      if(prevChar[0]!= L'\0')
	{
	  currentChar[0] = (*row)[r].s[(*row)[r].len-1];	 
	  cursorX=0;
	}
      if((*row)[r].len > width)
	{
	  (*row)[r].len--;
	  currentChar[0]= (*row)[r].s[(*row)[r].len-1];
	  prevChar[0]= (prevChar[0]== L'\0') ? *c : prevChar[0];
	  shiftAndInserChar(row,*prevChar,len,r,xDest,xOrigin,size);
	  prevChar[0]= currentChar[0];
	  cursorX=0;
	  if((r+1)>(*capr-1))
	    {
	      appendRow(row,L"",0,capr); // secrea una fila vacia
	      if(*capr >height+ofsetY)
		{
		  *capExceeded=1;
		 
		}
	    }
	  continue;
	}
      else
	{
	  prevChar[0]= (prevChar[0]== L'\0') ? *c : prevChar[0];
	  
	  if(size == 0) size=1;
	  wmemmove(&(*row)[r].s[xDest], &(*row)[r].s[xOrigin],size);
	  wmemcpy(&(*row)[r].s[xOrigin], &prevChar[0], len);
	  prevChar[0]= currentChar[0];
	  continue;
	}
    }
  //  cursor->x++; // aumento del cursor
}

int isYiRowisMainAndLenisEqual7(Row **row,int y,int x,int ofset)
{
  return (((*row)[y].rowType == ROW_MAIN)&&(x == ofset));
}
int deleteOneChar(Row **row,int y,int x)
{
  int f=0;
  if(x == (*row)[y].len)
    {
      if(isZero((*row)[y].len))
	{
	  f= 1;
	  goto is_zero;
	}
      (*row)[y].s[(*row)[y].len-1]=L'\0';
      (*row)[y].len--;
      f=1;
    }
 is_zero:
   return f;
  
}


void delCharOfRow(Row **row,int x, int y,int *capr)
{

  if(isYiRowisEmpty(row,y,capr))return;
  if(isYiRowisMainAndLenisEqual7(row,y,x,lenDirectory))return;
  if(deleteOneChar(row,y,x))return;
  int xPrev = x;
  for(int r = y; r < *capr; r++ )
    {
      int xDest=0;
      int xOrigin=0;
      int size=0;
      wchar_t c;
      if((xPrev > 0) && (xPrev <=(*row)[r].len))
	{
	  xDest= (xPrev-1);
	  xOrigin= xPrev;
	  size= (*row)[r].len - xOrigin;
#define isYiRowHaveOnlyOneChar(_size)(_size == 0)
	  
	  if(isYiRowHaveOnlyOneChar(size))
	    {
	      deleteRow(row,r,capr);
	      return;
	    }
#define hasYnextrow(_r,_cap)(((_r)+1) < (_cap))
	  // guardamos el ultimo caracter la siguiente fila
	  if(hasYnextrow(r,*capr))c = (*row)[r+1].s[0]; 
	  wmemmove(&(*row)[r].s[xDest], &(*row)[r].s[xOrigin],size);
	  
	  if(r < (*capr-1))
	    {
	      if(isZero(row[r]->len))return;
	      (*row)[r].s[(*row)[r].len-1]=c;
	    }
	  else
	    {
	      (*row)[r].len--;
	      (*row)[r].s[(*row)[r].len]=L'\0';
	      return;
	    }
	  xPrev=1;
	}
      else
	{
	  xDest   = xPrev;
	  xOrigin = xPrev+1;
	  size=(xOrigin == (*row)[r].len) ? 1 : ((*row)[r].len - xOrigin);
	  c = (*row)[r].s[0];
	  wmemmove(&(*row)[r].s[xDest], &(*row)[r].s[xOrigin],size);
	  if(r < (*capr))
	    {
	      if(isZero(r))return;
	      
	      (*row)[r-1].s[(*row)[r-1].len-1]=c;
	    }
	  
	  xPrev=0;
	}
    }
  if(isZero(*capr))return;
  (*row)[*capr-1].s[(*row)[*capr-1].len-1]=L'\0';
  (*row)[*capr-1].len--;
  if(((*row)[*capr-1].len ==0))deleteRow(row,(*capr-1),capr);  
}










