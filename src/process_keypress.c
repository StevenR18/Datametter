
#include "..\..\Vt\includes\process_keypress.h"
#include "..\..\Vt\includes\row.h"
#include "..\..\Vt\includes\main_datametter.h"
extern size_t lendDirectory;

/*
  Procesamiento  se cuencias de escape csi
 */

void delChar()
{
  int ofx=0;
  if(ROW_TYPE(arrRow,(vEmu.vCursor.y+vEmu.ofsetY))== ROW_MAIN) ofx=lenDirectory;
  if((ROW_TYPE(arrRow,(vEmu.vCursor.y+vEmu.ofsetY))== ROW_MAIN)&&
     (/*ROW_LEN(arrRow,(vEmu.vCursor.y+vEmu.ofsetY))*/
      (vEmu.vCursor.x+vEmu.ofsetX+ofx)== lenDirectory))
    {	    
    return;
    }
  
  delCharOfRow(arrRow,(vEmu.vCursor.x+vEmu.ofsetX+ofx),(vEmu.vCursor.y+vEmu.ofsetY),&vEmu.capRow);
  vEmu.vCursor.x--;
  if((vEmu.vCursor.x+vEmu.ofsetX+ofx) < 0)
    {
      vEmu.vCursor.y--;
      vEmu.ofsetX= (vEmu.widthBuffer- vEmu.width);
      if(ROW_TYPE(arrRow,(vEmu.vCursor.y+vEmu.ofsetY))== ROW_MAIN)
	vEmu.vCursor.x=(vEmu.width-lenDirectory)-1;
      else
	vEmu.vCursor.x = (vEmu.width-1);
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

int isBlankSpace(int cursor_x,int cursor_y,ArrRow arr)
{
  return ((ROW_CHARAT(arr,cursor_y,cursor_x)== L' ')&&
	  ((ROW_CHARAT(arr,cursor_y,cursor_x)) != L'\0'));
  
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
  if(ROW_TYPE(arr,(rowIndex))== ROW_MAIN) lenD= lenDirectory;
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
  if(ROW_TYPE(arr,(rowIndex))== ROW_MAIN) lenD= lenDirectory;
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
				&vEmu.ofsetY,
				&vEmu.ofsetX,
				vEmu.width,
				vEmu.widthBuffer,
				arrRow
				);
	  break;
	case L'D':
	  moveBetweenWordsLeft(&vEmu.vCursor,
				&vEmu.ofsetY,
				&vEmu.ofsetX,
				vEmu.width,
				vEmu.widthBuffer,
				arrRow
				);
	  break;
	  // TODO: implementar esta deven hacer escoroll vertical arriba abajo
	case L'A':break;
	case L'B':break;
	 
	};
      
      
    }
}
/***************************************************************************************/

/*
  Procesar caracter \n nueva linea
  
 */

void processNewLine()
{
#define onlyReturnifisCommandRow (isRowMain((vEmu.vCursor.y+vEmu.ofsetY)) && (getLastRowLen() == lenDirectory))
  
  if(onlyReturnifisCommandRow)
    {
      if(!(scrollDown(vEmu.height-1)))vEmu.vCursor.y++;
      //else //vEmu.vCursor.y= vEmu.ofsetY;
      appendRow(arrRow,(wchar_t *)vEmu.currentDirectory,
		wcslen((wchar_t *)vEmu.currentDirectory),&vEmu.capRow);
      //row[vEmu.vCursor.y+vEmu.ofsetY].rowType = ROW_MAIN;
      ROW_TYPE(arrRow,vEmu.vCursor.y+vEmu.ofsetY)=ROW_MAIN;
      //// crear una funcion para procesar los comandos de la terminal
    }
  else
    {
      wchar_t command[500];
      extractCommand(command);
      procesingCommands(command);
      // TODO: CORREGIR ESTA PARTE
      // luego de la ejecucion se reinicia la terminal
      appendRow(arrRow,(wchar_t *)vEmu.currentDirectory,wcslen((wchar_t *)vEmu.currentDirectory),&vEmu.capRow);
      vEmu.vCursor.x=0;
      if(!scrollDown(vEmu.height))
	{
	  // crear una funcion que devuelva la posicion y
	  // de la ultima fila
	  vEmu.vCursor.y=(vEmu.capRow-1);
	}
      else
	{
	  vEmu.vCursor.y= vEmu.height -1;
	}
      //row[vEmu.vCursor.y+vEmu.ofsetY].rowType = ROW_MAIN;      
      ROW_TYPE(arrRow,vEmu.vCursor.y+vEmu.ofsetY)=ROW_MAIN;
   }
}
/***************************************************************************************/

/*
  procesamiento de caracteres imprimibles

 */
void adjustCursor(int *capRowExceded)
{
  
  int ox = (ROW_TYPE(arrRow,(vEmu.vCursor.y+vEmu.ofsetY)) == ROW_MAIN) ? lenDirectory : 0;
  if((vEmu.vCursor.x+vEmu.ofsetX + ox) >= (vEmu.widthBuffer) || (*capRowExceded))
    {
      if(!(scrollDown(vEmu.height)))
	{
	 vEmu.vCursor.y++;
	 vEmu.ofsetX=0;
	}
      else
	{
	  if((vEmu.vCursor.x +vEmu.ofsetX + ox) != vEmu.widthBuffer)
	    {
	       if(vEmu.vCursor.y != 0)vEmu.vCursor.y--;
	       vEmu.ofsetX=0;
	    }
	  else
	    {
	      vEmu.vCursor.y = vEmu.height-1;
	      vEmu.ofsetX=0;
	    }
	}
      if(*capRowExceded != 1)vEmu.vCursor.x=0;
      *capRowExceded=0;
    }
  else
    {
      if((vEmu.vCursor.x+ ox) >= vEmu.width)
	{
	  if(vEmu.ofsetX < (vEmu.widthBuffer - vEmu.width))
	    {
	      vEmu.ofsetX++;
	      vEmu.vCursor.x = (vEmu.width-1)- ox;
	    }
	}
    }

}

void processChar(wchar_t *str, int len)
{
  int caprExeded=0;
  appendCharsRow(arrRow,&str[0],(vEmu.vCursor.x+vEmu.ofsetX),
		 (vEmu.vCursor.y+vEmu.ofsetY),vEmu.ofsetY,1,
		 &caprExeded,
		 &vEmu.capRow,
		 vEmu.widthBuffer,
		 vEmu.height);
  vEmu.vCursor.x++;
  adjustCursor(&caprExeded);
}

void  proceesKeyPress(List *inp)
{
  int capRowExceded=0;
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
      else
	{
	  // procesameinto de carateres normales
	  processChar(str,len);
	}
    }
}



