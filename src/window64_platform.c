#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <stdbool.h>

#define STRUCT_INIC(W) memset((char *)(W),0,sizeof((*W)))
#define CLASS_NAME "Class_Window64"
#define WINDOW_NAME "DattaMetter" 
static HINSTANCE GHInst;
static void *GPixels=NULL;
static BITMAPINFO GPBmi;
int Runnig=1;
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\keyboard.h"
Win32ScreenBuffer GBuffer;
HDC GHdc=NULL;
HWND Wwindow;
#if 1
extern unsigned char enableBlinkCursor;
#define ID_TIMER_BLINK_CURSOR 1 // timer para habilitar el parpadeo del cursor
#include "..\..\Vt\includes\main_datametter.h"
#endif
#define FRAMES 300.0
#define TIME_PER_FRAME_TARGET (1000.0 / FRAMES)

double avarageFps=0.0;
double avarageTimePerFrame=0.0;
double sumdt=0.0;
int frames=0;

KeyboardEvent keyEvent;
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
  

  if(Buffer->BackBuffer)
    {
      // VirtualFree(Buffer->Pixels,0,MEM_RELEASE);

      SelectObject(Buffer->BackBuffer,Buffer->HoldBitmap);
      DeleteObject(Buffer->Hbitmap);
      DeleteDC(Buffer->BackBuffer);
      Buffer->BackBuffer=NULL;
    }
  
  Buffer->Width=Width;
  Buffer->Height=Height;
  #if 1 
  updateDim(Width,Height);
  #endif
  
  int BytesPerPixel= sizeof(int); //4 bytes
  STRUCT_INIC(&Buffer->Info);
  // Llenar la estructura bitmap info
  Buffer->Info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
  Buffer->Info.bmiHeader.biWidth=Width;
  Buffer->Info.bmiHeader.biHeight= -Height;
  Buffer->Info.bmiHeader.biPlanes=1;
  Buffer->Info.bmiHeader.biBitCount=32;
  Buffer->Info.bmiHeader.biCompression=BI_RGB;
  Buffer->BackBuffer=CreateCompatibleDC(GHdc);
  Buffer->Hbitmap= CreateCompatibleBitmap(GHdc,Width,Height);
  Buffer->HoldBitmap=(HBITMAP) SelectObject(Buffer->BackBuffer,Buffer->Hbitmap);
  Buffer->Pitch= Buffer->Width*BytesPerPixel;
  
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
LRESULT CALLBACK MainWndProc(
    HWND HWindow,        // handle to window
    UINT UMsg,        // message identifier
    WPARAM WParam,    // first message parameter
    LPARAM LParam)    // second message parameter
{
   LRESULT Result=0;
  switch(UMsg)
    {
    static int firstKey = 0;
    case  WM_KEYDOWN:
    case  WM_KEYUP:
      {
	if(UMsg == WM_KEYUP)
	  {
	    OutputDebugStringA("KEYUP\n");
	    if(firstKey != 0)
	    firstKey--;
	  }
	else
	  {
	     OutputDebugStringA("KEYDOWN\n");
	     if(UMsg == WM_KEYDOWN)
	       {
		 firstKey++;
	       }
	  }
	unsigned char keyStates[256];
        GetKeyboardState(keyEvent.keystate);
	unsigned int virtualKey = (unsigned int)WParam;
	keyEvent.buttons[virtualKey].vk=virtualKey;  /// este es el primero
	keyEvent.buttons[virtualKey].ordinal=firstKey;    /// marcamos como la primera
	keyEvent.buttons[virtualKey].repet = LOWORD(LParam);
	unsigned int  previousState = ((LParam & (1 << 30)) != 0);
	unsigned int  isDown = ((LParam & (1 << 31)) == 0);//1 0
	for(int currentButton = 1; currentButton < BUTTON_COUNT; currentButton++)
	  {
	    if((keyStates[currentButton] & 0x80)) 
	      {
	        keyEvent.buttons[currentButton].vk=currentButton;
		
	      }
	    if((virtualKey  == keyEvent.buttons[currentButton].vk)&&
	       (keyEvent.buttons[currentButton].processed != 1))
	      {
		keyEvent.buttons[currentButton].changed =(isDown != previousState); /*isDown != keyEvent.buttons[currentButton].isDown;*/
		 keyEvent.buttons[currentButton].isDown= isDown;// 0
		 keyEvent.buttons[currentButton].processed=1;// 0
	      }
	    else
	      {
		continue;
	      }
	    
	    }
      }break;
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
	 GHdc= BeginPaint(HWindow,&Paintstr); // devuelve un hdc con la region a acutaliar
	  Win32WindowDimension Dimension=Win32GetWindowDimension(HWindow);
	  Win32ResizeDibSection(&GBuffer,Dimension.Width,Dimension.Height);
	  
	  Win32RenderBuffer(GHdc,
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
      Wwindow= CreateWindowExA(0,
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

      if(Wwindow)
	{
	  Win32WindowDimension Dim = Win32GetWindowDimension(Wwindow);
	   inic_app(Dim);
	   // memset(&gclok, 0, sizeof(Clock));
	   OutputDebugStringA("SE CREO CORRECTAMENTE LA VENTANA\n");
	   GHdc= GetDC(Wwindow);
	   char debugBuffer[256];
	   static LARGE_INTEGER Hz;
	   memset(&Hz, 0, sizeof(Hz));
	   QueryPerformanceFrequency(&Hz);
	   while(Runnig)
	     {
	       
	      
	       MSG Message;
	       for(int x=1; x < BUTTON_COUNT; x++)
		 {
		 keyEvent.buttons[x].changed=0;
		 keyEvent.buttons[x].processed=0;
		 keyEvent.buttons[x].isDown=0;
		 }
	       while(PeekMessage(&Message,0,0,0,PM_REMOVE))// nos permite vaciar la cola
		 {
		   if(Message.message == WM_QUIT)
		     {
		       Runnig=0;
		     }
		    TranslateMessage(&Message);
                    DispatchMessageA(&Message);	     		    
		 }
	      
	       Win32WindowDimension Dimension = Win32GetWindowDimension(Wwindow);
	       Win32RenderBuffer(GHdc,Dimension.Width, Dimension.Height,&GBuffer);
	       // obtener este tiempo
	       LARGE_INTEGER t0,tf;
	       QueryPerformanceCounter(&t0);
	       main_app(&GBuffer, &keyEvent);
	       Sleep(1);
	       QueryPerformanceCounter(&tf);
	       LONGLONG elapsed = (tf.QuadPart - t0.QuadPart);
	       double dtms= (double)elapsed /(double)Hz.QuadPart;
	       sumdt+=(dtms*1000.0);
	       frames++;
	       if(frames == FRAMES)
		 {
		   avarageTimePerFrame = sumdt/FRAMES;
		   double slepTime = 1000.0 - sumdt;
		   if(slepTime > 0)
		     {
		       Sleep((DWORD)slepTime);
		     }
		   avarageFps =(FRAMES/(sumdt/1000.0));
		   char b[100];
		   sprintf(b,"fps %f\n",(sumdt+slepTime)/frames);
		   OutputDebugStringA(b);
		   frames=0;
		   sumdt=0;
		 }
	     }
	   SelectObject(GBuffer.BackBuffer, GBuffer.HoldBitmap);
           DeleteObject(GBuffer.Hbitmap);
           DeleteDC(GBuffer.BackBuffer);
	   KillTimer(Wwindow, ID_TIMER_BLINK_CURSOR);
           ReleaseDC(Wwindow, GHdc);
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


