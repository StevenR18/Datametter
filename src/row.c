#include "..\..\Vt\includes\row.h"
#include "..\..\Vt\includes\main_datametter.h"
#include <stdlib.h>
#define ROW_MAIN 1
#define ROW_DEFAULT 0
#define isZero(x)((x) <= 0)
#define rowisEmpty(r, index)((*r)[index].len == 0)
#define ISROWMAIN(r,index) ((*r)[index].rowType == ROW_MAIN)
#define xIsEqualRowLen(r,x,index)(x == (*r)[index].len)
extern View vEmu;

int isRowMain(int indexRow)
{
  return (vEmu.row[indexRow].rowType == ROW_MAIN);
}

int getLastRowLen()
{
  return vEmu.row[vEmu.capacityRow-1].len;
}

/*
  adres [row*]-->adress[row]
  Nombre de la funcion: appendRow()
  Parametros de entrada:
  arr: es un doble puntero a row, es un array dinamico de estructura Row
  c : un  puntero a wchar_t contiene la cadena que se ba a ingresar 
  len : es la longitud de la cade "c"
  capr: es un puntero a int sirve para actualizar la capacidad del array
  decripcion: agrega un estructura Row al final del array en cada llamda
              y acutaliza la  capcidad capr

  [ArrRow]->[RowPtr]-->[row][row1]
                appendRow() appendRow()
                 t0         t1
*/


void appendRow(ArrRow arr, wchar_t * c, int len, int *capr)
{

  // si la cadena que se ba ingresar a la row esta viacia salimos
  if(c == NULL)return;
  // se reserva memoria para la primera fila
  *arr = (RowPtr)realloc(*arr,sizeof(Row)*(*capr+1));
  // se reserva memoria para el string que contiene la fila
  ROW_STRING(arr,*capr) = (wchar_t *)malloc(sizeof(wchar_t)*(len+1));
  //se agrega la cadena "c" al espacio reservado
  wcsncpy(ROW_STRING(arr,*capr), c,len);
  //se asigna la longitud de la cadena sin contar el cero de terminacion
  ROW_LEN(arr,*capr)=len;
  //se agrega el cero de terminacion a la cadena
  NULL_TERMINATE(arr,*capr,ROW_LEN(arr,*capr));
  // inicializar a cero rowType
  ROW_TYPE(arr,*capr) = ROW_DEFAULT;
  // se aumenta la capacidad del array en 1
  (*capr)++;
  vEmu.ofsetYbottom++;  //ojo  esta variable deve pertenecer a rows
}
/*
  Nombre de la funcion: delteRow()
  Parametros de entrada:
  arr: es un doble puntero a row, es un array dinamico de estructura Row
  idexRow: indice de fila
  capr: capcidad actual arr decrementa en cada llamda
  descripcion: elimina la ultima fila de arr

 to [ArrRow]->[RowPtr]-->[row][row1][row2][rown]
                                            ^
                                          delete()
 t1 [ArrRow]->[RowPtr]-->[row][row1][row2][x]
                                       ^
				       delete()

 */


void deleteRow(ArrRow arr,int indexRow, int *capr)
{
  if(!((indexRow >= 0) &&(indexRow <= *capr)))return;
  free(ROW_STRING(arr,indexRow)); 
  // free((*row)[indexRow]);
  (*capr)--;
  vEmu.ofsetYbottom--; // esta variable dever pertenecer a rows
  
  
}

/*
  Nombre de la funcio: freRow()
  Parametros de entrada:
  arr    :es un doble puntero a row, es un array dinamico de estructura Row
  capr   :cpapcidad del arrray (cuenta el numero de filas que tiene el array actualmete)
  ofset1 : desplazamiento
  ofset2 : desplazamiento
  descripcion : libera todo el espacio del arr
 */

void freeRow(ArrRow arr, int *capr, int *ofset1, int *ofset2)
{
  if(*arr == NULL)return;
  for(int r =0; r < (*capr); r++)
    {
      free(ROW_STRING(arr,r));
    }
  free((*arr));
  (*arr)=NULL;
  *capr=0;
  *ofset1=0;
  *ofset2=0;
}

/*
  Nombre de la funcion: RowisEmpty()
  Parametros de entrada:
  arr      :es un doble puntero a row, es un array dinamico de estructura Row
  indexRow : indice de fila
  Parametros de salida:
  capr: cantidad total de filas que tiene el array actualmente
  descripcion:
  Revisa si la fila esta vacia, "Para que una fila este vacia"-
  (la fila deve existir y la longitud de su caedena es cero)

 */
//int isYiRowisEmpty(Row **row ,int indexRow,int *capr)
int RowisEmpty(ArrRow arr,int indexRow)
{
  return (ROW_LEN(arr,indexRow) == 0); 
}

/*
  Nombre de la funcion: RowisEmpty()
  Parametros de entrada:
  arr      :es un doble puntero a row, es un array dinamico de estructura Row
  indexRow :indice de fila
  Valor devuelto: devuelve la posicion donde se cuentra la ultima fila principal
  descripcion:buca en el array la ultimam fila principla
  to [ArrRow]->[RowPtr]-->[row][row1][row2][row3][rowmain][rown]
                                                            ^
                                                            find()
  t1 [ArrRow]->[RowPtr]-->[row][row1][row2][row3][rowmain][rown]
                                                     ^
                                                   find()= devuelveLaPocicion
 */
int findLastRowmain(ArrRow arr, int indexRow/*sumar el ofset_y*/)
{
  int position=0;
  for(int y= (indexRow); y > 0; y--)
    {
      if(ROW_TYPE(arr,y)== ROW_MAIN)
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

void appendCharToRow(ArrRow arr, wchar_t * c, int indexRow, int len)
{
  int prevLen= ROW_LEN(arr,indexRow);
  ROW_STRING(arr,indexRow)=(wchar_t *) realloc(ROW_STRING(arr,indexRow),sizeof(wchar_t)*(prevLen+len+1));
  //ROW_LEN(arr,indexRow)++;
  ROW_LEN(arr,indexRow)+=len;
  NULL_TERMINATE(arr,indexRow,ROW_LEN(arr,indexRow));
  wmemcpy(&(ROW_CHARAT(arr,indexRow,prevLen)), c, len);
  //wmemcpy(&(*arr)[indexRow].s[(*arr)[indexRow].len-1], c, len);
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
void reservMemoryRow(ArrRow arr,int indexRow,int len)
{
  int prevLen= ROW_LEN(arr,indexRow);
  ROW_STRING(arr,indexRow)=(wchar_t *)realloc(ROW_STRING(arr,indexRow),sizeof(wchar_t)*(prevLen+len+1));
  ROW_LEN(arr,indexRow)++;
  //  (*row)[indexRow].s[(*row)[yIndex].len]=L'\0';
  NULL_TERMINATE(arr,indexRow,ROW_LEN(arr,indexRow));
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
void shiftAndInserChar(ArrRow arr,
		       wchar_t c ,
		       int len,
		       int indexRow,
		       int dest,
		       int origin,
		       int moveCount)
{
  //wmemmove(&(*arr)[indexRow].s[dest],&(*arr)[indexRow].s[origin] ,moveCount-1);
  wmemmove(&(ROW_CHARAT(arr,indexRow,dest)),&(ROW_CHARAT(arr,indexRow,origin)) ,moveCount-1);
  
  wmemcpy(&(ROW_CHARAT(arr,indexRow,origin)), &c, len);
  NULL_TERMINATE(arr,indexRow,ROW_LEN(arr,indexRow));
}

/*
  Nombre de la funcion : appendCharsRow()

*/

void appendCharsRow(ArrRow arr,
		    wchar_t * c,
		    int cursorX,
		    int cursorY,
		    int ofsetY,
		    int len,
		    int *capExceeded,
		    int *capr,
		    int width,
		    int height)
  
{
  if(RowisEmpty(arr,cursorY))
    {
      appendCharToRow(arr,c,cursorY,len);
      // cursor->x++; // aumenta el cursor
      return;
    }
  cursorX+=(ISROWMAIN(arr,cursorY))? vEmu.lenDirectory : 0;
  
  if(xIsEqualRowLen(arr,cursorX,cursorY))
    {
      int prevLen= ROW_LEN(arr,cursorY);
      appendCharToRow(arr,c,cursorY,len);
      //cursorX++; // aumenta el cursor
      if((cursorX+1) == width)
	{
	  appendRow(arr,L"",0,capr);
	  
	  
	}
	
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
      size= ((ROW_LEN(arr,r) - xOrigin));
      reservMemoryRow(arr,r,len);
      if(prevChar[0]!= L'\0')
	{
	  currentChar[0] = (*arr)[r].s[(*arr)[r].len-1];	 
	  cursorX=0;
	}
      if(ROW_LEN(arr,r) > width)
	{
	  ROW_LEN(arr,r)--;
	  currentChar[0]= ROW_CHARAT(arr,r,(ROW_LEN(arr,r)-1));
	  prevChar[0]= (prevChar[0]== L'\0') ? *c : prevChar[0];
	  shiftAndInserChar(arr,*prevChar,len,r,xDest,xOrigin,size);
	  prevChar[0]= currentChar[0];
	  cursorX=0;
	  if((r+1)>(*capr-1))
	    {
	      appendRow(arr,L"",0,capr); // secrea una fila vacia
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
	  
	  wmemmove(&(ROW_CHARAT(arr,r,xDest)), &(ROW_CHARAT(arr,r,xOrigin)),size);
	  wmemcpy(&(ROW_CHARAT(arr,r,xOrigin)), &prevChar[0], len);
	  prevChar[0]= currentChar[0];
	  continue;
	}
    }
  //  cursor->x++; // aumento del cursor
}

/*
  Nombre de la funcion RowisMain()
  
  descripcion: revisa si la acutal fila es la fila principal
  
 */
//int isYiRowisMainAndLenisEqual7(Row **row,int y,int x,int ofset)
int RowisMainAndLenisEqualofset(ArrRow arr,int indexRow,int indexCell,int ofset)
{
  return ((ROW_TYPE(arr,indexRow) == ROW_MAIN)&&(indexCell == ofset));
}
/*
  Nombre de la funcion: deleteOneChar()
  Parametros de entrada:
  arr: array de  de estructuras filas
  indexRow: indice de la fila a la que se le ba eliminar el caracter
  indexCell: indice de la celda
  descripcion: elimina un solo caractere de una fila
 */
int deleteOneChar(ArrRow arr,int indexRow, int indexCell)
{
  int f=0;
  if(indexCell == ROW_LEN(arr,indexRow))
    {
      if(isZero(ROW_LEN(arr,indexRow)))
	{
	  f= 1;
	  goto is_zero;
	}
     
       NULL_TERMINATE(arr,indexRow,(ROW_LEN(arr,indexRow)-1));
       ROW_LEN(arr,indexRow)--;
      f=1;
    }
 is_zero:
   return f;
  
}


void delCharOfRow(ArrRow arr,int indexCell, int indexRow,int *capr)
{

  if(RowisEmpty(arr,indexRow))
    {
      deleteRow(arr,indexRow,capr);
      if((indexRow-1) >= 0) // revisa si hay un fila anterior
	{
	  NULL_TERMINATE(arr,(indexRow-1),ROW_LEN(arr,(indexRow-1)));
	  ROW_LEN(arr,(indexRow-1))--;
	}
       return;
    }
 
  if(RowisMainAndLenisEqualofset(arr,indexRow,indexCell,vEmu.lenDirectory))return;
  if(deleteOneChar(arr,indexRow,indexCell))return;
  int xPrev = indexCell;
  for(int r = indexRow; r < *capr; r++ )
    {
      int xDest=0;
      int xOrigin=0;
      int size=0;
      wchar_t c;
      if((xPrev > 0) && (xPrev <=(*arr)[r].len))
	{
	  xDest= (xPrev-1);
	  xOrigin= xPrev;
	  size=  ROW_LEN(arr,r) - xOrigin;
#define isYiRowHaveOnlyOneChar(_size)(_size == 0)
	  
	  if(isYiRowHaveOnlyOneChar(size))
	    {
	      deleteRow(arr,r,capr);
	      return;
	    }
#define hasYnextrow(_r,_cap)(((_r)+1) < (_cap))
	  // guardamos el ultimo caracter la siguiente fila
	 
	  if(hasYnextrow(r,*capr))c = ROW_CHARAT(arr,(r+1),0);
	  wmemmove(&(ROW_CHARAT(arr,(r),xDest)), &(ROW_CHARAT(arr,(r),xOrigin)),size);
	  if(r < (*capr-1))
	    {
	      if(isZero(ROW_LEN(arr,r)))return;
	      ROW_CHARAT(arr,r,(ROW_LEN(arr,r)-1))=c;
	    }
	  else
	    {
	      ROW_LEN(arr,r)--;
	      NULL_TERMINATE(arr,r,ROW_LEN(arr,r));
	      return;
	    }
	  xPrev=1;
	}
      else
	{
	  xDest   = xPrev;
	  xOrigin = xPrev+1;
	  size=(xOrigin == ROW_LEN(arr,r)) ? 1 : ((ROW_LEN(arr,r)) - xOrigin);
	  c = ROW_CHARAT(arr,r,0);
	  wmemmove(&(ROW_CHARAT(arr,r,xDest)),&(ROW_CHARAT(arr,r,xOrigin)),size);
	  if(r < (*capr))
	    {
	      if(isZero(r))return;
	      ROW_CHARAT(arr,(r-1),ROW_LEN(arr,(r-1)))=c;
	    }
	  xPrev=0;
	}
    }
  if(isZero(*capr))return;
  NULL_TERMINATE(arr,(*capr-1),ROW_LEN(arr,(*capr-1)));
  ROW_LEN(arr,(*capr-1))--;
  if(ROW_LEN(arr,(*capr-1)) ==0)deleteRow(arr,(*capr-1),capr);  
}













