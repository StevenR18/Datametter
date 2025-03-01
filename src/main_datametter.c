#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\main_datametter.h"
#include "..\..\Vt\includes\row.h"
#include "..\..\Vt\includes\font.h"
#include "..\..\Vt\includes\newwvt.h"
#include "..\..\Vt\includes\static_list.h"
#include "..\..\Vt\includes\vt100_commands.h"
#include "..\..\Vt\includes\data_structure.h"
#include "..\..\Vt\includes\scrollbar.h"
#include "..\..\Vt\includes\ringbuffer.h"
#include "..\..\Vt\includes\dbg.h"
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
extern List listOfStrings; 
#define MAX 100
wchar_t CurrentDirectory[MAX]={L'\0'};
size_t  lenDirectory=0;


//implementar el scroll vertical pra la consola


View vEmu;
#define WIDTH_TERMV  60 // ancho en caracteres de patalla
#define HEIGHT_TERMV 25 // alto en caracteres de pantalla
#define WIDTH_BUFFER 10 // maximo 100 caracteres  pero solo se vizualizarn 60
#define HEIGHT_BUFFER 5 //maximo 100 rows pero solo se vizualizaran 25
#define stri vEmu.listOfStrings // lista statica de  strings
#define cursor_x vEmu.vCursor.x
#define cursor_y vEmu.vCursor.y
#define MODO_CURSOR vEmu.cursorMod
#define ppchar vEmu.pixelPerChar
#define ofset_y vEmu.ofsetY
#define ofset_x vEmu.ofsetX // ofset x para hacar escroll  horizontal
#define Cursor  vEmu.vCursor
#define width  vEmu.width
#define height vEmu.height
#define wBuffer vEmu.widthBuffer
#define hBuffer vEmu.heightBuffer
#define row vEmu.row // Row*
#define caprow vEmu.capRow
#define ofsetrow vEmu.ofsetCapRow
ArrRow arrRow=NULL; //array de Row
RingBuffer ring;

// TODO: IMPLEMENTAR UNA FUNCION QUE TE PERMITA MOVER EL CURSOR DE FORMA LIBRE
//       CORREGIR  NO MUESTRA CORRECTAMENTE CARACTERES GRAFICOS
//       IMPLEMENTAR BARRA DE SCROLL VERTICAL
//	 IMPLEMENTA  UNA FORMA DE OCULTAR LAS BARRA DE ESCROLL CON ATAJO DE TECLAS
//       IMPLEMENTA EL BORRADO DE CARACTERES MANTENIENDO LA TECLA PRESIONADA


List * getListOfString()
{
  return (&stri);
}
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
  switch(MODO_CURSOR)
    {
    case MOD_FREE_CURSOR:
      {
	#include "free_cursor.c"	
      }break;

      case MOD_TERM_CURSOR:
      {
	#include "term_cursor.c"
	
      }break;
      
    }
}

/*funcion utilizada para tokenizar los parametros */
void extractCommand(wchar_t *ouput)
{
  //[cmd>hola mundo] len = 14
  wchar_t buffer[500]={0};
  int totlen=0;
  int posLastRowmain = findLastRowmain(arrRow,(cursor_y+ofset_y));
  
  totlen= ((ROW_LEN(arrRow,posLastRowmain)-lenDirectory));
  
  wcsncat(buffer,&(ROW_CHARAT(arrRow,posLastRowmain,lenDirectory)),
	  ((ROW_LEN(arrRow,posLastRowmain)-lenDirectory)));
  
  for(int x=(posLastRowmain+1); x < (caprow); x++)
    {
      
      wcsncat(buffer,&ROW_CHARAT(arrRow,x,0),(wcslen(ROW_STRING(arrRow,x))));
      totlen+= wcslen(ROW_STRING(arrRow,x));
    }
  memcpy(ouput, buffer,(totlen*sizeof(wchar_t)));
  ouput[totlen]=L'\0';
}

// funcion para procesesar los eventos del teclado
#include "process_keypress.c"

void debugBar()
{
     wchar_t buffer[150];
     wchar_t heightb[80];
     wchar_t widthb[80];
     int ofsetx=0;
     if(ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN)
       ofsetx=lenDirectory;
     
     swprintf(heightb,80,L"\x1b[%d;0H",(height));
     appendBuffer(heightb,wcslen(heightb));
     appendBuffer(L"\x1b[48;2;188;255;0m",wcslen(L"\x1b[48;2;128;128;0m"));
     swprintf(widthb,80,L"\x1b[%d@",width);
     appendBuffer(widthb,wcslen(widthb));
     appendBuffer(L"\x1b[38;2;0;0;0m",wcslen(L"\x1b[38;2;0;0;0m"));
     swprintf(buffer,150,
	      L"[x = %d, y = %d]"
	      L" Row %d"
	      L" OfsetX %d"
	      L" Fps %.2f s"
	      L" Avarage frame %.2f ms"
	      L" Width %d Height %d ",
	      cursor_x+ofset_x+ofsetx,
	      cursor_y,
	      caprow,
	      ofset_x,
	      avarageFps,
	      avarageTimePerFrame,
	      width,
	      height);
     int len = wcslen(buffer);
   appendBuffer(buffer,len);
   appendBuffer(L"\x1b[0m",wcslen(L"\x1b[0m"));
}

void term_drawrow()
{
  
  for(int r = (0+ofset_y); r < (caprow - ofsetrow); r++)
    {
      
       appendBuffer(ROW_STRING(arrRow,r),ROW_LEN(arrRow,r));
       appendBuffer(L"\n",1);     
    }
  debugBar();
}

void term_draw_cursor(HDC * memdc)
{
  
  wchar_t arrGlifo[2]={L'█',L'\0'};
  #if 1
  int ofsetx=0;
  if(ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN)
    ofsetx=lenDirectory;
  else
    ofsetx=0;
  draw_font(memdc,(const wchar_t *)arrGlifo,cursor_x+ofsetx ,(cursor_y));
  #else
  draw_font(memdc,(const wchar_t *)arrGlifo,cursor_x,cursor_y);
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
  write(&Buffer->BackBuffer,ofset_x);
  term_draw_cursor(&Buffer->BackBuffer);
  debugInputBuffer(Buffer,&input,width,height,0.5);
 
}

void freeall()
{
  clsList(&stri);
  clsList(&input);//liberar el inputbuffer
  
}

void main_app(Win32ScreenBuffer *Buffer,KeyboardEvent *k)
{
  
  #if 1 
  // drawGrid(Buffer,width,height);
  int widthBar = calculateScrollbarWidth(width, wBuffer);
  drawHScrollBar(Buffer,height+1,ofset_x,widthBar,RGB(255,0,0),width,wBuffer);
  #endif
 
  processInput(k,&input); //traduce los enventos del teclado a secuencia
  term_refreshScreen(Buffer);// dibuja el primer frame
  term_process_keypress(&input);//captura los eventos del teclado
  freeall();
}

/// acutaliza las dimensiones de la termina width y height
void updateDim(int w, int h)
{
  
  width  = (((w)/(ppchar/2))); 
  height =(((((h)/(ppchar)))-1)-1);
  wBuffer= width + WIDTH_BUFFER; 
  hBuffer = height + HEIGHT_BUFFER;
  
  wchar_t buffer[32];
  swprintf(buffer, 32 ,L"widht=%d height= %d\n" ,width,height);
  OutputDebugStringW(buffer);
  
}
//Todo: una funcion que te permita captura el directorio actual
//TODO: tambien necesitas corregir para poder cambiar de direcotorios con cd

void inic_app(Win32WindowDimension Dim)
{
  inicRingBuffer(&ring);
  arrRow = &row;
  vEmu.vCursor=(POINT){0,0};
  vEmu.cursorMod = MOD_TERM_CURSOR; // cursor modo terminal
  vEmu.ofsetY=0;
  ppchar=16;  // 8 pixeles de ancho y 8 pixeles de alto
  input.head = createList(255); // creacion del buffer de imput
  stri.head = createList(1000); // creacion del buffer string buffer principal
  width=(((Dim.Width)/(ppchar/2)));
  height=((((Dim.Height)/(ppchar))-1)-1);
  wBuffer= width + WIDTH_BUFFER; 
  hBuffer = height + HEIGHT_BUFFER;
  
  
  lenDirectory = GetCurrentDirectoryW(MAX,CurrentDirectory);
  CurrentDirectory[lenDirectory] = CurrentDirectory[lenDirectory+1];
  CurrentDirectory[lenDirectory] = L'>';
  lenDirectory++;
  appendRow(arrRow,CurrentDirectory,lenDirectory,&caprow);
  ROW_TYPE(arrRow,0) = ROW_MAIN;
  
}
void end_app()
{

  
}

