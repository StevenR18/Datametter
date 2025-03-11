#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <stdbool.h>
#define STRUCT_INIC(W) memset((char *)(W),0,sizeof((*W)))
#define CLASS_NAME "Class_Window64"
#define WINDOW_NAME "DattaMetter" 
static HINSTANCE GHInst;
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\keyboard.h"
#include "..\..\Vt\includes\main_datametter.h"
////////////////////////////////////////////////////////////////////////////////////////
//                                VARAIBLES GLOBALES
Win32ScreenBuffer GBuffer;
Win32Platform GWindowPlatform = {.primaryHdc=NULL,
				 .Window=NULL,
				 .Running=1,
				 .avarageFps=0.0,
				 .avarageTimePerFrame=0.0,
				 .sumdt=0.0,
				 .frames=0};

KeyboardEvent keyEvent;

//////////////////////////////////////////////////////////////////////////////////////////

#if 1
extern unsigned char enableBlinkCursor;
#define ID_TIMER_BLINK_CURSOR 1 // timer para habilitar el parpadeo del cursor
#endif

/*********************GETER*******************************************************/
Win32ScreenBuffer * getWindow32ScreenBuffer()
{
  return &GBuffer;
}
/**************************************************************************************/



void clear_screen_Patblt(HDC hdc,int width, int height, COLORREF color)
{

  HBRUSH hBrush = CreateSolidBrush(color);
  HBRUSH holdBrush = (HBRUSH) SelectObject(hdc, hBrush);
  PatBlt(hdc,0,0,width,height,PATCOPY);
  SelectObject(hdc,holdBrush);
  DeleteObject(hBrush);
}

void Win32RenderBuffer(HDC Hdc,
		      int WindowWidth,
		      int WindowHeight,
		      Win32ScreenBuffer *Buffer)
{

  HDC BackBuffer = Buffer->BackBuffer;
  
  

 

  StretchBlt(Hdc,0,0,WindowWidth,WindowHeight,BackBuffer,0,0,WindowWidth,WindowHeight,SRCCOPY);
  clear_screen_Patblt(BackBuffer,WindowWidth, WindowHeight, RGB(0,0,0));
 
}



void Win32ResizeDibSection(Win32ScreenBuffer *Buffer,
			   int Width,
			   int Height)
{
  
  Buffer->Width=Width;
  Buffer->Height=Height;
 
 
 
  Buffer->BackBuffer=CreateCompatibleDC(GWindowPlatform.primaryHdc);
  Buffer->Hbitmap= CreateCompatibleBitmap(GWindowPlatform.primaryHdc,Width,Height);
  Buffer->HoldBitmap=(HBITMAP) SelectObject(Buffer->BackBuffer,Buffer->Hbitmap);
  updateDim(Width,Height,&Buffer->BackBuffer); 
 
}

Win32WindowDimension Win32GetWindowDimension(HWND Window )
{
  Win32WindowDimension Result;
  RECT ClientRect;
  GetClientRect(Window,&ClientRect);
  Result.Width=ClientRect.right - ClientRect.left;
  Result.Height=ClientRect.bottom -ClientRect.top;
  return Result;
}

/*****************************PROCEDIMIENTO DE LA VENTANA*******************************/
static int firstKey = 0;
static int  cEvent=0;
LRESULT CALLBACK MainWndProc(
    HWND HWindow,        // handle to window
    UINT UMsg,        // message identifier
    WPARAM WParam,    // first message parameter
    LPARAM LParam)    // second message parameter
{
   LRESULT Result=0;
  switch(UMsg)
    {
    case WM_CREATE:
      {
	 OutputDebugStringA("WM_CREATE\n");
	
	 
      }break;
    case WM_TIMER:
      {
	#if 1
	if(WParam == ID_TIMER_BLINK_CURSOR)
	  {
	    //printf("TIMER\n");
	    
	    if(enableBlinkCursor == 0)
	       enableBlinkCursor=1;
	    else
	      enableBlinkCursor=0;
	  }
	#endif
      }break;
    case WM_PAINT:
      {
	
	/*Este es el sgundo mensaje que se procesa*/
	 OutputDebugStringA("WM_PAINT\n");
	 /*se deve procesar el mensaje por lo menos una vez
	  para indicarle a windows que la patalla a sido pintada
	 y no vuelva a pedir que se pinte*/
         PAINTSTRUCT Paintstr; // region que necesita ser redibujada
	 GWindowPlatform.primaryHdc= BeginPaint(HWindow,&Paintstr); // devuelve un hdc con la region a acutaliar
	  Win32WindowDimension Dimension=Win32GetWindowDimension(HWindow);
	  Win32ResizeDibSection(&GBuffer,Dimension.Width,Dimension.Height);
	  
	  Win32RenderBuffer(GWindowPlatform.primaryHdc,
			   Dimension.Width,
		           Dimension.Height,
			   &GBuffer);	 
	 EndPaint(HWindow, &Paintstr); // libera el contexto y valida la region apintar
	                            // esto hace que no se envie mensajes del WM_PAINT
                                    // de forma indefinida
      }break;
    case WM_SIZE:
      {
	 OutputDebugStringA("WM_SIZE\n");
	  
	
      }break;
    case WM_DESTROY:
      {
	OutputDebugStringA("WM_DESTROY.\n");
	PostQuitMessage(0);	
      }break;
    default:
      Result=DefWindowProc(HWindow,UMsg,WParam,LParam);
    }
 
return Result;
 
}

void resetKeyBoard()
{
  memset(keyEvent.buttons, 0, sizeof(keyEvent.buttons));

}

/***************************************************************************************/
int WINAPI WinMain(HINSTANCE HInst,
		   HINSTANCE HInstPrev,
		   PSTR Pszcmdline,
		   int Ncmdshow)
{

  GHInst= HInst;
  //Win32ResizeDibSection(&GBuffer,1280,720);
  WNDCLASSEXA WindowClass;
  STRUCT_INIC(&WindowClass);
  WindowClass.cbSize= sizeof(WNDCLASSEXA);
  WindowClass.style=CS_HREDRAW|CS_OWNDC|CS_VREDRAW;
  WindowClass.lpfnWndProc=MainWndProc;
  WindowClass.hInstance= HInst;
  WindowClass.hIcon=NULL;
  WindowClass.hCursor=NULL;
  WindowClass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
  WindowClass.lpszMenuName=NULL;
  WindowClass.lpszClassName=CLASS_NAME;
  WindowClass.hIconSm=NULL;

  

  if(RegisterClassExA(&WindowClass))
    {
      OutputDebugStringA("SE REGISTRO CORRECTAMENTE WindowClassS\n");
      GWindowPlatform.Window= CreateWindowExA(0,
				    CLASS_NAME,
				    WINDOW_NAME,
				    WS_VISIBLE|WS_OVERLAPPEDWINDOW,
				    CW_USEDEFAULT ,/*CORDENADA X DE LA VENTAN*/
				    CW_USEDEFAULT, /*CORDENADA Y DE LA VENTANA*/
				    CW_USEDEFAULT,/*ANCHO DE LA VENTANA*/
				    CW_USEDEFAULT,/*ALTO DE LA VENTANA*/
				    NULL,         /*MANEJADOR DE LA VENTANA PADRE*/
				    NULL,
				    HInst,
				    NULL);

      if(GWindowPlatform.Window)
	{
	   Win32WindowDimension Dim = Win32GetWindowDimension(GWindowPlatform.Window);
	   inic_app(Dim);
	   // memset(&gclok, 0, sizeof(Clock));
	   OutputDebugStringA("SE CREO CORRECTAMENTE LA VENTANA\n");
	   //  GHdc= GetDC(Wwindow); ///GHdc este es el buffer principal
	   char debugBuffer[256];
	   static LARGE_INTEGER Hz;
	   memset(&Hz, 0, sizeof(Hz));
	   QueryPerformanceFrequency(&Hz);
	   while(GWindowPlatform.Running)
	     {
	       MSG Message;
	       resetKeyBoard(); /// resetear el telcado en cada fotograma
	       static int lastKey=0;
	       while(PeekMessage(&Message,0,0,0,PM_REMOVE))// nos permite vaciar la cola
		 {
		   switch(Message.message)
		     {
		     case  WM_KEYDOWN:
		     case  WM_KEYUP:
		     case  WM_SYSKEYDOWN:
		     case  WM_SYSKEYUP:
		       {
#define CTRL_DOWN (GetKeyState(VK_CONTROL) & 0x8000)
#define ALT_DOWN  (GetKeyState(VK_MENU) & 0x8000)
#define SHIFT_DOWN (GetKeyState(VK_SHIFT) & 0x8000)
			 
			 lastKey++;
			 keyEvent.countEvent++; // no olvidar resetear
			 unsigned int virtualKey = (unsigned int) Message.wParam;
			 unsigned int preState   = (((Message.lParam)&(1 << 30)) != 0);
			 unsigned int isDown     = (((Message.lParam)&(1 << 31)) == 0);
			 // procesar las teclas
			 keyEvent.buttons[virtualKey].ordinal  =   lastKey;
			 keyEvent.buttons[virtualKey].actual   =   ((Message.lParam)&(1 << 31));
			 keyEvent.buttons[virtualKey].previous =   ((Message.lParam)&(1 << 30));
			 keyEvent.buttons[virtualKey].vk       =   virtualKey;
			 keyEvent.buttons[virtualKey].isDown   =   isDown;
			 keyEvent.buttons[virtualKey].changed  =   (preState != isDown);
			 /// agregar la secuencias de escape y el caracter unicode+
			 wchar_t alpha[MAX_ESCAPE]={0};
			 GetKeyboardState(keyEvent.keystate);
			 int result= ToUnicode(keyEvent.buttons[virtualKey].vk,
					       0,keyEvent.keystate,alpha,2,0);
			 keyEvent.buttons[virtualKey].unicode=alpha[0];
			 wcsncpy(keyEvent.buttons[virtualKey].escape, alpha, MAX_ESCAPE);
			 
                         /// agregar secuencias de escape que falten
			 
			 if (CTRL_DOWN && virtualKey == 'C')
			   {
			     wcsncpy(keyEvent.buttons[virtualKey].escape,
				     L"^C",MAX_ESCAPE);
			   }
			 #include "vk_to_escape.c"
			 
		       }break;
		     case WM_QUIT:
		       {
			 GWindowPlatform.Running=0;
		       }
		     default:
		       {
			 TranslateMessage(&Message);
			 DispatchMessageA(&Message);
		       }break;
		     }
		 }
	       lastKey=0;
	       Win32WindowDimension Dimension = Win32GetWindowDimension(GWindowPlatform.Window);
	       Win32RenderBuffer(GWindowPlatform.primaryHdc,Dimension.Width, Dimension.Height,&GBuffer);
	       // obtener este tiempo
	       LARGE_INTEGER t0,tf;
	       QueryPerformanceCounter(&t0);
	       main_app(&GBuffer, &keyEvent);
	       Sleep(1);
	       QueryPerformanceCounter(&tf);
	       LONGLONG elapsed = (tf.QuadPart - t0.QuadPart);
	       double dtms= (double)elapsed /(double)Hz.QuadPart;
	       GWindowPlatform.sumdt+=(dtms*1000.0);
	       GWindowPlatform.frames++;
	       if(GWindowPlatform.frames == FRAMES)
		 {
		   GWindowPlatform.avarageTimePerFrame = GWindowPlatform.sumdt/FRAMES;
		   double slepTime = 1000.0 - GWindowPlatform.sumdt;
		   if(slepTime > 0)
		     {
		       Sleep((DWORD)slepTime);
		     }
		   GWindowPlatform.avarageFps =(FRAMES/(GWindowPlatform.sumdt/1000.0));
		   char b[100];
		   sprintf(b,"fps %f\n",(GWindowPlatform.sumdt+slepTime)/GWindowPlatform.frames);
		   OutputDebugStringA(b);
		   GWindowPlatform.frames=0;
		   GWindowPlatform.sumdt=0;
		 }
	     }
	   SelectObject(GBuffer.BackBuffer, GBuffer.HoldBitmap);
           DeleteObject(GBuffer.Hbitmap);
           DeleteDC(GBuffer.BackBuffer);
	   KillTimer(GWindowPlatform.Window, ID_TIMER_BLINK_CURSOR);
           ReleaseDC(GWindowPlatform.Window, GWindowPlatform.primaryHdc);
	}
      else
	{
	   OutputDebugStringA("FALLO LA CREACION DE LA VENTANA");

	}
    }
  else
    {
       OutputDebugStringA("FALLÓ EL REGISTRO DE LA CLASE WindowClass");
    }

  return 0;
}


