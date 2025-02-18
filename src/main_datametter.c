#include <windows.h>
#include <stdio.h>
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\main_datametter.h"
#include "..\..\Vt\includes\row.h"
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
extern double avarageFps;
extern double avarageTimePerFrame;
#define MAX 100
wchar_t CurrentDirectory[MAX]={L'\0'};
size_t  lenDirectory=0;


//implementar el scroll vertical pra la consola
enum
  {
    EDIT,
    TERMINAL
  };
View vEmu;
#define WIDTH_TERMV  60 // ancho en caracteres de patalla
#define HEIGHT_TERMV 25 // alto en caracteres de pantalla
#define WIDTH_BUFFER 100; // maximo 100 caracteres  pero solo se vizualizarn 60
#define HEIGHT_BUFFER 100; //maximo 100 rows pero solo se vizualizaran 25
#define OFSET_WINDOW_AND_BUFFER (WIDTH_TERMV - WIDTH_BUFFER) // servira para hacer scroll 
#define comand vEmu.buffercomand
#define cursor_x vEmu.vCursor.x
#define cursor_y vEmu.vCursor.y
#define ppchar vEmu.pixelPerChar
#define ofset_y vEmu.ofsetY
#define ofset_x vEmu.ofsetX // ofset x para hacar escroll  horizontal
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
  if (caprow > (height + ofset_y))
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
  return (row[y].rowType == ROW_MAIN);
  
}
int getLastRowLen()
{
  return row[caprow-1].len;
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
      int  ofx = (row[(cursor_y+ofset_y)].rowType == ROW_MAIN) ? lenDirectory : 0;
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
	  if(row[cursor_y + ofset_y].rowType == ROW_MAIN)
	    {
	      cursor_x= (width - 8);
	    }
	  else
	    {
	      cursor_x= width-1;
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
      int  ofx = (row[(cursor_y+ofset_y)].rowType == ROW_MAIN) ? lenDirectory : 0;
      if((cursor_x+ofx) != (row[cursor_y+ofset_y].len))
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
	 if(row[cursor_y+ofset_y].rowType == ROW_MAIN)
	 cursor_x=row[cursor_y+ofset_y].len - lenDirectory;
	 else cursor_x=row[cursor_y+ofset_y].len;
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
	if((cursor_y) < (height-1)&&
	   ((cursor_y))< (caprow-1))
	{
	  cursor_y++;
	  if(row[cursor_y+ofset_y].rowType == ROW_MAIN)
	    cursor_x=row[cursor_y+ofset_y].len - lenDirectory;
	  else
	    cursor_x=row[cursor_y+ofset_y].len;
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
void extractCommand(wchar_t *ouput)
{
  //[cmd>hola mundo] len = 14
  wchar_t buffer[500]={0};
  int totlen=0;
  int posLastRowmain = findLastRowmain(&row,(cursor_y+ofset_y));
  totlen= ((row[posLastRowmain].len-lenDirectory));
  wcsncat(buffer, &row[posLastRowmain].s[lenDirectory], ((row[posLastRowmain].len-lenDirectory))); 
  for(int x=(posLastRowmain+1); x < (caprow); x++)
    {
    wcsncat(buffer,&row[x].s[0],(wcslen(row[x].s)));
    totlen+= wcslen(row[x].s);
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
		if(row[(cursor_y+ofset_y)].rowType == ROW_MAIN) xof=lenDirectory;
		delCharOfRow(&row,(cursor_x+xof), (cursor_y+ofset_y),&caprow);
		if((row[(cursor_y+ofset_y)].rowType == ROW_MAIN)&&
		   cursor_x+xof > lenDirectory)
		  {
		    cursor_x--;
		  }
		else
		  {
		    if((row[(cursor_y+ofset_y)].rowType != ROW_MAIN)&&
		       ((cursor_x+xof)!=0))
		      {
			cursor_x--;
		      }
		    else
		      {
		        if(cursor_y != 0)
			  {
			    cursor_y--;
			    if((row[(cursor_y+ofset_y)].rowType == ROW_MAIN))
			    cursor_x= (width-1)-7;
			    else cursor_x= (width-1);
			      
			  }
		      }
		  }
		
	      }
	  }break;
	  #include"ctrlk.c"
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
#define onlyReturnifisCommandRow (isRowMain((cursor_y+ofset_y)) && (getLastRowLen() == lenDirectory))
	    
	    if(onlyReturnifisCommandRow)
	      {
		
		if(!(scrollDown(height-1)))cursor_y++;
		//else //cursor_y= ofset_y;
		  
		appendRow(&row,CurrentDirectory,wcslen(CurrentDirectory),&caprow);
		
		row[cursor_y+ofset_y].rowType = ROW_MAIN;

		//// crear una funcion para procesar los comandos de la terminal
		
	      }
	    else
	      {
		    wchar_t command[500];
		    extractCommand(command);
		    procesingCommands(command);

		    // luego de la ejecucion se reinicia la terminal
		    appendRow(&row,CurrentDirectory,wcslen(CurrentDirectory),&caprow);
		    cursor_x=0;
		    if(!scrollDown(height))
		      {
			// crear una funcion que devuelva la posicion y
			// de la ultima fila
			cursor_y=(caprow-1);   
			
		      }
		    else
		      {
			 cursor_y= height -1;   
		          
		      }		   
		    row[cursor_y+ofset_y].rowType = ROW_MAIN;
	      }
	       
	  }break;
	default:
	  // AQUI TENDRÍA QUE PROCESAR LAS TECLAS QUE SE SE VAN A MOSTRAR
	  // EN LA PANTALLA
          // arreglar para que funcionen las teclas espaciadoras
	  // y tambien para que funcionen las teclas wasd
	  
	  appendCharsRow(&row,&str[0],
			 cursor_x,
			 (cursor_y+ofset_y),
			 ofset_y,
			 1,
			 &capRowExceded,
			 &caprow,
			 width,
			 height);
	  cursor_x++;

	   int  ox = (row[(cursor_y+ofset_y)].rowType == ROW_MAIN) ? lenDirectory : 0;	
	   if((cursor_x + ox) >= (width) || (capRowExceded))
	    {
	       if(!(scrollDown(height)))
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
		      cursor_y = height-1;
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
     wchar_t heightb[80];
     wchar_t widthb[80];
     swprintf(heightb,80,L"\x1b[%d;0H",(height));
     appendBuffer(heightb,wcslen(heightb));
     appendBuffer(L"\x1b[48;2;188;255;0m",wcslen(L"\x1b[48;2;128;128;0m"));
     swprintf(widthb,80,L"\x1b[%d@",width);
     appendBuffer(widthb,wcslen(widthb));
     appendBuffer(L"\x1b[38;2;0;0;0m",wcslen(L"\x1b[38;2;0;0;0m"));
     swprintf(buffer,80,
	      L"[x = %d, y = %d]"
	      L" Row %d"
	      L" OfsetY %d"
	      L" Fps %.2f s"
	      L" Avarage frame %.2f ms",
	      cursor_x,
	      cursor_y,
	      caprow,
	      ofset_y,
	      avarageFps,
	      avarageTimePerFrame);
     int len = wcslen(buffer);
   appendBuffer(buffer,len);
   appendBuffer(L"\x1b[0m",wcslen(L"\x1b[0m"));
}

void term_drawrow()
{
  
  for(int r = (0+ofset_y); r < (caprow - ofsetrow); r++)
    {
       appendBuffer(row[r].s,row[r].len);
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
  if(row[(cursor_y+ofset_y)].rowType == ROW_MAIN)
    ofsetx=lenDirectory;
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

void drawGrid(Win32ScreenBuffer *b)
{
  int widthCell      = 8;
  int heightCell     = 16;
  int pxWidth        = width * widthCell;
  int pxHeight       = height * heightCell;
  int maxVerticals   = (pxWidth/widthCell);
  int maxHorizontals = (pxHeight/heightCell);

  HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 120, 120)); // Línea roja de grosor 2
  SelectObject(b->BackBuffer, pen);

  for(int x=0; x <= maxVerticals; x++)
    {
      MoveToEx(b->BackBuffer,(x*widthCell), 0, NULL); // Punto inicial (x = 100, y = 50)
      LineTo(b->BackBuffer,(x*widthCell), pxHeight);        // Punto final (x = 100, y = 200)
    }
  for(int y=0; y <= maxHorizontals; y++ )
    {
      MoveToEx(b->BackBuffer,0, (y*heightCell), NULL); // Punto inicial (x = 100, y = 50)
      LineTo(b->BackBuffer,pxWidth,(y*heightCell));        // Punto final (x = 100, y = 200)
    }
  DeleteObject(pen);
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
  #if 1 
  drawGrid(Buffer);
  #endif
  translate_keyevent_tovt_sequence(*k); //traduce los enventos del teclado a secuencia
  term_refreshScreen(Buffer);// dibuja el primer frame
  term_process_keypress();//captura los eventos del teclado
  freeall();
}

/// acutaliza las dimensiones de la termina width y height
void updateDim(int w, int h)
{
  
  width  = (((w)/(ppchar/2))); 
  height =((((h)/(ppchar)))-1);
  wchar_t buffer[32];
  swprintf(buffer, 32 ,L"widht=%d height= %d\n" ,width,height);
  OutputDebugStringW(buffer);
  
}
//Todo: una funcion que te permita captura el directorio actual
//TODO: tambien necesitas corregir para poder cambiar de direcotorios con cd

void inic_app(Win32WindowDimension Dim)
{
  vEmu.vCursor=(POINT){0,0};
  vEmu.ofsetY=0;
  ppchar=16;  // 8 pixeles de ancho y 8 pixeles de alto
  comand=NULL;
  width=(((Dim.Width)/(ppchar/2)));
  height=(((Dim.Height)/(ppchar))-1);
  
  lenDirectory = GetCurrentDirectoryW(MAX,CurrentDirectory);
  CurrentDirectory[lenDirectory] = CurrentDirectory[lenDirectory+1];
  CurrentDirectory[lenDirectory] = L'>';
  lenDirectory++;
  appendRow(&row,CurrentDirectory,lenDirectory,&caprow);
  
  row[0].rowType = ROW_MAIN;
}
void end_app()
{

  
}

