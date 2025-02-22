#include <windows.h>
#include <stdio.h>
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\main_datametter.h"
#include "..\..\Vt\includes\font.h"
#include "..\..\Vt\includes\newwvt.h"
#include "..\..\Vt\includes\liked_list.h"
#include "..\..\Vt\includes\memory_traker.h"
#include "..\..\Vt\includes\vt100_commands.h"
#define CTRL_KEY(ascii) ((ascii) & 0x1F)
#define ARROW_UP(s0)    (((s0) & 0x7700)) // Representa 'W'
#define ARROW_DOWN(s0)  (((s0) & 0x7300)) // Representa 'S'
#define ARROW_LEFT(s0)  (((s0) & 0x6100)) // Representa 'A'
#define ARROW_RIGHT(s0) (((s0) & 0x6400)) // Representa 'D'
#define OFSET_X 7
#define ROW_MAIN 1
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define ATCITIVE_WASD 0
#define ROW_MAIN 1
#define ifYexceedTOrowcap(y) (( (y) +1 ) > (caprow - 1))
extern Win32ScreenBuffer GBuffer;
extern int Runnig;
extern unsigned char enableBlinkCursor; // se utiliza para el parapadeo del cursor
extern Termv Emu; /// estructura de newwvt.c
/// implementar el scroll vertical para la consola
enum
  {
    EDIT,
    TERMINAL
  };
View vEmu;
#define WIDTH_TERMV  60
#define HEIGHT_TERMV 25
#define comand vEmu.buffercomand
#define cursor_x vEmu.vCursor.x
#define cursor_y vEmu.vCursor.y
#define ofset_y vEmu.ofsetY
#define ofset_x vEmu.ofsetX
#define Cursor  vEmu.vCursor
#define width  vEmu.width
#define height vEmu.height
#define row vEmu.row
#define caprow vEmu.capRow
#define ofsetrow vEmu.ofsetCapRow

int scrollUp()
{
  int result=0;
  if((caprow != 0) && (ofset_y != 0 ))
    {
      ofset_y--;
      ofsetrow++;
      result=1;
    }
  return result;
}

int scrollDowWithCursor()
{
  int t = 0;
  if (caprow > (HEIGHT_TERMV + ofset_y))
  {
    ofset_y++;
    if (ofsetrow != 0) ofsetrow--;
    t = 1;
  }
  return t;
} // donde está el error

int scrollDown(int heightVt)
{
  int f=0;
  if(caprow > heightVt+ofset_y)
    {
      int dy=(caprow - (heightVt+ofset_y));
      ofset_y+=dy;
      if(ofsetrow !=0)ofsetrow--;
      f=1;
    }
  return f;
}



void appendRow(const wchar_t * s, int len)
{
  if(s == NULL)return;
  row = (Row **) realloc(row,sizeof(Row* )*(caprow+1));
  row[caprow]= (Row *) malloc(sizeof(Row));
  row[caprow]->s = (wchar_t *)malloc(sizeof(wchar_t)*(len+1));
  wcsncpy(row[caprow]->s, s,len); 
  row[caprow]->len=len;
  row[caprow]->s[len]=L'\0';
  caprow++;
}

void deleteRow(int y)
{
  if(!((y >= 0) &&(y <= caprow)))return;
  free(row[y]->s); 
  free(row[y]);
  // free(&row[y]);
  caprow--;
}

void freeRow()
{
  if(row == NULL)return;
  for(int r =0; r <caprow; r++)
    {
      free(row[r]->s);
      free(row[r]);
    }
   row=NULL;
   caprow=0;
   ofset_y=0;
   ofsetrow=0;
}
#define isZero(x)((x) <= 0)
int absoluteValue(int x)
{
  if(x < 0) return(x*-1);
  return x;
}

#define rowisEmpty(r, index)(r[index]->len == 0)
#define ISROWMAIN(r,index) (r[index]->rowType == ROW_MAIN)
#define xIsEqualRowLen(r,x,index)(x == r[index]->len)
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
void appendChar(const wchar_t *c, int yIndex, int len)
{
  int prevLen= row[yIndex]->len;
  row[yIndex]->s= (wchar_t *) realloc(row[yIndex]->s,sizeof(wchar_t)*(prevLen+len+1));
  row[yIndex]->len++;
  row[yIndex]->s[row[yIndex]->len]=L'\0';
  wmemcpy(&row[yIndex]->s[row[yIndex]->len-1], c, len);
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
void reservMemoryRow(int yIndex,int len)
{
  int prevLen= row[yIndex]->len;
  row[yIndex]->s= (wchar_t *) realloc(row[yIndex]->s,sizeof(wchar_t)*(prevLen+len+1));
  row[yIndex]->len++;
  row[yIndex]->s[row[yIndex]->len]=L'\0';
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
void shiftAndInserChar(wchar_t c ,int len, int yIndex,int dest, int origin,int moveCount)
{
  wmemmove(&row[yIndex]->s[dest],&row[yIndex]->s[origin] ,moveCount-1);
  wmemcpy(&row[yIndex]->s[origin], &c, len);
  row[yIndex]->s[row[yIndex]->len]=L'\0'; ///finalizamos la cadena
}

/*
  Nombre de la funcion : appendCharsRow()


 */

void appendCharsRow(const wchar_t *c, int x, int y, int len, int *capExceeded )
{
  if(rowisEmpty(row,y))
    {
      appendChar(c,y,len);
      cursor_x++;
      return;
    }
  x+=(ISROWMAIN(row,y))? 7 : 0;
  
  if(xIsEqualRowLen(row,x,y))
    {
      int prevLen= row[y]->len;
      appendChar(c,y,len);      
      cursor_x++;
      if((x+1) == WIDTH_TERMV)appendRow(L"",0);
      return;
    }  
  wchar_t  prevChar[2]={L'\0',L'\0'};
  wchar_t  currentChar[2]={L'\0',L'\0'};
  int xDest=0;
  int xOrigin=0;
  int size=0;
  for(int r= y; r< caprow; r++)
    {
      xDest   = ( x +1);
      xOrigin = x;
      size= ((row[r]->len - xOrigin));
      reservMemoryRow(r,len);
      if(prevChar[0]!= L'\0')
	{
	  currentChar[0] = row[r]->s[row[r]->len-1];	 
	  x=0;
	}
      if(row[r]->len > WIDTH_TERMV)
	{
	  row[r]->len--;
	  currentChar[0]= row[r]->s[row[r]->len-1];
	  prevChar[0]= (prevChar[0]== L'\0') ? *c : prevChar[0];
	  shiftAndInserChar(*prevChar,len,r,xDest,xOrigin,size);
	  prevChar[0]= currentChar[0];
	  x=0;
	  if((r+1)>(caprow-1))
	    {
	      appendRow(L"",0); // secrea una fila vacia
	      if(caprow >HEIGHT_TERMV+ofset_y)
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
	  wmemmove(&row[r]->s[xDest], &row[r]->s[xOrigin],size);
	  wmemcpy(&row[r]->s[xOrigin], &prevChar[0], len);
	  prevChar[0]= currentChar[0];
	  continue;
	}
    }
   cursor_x++;
}


int isYiRowisEmpty(int y)
{
  int f=0;
   if(row[y]->len == 0)
    {
      deleteRow(y);
      if((y-1) >= 0)
	{
	  if(isZero(y))
	    {
	      f=1;
	      goto is_zero;
	      
	    }
	  row[y-1]->s[row[y-1]->len - 1]=L'\0';
	  row[y-1]->len--;
	  f=1;
	}
    }
 is_zero:
   return f;
}
int isYiRowisMainAndLenisEqual7(int y, int x)
{
  return ((row[y]->rowType == ROW_MAIN)&&(x == OFSET_X));
}
int deleteOneChar(int y,int x)
{
  int f=0;
   if(x == row[y]->len)
    {
      if(isZero(row[y]->len))
	{
	  f= 1;
	  goto is_zero;
	}
      row[y]->s[row[y]->len-1]=L'\0';
      row[y]->len--;
      f=1;
    }
 is_zero:
   return f;
  
}

void delCharOfRow(int x, int y)
{

  if(isYiRowisEmpty(y))return;
  if(isYiRowisMainAndLenisEqual7(y,x))return;
  if(deleteOneChar(y,x))return;
  int xPrev = x;
  for(int r = y; r < caprow; r++ )
    {
      int xDest=0;
      int xOrigin=0;
      int size=0;
      wchar_t c;
      if((xPrev > 0) && (xPrev <= row[r]->len))
	{
	  xDest= (xPrev-1);
	  xOrigin= xPrev;
	  size= row[r]->len - xOrigin;
#define isYiRowHaveOnlyOneChar(_size)(_size == 0)
	  
	  if(isYiRowHaveOnlyOneChar(size))
	    {
	      deleteRow(r);
	      return;
	    }
#define hasYnextrow(_r,_cap)(((_r)+1) < (_cap))
	  // guardamos el ultimo caracter la siguiente fila
	  if(hasYnextrow(r,caprow))c = row[r+1]->s[0]; 
	  wmemmove(&row[r]->s[xDest], &row[r]->s[xOrigin],size);
	  
	  if(r < (caprow-1))
	    {
	      if(isZero(row[r]->len))return;
	      row[r]->s[row[r]->len-1]=c;
	    }
	  else
	    {
	      row[r]->len--;
	      row[r]->s[row[r]->len]=L'\0';
	      return;
	    }
	  xPrev=1;
	}
      else
	{
	  xDest   = xPrev;
	  xOrigin = xPrev+1;
	  size=(xOrigin == row[r]->len) ? 1 : (row[r]->len - xOrigin);
	  c = row[r]->s[0];
	  wmemmove(&row[r]->s[xDest], &row[r]->s[xOrigin],size);
	  if(r < (caprow))
	    {
	      if(isZero(r))return;
	      
	      row[r-1]->s[row[r-1]->len-1]=c;
	    }
	  
	  xPrev=0;
	}
    }
  if(isZero(caprow))return;
  row[caprow-1]->s[row[caprow-1]->len-1]=L'\0';
  row[caprow-1]->len--;
   if((row[caprow-1]->len ==0))deleteRow(caprow-1);  
}

void exit_app()
{
  Runnig=0; // se cierra la aplicacion
}
void term_readkey()
{
  /// devuelve el caracter leido  desde el teclado
  Nodo *inputbuffer= getInput();
  Nodo **buffercomand= &comand;
  if(inputbuffer == NULL) return; // la lista esta vacia
  while(inputbuffer)
    {
      wchar_t * str = (wchar_t *)inputbuffer->data;
      int len=sizeof(wchar_t)*2;
	      wchar_t up[2];
	      wchar_t down[2];
	      wchar_t right[2];
	      wchar_t left[2];
	      wchar_t back[5]={L'\x1b',L'[',L'3',L'~',L'\0'};
	      up[1]=L'\0';
	      down[1]=L'\0';
	      right[1]=L'\0';
	      left[1]=L'\0';
	      *up=ARROW_UP(0x5E00);
	      *down=ARROW_DOWN(0x5E00);
	      *right=ARROW_RIGHT(0x5E00);
	      *left=ARROW_LEFT(0x5E00);
      if(str[0] == L'\x1b')
	{
	  if(str[1]== L'[')
	    {
	      switch(str[2])
		{
		case L'A':insert_nodo_end(buffercomand,&up,len);break;
		case L'B':insert_nodo_end(buffercomand,&down,len); break;
		case L'C':insert_nodo_end(buffercomand,&right,len); break;
		case L'D':insert_nodo_end(buffercomand,&left,len); break;
		case L'3':
		  {
		    insert_nodo_end(buffercomand,&back,4);
		    //insert_nodo_end(buffercomand,&left,2*sizeof(wchar_t));
		  } break;
		}
	    }
	}
      else
	{
	  if((str[0] >= L'\a') && (str[0] <= L'\r'))
	    {
	      insert_nodo_end(buffercomand,&str[0],4);
	    }
	  else
	    {
	      int len= (wcslen(&str[0])+1)* sizeof(wchar_t);
	      switch(str[0])
		{
		case CTRL_KEY('i'):return;break;
		case CTRL_KEY('a'):return;break;
		case CTRL_KEY('f'):return;break;
		case CTRL_KEY('j'):return;break;
		  //case L'':return;break; ñ
		case CTRL_KEY('c'):return;break;
		case CTRL_KEY('v'):return;break;
		case CTRL_KEY('h'):return;break;
		case CTRL_KEY('q'):
		case CTRL_KEY('w'):
		case CTRL_KEY('e'):
		case CTRL_KEY('r'):
		case CTRL_KEY('t'):
		case CTRL_KEY('y'):
		case CTRL_KEY('u'):
		case CTRL_KEY('o'):
		case CTRL_KEY('p'):
		case CTRL_KEY('s'):
		case CTRL_KEY('d'):
		case CTRL_KEY('g'):
		case CTRL_KEY('k'):
		case CTRL_KEY('l'):
		case CTRL_KEY('z'):
		case CTRL_KEY('x'):
		case CTRL_KEY('b'):
		case CTRL_KEY('n'):
		  {
		    // comando correspondiente ctrl+ el alfabeto
		    // tienen que ser mappeados
		    insert_nodo_end(buffercomand,&str[0],len);
		    insert_nodo_end(buffercomand,&right,2*sizeof(wchar_t));
		    insert_nodo_end(buffercomand,&right,2*sizeof(wchar_t));
		  }break;
		default:
		  {
		    /// todo: aqui ingresan los caracteres al buffer de comandos
		    insert_nodo_end(buffercomand,&str[0],len);
		    // se agrega una secuencia para mover el cursor despues de cada
		    // ingreso de tecla
		    // insert_nodo_end(buffercomand,&right,2*sizeof(wchar_t));
		  }break;
		}
	    }
	}
      
      inputbuffer= inputbuffer->next;
    }
}

int isRowMain(y)
{
  return (row[y]->rowType == ROW_MAIN);
  
}
int getLastRowLen()
{
  return row[caprow-1]->len;
}
int isCommandValid()
{
  return 0;
}

/*permite mover el cursor*/

void term_move_cursor(wchar_t key)
{

  
  switch (key) {
  case ARROW_LEFT(0x5E00):
    {
      int  ofx = (row[(cursor_y+ofset_y)]->rowType == ROW_MAIN) ? OFSET_X : 0;
      if((cursor_x+ofx) != ofx ) 
	{
	  cursor_x--;
	}
      #if 0
      else
	{
	  if(cursor_y != 0)
	    {
	       cursor_y--;
	      
	    }
	  else
	    {
	      scrollUp(); // se hace escroll hacia arriba
        
	    }
	  if(row[cursor_y + ofset_y]->rowType == ROW_MAIN)
	    {
	      cursor_x= (WIDTH_TERMV - 8);
	    }
	  else
	    {
	      cursor_x= WIDTH_TERMV-1;
	    }
	}
      #else
      
      #endif
    } break;
  case ARROW_RIGHT(0x5E00):
    {
      #if 0
    if(cursor_x < width)cursor_x++; // modoel<
      #else
      //comand>as
      int  ofx = (row[(cursor_y+ofset_y)]->rowType == ROW_MAIN) ? OFSET_X : 0;
      if((cursor_x+ofx) != (row[cursor_y+ofset_y]->len))
	{
	  cursor_x++;
	}
      #endif
    } break;
  case ARROW_UP(0x5E00):
    {
    #if 0
    if((cursor_y) != 0)
      {
	 cursor_y--;
	 if(row[cursor_y+ofset_y]->rowType == ROW_MAIN)
	 cursor_x=row[cursor_y+ofset_y]->len - OFSET_X;
	 else cursor_x=row[cursor_y+ofset_y]->len;
      }
    else
      {
	 scrollUp();	
      }
    #else

    /// implementar ring buffer

    
    #endif
    }break;
    case ARROW_DOWN(0x5E00):
      {
	#if 0
	if((cursor_y) < (HEIGHT_TERMV-1)&&
	   ((cursor_y))< (caprow-1))
	{
	  cursor_y++;
	  if(row[cursor_y+ofset_y]->rowType == ROW_MAIN)
	    cursor_x=row[cursor_y+ofset_y]->len - OFSET_X;
	  else
	    cursor_x=row[cursor_y+ofset_y]->len;
	}
      else
	{
	  scrollDowWithCursor();
	}

	#else
	
	
	#endif
	
      } break;
  }
}
/*funcion utilizada para tokenizar los parametros */
int findLastRowmain()
{
  int position=0;
  for(int y= (cursor_y+ ofset_y); y > 0; y--)
    {
      if(row[y]->rowType== ROW_MAIN)
	{
	  position= y;
	  break;
	}
    }
  return position;
}
void extractCommand(wchar_t *ouput)
{
  //[cmd>hola mundo] len = 14
  wchar_t buffer[500]={0};
  int totlen=0;
  int posLastRowmain = findLastRowmain();
  totlen= ((row[posLastRowmain]->len-OFSET_X));
  wcsncat(buffer, &row[posLastRowmain]->s[OFSET_X], ((row[posLastRowmain]->len-OFSET_X))); 
  for(int x=(posLastRowmain+1); x < (caprow); x++)
    {
    wcsncat(buffer,&row[x]->s[0],(wcslen(row[x]->s)));
    totlen+= wcslen(row[x]->s);
    }
  memcpy(ouput, buffer,(totlen*sizeof(wchar_t)));
  ouput[totlen]=L'\0';
}

void term_process_keypress()
{
  /// procesa la tecla pulsada
  term_readkey(); // retorna la tecla  presionada
  int capRowExceded=0;
  Nodo *bufferComand = comand;
  wchar_t * resto=NULL; 
  while(bufferComand!=NULL)
    {
      wchar_t * str =(wchar_t *) bufferComand->data;
    
      switch(str[0])
	{
	case L'\x1b':
	  {
	    if(str[1] == L'[')
	      {
		int xof=0;
		if(row[(cursor_y+ofset_y)]->rowType == ROW_MAIN) xof=7;
		delCharOfRow(cursor_x+xof, (cursor_y+ofset_y));
		if((row[(cursor_y+ofset_y)]->rowType == ROW_MAIN)&&
		   cursor_x+xof > 7)
		  {
		    cursor_x--;
		  }
		else
		  {
		    if((row[(cursor_y+ofset_y)]->rowType != ROW_MAIN)&&
		       ((cursor_x+xof)!=0))
		      {
			cursor_x--;
		      }
		    else
		      {
		        if(cursor_y != 0)
			  {
			    cursor_y--;
			    if((row[(cursor_y+ofset_y)]->rowType == ROW_MAIN))
			    cursor_x= (WIDTH_TERMV-1)-7;
			    else cursor_x= (WIDTH_TERMV-1);
			      
			  }
		      }
		  }
		
	      }
	  }break;
	case CTRL_KEY('q'):
	  {
	    exit_app();
	  }break;
	case CTRL_KEY('w'):appendCharsRow(L"^W",cursor_x,cursor_y,2,NULL);break; 
	case CTRL_KEY('e'):appendCharsRow(L"^E",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('r'):appendCharsRow(L"^R",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('t'):appendCharsRow(L"^T",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('y'):appendCharsRow(L"^Y",cursor_x,cursor_y,2,NULL);break;
        case CTRL_KEY('u'):appendCharsRow(L"^U",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('o'):appendCharsRow(L"^O",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('p'):appendCharsRow(L"^P",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('s'):appendCharsRow(L"^S",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('d'):appendCharsRow(L"^D",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('g'):appendCharsRow(L"^G",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('k'):appendCharsRow(L"^K",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('l'):appendCharsRow(L"^L",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('z'):appendCharsRow(L"^Z",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('x'):appendCharsRow(L"^X",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('b'):appendCharsRow(L"^B",cursor_x,cursor_y,2,NULL);break; 
        case CTRL_KEY('n'):appendCharsRow(L"^N",cursor_x,cursor_y,2,NULL);break; 
	case ARROW_UP(0x5E00):
	case ARROW_DOWN(0x5E00):
	case ARROW_LEFT(0x5E00):
	case ARROW_RIGHT(0x5E00):
	  {
	    term_move_cursor(str[0]);
	  }break;
	case L'\n':
	  {
	    /// aumentar capacity row +1
#define onlyReturnifisCommandRow (isRowMain((cursor_y+ofset_y)) && (getLastRowLen() == OFSET_X))
	    
	    if(onlyReturnifisCommandRow)
	      {
		wchar_t * msg= L"comand>";
		if(!(scrollDown(HEIGHT_TERMV-1)))cursor_y++;
		//else //cursor_y= ofset_y;
		  
		appendRow(msg,wcslen(msg));
		
		row[cursor_y+ofset_y]->rowType = ROW_MAIN;

		//// crear una funcion para procesar los comandos de la terminal
		
	      }
	    else
	      {
		    wchar_t command[500];
		    extractCommand(command);
		    procesingCommands(command);

		    // luego de la ejecucion se reinicia la terminal
		    appendRow(L"comand>",7);
		    cursor_x=0;
		    if(!scrollDown(HEIGHT_TERMV))
		      {
			// crear una funcion que devuelva la posicion y
			// de la ultima fila
			cursor_y=(caprow-1);   
			
		      }
		    else
		      {
			 cursor_y= HEIGHT_TERMV -1;   
		          
		      }		   
		    row[cursor_y+ofset_y]->rowType = ROW_MAIN;
	      }
	       
	  }break;
	default:
	  // AQUI TENDRÍA QUE PROCESAR LAS TECLAS QUE SE SE VAN A MOSTRAR
	  // EN LA PANTALLA
          // arreglar para que funcionen las teclas espaciadoras
	  // y tambien para que funcionen las teclas wasd
	  appendCharsRow(&str[0],cursor_x,(cursor_y+ofset_y),1,&capRowExceded);

	   int  ox = (row[(cursor_y+ofset_y)]->rowType == ROW_MAIN) ? OFSET_X : 0;
      
     	
	   if((cursor_x + ox) >= (width) || (capRowExceded))
	    {
	       if(!(scrollDown(HEIGHT_TERMV)))
		{
		 cursor_y++; 
		}
	      else
		{
		  if((cursor_x + ox) != width)
		    {
		       if(cursor_y != 0)cursor_y--;
		    }
		  else
		    {
		      cursor_y = HEIGHT_TERMV-1;
		    }
		}
	       if(capRowExceded != 1)cursor_x=0;
	       capRowExceded=0;
	    } 
	  break;
	}
      // if(resto != NULL){ free(resto); resto=NULL;}
      bufferComand=(Nodo*) bufferComand->next;
    }
}

void debugBar()
{
     wchar_t buffer[80];
     appendBuffer(L"\x1b[25;0H",wcslen(L"\x1b[29;0H"));
     appendBuffer(L"\x1b[48;2;188;255;0m",wcslen(L"\x1b[48;2;128;128;0m"));
     appendBuffer(L"\x1b[60@",wcslen(L"\x1b[30@"));
     appendBuffer(L"\x1b[38;2;0;0;0m",wcslen(L"\x1b[38;2;0;0;0m"));
     swprintf(buffer,80,
	      L"[x = %d, y = %d]"
	      L" Row %d"
	      L" OfsetY %d"
	      L" Fps ",
	      cursor_x,
	      cursor_y,
	      caprow,
	      ofset_y);
     int len = wcslen(buffer);
   appendBuffer(buffer,len);
   appendBuffer(L"\x1b[0m",wcslen(L"\x1b[0m"));
}

void term_get_window_size()
{
  // obtienes el tamaño de la ventan
  //width =80;
  //height =20;
}
void term_drawrow()
{
  
  for(int r = (0+ofset_y); r < (caprow - ofsetrow); r++)
    {
       appendBuffer(row[r]->s,row[r]->len);
       appendBuffer(L"\n",1);     
    }
  debugBar();
}

 

void term_draw_cursor(HDC * memdc)
{
  
  wchar_t * arrGlifo= getGlifo();
  wchar_t g= arrGlifo[0];
  #if 1
  int ofsetx=0;
  if(row[(cursor_y+ofset_y)]->rowType == ROW_MAIN)
    ofsetx=7;
  else
    ofsetx=0;
  draw_font(memdc,(const wchar_t *)&g,cursor_x+ofsetx ,(cursor_y));
  #else
  draw_font(memdc,(const wchar_t *)&g,cursor_x,cursor_y);
  #endif
  
}

void  inicCursor00()
{
  wchar_t buffer[32];
  swprintf(buffer, 32 ,L"\x1b[%d;%dH",0,0);
  appendBuffer(buffer,wcslen(L"\x1b[%d;%dH")); 
  
}

void term_refreshScreen(Win32ScreenBuffer *Buffer)
{
 
  term_drawrow();
  inicCursor00(); // si no reinicias el cursor los cursores aumentaras indefinidamente
  write(&Buffer->BackBuffer);
  term_draw_cursor(&Buffer->BackBuffer);
  
}

void freeall()
{
  freeBuffer(); // libera el buffer principal
  free_lista(getRootInput());//liberar el inputbuffer
  free_lista(&comand);//libera el buffer de coandos
  //freeRow();// libera el burffer que almazena el texto principal
  freeAll();// libera todo por si acaso
 
}

void main_app(Win32ScreenBuffer *Buffer,KeyboardEvent *k)
{
 
  translate_keyevent_tovt_sequence(*k); //traduce los enventos del teclado a secuencia
  term_refreshScreen(Buffer);// dibuja el primer frame
  term_process_keypress();//captura los eventos del teclado
  freeall();
  
}

void inic_app()
{
  vEmu.vCursor=(POINT){0,0};
  vEmu.ofsetY=0;
  comand=NULL;
  width=WIDTH_TERMV;
  height=HEIGHT_TERMV;
  wchar_t * msg= L"comand>";
  appendRow(msg,wcslen(msg));
  
  row[0]->rowType = ROW_MAIN;
}
void end_app()
{

  
}

