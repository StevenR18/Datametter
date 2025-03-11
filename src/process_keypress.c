
#include "..\..\Vt\includes\process_keypress.h"
#include "..\..\Vt\includes\row.h"
#include "..\..\Vt\includes\main_datametter.h"
extern View vEmu;
extern Win32Platform GWindowPlatform;
#define OFSET_WIDTHINCHAR 2

/*
  Procesamiento  se cuencias de escape csi
 */

void delChar()
{
  int ofx=0;
  if(ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))== ROW_MAIN) ofx=vEmu.lenDirectory;
  if((ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))== ROW_MAIN)&&
     (/*ROW_LEN(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))*/
      (vEmu.vCursor.x+vEmu.ofsetX+ofx)== vEmu.lenDirectory))
    {	    
    return;
    }
  
  delCharOfRow(vEmu.arrRow,(vEmu.vCursor.x+vEmu.ofsetX+ofx),(vEmu.vCursor.y+vEmu.ofsetYTop),&vEmu.capacityRow);
  vEmu.vCursor.x--;
  if((vEmu.vCursor.x+vEmu.ofsetX+ofx) < 0)
    {
      vEmu.vCursor.y--;
      vEmu.ofsetX= (vEmu.widthBufferInChar- vEmu.widthInChar);
      if(ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))== ROW_MAIN)
	vEmu.vCursor.x=(vEmu.widthInChar-vEmu.lenDirectory)-1;
      else
	vEmu.vCursor.x = (vEmu.widthInChar-1);
    }
  else
    {
      if((vEmu.vCursor.x+vEmu.ofsetX+ofx) < vEmu.ofsetX)
	{
	  vEmu.vCursor.x=0;
	  vEmu.ofsetX--;
	}
    }
}

int isEscapeDelete(wchar_t * str,int len)
{
  return (((str[1] == L'[') && (str[2]==L'3'))&&(str[3] == L'~'));
}
int isEscapeMoveCursor(wchar_t *str,int len)
{
  return ((str[1] == L'[')&&((str[2]==L'A')||
			     (str[2]==L'B')||
			     (str[2]==L'C')||
			     (str[2]==L'D')));
}

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  Nombre de la funcion: freeScrollUp()
  Descripcion: 
                    t0
                 Emu.arrRow                     height=3
		     |
                     |               Si (ofsetYtop > 0) scrollup
row ---------------------------------------                    
 1  x ...                                x                     
    ---------------------------------------                    
 2  x ...                                x    (ofsetYTop =1)                    
    ---------------------------------------                    
 3  x ...                                x                           
    ---------------------------------------                    
 4  x ...                                x   (ofsetYbottom=4)              
    ---------------------------------------                     
 5  x ...                                x               
    ---------------------------------------                                             
 6  x...                                 x   
    ---------------------------------------              
 7  x...                                 x   
    ---------------------------------------        
 8  x..                                  x   
    ---------------------------------------
 9  x..                                  x   
    ---------------------------------------  
 10  x..                                  x  
    ---------------------------------------
*****************************************************************************************/
void freeScrollUp(int *ofsetYTop,int *ofsetYbottom)
{
  /// si el ofset_y es distinto de cero  se puede hacer escroll
  if(*ofsetYTop > 0 )
    {
      (*ofsetYTop)--;
      (*ofsetYbottom)--;
    }
}

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  Nombre de la funcion: freeScrollUp()
  Descripcion: 
                    t0
                 Emu.arrRow                     height=3
		     |
                     |               Si (ofsetYbottom !=capcityRow) scrollDown
row ---------------------------------------                    
 1  x ...                                x                     
    ---------------------------------------                    
 2  x ...                                x    (ofsetYTop =1)                    
    ---------------------------------------                    
 3  x ...                                x                           
    ---------------------------------------                    
 4  x ...                                x   (ofsetYbottom=4)              
    ---------------------------------------                     
 5  x ...                                x               
    ---------------------------------------                                             
 6  x...                                 x   
    ---------------------------------------              
 7  x...                                 x   
    ---------------------------------------        
 8  x..                                  x   
    ---------------------------------------
 9  x..                                  x   
    ---------------------------------------  
 10  x..                                  x  
    ---------------------------------------
*****************************************************************************************/
void freeScrollDown(int *ofsetYTop, int *capacityRow,int *ofsetYbottom)
{
  if (*ofsetYbottom != *capacityRow)
    {
      (*ofsetYTop)++;
      (*ofsetYbottom)++;
    }
}


/*
  funciones para abanzar entre palabras  para abanzar entre palabras 
 */


void moveBetweenWordsLeft(POINT *Cursor,int *ofset_y,int *ofset_x,int Width,
			   int WidthBuffer,
			   ArrRow arr)
{
  
  int lenD=0;
  int rowIndex = Cursor->y+*ofset_y;
  if(ROW_TYPE(arr,(rowIndex))== ROW_MAIN) lenD= vEmu.lenDirectory;
  int cx=Cursor->x;
  int cy=Cursor->y;
  if((cx + *ofset_x + lenD) == lenD)return;
  /// crear un funcion que revise si estas en una letra o un espacio en blanco
  wchar_t lastChar=ROW_CHARAT(arr,(cy+*ofset_y),(cx+*ofset_x+lenD));
  wchar_t inicRow = ROW_CHARAT(arr,(cy + *ofset_y),(lenD-1));
  while((lastChar != inicRow))
    {
      if (iswspace(lastChar))
	{
	  while(iswspace(lastChar)&& lastChar != inicRow)
	    {
	      cx--;
	      lastChar=ROW_CHARAT(arr,(cy + *ofset_y),(cx + *ofset_x + lenD));
	    }
	  break;
	}
      else if(!iswspace(lastChar))
	{
	  while(lastChar != L' '&& lastChar != inicRow)
	    {
	      cx--;
	      lastChar=ROW_CHARAT(arr,(cy + *ofset_y),(cx + *ofset_x + lenD));
	    }
	}
    }
   if(lastChar == inicRow)Cursor->x=0;
   else Cursor->x =cx; // actualizamos la nueva posicion
   /// verificamos si se hizo scorll horizontal
   if((Cursor->x + *ofset_x + lenD) == 0)
     {
       Cursor->y--;
       Cursor->x=WidthBuffer;
       moveBetweenWordsLeft(Cursor,ofset_y,ofset_x,Width,WidthBuffer,arr);
       return;
     }
   
   /*
   int hasScrolled =(((Width + *ofset_x) - (Cursor->x + *ofset_x + lenD)) < 0) ? 1:0;
   if(hasScrolled) *ofset_x =((Cursor->x + *ofset_x + lenD)-(Width + *ofset_x));
   */
}

void moveBetweenWordsRight(POINT *Cursor,int *ofset_y,int *ofset_x,int Width,
			   int WidthBuffer,
			   ArrRow arr)
{
  
  int lenD=0;
  int rowIndex = Cursor->y+*ofset_y;
  if(ROW_TYPE(arr,(rowIndex))== ROW_MAIN) lenD= vEmu.lenDirectory;
  int cx=Cursor->x;
  int cy=Cursor->y;
  if((cx + *ofset_x + lenD) == ROW_LEN(arr,rowIndex))return;
  /// crear un funcion que revise si estas en una letra o un espacio en blanco
  wchar_t lastChar=ROW_CHARAT(arr,(cy+*ofset_y),(cx+*ofset_x+lenD));
  
  while((lastChar != L'\0'))
    {
      if (iswspace(lastChar))
	{
	  while(iswspace(lastChar)&& lastChar != L'\0')
	    {
	      cx++;
	      lastChar=ROW_CHARAT(arr,(cy + *ofset_y),(cx + *ofset_x + lenD));
	    }
	  break;
	}
      else if(!iswspace(lastChar))
	{
	  while(lastChar != L' '&& lastChar != '\0')
	    {
	      cx++;
	      lastChar=ROW_CHARAT(arr,(cy + *ofset_y),(cx + *ofset_x + lenD));
	    }
	}
    }
   if(lastChar == L'\0')Cursor->x=(ROW_LEN(arr,(cy + *ofset_y))-(*ofset_x+lenD));
   else Cursor->x =cx; // actualizamos la nueva posicion
   /// verificamos si se hizo scorll horizontal
   if((Cursor->x + *ofset_x + lenD) == WidthBuffer)
     {
       Cursor->y++;
       Cursor->x=0;
       moveBetweenWordsRight(Cursor,ofset_y,ofset_x,Width,WidthBuffer,arr);
       return;
     }
   /*
   int hasScrolled =(((Width + *ofset_x) - (Cursor->x + *ofset_x + lenD)) < 0) ? 1:0;
   if(hasScrolled) *ofset_x =((Cursor->x + *ofset_x + lenD)-(Width + *ofset_x));
   */
}

int isEscapeCtrl(wchar_t *str, int len)
{

  /// indentificar un secuencia de escape generada por una combinacion Ctrl+
  int flag  = ((str[1]== L'[')&&(str[2]== L'1')&&(str[3]== L';')&&(str[4]== L'5'));
  int flag2 = ((str[5]==L'C')||(str[5]==L'D')||(str[5]==L'A')||(str[5]==L'B'));
  return (flag && flag2);
}


void processSequences(wchar_t * str, int len)
{
  if(isEscapeDelete(str,len))
    {
      delChar(); 
    }
  else if(isEscapeMoveCursor(str,len))
    {
      term_move_cursor(str[2]);
    }
  else if(isEscapeCtrl(str,len))
    {
      wchar_t func= str[5];
      switch(func)
	{
	case L'C':
	  moveBetweenWordsRight(&vEmu.vCursor,
				&vEmu.ofsetYTop,
				&vEmu.ofsetX,
				vEmu.widthInChar,
				vEmu.widthBufferInChar,
				vEmu.arrRow
				);
	  break;
	case L'D':
	  moveBetweenWordsLeft(&vEmu.vCursor,
				&vEmu.ofsetYTop,
				&vEmu.ofsetX,
				vEmu.widthInChar,
				vEmu.widthBufferInChar,
				vEmu.arrRow
				);
	  break;
	case L'A':
	  freeScrollUp(&vEmu.ofsetYTop,&vEmu.ofsetYbottom);
	  break;
	case L'B':freeScrollDown(&vEmu.ofsetYTop, &vEmu.capacityRow,&vEmu.ofsetYbottom);
	  break;
	}
    }
}
/***************************************************************************************/

/*
  Procesar caracter \n nueva linea
  
 */

void processNewLine()
{
#define onlyReturnifisCommandRow (isRowMain((vEmu.vCursor.y+vEmu.ofsetYTop)) && (getLastRowLen() == vEmu.lenDirectory))
  
  if(onlyReturnifisCommandRow)
    {
             //// crear una funcion para procesar los comandos de la terminal
      
      if(!(scrollDown(/*vEmu.heightInChar-1*/)))vEmu.vCursor.y++;
      //else //vEmu.vCursor.y= vEmu.ofsetYTop;

      appendRow(vEmu.arrRow,(wchar_t *)vEmu.currentDirectory,
		wcslen((wchar_t *)vEmu.currentDirectory),&vEmu.capacityRow);
      //row[vEmu.vCursor.y+vEmu.ofsetYTop].rowType = ROW_MAIN;
      ROW_TYPE(vEmu.arrRow,vEmu.vCursor.y+vEmu.ofsetYTop)=ROW_MAIN;

    }
  else
    {
      wchar_t command[500];
      extractCommand(command);
      procesingCommands(command);
      
      appendRow(vEmu.arrRow,(wchar_t *)vEmu.currentDirectory,wcslen((wchar_t *)vEmu.currentDirectory),&vEmu.capacityRow);
      vEmu.vCursor.x=0;
      if(!scrollDown(/*vEmu.heightInChar*/))
	{
	  // crear una funcion que devuelva la posicion y
	  // de la ultima fila
	  vEmu.vCursor.y=(vEmu.capacityRow-1);
	}
      else
	{
	  // el cursor y deve ubicarse en la ultima row
	  int indexLastRow= ((vEmu.capacityRow-1)- vEmu.ofsetYTop); 
	  vEmu.vCursor.y=indexLastRow;
	}
      //row[vEmu.vCursor.y+vEmu.ofsetYTop].rowType = ROW_MAIN;      
      ROW_TYPE(vEmu.arrRow,vEmu.vCursor.y+vEmu.ofsetYTop)=ROW_MAIN;
   }
}
/***************************************************************************************/

/*
  procesamiento de caracteres imprimibles

 */
void adjustCursor(int *capRowExceded,int lastLen)
{
  
  int ox = (ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop)) == ROW_MAIN) ? vEmu.lenDirectory : 0;
  if((vEmu.vCursor.x+vEmu.ofsetX + ox) >= (vEmu.widthBufferInChar) || (*capRowExceded))
    {
      int sizeRest=0;
      if(!(scrollDown(vEmu.heightInChar)))
	{
	  // revisar si la row anterior ha sobrepasado el ancho del buffer
	  // si sobrebasa el ancho del buffer restar a su len loue sobre
	  // reubicar el nuevo cero de terminacion para la row
	  // el sobrante colocarlo en la fila siguiente

	  /*
                width=35                           el width es impar
	  -----------------------------------  -----
	  ^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^  B   ----> sobranete de 1 cuando ancho imp    
	  -----------------------------------  -----
	                                    x   X        x=34  X=36  size= 35-36= 1

	  
                width=36                           el width es impar
	  ------------------------------------  ----
	  ^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B  ---> no hay sobrante
	  ------------------------------------  ----   
	                                    x   X        x=34  X=37 size= len-36 =0
	  */	  
	  wchar_t rest[10]={L'\0'};
	  sizeRest=(vEmu.widthBufferInChar - ROW_LEN(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))); 
	  if(sizeRest < 0)
	    {
	      sizeRest*=-1;
	      int prevLen= (*vEmu.arrRow)[vEmu.vCursor.y+vEmu.ofsetYTop].len;
	      //si entra aqui el tamaño de la fila es mayor que el buffer
	      wmemcpy(rest,
		      &(*vEmu.arrRow)[vEmu.vCursor.y+vEmu.ofsetYTop].s[prevLen-1],
		      sizeRest);

	      NULL_TERMINATE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop),(int)vEmu.widthBufferInChar);
	      ROW_LEN(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))-=sizeRest;
	    } 
	 vEmu.vCursor.y++;
	 vEmu.ofsetX=0;
	 vEmu.vCursor.x=0;
	 if(sizeRest != 0)processChar(rest, sizeRest);
	}
      else
	{
	  if((vEmu.vCursor.x +vEmu.ofsetX + ox) != vEmu.widthBufferInChar)
	    {
	       if(vEmu.vCursor.y != 0)vEmu.vCursor.y--;
	       vEmu.ofsetX=0;
	    }
	  else
	    {
	      vEmu.vCursor.y = vEmu.heightInChar-1;
	      vEmu.ofsetX=0;
	    }
	}
      /// aqui tienes que arreglar
      if(*capRowExceded != 1)
	vEmu.vCursor.x=0;
      if(sizeRest != 0)
	vEmu.vCursor.x=1;
	
      *capRowExceded=0;
    }
  else
    {
      // se modifica el ofsetX
      if((vEmu.vCursor.x+ ox) >= vEmu.widthInChar)
	{
	  /*
	   -----------------------------------------------------------------------------------------------------
            Caso en que el ancho de la ventana es inpar
	    WIDTH = 35
	    ----------------------------------- ----------
            ^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B   X
	    ----------------------------------- ----------
                                              x  X                 x=34   X=36 =OF_2
	     WIDTH = 35
	    ----------------------------------- ----------
            C^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B   X
	    ----------------------------------- ----------
                                             x  X                 x=33   X=35 OF_1
----------------------------------------------------------------------------------------------------------------
	    Caso en el que el ancho de la ventana es par

	    WIDTH = 34
	    ---------------------------------- ----------
            ^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B   X
	    ---------------------------------- ----------
                                            x  X                 x=32   X=34 OF_1

	    WIDTH = 34
	    ---------------------------------- ----------
            C^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B^B   X
	    ---------------------------------- ----------
                                             x  X                 x=33   X=35 OF_2
----------------------------------------------------------------------------------------------------------------

	   */
      
	  if(vEmu.ofsetX < (vEmu.widthBufferInChar - vEmu.widthInChar))
	    {
	      if((int)vEmu.widthInChar % 2 == 0)
		{
		  if((vEmu.vCursor.x/*+vEmu.ofsetX */+ ox) % 2 == 0)
		    {
		      vEmu.ofsetX++;
		      vEmu.vCursor.x = (vEmu.widthInChar-1)- ox;
		    }
		  else
		    {
		      vEmu.ofsetX+=2;
		      vEmu.vCursor.x = (vEmu.widthInChar-1)- ox;
		    }
		}
	      else
		{
		   if((vEmu.vCursor.x/*+vEmu.ofsetX */+ ox) % 2 == 0)
		    {
		       vEmu.ofsetX+=2;
		      vEmu.vCursor.x = (vEmu.widthInChar-1)- ox;
		      
		    }
		  else
		    {
		      vEmu.ofsetX++;
		      vEmu.vCursor.x = (vEmu.widthInChar-1)- ox;
		     
		    }
		  
		} 
	    }
	}
    }
}

void processChar(wchar_t *str, int len)
{
  int caprExeded=0;
  appendCharsRow(vEmu.arrRow,&str[0],(vEmu.vCursor.x+vEmu.ofsetX),
		 (vEmu.vCursor.y+vEmu.ofsetYTop),vEmu.ofsetYTop,len,
		 &caprExeded,
		 &vEmu.capacityRow,
		 vEmu.widthBufferInChar,
		 vEmu.heightInChar);
  vEmu.vCursor.x+=len;
  adjustCursor(&caprExeded,len);
}


void chageLayout(LAYOUT layout)
{
  vEmu.layoutType= layout;
  RedrawWindow(GWindowPlatform.Window,NULL,NULL,RDW_INVALIDATE);
}

void  proceesKeyPress(List *inp)
{
  //int capRowExceded=0;
  wchar_t * resto=NULL;
  for(int x=0; x<(BUTTON_COUNT -1);x++)
    {
      wchar_t * str =(wchar_t *)inp->head[x].data;
      int len= wcslen(str);
      if(len == 0) return;
      if(str[0]== L'\x1b')
	{
	  // secuencias de escape csi
	  processSequences(str,len);
	  
	}
      else if(str[0]== L'\n')
	{
	  // secencias de control de nueva linia
	  processNewLine();
	}
      else if(str[0] == L'^')
	{
	  // combinaciones con control sin asignar
	  if(iswalpha(str[1])) processChar(str,2);
	}
      else if(str[0]==L'*')
	{
	  // combinaciones con ctrl asignadas
	  wchar_t letter= str[1];
	  switch(letter)
	    {
	    case L'A':break;
	    case L'C':break;
	    case L'F':break;
	    case L'H':break;
	    case L'I':break;
	    case L'J':break;
	    case L'L':
	      /// fucion para cambiar la distribucionde las ventanas
	      #define MAX_LAYOUT 3
	      static LAYOUT layouts[3]={LAYOUT_2,LAYOUT_3,LAYOUT_DEFECT};
	      static int indexLayout=0;
	      chageLayout(layouts[indexLayout]);
	      indexLayout++;
	      if(indexLayout == MAX_LAYOUT) indexLayout=0;
	      
	      break;
	    case L'M':/*activar el cursor en modo libre*/
	      break;
	    case L'V':break;
	    default:
	      GWindowPlatform.Running=0;
	      break;
	      
	    }
	  
	}
      else
	{
	  // procesameinto de carateres normales
	  processChar(str,len);
	}
    }
}



