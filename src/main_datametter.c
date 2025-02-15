#include <windows.h>
#include <stdio.h>
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\main_datametter.h"
#include "..\..\Vt\includes\font.h"
#include "..\..\Vt\includes\newwvt.h"
#include "..\..\Vt\includes\liked_list.h"
#include "..\..\Vt\includes\memory_traker.h"
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

typedef struct
{
  wchar_t *s;
  int len;
  int rowType;
}Row;

typedef struct
{
  POINT vCursor;
  Nodo *buffercomand;
  int  width;
  int  height;
  Row **row; // array del buffe principal
  int capRow;
  int ofsetY; // se utiliza para hacer escroll vertical
  int ofsetX; // se utilzia pra hacer escroll horizontal
  int ofsetCapRow;
}View;

#define WIDTH_TERMV  10
#define HEIGHT_TERMV 3

View vEmu;

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
int scrollDown(int heightVt)
{
  int f=0;
  if(caprow > heightVt+ofset_y)
    {
      ofset_y++;
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
}
#define isZero(x)((x) <= 0)
int absoluteValue(int x)
{
  if(x < 0) return(x*-1);
  return x;
}


/// funcion  agrea un caracter la final  de cada row
//Capacity exceeded

void appendCharsRow(const wchar_t *c, int x, int y, int len, int *capExceeded )
{
  if(row[y]->len == 0)
    {
      row[y]->s= (wchar_t *) realloc(row[y]->s,sizeof(wchar_t)*(len+1));
      row[y]->len++;
      row[y]->s[row[y]->len]=L'\0';
      wmemcpy(&row[y]->s[0], c, len);
      cursor_x++;
      return;
    }
  
  if(row[y]->rowType == ROW_MAIN)x+=7;
  if( (x  ==  row[y]->len))
    {
      int prevLen= row[y]->len;
      row[y]->s = (wchar_t *) realloc(row[y]->s,sizeof(wchar_t)*(prevLen+len+1));
      row[y]->len++;
      row[y]->s[row[y]->len]=L'\0';
      wmemcpy(&row[y]->s[row[y]->len - 1], c, len);
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
      int prevLen= row[r]->len;
      row[r]->s=(wchar_t *)realloc(row[r]->s,sizeof(wchar_t)*(prevLen + len+ 1));
      row[r]->len++;
      row[r]->s[row[r]->len]=L'\0';
      if(prevChar[0]!= L'\0')
	{
	  currentChar[0] = row[r]->s[row[r]->len-1];
	  // wmemmove(&row[r]->s[xDest],&row[r]->s[xOrigin] , size);
	  //wmemcpy(&row[r]->s[xOrigin], &prevChar[0], len);
	  // row[r]->s[row[r]->len]='\0';
	 
	  x=0;
	  // continue;
	  // cursor_y++;
	}
      if(row[r]->len > WIDTH_TERMV)
	{
	  row[r]->len--;
	  currentChar[0]= row[r]->s[row[r]->len-1];
	  wmemmove(&row[r]->s[xDest],&row[r]->s[xOrigin] , size-1);
	  wmemcpy(&row[r]->s[xOrigin], c, len);
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
	  if(currentChar[0] == L'\0') c= &prevChar[0];
	  if(size == 0) size=1;
	  wmemmove(&row[r]->s[xDest],&row[r]->s[xOrigin] , size);
	  wmemcpy(&row[r]->s[xOrigin], c, len);
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
      if((cursor_x+ofx) != ofx)
	{
	  cursor_x--;
	}
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
    } break;
  case ARROW_RIGHT(0x5E00):
    {
      #if 0
    if(cursor_x < width)cursor_x++; // modoel<
      #else
      //comand>as
      int  ofx = (row[(cursor_y+ofset_y)]->rowType == ROW_MAIN) ? OFSET_X : 0;
       if((cursor_x+ofx) != WIDTH_TERMV-1)
	{
	  cursor_x++;
	}
      else
	{
	  if(cursor_y != HEIGHT_TERMV-1)
	    {
	       cursor_y++;
	      
	    }
	  else
	    {
	      scrollDown(HEIGHT_TERMV);
        
	    }
	  if(row[cursor_y + ofset_y]->rowType == ROW_MAIN)
	    {
	      cursor_x= (WIDTH_TERMV - 8);
	    }
	  else
	    {
	      cursor_x=0;
	    }
	}
      
      #endif
    } break;
  case ARROW_UP(0x5E00):
    {
    
    if((cursor_y) != 0)
      {
	 cursor_y--;
      }
    else
      {
	 scrollUp();	
      }
    }break;
    case ARROW_DOWN(0x5E00):
      {
	if((cursor_y) < (HEIGHT_TERMV-1)&&
	   ((cursor_y))< (caprow-1))
	{
	  cursor_y++;
	}
      else
	{
	   scrollDown(HEIGHT_TERMV);
	}
	
      } break;
  }
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
		if(!(scrollDown(HEIGHT_TERMV)))cursor_y++;
		//else //cursor_y= ofset_y;
		  
		appendRow(msg,wcslen(msg));
		
		row[cursor_y+ofset_y]->rowType = ROW_MAIN;

		//// crear una funcion para procesar los comandos de la terminal
		
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
		  if(cursor_y != 0)cursor_y--;
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

