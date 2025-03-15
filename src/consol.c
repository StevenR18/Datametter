#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\consol.h"
#include "..\..\Vt\includes\row.h"
#include "..\..\Vt\includes\font.h"
#include "..\..\Vt\includes\newwvt.h"
#include "..\..\Vt\includes\static_list.h"
#include "..\..\Vt\includes\vt100_commands.h"
#include "..\..\Vt\includes\data_structure.h"
#include "..\..\Vt\includes\scrollbar.h"
#include "..\..\Vt\includes\ringbuffer.h"
#include "..\..\Vt\includes\dbg.h"
#include "..\..\Vt\includes\process_keypress.h"
#include "..\..\Vt\includes\util_sprites.h"

#define MAX_LENDIRECTORY 100
#define WIDTH_TERMV  60 // ancho en caracteres de patalla
#define HEIGHT_TERMV 25 // alto en caracteres de pantalla
#define WIDTH_BUFFER 10 // maximo 100 caracteres  pero solo se vizualizarn 60
#define HEIGHT_BUFFER 5 //maximo 100 rows pero solo se vizualizaran 25
#define OFSET_WIDTHINCHAR 2
#define CTRL_KEY(ascii) ((ascii) & 0x1F)
#define ARROW_UP(s0)    (((s0) & 0x7700)) // Representa 'W'
#define ARROW_DOWN(s0)  (((s0) & 0x7300)) // Representa 'S'
#define ARROW_LEFT(s0)  (((s0) & 0x6100)) // Representa 'A'
#define ARROW_RIGHT(s0) (((s0) & 0x6400)) // Representa 'D'
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define ATCITIVE_WASD 0
#define ifYexceedTOrowcap(y) (( (y) +1 ) > (caprow - 1))

/////////////////////////////////////////////////////////////////////////////////////////////                                      VARIABLES EXTERNAS
extern unsigned char enableBlinkCursor; // se utiliza para el parapadeo del cursor

//////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////                                       VARIABLES GLOBALES
View vEmu;
//
//////////////////////////////////////////////////////////////////////////////////////////

// TODO: IMPLEMENTAR UNA FUNCION QUE TE PERMITA MOVER EL CURSOR DE FORMA LIBRE
//       CORREGIR  NO MUESTRA CORRECTAMENTE CARACTERES GRAFICOS
//       IMPLEMENTAR BARRA DE SCROLL VERTICAL
//	 IMPLEMENTA  UNA FORMA DE OCULTAR LAS BARRA DE ESCROLL CON ATAJO DE TECLAS
//       IMPLEMENTA EL BORRADO DE CARACTERES MANTENIENDO LA TECLA PRESIONADA

List * getListOfString()
{
  return (&vEmu.renderBuffer);
}

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  Nombre de la funcion: scrollUp()
  Descripcion: decremante en 1 el ofsetYTop
                    t0
                 Emu.arrRow
		     |
                     |
row ---------------------------------------
 1                                           
    ---------------------------------------
 2                                           (*ofsetYTop --)
    ---------------------------------------
 3                                           ofsetYTop
    ---------------------------------------
 4
    ---------------------------------------
 5
    ---------------------------------------
 6                                          
    ---------------------------------------
 7                                           vEmu.height
    ---------------------------------------
 8
    ---------------------------------------
 9
    ---------------------------------------
 10
    ---------------------------------------
 11
    ---------------------------------------
 12
    ---------------------------------------
 13                                         vEmu.capacityRow
    ---------------------------------------
*****************************************************************************************/
int scrollUp()
{
  int result=0;
  if((vEmu.capacityRow != 0) && (vEmu.ofsetYTop != 0 ))
    {
      vEmu.ofsetYTop--;
      // vEmu.ofsetYbottom++;
      result=1;
    }
  return result;
}

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  Nombre de la funcion: scrollDownWidthCursor()
  Descripcion: incrementa en  el ofsetYTop y ofsetYbottom
                    t0
                 Emu.arrRow                     height=3
		     |
                     |
row --------------------------------------- 
 1  x ...                                x  
    ---------------------------------------
 2  x ...                                x  
    ---------------------------------------                      -
 3  x ...                                x   (ofsetYTop =3)      | 3        
    ---------------------------------------                      
 4  x ...                                x                       |  (dy) >height
    ---------------------------------------                      
 5  x ...                                x                       |
    ---------------------------------------                                             
 6  x...                                 x   (ofsetYbottom =6)   | 6
    ---------------------------------------                      - 
 7  x...                                 x   (ofsetYTop =7)        
    ---------------------------------------        
 8  x..                                  x   
    ---------------------------------------
 9  x..                                  x   (ofsetYbottom-1)
    ---------------------------------------  
 10  x..                                  x  capacityRow = 10  ofsetYbottom=10   
    ---------------------------------------
*****************************************************************************************/

static int scrollDowWithCursor()
{
  int t = 0;
  int dy = vEmu.ofsetYbottom - vEmu.ofsetYTop;
  if (dy > vEmu.heightInChar-2
      /*vEmu.capacityRow > (vEmu.heightInChar + vEmu.ofsetYTop)*/)
  {
    vEmu.ofsetYTop++;
    vEmu.ofsetYbottom++;
    t = 1;
  }
  return t;
} // donde está el error

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  Nombre de la funcion: scrollDown()
  Descripcion: incrementa en dy el ofsetYTop y ofsetYbottom
                    t0
                 Emu.arrRow                  heiight= 3
		     |
                     |
row ---------------------------------------                - 
 1  x ...                                x  ofsetYTop=0    |     
    ---------------------------------------                
 2  x ...                                x                 |  dy=(ofsetYbottom-height)-1
    ---------------------------------------                
 3  x ...                                x                 |  dy= 3
    ---------------------------------------                
 4  x ...                                x  ofsetYTop+dy   |
    ---------------------------------------                -    
 5  x ...                                x                 |
    ---------------------------------------                
 6  x ...                                x                 |
    ---------------------------------------                
 7  x ...                                x   capacity = 7  |  ofsetYbottom 7
    ---------------------------------------                -
 8  x ...                                x                                      
    ---------------------------------------
 9  x ...                                x
    ---------------------------------------  
 10 x ..                                 x      
    ---------------------------------------

    
*****************************************************************************************/
int scrollDown()
{
  int f=0;
  int height = (vEmu.heightInChar-OFSET_WIDTHINCHAR);   
 
  if(((vEmu.ofsetYbottom + 1) - vEmu.ofsetYTop)> height
       /*vEmu.capacityRow > (vEmu.ofsetYTop+heightVt)*/)
    {
      // int dy=(vEmu.capacityRow - (heightVt+vEmu.ofsetYTop));
      int dy= ((vEmu.ofsetYbottom + 1)  - (height));
      vEmu.ofsetYTop=dy;
      //if(vEmu.ofsetYbottom !=0)vEmu.ofsetYbottom+=dy;
      f=1;
    }
  return f;
}

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  Nombre de la funcion exit_app()
  Descripcion hace que finalize la aplicacion 
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/
void exit_app()
{
  //GWindowPlatform.Running=0; // se cierra la aplicacion
  Window32SetRunning(0);
}


int isCommandValid()
{
  return 0;
}

/*permite mover el cursor*/

void term_move_cursor(wchar_t key)
{
  switch(vEmu.cursorType)
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
  int posLastRowmain = findLastRowmain(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop));
  
  totlen= ((ROW_LEN(vEmu.arrRow,posLastRowmain)-vEmu.lenDirectory));
  
  wcsncat(buffer,&(ROW_CHARAT(vEmu.arrRow,posLastRowmain,vEmu.lenDirectory)),
	  ((ROW_LEN(vEmu.arrRow,posLastRowmain)-vEmu.lenDirectory)));
  
  for(int x=(posLastRowmain+1); x < (vEmu.capacityRow); x++)
    {
      
      wcsncat(buffer,&ROW_CHARAT(vEmu.arrRow,x,0),(wcslen(ROW_STRING(vEmu.arrRow,x))));
      totlen+= wcslen(ROW_STRING(vEmu.arrRow,x));
    }
  memcpy(ouput, buffer,(totlen*sizeof(wchar_t)));
  ouput[totlen]=L'\0';
}

void debugBar()
{
  double avarageFps            = Window32GetAvarageFps();
  double avarageTimePerFrame   = Window32GetAvarageTimePerFrame();
     wchar_t buffer[150];
     wchar_t heightb[80];
     wchar_t widthb[80];
     int ofsetx=0;
     if(ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop)) == ROW_MAIN)
       ofsetx=vEmu.lenDirectory;
     
     swprintf(heightb,80,L"\x1b[%d;0H",(vEmu.heightInChar-OFSET_WIDTHINCHAR));
     appendBuffer(heightb,wcslen(heightb));
     appendBuffer(L"\x1b[48;2;188;255;0m",wcslen(L"\x1b[48;2;128;128;0m"));
     swprintf(widthb,80,L"\x1b[%d@",vEmu.widthInChar);
     appendBuffer(widthb,wcslen(widthb));
     appendBuffer(L"\x1b[38;2;0;0;0m",wcslen(L"\x1b[38;2;0;0;0m"));
     swprintf(buffer,150,
	      L"[x = %d, y = %d]"
	      L" Row %d"
	      L" OfsetX %d"
	      L" Fps %.2f s"
	      L" Avarage frame %.2f ms"
	      L" Width %d Height %d "
	      L" Capr %d "
	      L" ofsetBottom %d "
	      L" ofsetTop %d ",
	      vEmu.vCursor.x+vEmu.ofsetX+ofsetx,
	      vEmu.vCursor.y,
	      vEmu.capacityRow,
	      vEmu.ofsetX,
	      avarageFps,
	      avarageTimePerFrame,
	      vEmu.widthInChar,
	      vEmu.heightInChar-OFSET_WIDTHINCHAR,
	      vEmu.capacityRow,
	      vEmu.ofsetYbottom,
	      vEmu.ofsetYTop);
     
     int len = wcslen(buffer);
   appendBuffer(buffer,len);
   appendBuffer(L"\x1b[0m",wcslen(L"\x1b[0m"));
}

void term_drawrow()
{
  
  for(int r = (0+vEmu.ofsetYTop); r < (vEmu.ofsetYbottom); r++)
    {
      
       appendBuffer(ROW_STRING(vEmu.arrRow,r),ROW_LEN(vEmu.arrRow,r));
       appendBuffer(L"\n",1);     
    }
  debugBar();
}

void term_draw_cursor(HDC * memdc)
{
  
  wchar_t arrGlifo[2]={L'█',L'\0'};
  #if 1
  int ofsetx=0;
  if((ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop)) == ROW_MAIN))
    ofsetx=vEmu.lenDirectory;
  else
    ofsetx=0;
  draw_font(memdc,(const wchar_t *)arrGlifo,vEmu.vCursor.x+ofsetx ,(vEmu.vCursor.y));
  #else
  draw_font(memdc,(const wchar_t *)arrGlifo,vEmu.vCursor.x,vEmu.vCursor.y);
  #endif
  
}

void  inicCursor00()
{
  wchar_t buffer[32];
  swprintf(buffer, 32 ,L"\x1b[%d;%dH",0,0);
  appendBuffer(buffer,wcslen(L"\x1b[%d;%dH")); 
  
}

void term_refreshScreen(HDC * hdcTerm)
{
 
  term_drawrow(); // agrega las filas al buffer de filas
  inicCursor00(); // si no reinicias el cursor los cursores aumentaras indefinidamente
  write(hdcTerm,vEmu.ofsetX);// dibuja la filas en la pantalla
  term_draw_cursor(hdcTerm); // dibuja el cursor en la pantalla
  vEmu.Mario.vPosition.y=(vEmu.heightInChar-OFSET_WIDTHINCHAR)*16; 
  vEmu.Mario.vPosition.x =(vEmu.widthInChar-40)*8;
  moveSprite(&vEmu.Mario,10);
  drawSprite(&vEmu.Mario,hdcTerm); // dibuja el sprite de mario
  
  debugInputBuffer(hdcTerm,vEmu.Input,vEmu.widthInChar,vEmu.heightInChar,0.5); // depura la entrada del teclado
 
}
void freeall()
{
  // libera el buffer principal
  clsList(&vEmu.renderBuffer);
  //liberar el inputbuffer
  // clsList(&input);
  clsList(vEmu.Input); 
}


void clearFrame(HDC *hdcTerm, int wDest,int hDest)
{
  
  HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
  HBRUSH holdBrush = (HBRUSH) SelectObject(*hdcTerm, hBrush);
  PatBlt(*hdcTerm,0,0,wDest,hDest,PATCOPY);
  SelectObject(*hdcTerm,holdBrush);
  DeleteObject(hBrush);
}

void renderFrame(Win32ScreenBuffer *Buffer, HDC * hdcTerm,
		POINT positionDest)
{
  HDC hdcDest = Buffer->BackBuffer;
  HDC hdcSrc  = *hdcTerm;
  int wDest   = (vEmu.widthInChar*8);
  int hDest   = (vEmu.heightInChar*16);
  
  int ok = BitBlt(hdcDest,positionDest.x,positionDest.y,wDest,hDest,hdcSrc,0,0,SRCCOPY);

  if(!ok)
    {
      OutputDebugStringA("Error al renderizar la terminal virtual\n");
      return;
    }
   clearFrame(&hdcSrc,wDest,hDest);
  /// limpiar  la pantalla despues de renderizar
}

void simulate_term(Win32ScreenBuffer *Buffer ,KeyboardEvent *k)
{

  List *list = getInput();
  #if 1 
  drawGrid(&vEmu.hdcTerm,vEmu.widthInChar,vEmu.heightInChar);
  int widthBar = calculateScrollbarWidth(vEmu.widthInChar, vEmu.widthBufferInChar);
  drawHScrollBar(&vEmu.hdcTerm,vEmu.heightInChar-1,vEmu.ofsetX,widthBar,RGB(255,0,0),
		 vEmu.widthInChar,vEmu.widthBufferInChar);
  #endif
 
  //processInput(k,&input); //traduce los enventos del teclado a secuencia
  processInput(k,list); //traduce los enventos del teclado a secuencia
  term_refreshScreen(&vEmu.hdcTerm);// dibuja el primer frame
  renderFrame(Buffer,&vEmu.hdcTerm,vEmu.positionTerm);
  //proceesKeyPress(&input);// procesa la entrada
  proceesKeyPress(list);// procesa la entrada
  freeall();
}

/// acutaliza las dimensiones de la termina width y height
void updateDimBitmap(int widthInPixel, int heightInPixel,HDC *backBuffer)
{
  if(vEmu.hdcTerm == NULL) vEmu.hdcTerm  = CreateCompatibleDC(*backBuffer);
  
  switch(vEmu.layoutType)
    {
    case LAYOUT_DEFECT:
      {
        vEmu.positionTerm.y=0;
        vEmu.positionTerm.x=0;
	vEmu.widthBitmapTerm  = (((float)(widthInPixel)/(vEmu.pixelPerChar/2)));
	vEmu.heightBitmapTerm = ((float)(heightInPixel)/(float)(vEmu.pixelPerChar));
	vEmu.widthInChar  = (int)vEmu.widthBitmapTerm;
        vEmu.heightInChar = (int)vEmu.heightBitmapTerm;
	vEmu.widthBufferInChar= vEmu.widthInChar + WIDTH_BUFFER;
	vEmu.heightBufferInChar = vEmu.heightInChar + HEIGHT_BUFFER;
	wchar_t buffer[32];
	swprintf(buffer, 32 ,L"widht=%d height= %d\n" ,vEmu.widthInChar,vEmu.heightInChar);
	OutputDebugStringW(buffer);
      }
      break;
    case LAYOUT_2:
	{
	  vEmu.positionTerm.y=0;
	  vEmu.positionTerm.x=0;
	  vEmu.widthBitmapTerm  = ((((float)widthInPixel/2.0f)/(vEmu.pixelPerChar/2)));
	  vEmu.heightBitmapTerm = ((float)heightInPixel)/(vEmu.pixelPerChar);
	  vEmu.widthInChar  = (int)vEmu.widthBitmapTerm;
	  vEmu.heightInChar = (int)vEmu.heightBitmapTerm;
	  vEmu.widthBufferInChar= vEmu.widthInChar + WIDTH_BUFFER;
	  vEmu.heightBufferInChar = vEmu.heightInChar + HEIGHT_BUFFER;
	  wchar_t buffer[32];
	  swprintf(buffer, 32 ,L"widht=%d height= %d\n" ,vEmu.widthInChar,vEmu.heightInChar);
	  OutputDebugStringW(buffer);
	} break;
    case LAYOUT_3:
	{
	  /// reposicionar la ventana a la parte de abajo
	  vEmu.widthBitmapTerm  =((((float)widthInPixel)/((float)vEmu.pixelPerChar/2.0f)));;
	  vEmu.heightBitmapTerm = ((((((float)heightInPixel/3.0f)/(vEmu.pixelPerChar)))-1)-1);
	  vEmu.widthInChar  = (int)vEmu.widthBitmapTerm;
	  vEmu.heightInChar = (int)vEmu.heightBitmapTerm;
	  vEmu.widthBufferInChar= vEmu.widthInChar + WIDTH_BUFFER;
	  vEmu.heightBufferInChar = vEmu.heightInChar + HEIGHT_BUFFER;
	  vEmu.positionTerm.y= heightInPixel-(heightInPixel/3.0);
	  vEmu.positionTerm.x=0;
	  wchar_t buffer[32];
	  swprintf(buffer, 32 ,L"widht=%d height= %d\n" ,vEmu.widthInChar,vEmu.heightInChar);
	  OutputDebugStringW(buffer);
	} break;
	
    }
  if(vEmu.hBitmapTerm)
    {
      SelectObject(vEmu.hdcTerm,vEmu.holdBitmap);
      DeleteObject(vEmu.hBitmapTerm);
    }
  vEmu.hBitmapTerm =(HBITMAP) CreateCompatibleBitmap(*backBuffer,
						     (int)(vEmu.widthBitmapTerm*8),
						     (int)(vEmu.heightBitmapTerm*16));
  vEmu.holdBitmap   = SelectObject(vEmu.hdcTerm,vEmu.hBitmapTerm);
}


void modSerialTermiminal()
{
  freeRow(vEmu.arrRow,&vEmu.capacityRow,&vEmu.ofsetYTop,&vEmu.ofsetYbottom);
  vEmu.vCursor.y=0;
  vEmu.vCursor.x=0;
  wmemset(vEmu.currentDirectory, L'\0', MAX_LENDIRECTORY);
  vEmu.lenDirectory=0;
  appendRow(vEmu.arrRow,vEmu.currentDirectory,vEmu.lenDirectory,&vEmu.capacityRow);
}

void exitModSerialTerminal()
{
  vEmu.lenDirectory = GetCurrentDirectoryW(MAX_LENDIRECTORY,vEmu.currentDirectory);
  vEmu.currentDirectory[vEmu.lenDirectory] = vEmu.currentDirectory[vEmu.lenDirectory+1];
  vEmu.currentDirectory[vEmu.lenDirectory] = L'>';
  vEmu.lenDirectory++;
  appendCharToRow(vEmu.arrRow,vEmu.currentDirectory,0,vEmu.lenDirectory);
  ROW_TYPE(vEmu.arrRow,0) = ROW_MAIN;
}


void inic_term(Win32WindowDimension Dim)
{
  //inicRingBuffer(&ring);
  vEmu.arrRow = &vEmu.row; // puntero al array de filas
  vEmu.vCursor=(POINT){0,0};
  vEmu.cursorType = MOD_TERM_CURSOR; // cursor modo terminal
  vEmu.layoutType = LAYOUT_DEFECT;
  vEmu.ofsetYTop=0;
  vEmu.ofsetYbottom =0;
  vEmu.pixelPerChar=16;  // 8 pixeles de ancho y 8 pixeles de alto
  inicInputBuffer(255); // creacion del buffer de imput
  vEmu.renderBuffer.head = createList(1000); // creacion del buffer string buffer principal
  vEmu.Input= getInput();
  
  vEmu.widthInChar=(((Dim.Width)/(vEmu.pixelPerChar/2)));
  vEmu.heightInChar=((((Dim.Height)/(vEmu.pixelPerChar))-1)-1);
  vEmu.widthBufferInChar= vEmu.widthInChar + WIDTH_BUFFER; 
  vEmu.heightBufferInChar = vEmu.heightInChar + HEIGHT_BUFFER;  
  vEmu.lenDirectory = GetCurrentDirectoryW(MAX_LENDIRECTORY,vEmu.currentDirectory);
  vEmu.currentDirectory[vEmu.lenDirectory] = vEmu.currentDirectory[vEmu.lenDirectory+1];
  vEmu.currentDirectory[vEmu.lenDirectory] = L'>';
  vEmu.lenDirectory++;
  appendRow(vEmu.arrRow,vEmu.currentDirectory,vEmu.lenDirectory,&vEmu.capacityRow);
  ROW_TYPE(vEmu.arrRow,0) = ROW_MAIN;
  vEmu.hdcTerm=NULL;
  vEmu.hBitmapTerm=NULL;
  
  // cargar el sprite de mario
  vEmu.Mario.anime[0] = loadBmp("E:\\Vt\\sprites\\mario\\mario.bmp");
  vEmu.Mario.anime[1] = loadBmp("E:\\Vt\\sprites\\mario\\mario2_256.bmp");
  vEmu.Mario.anime[2] = loadBmp("E:\\Vt\\sprites\\mario\\mario3_256.bmp");
  vEmu.Mario.anime[3] = loadBmp("E:\\Vt\\sprites\\mario\\mario4_256.bmp");
  vEmu.Mario.mask[0] = loadBmp("E:\\Vt\\sprites\\mario\\mario_mask.bmp");
  vEmu.Mario.mask[1] = loadBmp("E:\\Vt\\sprites\\mario\\mario2_256_mask.bmp");
  vEmu.Mario.mask[2] = loadBmp("E:\\Vt\\sprites\\mario\\mario3_256_mask.bmp");
  vEmu.Mario.mask[3] = loadBmp("E:\\Vt\\sprites\\mario\\mario4_256_mask.bmp");
  vEmu.Mario.currentFrame=1;
  vEmu.Mario.vPosition.x=0;
  vEmu.Mario.vPosition.y=0;
  vEmu.Mario.vVelocity.x=0;
  
}
void end_simulate()
{

  
}

